#ifndef BUTTON_WIDGET_H
#define BUTTON_WIDGET_H

#include "Widget.h"

/**
 * @brief A button widget that can be pressed
 * 
 * This widget draws a button with configurable border, text, and state.
 * It can detect and respond to press events.
 */
class ButtonWidget : public Widget {
public:
    /**
     * @brief Button state enumeration
     */
    enum class State {
        NORMAL,     ///< Default state
        PRESSED,    ///< Button is currently pressed
        INACTIVE    ///< Button is disabled and cannot be pressed
    };
    
    /**
     * @brief Construct a new Button Widget
     * 
     * @param x X position
     * @param y Y position
     * @param width Width of button
     * @param height Height of button
     * @param label Text label for button
     * @param visible Initial visibility
     */
    ButtonWidget(int16_t x, int16_t y, int16_t width, int16_t height, 
                const String& label, bool visible = true);
                
    /**
     * @brief Draw the button on the specified display
     * 
     * @param display The display to render on
     */
    virtual void draw(Adafruit_GFX& display) override;
    
    /**
     * @brief Handle input events
     * 
     * Process button press events
     * 
     * @param eventType Type of input event
     * @param eventData Additional event data
     * @return true if event was handled
     * @return false if event was not handled
     */
    virtual bool handleInput(uint8_t eventType, int32_t eventData) override;
    
    /**
     * @brief Set the button label text
     * 
     * @param label New label text
     */
    void setLabel(const String& label);
    
    /**
     * @brief Get the current button label
     * 
     * @return const String& Current label text
     */
    const String& getLabel() const;
    
    /**
     * @brief Set the button state
     * 
     * @param state New state
     */
    void setState(State state);
    
    /**
     * @brief Get the current button state
     * 
     * @return State Current state
     */
    State getState() const;
    
    /**
     * @brief Set callback function for button press
     * 
     * @param callback Function to call when button is pressed
     */
    void setOnPressCallback(void (*callback)(ButtonWidget*));

    /**
     * @brief Simulate a button press from code
     * 
     * This method allows triggering button press programmatically
     */
    void simulatePress();

private:
    String label;                          ///< Button label text
    State state;                           ///< Current button state
    void (*onPressCallback)(ButtonWidget*); ///< Callback function for press events
};

#endif // BUTTON_WIDGET_H
