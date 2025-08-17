#include "TaskManager.h"

// Initialize static members
MainUI* TaskManager::mainUI = nullptr;
InputManager* TaskManager::inputManager = nullptr;
TaskHandle_t TaskManager::uiTaskHandle = nullptr;
TaskHandle_t TaskManager::inputTaskHandle = nullptr;
TaskHandle_t TaskManager::sensorTaskHandle = nullptr;
TaskHandle_t TaskManager::networkTaskHandle = nullptr;
TaskHandle_t TaskManager::storageTaskHandle = nullptr;
TaskHandle_t TaskManager::powerTaskHandle = nullptr;

// Task status tracking
unsigned long TaskManager::lastUITaskAlive = 0;
unsigned long TaskManager::lastInputTaskAlive = 0;
unsigned long TaskManager::lastSensorTaskAlive = 0;
unsigned long TaskManager::lastNetworkTaskAlive = 0;
unsigned long TaskManager::lastStorageTaskAlive = 0;
unsigned long TaskManager::lastPowerTaskAlive = 0;
uint8_t TaskManager::uiTaskErrors = 0;
uint8_t TaskManager::inputTaskErrors = 0;
uint8_t TaskManager::sensorTaskErrors = 0;
uint8_t TaskManager::networkTaskErrors = 0;
uint8_t TaskManager::storageTaskErrors = 0;
uint8_t TaskManager::powerTaskErrors = 0;

