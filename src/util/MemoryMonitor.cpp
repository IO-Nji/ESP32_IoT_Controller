#include "MemoryMonitor.h"

// Initialize static variables
uint8_t MemoryMonitor::criticalThreshold = 10;
uint8_t MemoryMonitor::warningThreshold = 20;
uint32_t MemoryMonitor::totalHeapSize = 0;

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
