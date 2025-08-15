#ifndef UI_EXAMPLE_H
#define UI_EXAMPLE_H

#include <Arduino.h>
#include <Adafruit_SSD1306.h>
#include "UIFramework.h"

/**
 * @brief Example usage of the UI Framework
 * 
 * This class demonstrates how to set up and use the UI Framework
 * with the two displays in the ESP32 IoT Controller.
 */
class UIExample {
public:
    /**
     * @brief Initialize the UI example
     * 
     * @param display1 First display (128x32 vertical)
     * @param display2 Second display (128x64 horizontal)
     */
    UIExample(Adafruit_SSD1306& display1, Adafruit_SSD1306& display2);
    
    /**
     * @brief Set up example screens and widgets
     */
    void setup();
    
    /**
     * @brief Update the UI (call in loop)
     */
    void update();
    
    /**
     * @brief Process input events
     * 
     * @param displayIndex Display index (0 or 1)
     * @param eventType Type of input event
     * @param eventData Additional event data
     */
    void handleInput(uint8_t displayIndex, uint8_t eventType, int32_t eventData);

private:
    // UI Manager and components
    UIManager uiManager;
    
    // Example callbacks
    static void onButton1Pressed();
    static void onButton2Pressed();
    
    // Keep track of some dynamic data for the example
    static unsigned long lastUpdateTime;
    static int counterValue;
};

#endif // UI_EXAMPLE_H