void TaskManager::init(MainUI* mainUI) {
    REPORT_INFO("Initializing task system");
    
    // Check if MainUI is valid
    if (!mainUI) {
        REPORT_CRITICAL(ErrorCategory::TASK, "NULL MainUI reference in TaskManager::init");
        return;
    }
    
    // Store UI instance
    TaskManager::mainUI = mainUI;
    
    try {
        // Initialize memory monitor
        MemoryMonitor::init();
        
        // Create input manager
        inputManager = new InputManager(mainUI);
        if (!inputManager) {
            REPORT_CRITICAL(ErrorCategory::MEMORY, "Failed to allocate InputManager");
            return;
        }
        
        // Check memory before creating tasks
        MemoryMonitor::checkMemory();
        
        // Create tasks
        BaseType_t result;
        
        // Create UI task
        result = xTaskCreatePinnedToCore(
            uiTaskFunction,        // Function to implement the task
            "UI_Task",             // Name of the task
            TASK_STACK_SIZE_UI,    // Stack size in words
            NULL,                  // Task input parameter
            TASK_PRIORITY_UI,      // Priority of the task
            &uiTaskHandle,         // Task handle
            1                      // Core where the task should run (1 = second core)
        );
        
        if (result != pdPASS) {
            REPORT_CRITICAL(ErrorCategory::TASK, "Failed to create UI task");
            return;
        }
        
        // Create input task
        result = xTaskCreatePinnedToCore(
            inputTaskFunction,     // Function to implement the task
            "Input_Task",          // Name of the task
            TASK_STACK_SIZE_INPUT, // Stack size in words
            NULL,                  // Task input parameter
            TASK_PRIORITY_INPUT,   // Priority of the task
            &inputTaskHandle,      // Task handle
            1                      // Core where the task should run (1 = second core)
        );
        
        if (result != pdPASS) {
            REPORT_CRITICAL(ErrorCategory::TASK, "Failed to create Input task");
            return;
        }
        
        // Create sensor task
        result = xTaskCreatePinnedToCore(
            sensorTaskFunction,    // Function to implement the task
            "Sensor_Task",         // Name of the task
            TASK_STACK_SIZE_SENSOR,// Stack size in words
            NULL,                  // Task input parameter
            TASK_PRIORITY_SENSOR,  // Priority of the task
            &sensorTaskHandle,     // Task handle
            0                      // Core where the task should run (0 = first core)
        );
        
        if (result != pdPASS) {
            REPORT_CRITICAL(ErrorCategory::TASK, "Failed to create Sensor task");
            return;
        }
        
        // Create network task
        result = xTaskCreatePinnedToCore(
            networkTaskFunction,      // Function to implement the task
            "Network_Task",           // Name of the task
            TASK_STACK_SIZE_NETWORK,  // Stack size in words
            NULL,                     // Task input parameter
            TASK_PRIORITY_NETWORK,    // Priority of the task
            &networkTaskHandle,       // Task handle
            0                         // Core where the task should run (0 = first core)
        );
        
        if (result != pdPASS) {
            REPORT_CRITICAL(ErrorCategory::TASK, "Failed to create Network task");
            return;
        }
        
        // Create storage task
        result = xTaskCreatePinnedToCore(
            storageTaskFunction,      // Function to implement the task
            "Storage_Task",           // Name of the task
            TASK_STACK_SIZE_STORAGE,  // Stack size in words
            NULL,                     // Task input parameter
            TASK_PRIORITY_STORAGE,    // Priority of the task
            &storageTaskHandle,       // Task handle
            0                         // Core where the task should run (0 = first core)
        );
        
        if (result != pdPASS) {
            REPORT_CRITICAL(ErrorCategory::TASK, "Failed to create Storage task");
            return;
        }
        
        // Create power management task
        result = xTaskCreatePinnedToCore(
            powerTaskFunction,      // Function to implement the task
            "Power_Task",           // Name of the task
            TASK_STACK_SIZE_POWER,  // Stack size in words
            NULL,                   // Task input parameter
            TASK_PRIORITY_POWER,    // Priority of the task
            &powerTaskHandle,       // Task handle
            0                       // Core where the task should run (0 = first core)
        );
        
        if (result != pdPASS) {
            REPORT_CRITICAL(ErrorCategory::TASK, "Failed to create Power Management task");
            return;
        }
        
        // Create a watchdog task to monitor other tasks
        TaskHandle_t watchdogHandle;
        result = xTaskCreatePinnedToCore(
            watchdogTaskFunction,  // Function to implement the task
            "Watchdog_Task",       // Name of the task
            2048,                  // Stack size in words
            NULL,                  // Task input parameter
            1,                     // Priority of the task (lowest)
            &watchdogHandle,       // Task handle
            0                      // Core where the task should run (0 = first core)
        );
        
        if (result != pdPASS) {
            REPORT_WARNING(ErrorCategory::TASK, "Failed to create Watchdog task");
        }
        
        // Initialize task status tracking
        lastUITaskAlive = millis();
        lastInputTaskAlive = millis();
        lastSensorTaskAlive = millis();
        lastNetworkTaskAlive = millis();
        lastStorageTaskAlive = millis();
        lastPowerTaskAlive = millis();
        
        REPORT_INFO("Task system initialized successfully");
    }
    catch (const std::exception& e) {
        REPORT_CRITICAL(ErrorCategory::TASK, "Exception during task initialization: " + String(e.what()));
    }
    catch (...) {
        REPORT_CRITICAL(ErrorCategory::TASK, "Unknown exception during task initialization");
    }
}

