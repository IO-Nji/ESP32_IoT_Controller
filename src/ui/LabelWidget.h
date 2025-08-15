#ifndef LABEL_WIDGET_H
#define LABEL_WIDGET_H

#include "Widget.h"

/**
 * @brief A widget for displaying text
 * 
 * This widget renders a text string with configurable font and alignment options.
 */
class LabelWidget : public Widget {
public:
    /**
     * @brief Text alignment options
     */
    enum class Alignment {
        LEFT,
        CENTER,
        RIGHT
    };
    
    /**
     * @brief Construct a new Label Widget object
     * 
     * @param x X position of the label
     * @param y Y position of the label
     * @param width Width of the label area
     * @param height Height of the label area
     * @param text Initial text content
     * @param textSize Font size multiplier (1 is default)
     * @param alignment Text alignment within the label bounds
     * @param visible Initial visibility
     */
    LabelWidget(int16_t x, int16_t y, int16_t width, int16_t height, 
               const String& text, 
               uint8_t textSize = 1,
               Alignment alignment = Alignment::LEFT,
               bool visible = true);
    
    /**
     * @brief Draw the label on the specified display
     * 
     * @param display The display to render on
     */
    virtual void draw(Adafruit_GFX& display) override;
    
    /**
     * @brief Set the text content of the label
     * 
     * @param text New text content
     */
    void setText(const String& text);
    
    /**
     * @brief Get the current text content
     * 
     * @return const String& Current text
     */
    const String& getText() const;
    
    /**
     * @brief Set the text alignment
     * 
     * @param alignment New alignment option
     */
    void setAlignment(Alignment alignment);
    
    /**
     * @brief Set the text size
     * 
     * @param size Font size multiplier (1-5 recommended)
     */
    void setTextSize(uint8_t size);
    
    /**
     * @brief Set the text color
     * 
     * @param color Text color (1 for white, 0 for black in monochrome displays)
     */
    void setTextColor(uint16_t color);

private:
    String text;             ///< Text content
    uint8_t textSize;        ///< Font size multiplier
    Alignment alignment;     ///< Text alignment
    uint16_t textColor;      ///< Text color
};

#endif // LABEL_WIDGET_H
