#include "TaskManager.h"

// Initialize static members
MainUI* TaskManager::mainUI = nullptr;
InputManager* TaskManager::inputManager = nullptr;
TaskHandle_t TaskManager::uiTaskHandle = nullptr;
TaskHandle_t TaskManager::inputTaskHandle = nullptr;
TaskHandle_t TaskManager::sensorTaskHandle = nullptr;

void TaskManager::init(MainUI* mainUI) {
    // Store UI instance
    TaskManager::mainUI = mainUI;
    
    // Create input manager
    inputManager = new InputManager(mainUI);
    
    // Create tasks
    xTaskCreatePinnedToCore(
        uiTaskFunction,        // Function to implement the task
        "UI_Task",             // Name of the task
        TASK_STACK_SIZE_UI,    // Stack size in words
        NULL,                  // Task input parameter
        TASK_PRIORITY_UI,      // Priority of the task
        &uiTaskHandle,         // Task handle
        1                      // Core where the task should run (1 = second core)
    );
    
    xTaskCreatePinnedToCore(
        inputTaskFunction,     // Function to implement the task
        "Input_Task",          // Name of the task
        TASK_STACK_SIZE_INPUT, // Stack size in words
        NULL,                  // Task input parameter
        TASK_PRIORITY_INPUT,   // Priority of the task
        &inputTaskHandle,      // Task handle
        1                      // Core where the task should run (1 = second core)
    );
    
    xTaskCreatePinnedToCore(
        sensorTaskFunction,    // Function to implement the task
        "Sensor_Task",         // Name of the task
        TASK_STACK_SIZE_SENSOR,// Stack size in words
        NULL,                  // Task input parameter
        TASK_PRIORITY_SENSOR,  // Priority of the task
        &sensorTaskHandle,     // Task handle
        0                      // Core where the task should run (0 = first core)
    );
    
    // Log task creation
    Serial.println("Tasks created and running");
}

// UI task function - handles UI updates and rendering
void TaskManager::uiTaskFunction(void* parameter) {
    TickType_t xLastWakeTime = xTaskGetTickCount();
    
    // Task loop
    for(;;) {
        // Calculate time since last update
        unsigned long currentTime = millis();
        static unsigned long lastUpdateTime = currentTime;
        unsigned long deltaTime = currentTime - lastUpdateTime;
        lastUpdateTime = currentTime;
        
        // Update UI state
        if (mainUI) {
            mainUI->update(deltaTime);
            mainUI->render();
        }
        
        // Delay task to maintain frequency
        vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(UI_UPDATE_INTERVAL));
    }
}

// Input task function - processes all user inputs
void TaskManager::inputTaskFunction(void* parameter) {
    TickType_t xLastWakeTime = xTaskGetTickCount();
    
    // Task loop
    for(;;) {
        // Process all inputs
        if (inputManager) {
            inputManager->update();
        }
        
        // Delay task to maintain frequency
        vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(INPUT_UPDATE_INTERVAL));
    }
}

// Sensor task function - reads and processes sensor data
void TaskManager::sensorTaskFunction(void* parameter) {
    TickType_t xLastWakeTime = xTaskGetTickCount();
    
    // Task loop
    for(;;) {
        // Update sensors
        hal_sensor_update();
        
        // Delay task to maintain frequency
        vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(SENSOR_UPDATE_INTERVAL));
    }
}