// Check status of all tasks and report issues
bool TaskManager::checkTaskStatus() {
    bool allTasksOk = true;
    unsigned long currentTime = millis();
    
    // Check UI task
    if (uiTaskHandle && eTaskGetState(uiTaskHandle) == eDeleted) {
        REPORT_CRITICAL(ErrorCategory::TASK, "UI task has crashed");
        allTasksOk = false;
    } else if (currentTime - lastUITaskAlive > 5000) {
        REPORT_ERROR(ErrorCategory::TASK, ErrorSeverity::ERROR, "UI task may be stuck");
        allTasksOk = false;
    }
    
    // Check input task
    if (inputTaskHandle && eTaskGetState(inputTaskHandle) == eDeleted) {
        REPORT_CRITICAL(ErrorCategory::TASK, "Input task has crashed");
        allTasksOk = false;
    } else if (currentTime - lastInputTaskAlive > 5000) {
        REPORT_ERROR(ErrorCategory::TASK, ErrorSeverity::ERROR, "Input task may be stuck");
        allTasksOk = false;
    }
    
    // Check sensor task
    if (sensorTaskHandle && eTaskGetState(sensorTaskHandle) == eDeleted) {
        REPORT_CRITICAL(ErrorCategory::TASK, "Sensor task has crashed");
        allTasksOk = false;
    } else if (currentTime - lastSensorTaskAlive > 5000) {
        REPORT_ERROR(ErrorCategory::TASK, ErrorSeverity::ERROR, "Sensor task may be stuck");
        allTasksOk = false;
    }
    
    // Check network task
    if (networkTaskHandle && eTaskGetState(networkTaskHandle) == eDeleted) {
        REPORT_CRITICAL(ErrorCategory::TASK, "Network task has crashed");
        allTasksOk = false;
    } else if (currentTime - lastNetworkTaskAlive > 5000) {
        REPORT_ERROR(ErrorCategory::TASK, ErrorSeverity::ERROR, "Network task may be stuck");
        allTasksOk = false;
    }
    
    // Check storage task
    if (storageTaskHandle && eTaskGetState(storageTaskHandle) == eDeleted) {
        REPORT_CRITICAL(ErrorCategory::TASK, "Storage task has crashed");
        allTasksOk = false;
    } else if (currentTime - lastStorageTaskAlive > 5000) {
        REPORT_ERROR(ErrorCategory::TASK, ErrorSeverity::ERROR, "Storage task may be stuck");
        allTasksOk = false;
    }
    
    // Check power task
    if (powerTaskHandle && eTaskGetState(powerTaskHandle) == eDeleted) {
        REPORT_CRITICAL(ErrorCategory::TASK, "Power task has crashed");
        allTasksOk = false;
    } else if (currentTime - lastPowerTaskAlive > 5000) {
        REPORT_ERROR(ErrorCategory::TASK, ErrorSeverity::ERROR, "Power task may be stuck");
        allTasksOk = false;
    }
    
    return allTasksOk;
}

