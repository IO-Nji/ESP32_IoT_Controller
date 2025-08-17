#include "MemoryMonitor.h"

// Initialize static variables
uint8_t MemoryMonitor::criticalThreshold = 10;
uint8_t MemoryMonitor::warningThreshold = 20;
uint32_t MemoryMonitor::totalHeapSize = 0;
unsigned long MemoryMonitor::lastReportTime = 0;

void MemoryMonitor::init(uint8_t criticalThreshold, uint8_t warningThreshold) {
    MemoryMonitor::criticalThreshold = criticalThreshold;
    MemoryMonitor::warningThreshold = warningThreshold;
    
    // Calculate total heap size (initial free + used)
    totalHeapSize = ESP.getHeapSize();
    
    // Report initial memory status
    REPORT_INFO("Memory monitor initialized. Total heap: " + 
                String(totalHeapSize) + " bytes, Free: " + 
                String(getFreeHeap()) + " bytes (" + 
                String(getFreeHeapPercentage()) + "%)");
}

bool MemoryMonitor::checkMemory() {
    uint8_t freePercentage = getFreeHeapPercentage();
    
    // Check for critical memory level
    if (freePercentage <= criticalThreshold) {
        REPORT_CRITICAL(ErrorCategory::MEMORY, 
                      "CRITICAL memory low: " + String(freePercentage) + 
                      "% free (" + String(getFreeHeap()) + " bytes)");
        return false;
    } 
    // Check for warning memory level
    else if (freePercentage <= warningThreshold) {
        REPORT_WARNING(ErrorCategory::MEMORY,
                     "Memory running low: " + String(freePercentage) + 
                     "% free (" + String(getFreeHeap()) + " bytes)");
        return false;
    }
    
    return true;
}

uint32_t MemoryMonitor::getFreeHeap() {
    return ESP.getFreeHeap();
}

uint32_t MemoryMonitor::getMinFreeHeap() {
    return ESP.getMinFreeHeap();
}

uint32_t MemoryMonitor::getMaxAllocHeap() {
    return ESP.getMaxAllocHeap();
}

uint8_t MemoryMonitor::getFreeHeapPercentage() {
    uint32_t freeHeap = getFreeHeap();
    return (uint8_t)((freeHeap * 100) / totalHeapSize);
}

uint8_t MemoryMonitor::getHeapFragmentation() {
    // ESP32 doesn't have getHeapFragmentation directly, use heap_caps_get_largest_free_block
    // Fragmentation = 100 - (largest free block * 100) / total free heap
    uint32_t freeHeap = heap_caps_get_free_size(MALLOC_CAP_DEFAULT);
    uint32_t largestBlock = heap_caps_get_largest_free_block(MALLOC_CAP_DEFAULT);
    
    // Avoid division by zero
    if (freeHeap == 0) return 0;
    
    // Calculate fragmentation percentage: 
    // 0% = no fragmentation (largest block = free heap)
    // 100% = complete fragmentation (largest block << free heap)
    uint8_t fragmentation = 100 - (uint8_t)((largestBlock * 100) / freeHeap);
    return fragmentation;
}

uint32_t MemoryMonitor::getFreePSRAM() {
#ifdef CONFIG_SPIRAM_SUPPORT
    if (ESP.getPsramSize() > 0) {
        return ESP.getFreePsram();
    }
#endif
    return 0; // Return 0 if PSRAM is not available
}

void MemoryMonitor::printMemoryReport() {
    unsigned long currentTime = millis();
    // Only print report every 5 seconds to avoid flooding
    if (currentTime - lastReportTime < 5000) {
        return;
    }
    lastReportTime = currentTime;
    
    // Heap information
    uint32_t freeHeap = getFreeHeap();
    uint32_t totalHeap = totalHeapSize;
    uint32_t usedHeap = totalHeap - freeHeap;
    uint8_t freePercentage = getFreeHeapPercentage();
    uint8_t fragmentation = getHeapFragmentation();
    
    String report = "===== MEMORY REPORT =====\n";
    report += "Heap - Total: " + String(totalHeap) + " bytes\n";
    report += "      - Used:  " + String(usedHeap) + " bytes (" + String(100 - freePercentage) + "%)\n";
    report += "      - Free:  " + String(freeHeap) + " bytes (" + String(freePercentage) + "%)\n";
    report += "      - Min Free Ever: " + String(getMinFreeHeap()) + " bytes\n";
    report += "      - Max Alloc Block: " + String(getMaxAllocHeap()) + " bytes\n";
    report += "      - Fragmentation: " + String(fragmentation) + "%\n";
    
    // PSRAM information if available
    uint32_t freePSRAM = getFreePSRAM();
    if (freePSRAM > 0 || ESP.getPsramSize() > 0) {
        report += "PSRAM - Total: " + String(ESP.getPsramSize()) + " bytes\n";
        report += "      - Free:  " + String(freePSRAM) + " bytes\n";
    } else {
        report += "PSRAM not available\n";
    }
    
    // Check stack usage for all tasks
    report += "\n----- TASK STACKS -----\n";
    checkStackUsage();
    
    REPORT_INFO(report);
}

