#include "ButtonLabelWidget.h"
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "../hal/hal_config.h"

ButtonLabelWidget::ButtonLabelWidget(int16_t x, int16_t y, int16_t width, int16_t height,
                             const String& text, uint8_t textSize, bool visible)
    : BaseWidget(x, y, width, height, visible),
      text(text),
      textSize(textSize),
      textColor(SSD1306_WHITE) {
}

void ButtonLabelWidget::draw(Adafruit_GFX& display) {
    if (!isVisible()) {
        return;
    }
    
    // Calculate absolute coordinates
    int16_t absX, absY;
    getAbsolutePosition(absX, absY);
    
    // Draw border around the button
    display.drawRect(absX, absY, width, height, SSD1306_WHITE);
    
    // Set text properties
    display.setTextSize(textSize);
    display.setTextColor(textColor);
    
    // Calculate text position to center in the button
    int16_t x1, y1;
    uint16_t textWidth, textHeight;
    display.getTextBounds(text, 0, 0, &x1, &y1, &textWidth, &textHeight);
    
    // Center text horizontally in the button
    int16_t textX = absX + (width - textWidth) / 2;
    
    // Fixed positioning from bottom of display 
    // For 64px display, SCREEN_HEIGHT_DISP2 - 11 = 53
    int16_t textY = SCREEN_HEIGHT_DISP2 - 11; // Fixed position from bottom
    
    // Draw the text
    display.setCursor(textX, textY);
    display.print(text);
}

void ButtonLabelWidget::setText(const String& text) {
    this->text = text;
}

const String& ButtonLabelWidget::getText() const {
    return text;
}

void ButtonLabelWidget::setTextSize(uint8_t size) {
    textSize = size;
}