// Restart a task that has crashed
bool TaskManager::restartTask(uint8_t taskType) {
    BaseType_t result = pdFAIL;
    
    switch(taskType) {
        case 0: // UI Task
            if (uiTaskHandle) {
                vTaskDelete(uiTaskHandle);
                uiTaskHandle = nullptr;
            }
            
            result = xTaskCreatePinnedToCore(
                uiTaskFunction,
                "UI_Task",
                TASK_STACK_SIZE_UI,
                NULL,
                TASK_PRIORITY_UI,
                &uiTaskHandle,
                1
            );
            
            if (result == pdPASS) {
                REPORT_INFO("UI task restarted successfully");
                lastUITaskAlive = millis();
                uiTaskErrors = 0; // Reset error counter
                return true;
            } else {
                REPORT_CRITICAL(ErrorCategory::TASK, "Failed to restart UI task");
                return false;
            }
            break;
        
        case 1: // Input Task
            if (inputTaskHandle) {
                vTaskDelete(inputTaskHandle);
                inputTaskHandle = nullptr;
            }
            
            result = xTaskCreatePinnedToCore(
                inputTaskFunction,
                "Input_Task",
                TASK_STACK_SIZE_INPUT,
                NULL,
                TASK_PRIORITY_INPUT,
                &inputTaskHandle,
                1
            );
            
            if (result == pdPASS) {
                REPORT_INFO("Input task restarted successfully");
                lastInputTaskAlive = millis();
                inputTaskErrors = 0; // Reset error counter
                return true;
            } else {
                REPORT_CRITICAL(ErrorCategory::TASK, "Failed to restart Input task");
                return false;
            }
            break;
            
        case 2: // Sensor Task
            if (sensorTaskHandle) {
                vTaskDelete(sensorTaskHandle);
                sensorTaskHandle = nullptr;
            }
            
            result = xTaskCreatePinnedToCore(
                sensorTaskFunction,
                "Sensor_Task",
                TASK_STACK_SIZE_SENSOR,
                NULL,
                TASK_PRIORITY_SENSOR,
                &sensorTaskHandle,
                0
            );
            
            if (result == pdPASS) {
                REPORT_INFO("Sensor task restarted successfully");
                lastSensorTaskAlive = millis();
                sensorTaskErrors = 0; // Reset error counter
                return true;
            } else {
                REPORT_CRITICAL(ErrorCategory::TASK, "Failed to restart Sensor task");
                return false;
            }
            break;
            
        case 3: // Network Task
            if (networkTaskHandle) {
                vTaskDelete(networkTaskHandle);
                networkTaskHandle = nullptr;
            }
            
            result = xTaskCreatePinnedToCore(
                networkTaskFunction,
                "Network_Task",
                TASK_STACK_SIZE_NETWORK,
                NULL,
                TASK_PRIORITY_NETWORK,
                &networkTaskHandle,
                0
            );
            
            if (result == pdPASS) {
                REPORT_INFO("Network task restarted successfully");
                lastNetworkTaskAlive = millis();
                networkTaskErrors = 0; // Reset error counter
                return true;
            } else {
                REPORT_CRITICAL(ErrorCategory::TASK, "Failed to restart Network task");
                return false;
            }
            break;
            
        case 4: // Storage Task
            if (storageTaskHandle) {
                vTaskDelete(storageTaskHandle);
                storageTaskHandle = nullptr;
            }
            
            result = xTaskCreatePinnedToCore(
                storageTaskFunction,
                "Storage_Task",
                TASK_STACK_SIZE_STORAGE,
                NULL,
                TASK_PRIORITY_STORAGE,
                &storageTaskHandle,
                0
            );
            
            if (result == pdPASS) {
                REPORT_INFO("Storage task restarted successfully");
                lastStorageTaskAlive = millis();
                storageTaskErrors = 0; // Reset error counter
                return true;
            } else {
                REPORT_CRITICAL(ErrorCategory::TASK, "Failed to restart Storage task");
                return false;
            }
            break;
            
        case 5: // Power Task
            if (powerTaskHandle) {
                vTaskDelete(powerTaskHandle);
                powerTaskHandle = nullptr;
            }
            
            result = xTaskCreatePinnedToCore(
                powerTaskFunction,
                "Power_Task",
                TASK_STACK_SIZE_POWER,
                NULL,
                TASK_PRIORITY_POWER,
                &powerTaskHandle,
                0
            );
            
            if (result == pdPASS) {
                REPORT_INFO("Power task restarted successfully");
                lastPowerTaskAlive = millis();
                powerTaskErrors = 0; // Reset error counter
                return true;
            } else {
                REPORT_CRITICAL(ErrorCategory::TASK, "Failed to restart Power task");
                return false;
            }
            break;
            
        default:
            REPORT_ERROR(ErrorCategory::TASK, ErrorSeverity::ERROR, "Unknown task type for restart: " + String(taskType));
            return false;
    }
    
    return false;
}