uint32_t MemoryMonitor::getTaskFreeStack(TaskHandle_t taskHandle) {
    if (taskHandle == nullptr) {
        return 0;
    }
    UBaseType_t watermark = uxTaskGetStackHighWaterMark(taskHandle);
    return (uint32_t)(watermark * sizeof(StackType_t));
}

String MemoryMonitor::getTaskName(TaskHandle_t taskHandle) {
    if (taskHandle == nullptr) {
        return "Unknown";
    }
    
    // FreeRTOS has pcTaskGetName function for getting task name
    char* taskName = pcTaskGetName(taskHandle);
    if (taskName == nullptr) {
        return "Unknown";
    }
    return String(taskName);
}

bool MemoryMonitor::defragmentHeap() {
    // Get current free heap and fragmentation
    uint32_t initialFreeHeap = getFreeHeap();
    uint8_t initialFragmentation = getHeapFragmentation();
    
    if (initialFragmentation < 20) {
        // No need to defragment if fragmentation is low
        return true;
    }
    
    // Try to allocate a large block to consolidate memory
    // Start with half the available heap
    uint32_t blockSize = initialFreeHeap / 2;
    void* block = nullptr;
    
    // Try progressively smaller blocks until allocation succeeds
    while (blockSize > 1024 && block == nullptr) {
        block = malloc(blockSize);
        if (block == nullptr) {
            blockSize /= 2;
        }
    }
    
    // If allocation succeeded, immediately free it
    if (block != nullptr) {
        memset(block, 0, blockSize); // Touch memory to ensure allocation
        free(block);
        
        // Check if fragmentation improved
        uint8_t newFragmentation = getHeapFragmentation();
        REPORT_INFO("Heap defragmentation: " + String(initialFragmentation) + 
                   "% -> " + String(newFragmentation) + "%");
        
        return (newFragmentation < initialFragmentation);
    }
    
    REPORT_WARNING(ErrorCategory::MEMORY, "Failed to defragment heap");
    return false;
}

bool MemoryMonitor::checkStackUsage() {
    const size_t maxTasks = STACK_MONITOR_MAX_TASKS;
    TaskStatus_t taskDetails[maxTasks];
    UBaseType_t taskCount;
    uint32_t totalRuntime;
    
    // Get information about all running tasks
    taskCount = uxTaskGetSystemState(taskDetails, maxTasks, &totalRuntime);
    
    bool allStacksOk = true;
    String taskReport = "";
    
    // Analyze each task
    for (UBaseType_t i = 0; i < taskCount; i++) {
        TaskHandle_t taskHandle = taskDetails[i].xHandle;
        uint32_t freeStack = getTaskFreeStack(taskHandle);
        uint32_t stackSize = taskDetails[i].usStackHighWaterMark * sizeof(StackType_t);
        uint32_t usedStack = stackSize - freeStack;
        uint8_t usedPercentage = (usedStack * 100) / stackSize;
        
        // Add to report
        taskReport += String(taskDetails[i].pcTaskName) + ": " +
                      String(usedPercentage) + "% used (" + 
                      String(freeStack) + " bytes free)\n";
        
        // Check if stack usage is getting high
        if (usedPercentage > (100 - STACK_WARNING_THRESHOLD)) {
            REPORT_WARNING(ErrorCategory::MEMORY, 
                         "Task " + String(taskDetails[i].pcTaskName) + 
                         " stack usage high: " + String(usedPercentage) + 
                         "% used (" + String(freeStack) + " bytes free)");
            allStacksOk = false;
        }
    }
    
    REPORT_INFO(taskReport);
    return allStacksOk;
}
