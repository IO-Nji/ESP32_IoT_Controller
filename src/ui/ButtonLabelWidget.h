#ifndef BUTTON_LABEL_WIDGET_H
#define BUTTON_LABEL_WIDGET_H

#include "Widget.h"
#include "../hal/hal_config.h"

/**
 * @brief A widget for displaying a boxed button label
 * 
 * This widget displays text inside a border to indicate a button function.
 */
class ButtonLabelWidget : public Widget {
public:
    /**
     * @brief Construct a new Button Label Widget
     * 
     * @param x X position
     * @param y Y position
     * @param width Width of the widget
     * @param height Height of the widget
     * @param text Text to display
     * @param textSize Text size multiplier
     * @param visible Visibility flag
     */
    ButtonLabelWidget(int16_t x, int16_t y, int16_t width, int16_t height,
                 const String& text, uint8_t textSize = 1, bool visible = true);
    
    /**
     * @brief Draw the button label widget on the display
     * 
     * @param display Display to draw on
     */
    virtual void draw(Adafruit_GFX& display) override;
    
    /**
     * @brief Set the button text
     * 
     * @param text New text
     */
    void setText(const String& text);
    
    /**
     * @brief Get the button text
     * 
     * @return const String& Current text
     */
    const String& getText() const;
    
    /**
     * @brief Set the text size
     * 
     * @param size Text size multiplier
     */
    void setTextSize(uint8_t size);

private:
    String text;                  ///< Button text
    uint8_t textSize;             ///< Text size multiplier
    uint16_t textColor;           ///< Text color (1 for white on monochrome displays)
};

#endif // BUTTON_LABEL_WIDGET_H
