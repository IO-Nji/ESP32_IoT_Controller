#include "ErrorHandler.h"

// Initialize static variables
bool ErrorHandler::loggingEnabled = true;
bool ErrorHandler::displayEnabled = true;
uint32_t ErrorHandler::errorCount = 0;
String ErrorHandler::lastErrorMessage = "";
bool ErrorHandler::errorActive = false;
unsigned long ErrorHandler::lastErrorTime = 0;

void ErrorHandler::init(bool enableLogging, bool enableDisplay) {
    loggingEnabled = enableLogging;
    displayEnabled = enableDisplay;
    errorCount = 0;
    errorActive = false;
    lastErrorMessage = "";
    
    if (loggingEnabled) {
        Serial.println("Error handling system initialized");
    }
}

uint32_t ErrorHandler::reportError(
    ErrorCategory category, 
    ErrorSeverity severity, 
    const String& message,
    const String& location
) {
    // Generate error ID
    errorCount++;
    uint32_t errorId = errorCount;
    
    // Construct formatted error message
    String timestamp = String(millis());
    String formattedMessage = 
        "[" + timestamp + "] " +
        severityToString(severity) + " " +
        categoryToString(category) + ": " +
        message;
    
    // Add location info if provided
    if (location.length() > 0) {
        formattedMessage += " (at " + location + ")";
    }
    
    // Log to serial if enabled
    if (loggingEnabled) {
        Serial.println(formattedMessage);
        Serial.flush();
    }
    
    // Store last error message
    lastErrorMessage = formattedMessage;
    errorActive = true;
    lastErrorTime = millis();
    
    // Display error on screen if enabled and severe enough
    if (displayEnabled && severity >= ErrorSeverity::ERROR) {
        displayError(message, severity);
    }
    
    // For critical errors, take additional action
    if (severity == ErrorSeverity::CRITICAL) {
        // Blink onboard LED or activate buzzer to indicate critical error
        // This would need to be implemented based on your hardware
        
        // Consider system restart for unrecoverable errors
        // ESP.restart();
    }
    
    return errorId;
}

void ErrorHandler::displayError(
    const String& message,
    ErrorSeverity severity,
    bool autoDismiss
) {
    // In a real implementation, this would display the error on screen
    // For now, we just log that we would have displayed it
    if (loggingEnabled) {
        Serial.println("DISPLAY: " + message);
    }
    
    // The actual implementation would:
    // 1. Show a message box or notification on the display
    // 2. Use color/icon based on severity
    // 3. Set a timer for auto-dismissal if requested
}

String ErrorHandler::getLastErrorMessage() {
    return lastErrorMessage;
}

void ErrorHandler::clearError() {
    errorActive = false;
}

bool ErrorHandler::hasActiveError() {
    return errorActive;
}

String ErrorHandler::categoryToString(ErrorCategory category) {
    switch (category) {
        case ErrorCategory::UI: return "UI";
        case ErrorCategory::INPUT_SYSTEM: return "INPUT";
        case ErrorCategory::HARDWARE: return "HARDWARE";
        case ErrorCategory::TASK: return "TASK";
        case ErrorCategory::MEMORY: return "MEMORY";
        case ErrorCategory::COMMUNICATION: return "COMM";
        case ErrorCategory::SYSTEM: return "SYSTEM";
        case ErrorCategory::SENSOR: return "SENSOR";
        default: return "UNKNOWN";
    }
}

String ErrorHandler::severityToString(ErrorSeverity severity) {
    switch (severity) {
        case ErrorSeverity::INFO: return "INFO";
        case ErrorSeverity::WARNING: return "WARN";
        case ErrorSeverity::ERROR: return "ERROR";
        case ErrorSeverity::CRITICAL: return "CRIT";
        default: return "UNKNOWN";
    }
}
