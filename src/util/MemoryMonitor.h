#ifndef MEMORY_MONITOR_H
#define MEMORY_MONITOR_H

#include <Arduino.h>
#include "ErrorHandler.h"

/**
 * @brief Utility class for monitoring memory usage
 * 
 * This class provides methods to check free memory, monitor heap fragmentation,
 * and report memory-related issues.
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
    
private:
    static uint8_t criticalThreshold;
    static uint8_t warningThreshold;
    static uint32_t totalHeapSize;
};

#endif // MEMORY_MONITOR_H
