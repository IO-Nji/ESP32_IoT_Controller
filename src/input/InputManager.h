#ifndef INPUT_MANAGER_H
#define INPUT_MANAGER_H

#include <Arduino.h>
#include "../ui/main/MainUI.h"
#include "../hal/input.h"
#include "../util/ErrorHandler.h"

class InputManager {
public:
    /**
     * @brief Constructor for InputManager
     * 
     * @param mainUI Pointer to MainUI instance
     */
    InputManager(MainUI* mainUI);
    
    /**
     * @brief Update input state and process inputs
     * 
     * @return true if inputs were processed successfully
     * @return false if an error occurred
     */
    bool update();
    
private:
    MainUI* mainUI;
    long lastEncoderValue;
    unsigned long lastErrorTime;
    uint8_t consecutiveErrors;
    
    void processEncoderInput();
    void processButtonInput();
    void processEncoderButtonInput();
    void processKeypadInput();
};

#endif // INPUT_MANAGER_H
