#include "InputManager.h"

InputManager::InputManager(MainUI* mainUI) 
    : mainUI(mainUI), lastEncoderValue(0) {
    // Initialize encoder position
    hal_input_set_encoder(0);
}

void InputManager::update() {
    // Process all input types
    processEncoderInput();
    processButtonInput();
    processEncoderButtonInput();
    // processKeypadInput(); // To be implemented
}

void InputManager::processEncoderInput() {
    // Read current encoder value
    long encoderValue = hal_input_read_encoder();
    
    // Process navigation if value changed
    if (encoderValue != lastEncoderValue) {
        mainUI->handleEncoderNavigation(encoderValue, lastEncoderValue);
        lastEncoderValue = encoderValue;
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
