#include "KeypadShortcutsWidget.h"
#include <Arduino.h>
#include "../hal/hal_config.h"

KeypadShortcutsWidget::KeypadShortcutsWidget(int16_t x, int16_t y, int16_t width, int16_t height, bool visible)
    : BaseWidget(x, y, width, height, visible), shortcutCount(0) {
    // Initialize arrays
    for (uint8_t i = 0; i < MAX_SHORTCUTS; i++) {
        keys[i] = '\0';
        functions[i] = "";
        abbreviations[i] = "";
    }
}

void KeypadShortcutsWidget::draw(Adafruit_GFX& display) {
    if (!isVisible() || shortcutCount == 0) {
        return;
    }
    
    // Calculate absolute coordinates
    int16_t absX, absY;
    getAbsolutePosition(absX, absY);
    
    // Set text properties
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    
    // Calculate spacing based on number of shortcuts to display
    int displayedShortcuts = shortcutCount;
    int shortcutHeight = min(10, height / displayedShortcuts);
    
    // Iterate through shortcuts
    for (uint8_t i = 0; i < shortcutCount; i++) {
        // Format as "A:HHH" where A is the key and HHH is the abbreviation
        String shortcutText = String(keys[i]) + ":" + abbreviations[i];
        
        // Calculate text dimensions
        int16_t x1, y1;
        uint16_t textWidth, textHeight;
        display.getTextBounds(shortcutText, 0, 0, &x1, &y1, &textWidth, &textHeight);
        
        // Position text right-justified
        int16_t textX = absX + width - textWidth - 2; // 2 pixel margin from right edge
        int16_t textY = absY + (i * shortcutHeight) + textHeight;
        
        // Draw the shortcut text
        display.setCursor(textX, textY);
        display.print(shortcutText);
    }
}

int KeypadShortcutsWidget::findKeyIndex(char key) {
    for (uint8_t i = 0; i < shortcutCount; i++) {
        if (keys[i] == key) {
            return i;
        }
    }
    return -1; // Not found
}

void KeypadShortcutsWidget::setShortcut(char key, const String& functionName) {
    int index = findKeyIndex(key);
    
    if (index >= 0) {
        // Update existing shortcut
        functions[index] = functionName;
        abbreviations[index] = generateAbbreviation(functionName);
    } else if (shortcutCount < MAX_SHORTCUTS) {
        // Add new shortcut
        keys[shortcutCount] = key;
        functions[shortcutCount] = functionName;
        abbreviations[shortcutCount] = generateAbbreviation(functionName);
        shortcutCount++;
    }
}

void KeypadShortcutsWidget::removeShortcut(char key) {
    int index = findKeyIndex(key);
    if (index >= 0) {
        // Move all subsequent items down by one
        for (uint8_t i = index; i < shortcutCount - 1; i++) {
            keys[i] = keys[i + 1];
            functions[i] = functions[i + 1];
            abbreviations[i] = abbreviations[i + 1];
        }
        shortcutCount--;
    }
}

void KeypadShortcutsWidget::clearShortcuts() {
    shortcutCount = 0;
}

String KeypadShortcutsWidget::generateAbbreviation(const String& functionName) {
    String abbr = "";
    
    // If function name is already 3 characters or less, use it directly
    if (functionName.length() <= 3) {
        abbr = functionName;
    } else {
        // Try to create an abbreviation from the first letter of each word
        int wordCount = 0;
        
        // Process up to 3 words
        for (unsigned int i = 0; i < functionName.length() && abbr.length() < 3; i++) {
            if (i == 0 || functionName[i-1] == ' ') {
                abbr += functionName[i];
                wordCount++;
            }
            
            // If we've processed 3 words, stop
            if (wordCount >= 3) break;
        }
        
        // If we don't have 3 characters yet, add the first consonants or vowels
        if (abbr.length() < 3) {
            for (unsigned int i = 1; i < functionName.length() && abbr.length() < 3; i++) {
                char c = functionName[i];
                // Skip spaces and characters already in the abbreviation
                bool alreadyInAbbr = false;
                for (unsigned int j = 0; j < abbr.length(); j++) {
                    if (toupper(abbr[j]) == toupper(c)) {
                        alreadyInAbbr = true;
                        break;
                    }
                }
                if (c != ' ' && !alreadyInAbbr) {
                    abbr += c;
                }
            }
        }
    }
    
    // Convert to uppercase and ensure exactly 3 characters
    abbr.toUpperCase();
    
    // Pad with spaces if less than 3 characters
    while (abbr.length() < 3) {
        abbr += " ";
    }
    
    // Truncate if more than 3 characters
    if (abbr.length() > 3) {
        abbr = abbr.substring(0, 3);
    }
    
    return abbr;
}