// UI task function - handles UI updates and rendering
void TaskManager::uiTaskFunction(void* parameter) {
    TickType_t xLastWakeTime = xTaskGetTickCount();
    
    // Task loop
    for(;;) {
        try {
            // Update task alive timestamp
            lastUITaskAlive = millis();
            
            // Calculate time since last update
            unsigned long currentTime = millis();
            static unsigned long lastUpdateTime = currentTime;
            unsigned long deltaTime = currentTime - lastUpdateTime;
            lastUpdateTime = currentTime;
            
            // Update UI state
            if (mainUI) {
                mainUI->update(deltaTime);
                mainUI->render();
            } else {
                REPORT_ERROR(ErrorCategory::UI, ErrorSeverity::ERROR, "NULL UI reference in UI task");
            }
        }
        catch (const std::exception& e) {
            uiTaskErrors++;
            REPORT_ERROR(ErrorCategory::TASK, ErrorSeverity::ERROR, 
                       String("Exception in UI task: ") + String(e.what()) + 
                       " (count: " + String(uiTaskErrors) + ")");
        }
        catch (...) {
            uiTaskErrors++;
            REPORT_ERROR(ErrorCategory::TASK, ErrorSeverity::ERROR, 
                       String("Unknown exception in UI task") + 
                       " (count: " + String(uiTaskErrors) + ")");
        }
        
        // Check for too many errors
        if (uiTaskErrors > 100) {
            REPORT_CRITICAL(ErrorCategory::TASK, "UI task encountered too many errors");
            // Consider restarting or rebooting
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
        try {
            // Update task alive timestamp
            lastInputTaskAlive = millis();
            
            // Process all inputs
            if (inputManager) {
                bool success = inputManager->update();
                if (!success) {
                    inputTaskErrors++;
                }
            } else {
                REPORT_ERROR(ErrorCategory::INPUT_SYSTEM, ErrorSeverity::ERROR, "NULL InputManager reference in input task");
                inputTaskErrors++;
            }
            
            // Check for too many errors
            if (inputTaskErrors > 100) {
                REPORT_CRITICAL(ErrorCategory::TASK, "Input task encountered too many errors");
                // Consider restarting or rebooting
            }
        }
        catch (const std::exception& e) {
            inputTaskErrors++;
            REPORT_ERROR(ErrorCategory::TASK, ErrorSeverity::ERROR, 
                       String("Exception in input task: ") + String(e.what()) + 
                       " (count: " + String(inputTaskErrors) + ")");
        }
        catch (...) {
            inputTaskErrors++;
            REPORT_ERROR(ErrorCategory::TASK, ErrorSeverity::ERROR, 
                       String("Unknown exception in input task") + 
                       " (count: " + String(inputTaskErrors) + ")");
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
        try {
            // Update task alive timestamp
            lastSensorTaskAlive = millis();
            
            // Update sensors
            bool success = hal_sensor_update();
            if (!success) {
                sensorTaskErrors++;
                REPORT_ERROR(ErrorCategory::SENSOR, ErrorSeverity::WARNING, "Sensor update failed");
            }
            
            // Check for too many errors
            if (sensorTaskErrors > 100) {
                REPORT_CRITICAL(ErrorCategory::TASK, "Sensor task encountered too many errors");
                // Consider restarting or rebooting
            }
        }
        catch (const std::exception& e) {
            sensorTaskErrors++;
            REPORT_ERROR(ErrorCategory::TASK, ErrorSeverity::ERROR, 
                       String("Exception in sensor task: ") + String(e.what()) + 
                       " (count: " + String(sensorTaskErrors) + ")");
        }
        catch (...) {
            sensorTaskErrors++;
            REPORT_ERROR(ErrorCategory::TASK, ErrorSeverity::ERROR, 
                       String("Unknown exception in sensor task") + 
                       " (count: " + String(sensorTaskErrors) + ")");
        }
        
        // Delay task to maintain frequency
        vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(SENSOR_UPDATE_INTERVAL));
    }
}

// Network task function - handles WiFi/BLE connectivity and network communications
void TaskManager::networkTaskFunction(void* parameter) {
    TickType_t xLastWakeTime = xTaskGetTickCount();
    
    // Allow some time for other tasks to initialize
    vTaskDelay(pdMS_TO_TICKS(1000));
    
    REPORT_INFO("Network task started");
    
    // Task loop
    for(;;) {
        try {
            // Update task alive timestamp
            lastNetworkTaskAlive = millis();
            
            // TODO: Network connection management
            // Check WiFi status
            // Reconnect if necessary
            // Handle MQTT or other protocol communication
            
            // Process any pending network transactions
            // ...
            
            // Check for too many errors
            if (networkTaskErrors > 100) {
                REPORT_CRITICAL(ErrorCategory::TASK, "Network task encountered too many errors");
                // Consider restarting or rebooting
            }
        }
        catch (const std::exception& e) {
            networkTaskErrors++;
            REPORT_ERROR(ErrorCategory::TASK, ErrorSeverity::ERROR, 
                       String("Exception in network task: ") + String(e.what()) + 
                       " (count: " + String(networkTaskErrors) + ")");
        }
        catch (...) {
            networkTaskErrors++;
            REPORT_ERROR(ErrorCategory::TASK, ErrorSeverity::ERROR, 
                       String("Unknown exception in network task") + 
                       " (count: " + String(networkTaskErrors) + ")");
        }
        
        // Delay task to maintain frequency
        vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(NETWORK_UPDATE_INTERVAL));
    }
}

