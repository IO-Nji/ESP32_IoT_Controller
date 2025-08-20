#ifndef BASE_WIDGET_H
#define BASE_WIDGET_H

#include <Arduino.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>
#include <ArduinoJson.h>

/**
 * @brief Base widget class for the UI framework
 * 
 * This class provides the foundation for all UI elements in the system.
 * It handles positioning, sizing, and basic rendering functions.
 * Specific widget types should inherit from this class and override the draw method.
 */
class BaseWidget {
public:
    /**
     * @brief Construct a new Widget object
     * 
     * @param x X position of the widget
     * @param y Y position of the widget
     * @param width Width of the widget
     * @param height Height of the widget
     * @param visible Initial visibility state
     */
    BaseWidget(int16_t x, int16_t y, int16_t width, int16_t height, bool visible = true);
    
    virtual ~BaseWidget() = default;
    
    /**
     * @brief Draw the widget on the specified display
     * 
     * This method must be implemented by derived classes to render
     * the widget on the given display device.
     * 
     * @param display The display to render on
     */
    virtual void draw(Adafruit_GFX& display) = 0;
    
    /**
     * @brief Update widget state (animations, values, etc.)
     * 
     * Called every frame to update the widget's internal state.
     * The base implementation does nothing; subclasses can override.
     * 
     * @param deltaTime Time elapsed since last update (milliseconds)
     * @return true if update succeeded, false if it failed
     */
    virtual bool update(unsigned long deltaTime);
    
    /**
     * @brief Process input events
     * 
     * Override in derived classes to handle input events.
     * Return true if the event was handled, false otherwise.
     * 
     * @param eventType Type of input event
     * @param eventData Additional event data (depends on event type)
    * @return true if event was handled
    * @return false if event was not handled
    */
    virtual bool handleInput(uint8_t eventType, int32_t eventData);

    /**
    * @brief Save widget state to JsonDocument (override in derived classes)
    */
    virtual void saveState(JsonDocument& state) {}

    /**
    * @brief Load widget state from JsonDocument (override in derived classes)
    */
    virtual void loadState(const JsonDocument& state) {}
    
    /**
     * @brief Set the visibility of the widget
     * 
     * @param visible True to show, false to hide
     */
    void setVisible(bool visible);
    
    /**
     * @brief Check if the widget is currently visible
     * 
     * @return true if visible
     * @return false if hidden
     */
    bool isVisible() const;
    
    /**
     * @brief Set the position of the widget
     * 
     * @param x New X position
     * @param y New Y position
     */
    void setPosition(int16_t x, int16_t y);
    
    /**
     * @brief Set the size of the widget
     * 
     * @param width New width
     * @param height New height
     */
    void setSize(int16_t width, int16_t height);
    
    /**
     * @brief Check if a point is inside the widget bounds
     * 
     * @param x X coordinate to check
     * @param y Y coordinate to check
     * @return true if the point is inside the widget
     * @return false if the point is outside the widget
     */
    bool contains(int16_t x, int16_t y) const;
    
    /**
     * @brief Get the widget's bounding rectangle
     * 
     * @param outX Output parameter for X position
     * @param outY Output parameter for Y position
     * @param outWidth Output parameter for width
     * @param outHeight Output parameter for height
     */
    void getBounds(int16_t& outX, int16_t& outY, int16_t& outWidth, int16_t& outHeight) const;

    /**
     * @brief Set the parent widget
     * 
     * Used for hierarchical widget organization
     * 
     * @param parent Pointer to parent widget
     */
    void setParent(BaseWidget* parent);
    
    /**
     * @brief Get the parent widget
     * 
     * @return Widget* Pointer to parent widget, or nullptr if no parent
     */
    BaseWidget* getParent() const;
    
    /**
     * @brief Get X position
     * 
     * @return int16_t X position relative to parent
     */
    int16_t getX() const { return x; }
    
    /**
     * @brief Get Y position
     * 
     * @return int16_t Y position relative to parent
     */
    int16_t getY() const { return y; }
    
    /**
     * @brief Get widget width
     * 
     * @return int16_t Width in pixels
     */
    int16_t getWidth() const { return width; }
    
    /**
     * @brief Get widget height
     * 
     * @return int16_t Height in pixels
     */
    int16_t getHeight() const { return height; }
    
    /**
     * @brief Calculate absolute position (including parent offsets)
     * 
     * @param absX Reference to store absolute X position
     * @param absY Reference to store absolute Y position
     */
    void getAbsolutePosition(int16_t& absX, int16_t& absY) const {
        absX = x;
        absY = y;
        const BaseWidget* currentParent = parent;
        while (currentParent) {
            absX += currentParent->x;
            absY += currentParent->y;
            currentParent = currentParent->parent;
        }
    }

protected:
    int16_t x;            ///< X position relative to parent
    int16_t y;            ///< Y position relative to parent
    int16_t width;        ///< Widget width
    int16_t height;       ///< Widget height
    bool visible;         ///< Visibility flag
    BaseWidget* parent;       ///< Parent widget, or nullptr if root widget
};

#endif // BASE_WIDGET_H
