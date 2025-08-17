#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "../UIFramework.h"
#include "AnimatedDashboardScreen.h"

/**
 * @brief Example application demonstrating animated screens using ImUI
 * 
 * This example shows how to:
 * 1. Set up a dashboard with animated elements
 * 2. Combine retained mode widgets with immediate mode UI
 * 3. Handle animation timing and dynamic data updates
 * 4. Process user input for both widget types
 * 
 * In a real application, you would derive from a base Application class,
 * but this example is self-contained for demonstration purposes.
 */

class DashboardApp {
public:
    DashboardApp() {
        // Initialize I2C
        Wire.begin();
        
        // Initialize displays
        // Display 1 (128x32)
        if (!display1.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
            Serial.println(F("Display 1 allocation failed"));
        }
        
        // Display 2 (128x64)
        if (!display2.begin(SSD1306_SWITCHCAPVCC, 0x3D)) {
            Serial.println(F("Display 2 allocation failed"));
        }
        
        // Create the animated dashboard for the larger display
        dashboard = new AnimatedDashboardScreen(display2);
        
        lastUpdateTime = millis();
    }
    
    void setup() {
        // Clear displays
        display1.clearDisplay();
        display1.display();
        display2.clearDisplay();
        display2.display();
        
        // Setup dashboard
        dashboard->setup();
        
        // Display welcome message on small display
        display1.setTextSize(1);
        display1.setTextColor(SSD1306_WHITE);
        display1.setCursor(0, 0);
        display1.println("IoT Controller");
        display1.println("Dashboard Demo");
        display1.println("");
        display1.println("Press B1 to pause");
        display1.println("Rotate Encoder to");
        display1.println("change speed");
        display1.display();
    }
    
    void looper() {
        // Calculate delta time
        unsigned long currentTime = millis();
        unsigned long deltaTime = currentTime - lastUpdateTime;
        lastUpdateTime = currentTime;
        
        // Check for input events (simulation)
        checkInput();
        
        // Update dashboard animation state
        dashboard->update(deltaTime);
        
        // Render the dashboard
        dashboard->render();
        
        // Limit frame rate to save power
        delay(16); // ~60fps
    }

private:
    Adafruit_SSD1306 display1 = Adafruit_SSD1306(128, 32, &Wire);
    Adafruit_SSD1306 display2 = Adafruit_SSD1306(128, 64, &Wire);
    AnimatedDashboardScreen* dashboard;
    unsigned long lastUpdateTime;
    
    // Input state tracking for demo
    bool lastButton1State = false;
    int lastEncoderValue = 0;
    
    void checkInput() {
        // In a real application, you would read from input HAL
        // Here we're just simulating with digital reads
        
        // Check button 1 (pin 26)
        bool button1State = !digitalRead(26); // Active low
        if (button1State && !lastButton1State) {
            // Button press event
            dashboard->handleInput(1, 0);
        }
        lastButton1State = button1State;
        
        // Check encoder rotation (simplified simulation)
        int encoderValue = analogRead(33) / 100; // Very simple simulation
        if (encoderValue != lastEncoderValue) {
            int delta = encoderValue - lastEncoderValue;
            dashboard->handleInput(2, delta);
            lastEncoderValue = encoderValue;
        }
    }
};

// Global app instance
DashboardApp* app = nullptr;

void setuper() {
    Serial.begin(115200);
    while (!Serial) delay(10);
    
    Serial.println("Animated Dashboard Example");
    
    // Initialize pins
    pinMode(26, INPUT_PULLUP); // Button 1
    
    app = new DashboardApp();
    app->setup();
}

void looper() {
    app->looper();
}