// Storage task function - handles data persistence and file operations
void TaskManager::storageTaskFunction(void* parameter) {
    TickType_t xLastWakeTime = xTaskGetTickCount();
    
    // Allow some time for other tasks to initialize
    vTaskDelay(pdMS_TO_TICKS(2000));
    
    REPORT_INFO("Storage task started");
    
    // Task loop
    for(;;) {
        try {
            // Update task alive timestamp
            lastStorageTaskAlive = millis();
            
            // TODO: Implement storage operations
            // - Check for data that needs to be persisted
            // - Handle file operations (read/write to SD card or flash)
            // - Manage configuration data
            
            // Check for too many errors
            if (storageTaskErrors > 100) {
                REPORT_CRITICAL(ErrorCategory::TASK, "Storage task encountered too many errors");
                // Consider restarting or rebooting
            }
        }
        catch (const std::exception& e) {
            storageTaskErrors++;
            REPORT_ERROR(ErrorCategory::TASK, ErrorSeverity::ERROR, 
                       String("Exception in storage task: ") + String(e.what()) + 
                       " (count: " + String(storageTaskErrors) + ")");
        }
        catch (...) {
            storageTaskErrors++;
            REPORT_ERROR(ErrorCategory::TASK, ErrorSeverity::ERROR, 
                       String("Unknown exception in storage task") + 
                       " (count: " + String(storageTaskErrors) + ")");
        }
        
        // Delay task to maintain frequency
        vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(STORAGE_UPDATE_INTERVAL));
    }
}

// Power management task function - handles power-related operations
void TaskManager::powerTaskFunction(void* parameter) {
    TickType_t xLastWakeTime = xTaskGetTickCount();
    
    // Allow some time for other tasks to initialize
    vTaskDelay(pdMS_TO_TICKS(3000));
    
    REPORT_INFO("Power management task started");
    
    // Task loop
    for(;;) {
        try {
            // Update task alive timestamp
            lastPowerTaskAlive = millis();
            
            // TODO: Implement power management
            // - Monitor battery voltage/level
            // - Handle power saving modes
            // - Manage deep sleep if supported
            // - Track power consumption
            
            // Check for too many errors
            if (powerTaskErrors > 100) {
                REPORT_CRITICAL(ErrorCategory::TASK, "Power task encountered too many errors");
                // Consider restarting or rebooting
            }
        }
        catch (const std::exception& e) {
            powerTaskErrors++;
            REPORT_ERROR(ErrorCategory::TASK, ErrorSeverity::ERROR, 
                       String("Exception in power task: ") + String(e.what()) + 
                       " (count: " + String(powerTaskErrors) + ")");
        }
        catch (...) {
            powerTaskErrors++;
            REPORT_ERROR(ErrorCategory::TASK, ErrorSeverity::ERROR, 
                       String("Unknown exception in power task") + 
                       " (count: " + String(powerTaskErrors) + ")");
        }
        
        // Delay task to maintain frequency
        vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(POWER_UPDATE_INTERVAL));
    }
}

