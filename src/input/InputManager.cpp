#include "InputManager.h"

InputManager::InputManager(MainUI* mainUI) 
    : mainUI(mainUI), lastEncoderValue(0), lastErrorTime(0), consecutiveErrors(0) {
    // Initialize encoder position
    try {
        hal_input_set_encoder(0);
    } catch (...) {
        REPORT_ERROR(ErrorCategory::INPUT_SYSTEM, ErrorSeverity::ERROR, "Failed to initialize encoder position");
    }
}

bool InputManager::update() {
    // Check if we have a valid UI reference
    if (!mainUI) {
        REPORT_ERROR(ErrorCategory::INPUT_SYSTEM, ErrorSeverity::CRITICAL, "NULL UI reference in InputManager");
        return false;
    }
    
    bool success = true;
    
    try {
        // Process all input types
        processEncoderInput();
        processButtonInput();
        processEncoderButtonInput();
        // processKeypadInput(); // To be implemented
        
        // Reset error counter on successful update
        if (consecutiveErrors > 0) {
            consecutiveErrors = 0;
            REPORT_INFO("Input processing recovered after errors");
        }
    } 
    catch (const std::exception& e) {
        success = false;
        consecutiveErrors++;
        
        // Only report errors periodically to avoid flooding
        unsigned long currentTime = millis();
        if (currentTime - lastErrorTime > 5000) { // Report every 5 seconds
            REPORT_ERROR(ErrorCategory::INPUT_SYSTEM, ErrorSeverity::ERROR, 
                       String("Exception in input processing: ") + String(e.what()) + 
                       " (consecutive errors: " + String(consecutiveErrors) + ")");
            lastErrorTime = currentTime;
        }
    }
    catch (...) {
        success = false;
        consecutiveErrors++;
        
        // Only report errors periodically
        unsigned long currentTime = millis();
        if (currentTime - lastErrorTime > 5000) { // Report every 5 seconds
            REPORT_ERROR(ErrorCategory::INPUT_SYSTEM, ErrorSeverity::ERROR, 
                       String("Unknown exception in input processing") + 
                       " (consecutive errors: " + String(consecutiveErrors) + ")");
            lastErrorTime = currentTime;
        }
    }
    
    // Check for critical error condition - too many consecutive failures
    if (consecutiveErrors > 10) {
        REPORT_CRITICAL(ErrorCategory::INPUT_SYSTEM, "Input system failed with 10+ consecutive errors");
    }
    
    return success;
}

void InputManager::processEncoderInput() {
    try {
        // Read current encoder value
        long encoderValue = hal_input_read_encoder();
        
        // Process navigation if value changed
        if (encoderValue != lastEncoderValue) {
            mainUI->handleEncoderNavigation(encoderValue, lastEncoderValue);
            lastEncoderValue = encoderValue;
        }
    }
    catch (...) {
        REPORT_ERROR(ErrorCategory::INPUT_SYSTEM, ErrorSeverity::ERROR, "Failed to process encoder input");
        throw; // Re-throw to be caught by the main update method
    }
}

void InputManager::processButtonInput() {
    // Check Home button (button 1)
    if (hal_input_read_button(1)) {
        mainUI->handleButtonPress(1);
        delay(200); // Debounce
    }
    
    // Check Back button (button 2)
    if (hal_input_read_button(2)) {
        mainUI->handleButtonPress(2);
        delay(200); // Debounce
    }
}

void InputManager::processEncoderButtonInput() {
    // Handle encoder button press
    if (hal_input_read_encoder_btn()) {
        int activeScreenId = mainUI->getUIManager()->getActiveScreenId(1);
        MenuWidget* activeMenu = mainUI->getMainMenu();
        
        // Get the active menu based on screen
        if (activeScreenId == UI::APPS_MENU) {
            activeMenu = mainUI->getAppsMenu();
        } else if (activeScreenId == UI::NETWORK_MENU) {
            activeMenu = mainUI->getNetworkMenu();
        }
        
        // Execute selection
        activeMenu->selectCurrentItem();
        delay(200); // Debounce
    }
}

// To be implemented later
void InputManager::processKeypadInput() {
    // Process matrix keypad input when available
}
