#ifndef UI_MANAGER_H
#define UI_MANAGER_H

#include <Arduino.h>
#include <Adafruit_SSD1306.h>
#include <map>
#include <memory>
#include "Screen.h"

/**
 * @brief UI Manager for multiple displays and screens
 * 
 * This class manages multiple displays and screen transitions.
 * It handles display-specific configurations and input routing.
 */
class UIManager {
public:
    /**
     * @brief Initialize the UI Manager
     * 
     * @param display1 First display (usually the 128x32 vertical OLED)
     * @param display2 Second display (usually the 128x64 horizontal OLED)
     */
    UIManager(Adafruit_SSD1306& display1, Adafruit_SSD1306& display2);
    
    /**
     * @brief Create a new screen for a specific display
     * 
     * @param displayIndex Display index (0 for display1, 1 for display2)
     * @param screenId Unique identifier for the screen
     * @return Screen* Pointer to the newly created screen
     */
    Screen* createScreen(uint8_t displayIndex, uint8_t screenId);
    
    /**
     * @brief Set the active screen for a display
     * 
     * @param displayIndex Display index (0 for display1, 1 for display2)
     * @param screenId Screen identifier to activate
     * @return true if screen was found and activated
     * @return false if screen was not found
     */
    bool setActiveScreen(uint8_t displayIndex, uint8_t screenId);
    
    /**
     * @brief Get the active screen for a display
     * 
     * @param displayIndex Display index (0 for display1, 1 for display2)
     * @return Screen* Pointer to active screen or nullptr if none
     */
    Screen* getActiveScreen(uint8_t displayIndex);
    
    /**
     * @brief Get the active screen ID for a display
     * 
     * @param displayIndex Display index (0 for display1, 1 for display2)
     * @return uint8_t ID of the active screen
     */
    uint8_t getActiveScreenId(uint8_t displayIndex);
    
    /**
     * @brief Update all active screens
     * 
     * @param deltaTime Time elapsed since last update (milliseconds)
     */
    void update(unsigned long deltaTime);
    
    /**
     * @brief Render all active screens to their displays
     */
    void render();
    
    /**
     * @brief Process input events and route to appropriate screen
     * 
     * @param displayIndex Display index to receive input (0 for display1, 1 for display2)
     * @param eventType Type of input event
     * @param eventData Additional event data
     * @return true if event was handled
     * @return false if event was not handled
     */
    bool handleInput(uint8_t displayIndex, uint8_t eventType, int32_t eventData);

private:
    Adafruit_SSD1306& display1;  ///< Reference to first display
    Adafruit_SSD1306& display2;  ///< Reference to second display
    
    uint16_t display1Width;      ///< Width of first display
    uint16_t display1Height;     ///< Height of first display
    uint16_t display2Width;      ///< Width of second display
    uint16_t display2Height;     ///< Height of second display
    
    // Map of screens for each display (key = screenId)
    std::map<uint8_t, std::unique_ptr<Screen>> screens1;
    std::map<uint8_t, std::unique_ptr<Screen>> screens2;
    
    // Currently active screen IDs
    uint8_t activeScreen1;
    uint8_t activeScreen2;
};

#endif // UI_MANAGER_H
