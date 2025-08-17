#ifndef TASK_MANAGER_H
#define TASK_MANAGER_H

#include <Arduino.h>
#include "../ui/main/MainUI.h"
#include "../input/InputManager.h"
#include "../hal/sensor.h"

// Task priorities
#define TASK_PRIORITY_UI       2
#define TASK_PRIORITY_INPUT    3
#define TASK_PRIORITY_SENSOR   1

// Task stack sizes
#define TASK_STACK_SIZE_UI     4096
#define TASK_STACK_SIZE_INPUT  2048
#define TASK_STACK_SIZE_SENSOR 2048

// Task execution frequencies in milliseconds
#define UI_UPDATE_INTERVAL     33  // 30Hz
#define INPUT_UPDATE_INTERVAL  20  // 50Hz
#define SENSOR_UPDATE_INTERVAL 100 // 10Hz

class TaskManager {
public:
    /**
     * @brief Initialize and start all system tasks
     * 
     * @param mainUI Pointer to MainUI instance
     */
    static void init(MainUI* mainUI);
    
private:
    static MainUI* mainUI;
    static InputManager* inputManager;
    
    // Task handles
    static TaskHandle_t uiTaskHandle;
    static TaskHandle_t inputTaskHandle;
    static TaskHandle_t sensorTaskHandle;
    
    // Task functions
    static void uiTaskFunction(void* parameter);
    static void inputTaskFunction(void* parameter);
    static void sensorTaskFunction(void* parameter);
};

#endif // TASK_MANAGER_H
