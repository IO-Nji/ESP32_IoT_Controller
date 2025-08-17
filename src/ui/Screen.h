#ifndef SCREEN_H
#define SCREEN_H

#include <vector>
#include "Widget.h"
#include <Adafruit_SSD1306.h>

/**
 * @brief Screen manager class for organizing widgets on a display
 * 
 * This class manages a collection of widgets and handles their rendering
 * and input processing on a specific display. It supports different display
 * sizes and orientations.
 */
class Screen {
public:
    /**
     * @brief Construct a new Screen object
     * 
     * @param display Reference to the display device
     * @param width Display width in pixels
     * @param height Display height in pixels
     */
    Screen(Adafruit_SSD1306& display, uint16_t width, uint16_t height);
    
    /**
     * @brief Add a widget to this screen
     * 
     * @param widget Pointer to widget to add
     */
    void addWidget(Widget* widget);
    
    /**
     * @brief Remove a widget from this screen
     * 
     * @param widget Pointer to widget to remove
     * @return true if widget was found and removed
     * @return false if widget was not found
     */
    bool removeWidget(Widget* widget);
    
    /**
     * @brief Draw all visible widgets to the display
     * 
     * Clears the display, renders all widgets, and calls display()
     * to update the physical screen.
     */
    void render();
    
    /**
     * @brief Update all widgets
     * 
     * @param deltaTime Time elapsed since last update (milliseconds)
     * @return true if update succeeded
     * @return false if update failed
     */
    bool update(unsigned long deltaTime);
    
    /**
     * @brief Process input events and forward to widgets
     * 
     * @param eventType Type of input event
     * @param eventData Additional event data
     * @return true if any widget handled the event
     * @return false if no widget handled the event
     */
    bool handleInput(uint8_t eventType, int32_t eventData);
    
    /**
     * @brief Get the list of widgets on this screen
     * 
     * @return const std::vector<Widget*>& Reference to widget collection
     */
    const std::vector<Widget*>& getWidgets() const { return widgets; }

private:
    Adafruit_SSD1306& display;    ///< Reference to the display device
    uint16_t width;               ///< Screen width
    uint16_t height;              ///< Screen height
    std::vector<Widget*> widgets; ///< Collection of widgets on this screen
};

#endif // SCREEN_H
