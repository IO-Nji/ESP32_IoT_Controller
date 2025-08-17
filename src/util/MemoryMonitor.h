#ifndef MEMORY_MONITOR_H
#define MEMORY_MONITOR_H

#include <Arduino.h>
#include <esp_heap_caps.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include "ErrorHandler.h"

// Task Stack Monitor definitions
#define STACK_MONITOR_MAX_TASKS 10  // Maximum number of tasks to monitor
#define STACK_WARNING_THRESHOLD 20   // Warn if free stack percentage is below this value

/**
 * @brief Utility class for monitoring memory usage
 * 
 * This class provides methods to check free memory, monitor heap fragmentation,
 * stack usage, and report memory-related issues.
 */
class MemoryMonitor {
public:
    /**
     * @brief Initialize the memory monitor
     * 
     * @param criticalThreshold Percentage of free memory below which to issue critical warnings
     * @param warningThreshold Percentage of free memory below which to issue warnings
     */
    static void init(uint8_t criticalThreshold = 10, uint8_t warningThreshold = 20);
    
    /**
     * @brief Check current memory status and report issues if needed
     * 
     * @return true if memory levels are normal
     * @return false if memory is low
     */
    static bool checkMemory();
    
    /**
     * @brief Check stack usage of all tasks
     * Analyzes the stack usage of all running tasks and reports warnings
     * for tasks that are close to stack overflow
     * 
     * @return true if all tasks have sufficient stack
     * @return false if any task is close to stack overflow
     */
    static bool checkStackUsage();
    
    /**
     * @brief Get the free heap size
     * 
     * @return uint32_t Free heap size in bytes
     */
    static uint32_t getFreeHeap();
    
    /**
     * @brief Get the minimum free heap size observed
     * 
     * @return uint32_t Minimum free heap in bytes
     */
    static uint32_t getMinFreeHeap();
    
    /**
     * @brief Get the maximum allocation possible
     * 
     * @return uint32_t Maximum allocation possible in bytes
     */
    static uint32_t getMaxAllocHeap();
    
    /**
     * @brief Get the percentage of free heap memory
     * 
     * @return uint8_t Percentage of free heap (0-100)
     */
    static uint8_t getFreeHeapPercentage();
    
    /**
     * @brief Get the current heap fragmentation
     * 
     * @return uint8_t Fragmentation percentage (0-100)
     * Higher values indicate more fragmentation
     */
    static uint8_t getHeapFragmentation();
    
    /**
     * @brief Get the PSRAM free size (if available)
     * 
     * @return uint32_t Free PSRAM in bytes, 0 if not available
     */
    static uint32_t getFreePSRAM();
    
    /**
     * @brief Print detailed memory report to Serial
     * Includes heap, stack, and fragmentation information
     */
    static void printMemoryReport();
    
    /**
     * @brief Get the free stack space for a task
     * 
     * @param taskHandle Handle to the task
     * @return uint32_t Free stack space in bytes
     */
    static uint32_t getTaskFreeStack(TaskHandle_t taskHandle);
    
    /**
     * @brief Get the name of a task
     * 
     * @param taskHandle Handle to the task
     * @return String Task name
     */
    static String getTaskName(TaskHandle_t taskHandle);
    
    /**
     * @brief Attempt to optimize memory allocation patterns
     * This method defragments the heap by allocating and immediately freeing
     * a larger block of memory, which can consolidate free blocks
     * 
     * @return true if defragmentation succeeded
     * @return false if defragmentation failed
     */
    static bool defragmentHeap();
    
private:
    static uint8_t criticalThreshold;
    static uint8_t warningThreshold;
    static uint32_t totalHeapSize;
    static unsigned long lastReportTime;
};

#endif // MEMORY_MONITOR_H
