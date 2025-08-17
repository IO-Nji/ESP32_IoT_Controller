#ifndef UI_MANAGER_H
#define UI_MANAGER_H

#include <Arduino.h>
#include <cstdint>
#include <Adafruit_SSD1306.h>
#include <map>
#include <memory>
#include "screens/Screen.h"

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
     * @brief Get a screen by ID for a specific display
     * 
     * @param displayIndex Display index (0 for display1, 1 for display2)
     * @param screenId Screen identifier to retrieve
     * @return Screen* Pointer to the screen or nullptr if not found
     */
    Screen* getScreen(uint8_t displayIndex, uint8_t screenId);

    /**
     * @brief Update all active screens
     * 
     * @param deltaTime Time elapsed since last update (milliseconds)
     * @return true if update succeeded
     */
    bool update(unsigned long deltaTime);

    /**
     * @brief Render all active screens to their respective displays
     */
    void render();

    /**
     * @brief Handle input events for a specific display
     * 
     * @param displayIndex Display index
     * @param eventType Type of input event
     * @param eventData Event-specific data
     * @return true if input was handled
     */
    bool handleInput(uint8_t displayIndex, uint8_t eventType, int32_t eventData);

    /**
     * @brief Save the state of the active screen for a display
     * 
     * @param displayIndex Display index
     */
    void saveActiveScreenState(uint8_t displayIndex);

    /**
     * @brief Restore the state of a screen for a display
     * 
     * @param displayIndex Display index
     * @param screenId Screen identifier
     */
    void restoreScreenState(uint8_t displayIndex, uint8_t screenId);

    /**
     * @brief Add a custom screen to a display
     * 
     * @param displayIndex Display index
     * @param screenId Screen identifier
     * @param screen Pointer to the screen object
     */
    void addScreen(uint8_t displayIndex, uint8_t screenId, Screen* screen);

    /**
     * @brief Get the display object for a given index
     * 
     * @param displayIndex Display index
     * @return Adafruit_SSD1306& Reference to the display object
     */
    Adafruit_SSD1306& getDisplay(uint8_t displayIndex);

private:
    Adafruit_SSD1306& display1;
    Adafruit_SSD1306& display2;
    uint8_t display1Width;
    uint8_t display1Height;
    uint8_t display2Width;
    uint8_t display2Height;

    // Active screen IDs for each display
    uint8_t activeScreen1;
    uint8_t activeScreen2;

    // Maps of screenId to Screen object for each display
    std::map<uint8_t, std::unique_ptr<Screen>> screens1;
    std::map<uint8_t, std::unique_ptr<Screen>> screens2;
};

#endif // UI_MANAGER_H