// Watchdog task function - monitors other tasks and restarts them if they crash
void TaskManager::watchdogTaskFunction(void* parameter) {
    const TickType_t xDelay = pdMS_TO_TICKS(WATCHDOG_CHECK_INTERVAL);
    
    // Give other tasks time to initialize
    vTaskDelay(pdMS_TO_TICKS(5000));
    
    REPORT_INFO("Watchdog task started");
    
    // Watchdog loop
    for(;;) {
        try {
            // Check for low memory conditions
            MemoryMonitor::checkMemory();
            
            // Check all task statuses
            bool tasksOk = checkTaskStatus();
            
            if (!tasksOk) {
                // If UI task is not responding, try to restart it
                if (uiTaskHandle && (eTaskGetState(uiTaskHandle) == eDeleted || 
                    (millis() - lastUITaskAlive > 10000))) {
                    REPORT_WARNING(ErrorCategory::TASK, "Attempting to restart UI task");
                    restartTask(0);
                }
                
                // If input task is not responding, try to restart it
                if (inputTaskHandle && (eTaskGetState(inputTaskHandle) == eDeleted || 
                    (millis() - lastInputTaskAlive > 10000))) {
                    REPORT_WARNING(ErrorCategory::TASK, "Attempting to restart Input task");
                    restartTask(1);
                }
                
                // If sensor task is not responding, try to restart it
                if (sensorTaskHandle && (eTaskGetState(sensorTaskHandle) == eDeleted || 
                    (millis() - lastSensorTaskAlive > 10000))) {
                    REPORT_WARNING(ErrorCategory::TASK, "Attempting to restart Sensor task");
                    restartTask(2);
                }
                
                // If network task is not responding, try to restart it
                if (networkTaskHandle && (eTaskGetState(networkTaskHandle) == eDeleted || 
                    (millis() - lastNetworkTaskAlive > 10000))) {
                    REPORT_WARNING(ErrorCategory::TASK, "Attempting to restart Network task");
                    restartTask(3);
                }
                
                // If storage task is not responding, try to restart it
                if (storageTaskHandle && (eTaskGetState(storageTaskHandle) == eDeleted || 
                    (millis() - lastStorageTaskAlive > 10000))) {
                    REPORT_WARNING(ErrorCategory::TASK, "Attempting to restart Storage task");
                    restartTask(4);
                }
                
                // If power task is not responding, try to restart it
                if (powerTaskHandle && (eTaskGetState(powerTaskHandle) == eDeleted || 
                    (millis() - lastPowerTaskAlive > 10000))) {
                    REPORT_WARNING(ErrorCategory::TASK, "Attempting to restart Power task");
                    restartTask(5);
                }
            }
            
            // Check if any task has excessive errors
            if (uiTaskErrors > 100 || inputTaskErrors > 100 || sensorTaskErrors > 100 || 
                networkTaskErrors > 100 || storageTaskErrors > 100 || powerTaskErrors > 100) {
                REPORT_CRITICAL(ErrorCategory::TASK, "Too many task errors, system may need a reset");
                
                // If using a hardware watchdog, could trigger a reset here
                // ESP.restart(); // Uncomment to enable auto-restart on critical errors
            }
        }
        catch (const std::exception& e) {
            REPORT_ERROR(ErrorCategory::TASK, ErrorSeverity::ERROR, 
                       String("Exception in watchdog task: ") + String(e.what()));
        }
        catch (...) {
            REPORT_ERROR(ErrorCategory::TASK, ErrorSeverity::ERROR, 
                       "Unknown exception in watchdog task");
        }
        
        // Delay until next check
        vTaskDelay(xDelay);
    }
}
