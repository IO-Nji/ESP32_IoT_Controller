#include "../UIFramework.h"

/**
 * @brief Example of immediate mode UI usage
 * 
 * This file demonstrates how to use the ImUI components for dynamic UI elements.
 * The key difference from retained mode widgets is that immediate mode elements
 * are drawn on-demand each frame and don't maintain their state.
 */

class ImUIExample {
public:
    ImUIExample(Adafruit_SSD1306& display) : display(display) {
        // Initialize variables
        sliderValue = 50;
        toggleState = false;
        animationPhase = 0.0f;
        
        // Initialize sample graph data
        for (size_t i = 0; i < 12; i++) {
            graphData[i] = 25 + 20 * sin(i * PI / 6);
        }
    }
    
    void update(unsigned long deltaTime) {
        // Update animation state
        animationPhase += deltaTime / 1000.0f;  // Convert ms to seconds
        if (animationPhase > 1.0f) {
            animationPhase -= floor(animationPhase);
        }
        
        // Update graph data with smooth animation
        for (size_t i = 0; i < 12; i++) {
            graphData[i] = 25 + 20 * sin((i * PI / 6) + animationPhase * 2.0f * PI);
        }
    }
    
    void render() {
        display.clearDisplay();
        
        // Draw a title
        display.setTextSize(1);
        display.setTextColor(SSD1306_WHITE);
        display.setCursor(2, 0);
        display.println("ImUI Demo");
        display.drawFastHLine(0, 9, display.width(), SSD1306_WHITE);
        
        // Draw a button
        bool buttonPressed = isButtonPressed(); // This would come from your input system
        ImUI::button(display, 5, 12, 50, 12, "Button", buttonPressed);
        
        // Draw a slider
        bool sliderActive = isSliderActive(); // This would come from your input system
        ImUI::slider(display, 60, 12, 64, 12, sliderValue, 0, 100, sliderActive);
        
        // Draw a progress bar using the slider value
        ImUI::progressBar(display, 5, 28, 119, 8, sliderValue / 100.0f, true);
        
        // Draw a toggle switch
        toggleState = ImUI::toggle(display, 5, 40, 30, 10, toggleState);
        
        // Draw a spinner animation
        ImUI::spinner(display, 55, 45, 8, animationPhase);
        
        // Draw a text field
        ImUI::textField(display, 70, 40, 54, 12, "Text", false);
        
        // Draw a graph with sample data
        ImUI::graph(display, 5, 55, 118, 30, graphData, 12, 0, 50, true);
        
        display.display();
    }

private:
    Adafruit_SSD1306& display;
    int32_t sliderValue;
    bool toggleState;
    float animationPhase;
    float graphData[12];
    
    // These would be replaced by actual input detection from your system
    bool isButtonPressed() {
        // Return true if the button area is being pressed
        // In a real implementation, this would check touch or button input
        return false;
    }
    
    bool isSliderActive() {
        // Return true if the slider is being manipulated
        // In a real implementation, this would check touch or button input
        return false;
    }
};

// Example usage in your main code:
/*
void setupImUIExample() {
    // Create the display
    Adafruit_SSD1306 display(128, 64, &Wire, -1);
    display.begin(SSD1306_SWITCHCAPVCC, 0x3D);
    
    // Create the example
    ImUIExample example(display);
    
    // Main loop
    unsigned long lastUpdate = millis();
    while (true) {
        unsigned long now = millis();
        unsigned long deltaTime = now - lastUpdate;
        lastUpdate = now;
        
        // Update and render
        example.update(deltaTime);
        example.render();
        
        // Add delay or other processing
        delay(16); // ~60fps
    }
}
*/
