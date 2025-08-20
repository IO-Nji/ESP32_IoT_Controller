#include "ButtonWidget.h"

ButtonWidget::ButtonWidget(int16_t x, int16_t y, int16_t width, int16_t height,
                         const String& label, bool visible)
    : BaseWidget(x, y, width, height, visible),
      label(label),
      state(State::NORMAL),
      onPressCallback(nullptr) {
}

void ButtonWidget::draw(Adafruit_GFX& display) {
    if (!isVisible()) {
        return;
    }
    
    // Calculate absolute coordinates if we have a parent
    int16_t absX, absY;
    getAbsolutePosition(absX, absY);
    
    // Draw button based on state
    switch (state) {
        case State::PRESSED:
            // Inverted colors for pressed state
            display.fillRect(absX, absY, width, height, 1); // White fill
            display.setTextColor(0); // Black text
            break;
            
        case State::INACTIVE:
            // Draw border with dashed line for disabled state
            for (int16_t i = 0; i < width; i += 2) {
                display.drawPixel(absX + i, absY, 1);
                display.drawPixel(absX + i, absY + height - 1, 1);
            }
            for (int16_t i = 0; i < height; i += 2) {
                display.drawPixel(absX, absY + i, 1);
                display.drawPixel(absX + width - 1, absY + i, 1);
            }
            display.setTextColor(1); // White text
            break;
            
        case State::NORMAL:
        default:
            // Draw border
            display.drawRect(absX, absY, width, height, 1);
            display.setTextColor(1); // White text
            break;
    }
    
    // Calculate text position (centered)
    int16_t x1, y1;
    uint16_t textWidth, textHeight;
    display.getTextBounds(label, 0, 0, &x1, &y1, &textWidth, &textHeight);
    
    int16_t textX = absX + (width - textWidth) / 2;
    int16_t textY = absY + (height - textHeight) / 2 + textHeight; // Add textHeight to align with baseline
    
    // Draw the label
    display.setCursor(textX, textY);
    display.print(label);
    
    // Reset text color to default
    display.setTextColor(1);
}

bool ButtonWidget::handleInput(uint8_t eventType, int32_t eventData) {
    // Only process events if button is enabled
    if (state == State::INACTIVE || !isVisible()) {
        return false;
    }
    
    // Check if event is a press or release
    if (eventType == 1) { // Press event (define appropriate constants)
        // Check if press coordinates are within button bounds
        int16_t pressX = eventData & 0xFFFF;         // Lower 16 bits for X
        int16_t pressY = (eventData >> 16) & 0xFFFF; // Upper 16 bits for Y
        
        if (contains(pressX, pressY)) {
            setState(State::PRESSED);
            return true;
        }
    } else if (eventType == 2) { // Release event
        if (state == State::PRESSED) {
            // Return to normal state
            setState(State::NORMAL);
            
            // Execute callback if set
            if (onPressCallback) {
                onPressCallback(this);
            }
            
            return true;
        }
    }
    
    return false;
}

void ButtonWidget::setLabel(const String& label) {
    this->label = label;
}

const String& ButtonWidget::getLabel() const {
    return label;
}

void ButtonWidget::setState(State state) {
    this->state = state;
}

ButtonWidget::State ButtonWidget::getState() const {
    return state;
}

void ButtonWidget::setOnPressCallback(void (*callback)(ButtonWidget*)) {
    onPressCallback = callback;
}

void ButtonWidget::simulatePress() {
    // First set to pressed state
    setState(State::PRESSED);
    
    // Then trigger the callback and return to normal state
    if (onPressCallback) {
        onPressCallback(this);
    }
    
    setState(State::NORMAL);
}
