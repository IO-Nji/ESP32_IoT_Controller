#ifndef ANIMATED_DASHBOARD_SCREEN_H
#define ANIMATED_DASHBOARD_SCREEN_H

#include "../UIFramework.h"
#include <vector>
#include <deque>

/**
 * @brief Dashboard screen with animated elements
 * 
 * This screen demonstrates how to combine retained mode widgets with
 * immediate mode elements to create a dynamic dashboard display.
 */
class AnimatedDashboardScreen {
public:
    /**
     * @brief Construct a new Animated Dashboard Screen
     * 
     * @param display Display to render on
     */
    AnimatedDashboardScreen(Adafruit_SSD1306& display);
    
    /**
     * @brief Set up the screen with initial widgets and state
     */
    void setup();
    
    /**
     * @brief Update all animations and dynamic content
     * 
     * @param deltaTime Time since last update in milliseconds
     */
    void update(unsigned long deltaTime);
    
    /**
     * @brief Render the screen with all widgets and dynamic elements
     */
    void render();
    
    /**
     * @brief Handle user input
     * 
     * @param eventType Type of input event
     * @param eventData Additional event data
     * @return true if input was handled
     * @return false if input was not handled
     */
    bool handleInput(uint8_t eventType, int32_t eventData);

private:
    // Display reference
    Adafruit_SSD1306& display;
    
    // Screen dimension
    uint16_t width;
    uint16_t height;
    
    // Retained mode widgets
    BaseWidget* titleWidget;
    BaseWidget* statusWidget;
    BaseWidget* menuButtonWidget;
    
    // Animation state variables
    float animationPhase;
    float graphAnimationSpeed;
    bool graphAnimationPaused;
    
    // Sensor data simulation
    std::deque<float> temperatureHistory;
    std::deque<float> humidityHistory;
    float currentTemperature;
    float currentHumidity;
    float batteryLevel;
    
    // Helper methods
    void updateSensorData(unsigned long deltaTime);
    void renderSensorGraphs();
    void renderStatusIndicators();
    void renderAnimatedIcons();
};

#endif // ANIMATED_DASHBOARD_SCREEN_H
