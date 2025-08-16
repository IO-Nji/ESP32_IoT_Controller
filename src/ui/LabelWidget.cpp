#include "LabelWidget.h"

LabelWidget::LabelWidget(int16_t x, int16_t y, int16_t width, int16_t height,
                       const String& text, uint8_t textSize,
                       Alignment alignment, bool visible)
    : Widget(x, y, width, height, visible),
      text(text),
      textSize(textSize),
      alignment(alignment),
      textColor(1) { // Default to white (1) for monochrome displays
}

void LabelWidget::draw(Adafruit_GFX& display) {
    if (!isVisible()) {
        return;
    }
    
    // Calculate absolute coordinates if we have a parent
    int16_t absX, absY;
    getAbsolutePosition(absX, absY);
    
    // Save current text properties - Adafruit_GFX doesn't have getters, so we'll just set and not restore
    // No need to save previous values
    
    // Set text properties
    display.setTextSize(textSize);
    display.setTextColor(textColor);
    
    // Calculate text position based on alignment
    int16_t textX = absX;
    int16_t textY = absY;
    
    // Get text bounds
    int16_t x1, y1;
    uint16_t textWidth, textHeight;
    display.getTextBounds(text, 0, 0, &x1, &y1, &textWidth, &textHeight);
    
    // Adjust position based on alignment
    switch (alignment) {
        case Alignment::CENTER:
            textX = absX + (width - textWidth) / 2;
            break;
        case Alignment::RIGHT:
            textX = absX + width - textWidth;
            break;
        case Alignment::LEFT:
        default:
            // Keep default left alignment
            break;
    }
    
    // Center vertically within the height
    // We need to account for the text baseline
    textY = absY + (height - textHeight) / 2 + textHeight;
    
    // Draw the text
    display.setCursor(textX, textY);
    display.print(text);
    
    // No need to restore text properties as we don't have a way to get the previous values
    // The parent widget or next drawing operation should set its own text properties
}

void LabelWidget::setText(const String& text) {
    this->text = text;
}

const String& LabelWidget::getText() const {
    return text;
}

void LabelWidget::setAlignment(Alignment alignment) {
    this->alignment = alignment;
}

void LabelWidget::setTextSize(uint8_t size) {
    textSize = size;
}

void LabelWidget::setTextColor(uint16_t color) {
    textColor = color;
}
