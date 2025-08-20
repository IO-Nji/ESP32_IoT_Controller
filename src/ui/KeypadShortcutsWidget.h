
#ifndef KEYPAD_SHORTCUTS_WIDGET_H
#define KEYPAD_SHORTCUTS_WIDGET_H

#include <Arduino.h>
#include <Adafruit_GFX.h>
#include "BaseWidget.h"

// ...existing code...

/**
 * @brief A widget for displaying keypad shortcut buttons
 * 
 * This widget displays shortcut labels for keypad buttons (A,B,C,D,#,*) 
 * and their associated functions at the bottom of the display.
 */
class KeypadShortcutsWidget : public BaseWidget {
public:
    // Maximum number of shortcuts
    static const uint8_t MAX_SHORTCUTS = 6;
    
    /**
     * @brief Construct a new Keypad Shortcuts Widget
     * 
     * @param x X position
     * @param y Y position
     * @param width Width of the widget
     * @param height Height of the widget
     * @param visible Visibility flag
     */
    KeypadShortcutsWidget(int16_t x, int16_t y, int16_t width, int16_t height, bool visible = true);
    
    /**
     * @brief Draw the keypad shortcuts on the display
     * 
     * @param display Display to draw on
     */
    virtual void draw(Adafruit_GFX& display) override;
    
    /**
     * @brief Set a shortcut with key and function name
     * 
     * @param key The shortcut key ('A', 'B', 'C', 'D', '#', '*')
     * @param functionName The function name to be displayed for this shortcut
     */
    void setShortcut(char key, const String& functionName);
    
    /**
     * @brief Remove a shortcut
     * 
     * @param key The shortcut key to remove
     */
    void removeShortcut(char key);
    
    /**
     * @brief Clear all shortcuts
     */
    void clearShortcuts();
    
private:
    /**
     * @brief Generate the abbreviation for a function name
     * 
     * @param functionName The full function name
     * @return String Abbreviated function name (max 3 characters)
     */
    String generateAbbreviation(const String& functionName);
    
    /**
     * @brief Find the index for a key
     * 
     * @param key The key to find
     * @return int Index in the arrays, or -1 if not found
     */
    int findKeyIndex(char key);
    
    char keys[MAX_SHORTCUTS];             ///< Array of shortcut keys
    String functions[MAX_SHORTCUTS];      ///< Array of function names
    String abbreviations[MAX_SHORTCUTS];  ///< Array of abbreviations
    uint8_t shortcutCount;                ///< Number of shortcuts defined
};

#endif // KEYPAD_SHORTCUTS_WIDGET_H
