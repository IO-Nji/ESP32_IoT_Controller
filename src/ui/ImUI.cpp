#include "ImUI.h"

bool ImUI::button(Adafruit_GFX& display, int16_t x, int16_t y, 
                int16_t w, int16_t h, const char* label, 
                bool isPressed, bool isEnabled) {
    // Draw button frame
    if (isEnabled) {
        if (isPressed) {
            // Filled button for pressed state
            display.fillRoundRect(x, y, w, h, 2, SSD1306_WHITE);
            display.setTextColor(SSD1306_BLACK);
        } else {
            // Outlined button for normal state
            display.drawRoundRect(x, y, w, h, 2, SSD1306_WHITE);
            display.setTextColor(SSD1306_WHITE);
        }
    } else {
        // Dashed outline for disabled state
        for (int16_t i = 0; i < w; i += 2) {
            display.drawPixel(x + i, y, SSD1306_WHITE);
            display.drawPixel(x + i, y + h - 1, SSD1306_WHITE);
        }
        for (int16_t i = 0; i < h; i += 2) {
            display.drawPixel(x, y + i, SSD1306_WHITE);
            display.drawPixel(x + w - 1, y + i, SSD1306_WHITE);
        }
        display.setTextColor(SSD1306_WHITE);
    }
    
    // Draw label
    int16_t textX, textY;
    uint16_t textW, textH;
    display.getTextBounds(label, 0, 0, &textX, &textY, &textW, &textH);
    
    // Center text
    display.setCursor(x + (w - textW) / 2, y + (h - textH) / 2 + textH);
    display.print(label);
    
    return isPressed;
}

bool ImUI::slider(Adafruit_GFX& display, int16_t x, int16_t y, 
                int16_t w, int16_t h, int32_t& value, 
                int32_t min, int32_t max, bool isActive) {
    // Draw slider track
    display.drawRoundRect(x, y + (h / 2) - 1, w, 3, 1, SSD1306_WHITE);
    
    // Calculate thumb position
    float normalizedValue = static_cast<float>(value - min) / (max - min);
    int16_t thumbX = x + static_cast<int16_t>(normalizedValue * (w - 6));
    
    // Draw thumb
    if (isActive) {
        display.fillRoundRect(thumbX, y, 6, h, 2, SSD1306_WHITE);
    } else {
        display.drawRoundRect(thumbX, y, 6, h, 2, SSD1306_WHITE);
    }
    
    return isActive;
}

void ImUI::progressBar(Adafruit_GFX& display, int16_t x, int16_t y, 
                     int16_t w, int16_t h, float progress, bool showValue) {
    // Constrain progress to valid range
    if (progress < 0.0f) progress = 0.0f;
    if (progress > 1.0f) progress = 1.0f;
    
    // Draw outline
    display.drawRoundRect(x, y, w, h, 2, SSD1306_WHITE);
    
    // Draw fill
    int16_t fillWidth = static_cast<int16_t>((w - 2) * progress);
    if (fillWidth > 0) {
        display.fillRoundRect(x + 1, y + 1, fillWidth, h - 2, 1, SSD1306_WHITE);
    }
    
    // Show percentage value if requested
    if (showValue) {
        char percentStr[5];
        sprintf(percentStr, "%d%%", static_cast<int>(progress * 100));
        
        int16_t textX, textY;
        uint16_t textW, textH;
        display.getTextBounds(percentStr, 0, 0, &textX, &textY, &textW, &textH);
        
        // Center text
        display.setCursor(x + (w - textW) / 2, y + (h - textH) / 2 + textH);
        
        // Inverse text color if overlapping fill
        if (fillWidth > (w / 2)) {
            display.setTextColor(SSD1306_BLACK);
        } else {
            display.setTextColor(SSD1306_WHITE);
        }
        
        display.print(percentStr);
        // Reset text color
        display.setTextColor(SSD1306_WHITE);
    }
}

bool ImUI::toggle(Adafruit_GFX& display, int16_t x, int16_t y, 
                int16_t w, int16_t h, bool state, bool isEnabled) {
    // Toggle should be approximately twice as wide as tall
    // Draw toggle background
    if (isEnabled) {
        display.drawRoundRect(x, y, w, h, h/2, SSD1306_WHITE);
    } else {
        // Dashed outline for disabled state
        for (int16_t i = 0; i < w; i += 2) {
            display.drawPixel(x + i, y, SSD1306_WHITE);
            display.drawPixel(x + i, y + h - 1, SSD1306_WHITE);
        }
        for (int16_t i = 0; i < h; i += 2) {
            display.drawPixel(x, y + i, SSD1306_WHITE);
            display.drawPixel(x + w - 1, y + i, SSD1306_WHITE);
        }
    }
    
    // Draw toggle position
    int16_t knobX = state ? x + w - h : x;
    if (state && isEnabled) {
        // Draw filled background when on
        display.fillRoundRect(x + w/2, y, w/2, h, h/2, SSD1306_WHITE);
    }
    
    // Draw knob
    if (isEnabled) {
        display.fillCircle(knobX + h/2, y + h/2, h/2 - 1, state ? SSD1306_BLACK : SSD1306_WHITE);
    } else {
        display.drawCircle(knobX + h/2, y + h/2, h/2 - 1, SSD1306_WHITE);
    }
    
    return state;
}

