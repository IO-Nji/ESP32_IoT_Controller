#ifndef IM_UI_H
#define IM_UI_H

#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

/**
 * @brief Immediate Mode UI utilities for dynamic components
 * 
 * This class provides static methods for drawing immediate mode UI elements.
 * Unlike retained mode widgets, these elements are drawn directly each frame
 * and don't maintain state between frames.
 * 
 * Use these for UI elements that need to be updated frequently or
 * when you need lightweight elements without the overhead of Widget objects.
 */
class ImUI {
public:
    /**
     * @brief Draw a button and handle interaction
     * 
     * @param display Display to draw on
     * @param x X position
     * @param y Y position
     * @param w Width
     * @param h Height
     * @param label Button label
     * @param isPressed Current pressed state
     * @param isEnabled Whether button is enabled
     * @return true if the button is currently pressed
     * @return false if not pressed
     */
    static bool button(Adafruit_GFX& display, int16_t x, int16_t y, 
                     int16_t w, int16_t h, const char* label, 
                     bool isPressed = false, bool isEnabled = true);
    
    /**
     * @brief Draw a slider and handle interaction
     * 
     * @param display Display to draw on
     * @param x X position
     * @param y Y position
     * @param w Width
     * @param h Height
     * @param value Current value (will be modified if slider moves)
     * @param min Minimum value
     * @param max Maximum value
     * @param isActive Whether the slider is currently being manipulated
     * @return true if the slider is currently active
     * @return false if inactive
     */
    static bool slider(Adafruit_GFX& display, int16_t x, int16_t y, 
                     int16_t w, int16_t h, int32_t& value, 
                     int32_t min, int32_t max, bool isActive = false);
    
    /**
     * @brief Draw a progress bar
     * 
     * @param display Display to draw on
     * @param x X position
     * @param y Y position
     * @param w Width
     * @param h Height
     * @param progress Progress value (0.0 to 1.0)
     * @param showValue Whether to show percentage text
     */
    static void progressBar(Adafruit_GFX& display, int16_t x, int16_t y, 
                          int16_t w, int16_t h, float progress, 
                          bool showValue = false);
    
    /**
     * @brief Draw a toggle switch
     * 
     * @param display Display to draw on
     * @param x X position
     * @param y Y position
     * @param w Width
     * @param h Height
     * @param state Current state (on/off)
     * @param isEnabled Whether toggle is enabled
     * @return true if on
     * @return false if off
     */
    static bool toggle(Adafruit_GFX& display, int16_t x, int16_t y, 
                     int16_t w, int16_t h, bool state, bool isEnabled = true);
    
    /**
     * @brief Draw a text field
     * 
     * @param display Display to draw on
     * @param x X position
     * @param y Y position
     * @param w Width
     * @param h Height
     * @param text Text content
     * @param isFocused Whether field has input focus
     */
    static void textField(Adafruit_GFX& display, int16_t x, int16_t y, 
                        int16_t w, int16_t h, const char* text, 
                        bool isFocused = false);
    
    /**
     * @brief Draw a graph with data points
     * 
     * @param display Display to draw on
     * @param x X position
     * @param y Y position
     * @param w Width
     * @param h Height
     * @param data Array of data points
     * @param dataSize Number of data points
     * @param minValue Minimum value for scaling
     * @param maxValue Maximum value for scaling
     * @param showAxes Whether to show axes
     */
    static void graph(Adafruit_GFX& display, int16_t x, int16_t y, 
                    int16_t w, int16_t h, const float* data, size_t dataSize, 
                    float minValue, float maxValue, bool showAxes = true);
    
    /**
     * @brief Draw a spinner/loading indicator
     * 
     * @param display Display to draw on
     * @param x X position (center)
     * @param y Y position (center)
     * @param radius Radius
     * @param phase Animation phase (0.0 to 1.0)
     */
    static void spinner(Adafruit_GFX& display, int16_t x, int16_t y, 
                      int16_t radius, float phase);
};

#endif // IM_UI_H
