#ifndef TASK_MANAGER_H
#define TASK_MANAGER_H

#include <Arduino.h>
#include "../ui/main/MainUI.h"
#include "../input/InputManager.h"
#include "../hal/sensor.h"
#include "../util/ErrorHandler.h"
#include "../util/MemoryMonitor.h"

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
#define WATCHDOG_CHECK_INTERVAL 5000 // 5 seconds

class TaskManager {
public:
    /**
     * @brief Initialize and start all system tasks
     * 
     * @param mainUI Pointer to MainUI instance
     */
    static void init(MainUI* mainUI);
    
    /**
     * @brief Check the status of all tasks
     * 
     * @return true if all tasks are running normally
     * @return false if any task has issues
     */
    static bool checkTaskStatus();
    
    /**
     * @brief Restart a specific task if it has crashed
     * 
     * @param taskType Type of task to restart (0=UI, 1=Input, 2=Sensor)
     * @return true if task was restarted successfully
     * @return false if restart failed
     */
    static bool restartTask(uint8_t taskType);
    
private:
    static MainUI* mainUI;
    static InputManager* inputManager;
    
    // Task handles
    static TaskHandle_t uiTaskHandle;
    static TaskHandle_t inputTaskHandle;
    static TaskHandle_t sensorTaskHandle;
    
    // Task status tracking
    static unsigned long lastUITaskAlive;
    static unsigned long lastInputTaskAlive;
    static unsigned long lastSensorTaskAlive;
    static uint8_t uiTaskErrors;
    static uint8_t inputTaskErrors;
    static uint8_t sensorTaskErrors;
    
    // Task functions
    static void uiTaskFunction(void* parameter);
    static void inputTaskFunction(void* parameter);
    static void sensorTaskFunction(void* parameter);
    static void watchdogTaskFunction(void* parameter);
};

#endif // TASK_MANAGER_H