void ImUI::textField(Adafruit_GFX& display, int16_t x, int16_t y, 
                   int16_t w, int16_t h, const char* text, bool isFocused) {
    // Draw text field border
    if (isFocused) {
        display.fillRoundRect(x, y, w, h, 2, SSD1306_WHITE);
        display.setTextColor(SSD1306_BLACK);
    } else {
        display.drawRoundRect(x, y, w, h, 2, SSD1306_WHITE);
        display.setTextColor(SSD1306_WHITE);
    }
    
    // Draw text with padding
    int16_t textX, textY;
    uint16_t textW, textH;
    display.getTextBounds(text, 0, 0, &textX, &textY, &textW, &textH);
    
    // Text alignment - left aligned with padding
    display.setCursor(x + 3, y + (h - textH) / 2 + textH);
    
    // Calculate available width and truncate if needed
    int16_t availableWidth = w - 6;  // 3px padding on each side
    
    // Truncate text if too long
    String displayText = String(text);
    while (textW > availableWidth && displayText.length() > 0) {
        displayText = displayText.substring(0, displayText.length() - 1);
        display.getTextBounds(displayText.c_str(), 0, 0, &textX, &textY, &textW, &textH);
    }
    
    display.print(displayText);
    
    // Draw cursor if focused
    if (isFocused) {
        display.drawFastVLine(x + 3 + textW + 1, y + 2, h - 4, SSD1306_BLACK);
    }
    
    // Reset text color
    display.setTextColor(SSD1306_WHITE);
}

void ImUI::graph(Adafruit_GFX& display, int16_t x, int16_t y, 
               int16_t w, int16_t h, const float* data, size_t dataSize, 
               float minValue, float maxValue, bool showAxes) {
    // Draw axes
    if (showAxes) {
        display.drawFastVLine(x, y, h, SSD1306_WHITE);      // Y-axis
        display.drawFastHLine(x, y + h - 1, w, SSD1306_WHITE);  // X-axis
    }
    
    // Ensure valid data and range
    if (data == nullptr || dataSize < 2 || maxValue <= minValue) {
        return;
    }
    
    // Calculate scaling factors
    float xScale = static_cast<float>(w - 2) / (dataSize - 1);
    float yScale = static_cast<float>(h - 2) / (maxValue - minValue);
    
    // Draw data points
    for (size_t i = 1; i < dataSize; i++) {
        float prevValue = data[i-1];
        float currValue = data[i];
        
        // Constrain values to range
        prevValue = max(min(prevValue, maxValue), minValue);
        currValue = max(min(currValue, maxValue), minValue);
        
        // Calculate point coordinates
        int16_t x1 = x + 1 + static_cast<int16_t>((i-1) * xScale);
        int16_t y1 = y + h - 1 - static_cast<int16_t>((prevValue - minValue) * yScale);
        int16_t x2 = x + 1 + static_cast<int16_t>(i * xScale);
        int16_t y2 = y + h - 1 - static_cast<int16_t>((currValue - minValue) * yScale);
        
        // Draw line between points
        display.drawLine(x1, y1, x2, y2, SSD1306_WHITE);
    }
}

void ImUI::spinner(Adafruit_GFX& display, int16_t x, int16_t y, 
                 int16_t radius, float phase) {
    // Normalize phase to 0.0-1.0
    phase = phase - floor(phase);
    
    // Calculate spinner segments
    const uint8_t numSegments = 8;
    
    for (uint8_t i = 0; i < numSegments; i++) {
        float angle = (i * 2.0f * PI / numSegments) + (phase * 2.0f * PI);
        float brightness = 0.3f + 0.7f * ((i / static_cast<float>(numSegments)));
        
        // Calculate segment endpoints
        int16_t x0 = x + static_cast<int16_t>(cos(angle) * (radius * 0.5f));
        int16_t y0 = y + static_cast<int16_t>(sin(angle) * (radius * 0.5f));
        int16_t x1 = x + static_cast<int16_t>(cos(angle) * radius);
        int16_t y1 = y + static_cast<int16_t>(sin(angle) * radius);
        
        // Draw segment - on monochrome display we can only do full brightness
        if (brightness > 0.5f) {
            display.drawLine(x0, y0, x1, y1, SSD1306_WHITE);
        }
    }
}
