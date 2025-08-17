#ifndef ERROR_HANDLER_H
#define ERROR_HANDLER_H

#include <Arduino.h>

/**
 * @brief Error categories to help organize and classify errors
 */
enum class ErrorCategory {
    UI,             // UI-related errors
    INPUT_SYSTEM,   // Input system errors (renamed from INPUT to avoid conflict with Arduino's INPUT macro)
    HARDWARE,       // Hardware/HAL errors
    TASK,           // Task management errors
    MEMORY,         // Memory-related errors
    COMMUNICATION,  // Communication errors (I2C, SPI, etc.)
    SYSTEM,         // General system errors
    SENSOR,         // Sensor-related errors
};

/**
 * @brief Error severity levels
 */
enum class ErrorSeverity {
    INFO,       // Informational, non-error events
    WARNING,    // Non-critical issues that should be noted
    ERROR,      // Errors that affect functionality but aren't fatal
    CRITICAL    // Critical errors that require immediate attention
};

/**
 * @brief Error Handler class for centralized error management
 * 
 * This class provides methods for reporting, logging, and responding to errors
 * throughout the system. It can be used both to report technical errors and
 * to communicate issues to the user via the UI when appropriate.
 */
class ErrorHandler {
public:
    /**
     * @brief Initialize the error handling system
     * 
     * @param enableLogging Whether to enable error logging to Serial
     * @param enableDisplay Whether to display errors on screen
     */
    static void init(bool enableLogging = true, bool enableDisplay = true);
    
    /**
     * @brief Report an error in the system
     * 
     * @param category Error category
     * @param severity Error severity
     * @param message Error message
     * @param location Source file and line number where error occurred
     * @return uint32_t Error ID that can be used for reference
     */
    static uint32_t reportError(
        ErrorCategory category, 
        ErrorSeverity severity, 
        const String& message,
        const String& location = ""
    );
    
    /**
     * @brief Display an error message on screen
     * 
     * @param message Error message to display
     * @param severity Error severity
     * @param autoDismiss Whether the message should be dismissed automatically after a timeout
     */
    static void displayError(
        const String& message,
        ErrorSeverity severity = ErrorSeverity::ERROR,
        bool autoDismiss = true
    );
    
    /**
     * @brief Get the most recent error message
     * 
     * @return String The most recent error message
     */
    static String getLastErrorMessage();
    
    /**
     * @brief Clear the current error state
     */
    static void clearError();
    
    /**
     * @brief Check if there is an active error
     * 
     * @return true if an error is active
     * @return false if no error is active
     */
    static bool hasActiveError();
    
private:
    static bool loggingEnabled;
    static bool displayEnabled;
    static uint32_t errorCount;
    static String lastErrorMessage;
    static bool errorActive;
    static unsigned long lastErrorTime;
    
    // Convert enum values to strings for readable logging
    static String categoryToString(ErrorCategory category);
    static String severityToString(ErrorSeverity severity);
};

// Convenience macros to simplify error reporting
#define REPORT_ERROR(category, severity, message) \
    ErrorHandler::reportError(category, severity, message, __FILE__ ":" + String(__LINE__))

#define REPORT_INFO(message) \
    ErrorHandler::reportError(ErrorCategory::SYSTEM, ErrorSeverity::INFO, message, __FILE__ ":" + String(__LINE__))

#define REPORT_WARNING(category, message) \
    ErrorHandler::reportError(category, ErrorSeverity::WARNING, message, __FILE__ ":" + String(__LINE__))

#define REPORT_CRITICAL(category, message) \
    ErrorHandler::reportError(category, ErrorSeverity::CRITICAL, message, __FILE__ ":" + String(__LINE__))

#endif // ERROR_HANDLER_H
