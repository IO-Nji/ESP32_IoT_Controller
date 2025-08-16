# UI Framework Example Code

This file contains example code demonstrating how to use the ESP32 IoT Controller UI framework. The example shows how to set up screens, create different types of widgets, and handle user input.

## Example Implementation

```cpp
#include <Arduino.h>
#include <Adafruit_SSD1306.h>
#include "src/ui/UIFramework.h"

// Example UI class
class UIExample {
public:
    // Constructor takes display references
    UIExample(Adafruit_SSD1306& display1, Adafruit_SSD1306& display2)
        : uiManager(display1, display2) {
    }
    
    // Setup method to initialize the UI
    void setup() {
        // Create screens for both displays
        Screen* screen1 = uiManager.createScreen(0, 0); // Display 1, Screen ID 0
        Screen* screen2 = uiManager.createScreen(1, 0); // Display 2, Screen ID 0
        
        if (screen1) {
            // Add widgets to display 1 (vertical 32x128)
            
            // Status label at top
            LabelWidget* statusLabel = new LabelWidget(0, 0, 32, 20, "STATUS", 1, LabelWidget::Alignment::CENTER);
            screen1->addWidget(statusLabel);
            
            // Value display in middle
            LabelWidget* valueLabel = new LabelWidget(0, 40, 32, 20, "0", 2, LabelWidget::Alignment::CENTER);
            screen1->addWidget(valueLabel);
            
            // Button at bottom
            ButtonWidget* menuButton = new ButtonWidget(4, 100, 24, 20, "MENU");
            menuButton->setOnPressCallback([](ButtonWidget* button) {
                Serial.println("Menu button pressed");
            });
            screen1->addWidget(menuButton);
        }
        
        if (screen2) {
            // Add widgets to display 2 (horizontal 128x64)
            
            // Title at top
            LabelWidget* titleLabel = new LabelWidget(0, 0, 128, 16, "ESP32 Controller", 1, LabelWidget::Alignment::CENTER);
            screen2->addWidget(titleLabel);
            
            // Status in middle
            LabelWidget* statusLabel = new LabelWidget(10, 24, 108, 16, "System Ready", 1, LabelWidget::Alignment::LEFT);
            screen2->addWidget(statusLabel);
            
            // Buttons at bottom
            ButtonWidget* button1 = new ButtonWidget(10, 44, 50, 16, "BTN 1");
            button1->setOnPressCallback([](ButtonWidget* button) {
                counterValue++;
                Serial.println("Button 1 pressed, counter: " + String(counterValue));
            });
            screen2->addWidget(button1);
            
            ButtonWidget* button2 = new ButtonWidget(68, 44, 50, 16, "BTN 2");
            button2->setOnPressCallback([](ButtonWidget* button) {
                counterValue--;
                if (counterValue < 0) counterValue = 0;
                Serial.println("Button 2 pressed, counter: " + String(counterValue));
            });
            screen2->addWidget(button2);
        }
        
        // Set active screens
        uiManager.setActiveScreen(0, 0);
        uiManager.setActiveScreen(1, 0);
        
        // Initial render
        uiManager.render();
    }
    
    // Update method to refresh the UI (call in loop)
    void update() {
        unsigned long currentTime = millis();
        unsigned long deltaTime = currentTime - lastUpdateTime;
        
        // Only update every 100ms to avoid excessive refreshes
        if (deltaTime >= 100) {
            lastUpdateTime = currentTime;
            
            // Update UI components
            uiManager.update(deltaTime);
            
            // Update dynamic content (example: counter)
            Screen* screen1 = uiManager.getActiveScreen(0);
            if (screen1) {
                // Find the value label and update it
                for (auto widget : screen1->getWidgets()) {
                    LabelWidget* label = dynamic_cast<LabelWidget*>(widget);
                    // Check if this is the right label by comparing Y position
                    if (label && label->getY() == 40) {
                        label->setText(String(counterValue));
                        break;
                    }
                }
            }
            
            // Render updates
            uiManager.render();
        }
    }
    
    // Input handler for button presses, encoder rotation, etc.
    void handleInput(uint8_t displayIndex, uint8_t eventType, int32_t eventData) {
        uiManager.handleInput(displayIndex, eventType, eventData);
    }

private:
    // UI Manager instance
    UIManager uiManager;
    
    // Timing variables
    static unsigned long lastUpdateTime;
    
    // Example state variables
    static int counterValue;
};

// Initialize static variables
unsigned long UIExample::lastUpdateTime = 0;
int UIExample::counterValue = 0;

// Example of usage in main code
void setup() {
    Serial.begin(115200);
    
    // Initialize displays
    Adafruit_SSD1306 display1(128, 32, &Wire, -1);
    Adafruit_SSD1306 display2(128, 64, &Wire, -1);
    
    display1.begin(SSD1306_SWITCHCAPVCC, 0x3C);
    display2.begin(SSD1306_SWITCHCAPVCC, 0x3D);
    
    display1.setRotation(3); // Rotate display 1 to portrait mode
    
    // Create example UI
    UIExample uiExample(display1, display2);
    uiExample.setup();
}

void loop() {
    // Update UI
    uiExample.update();
    
    // Handle input events (simplified example)
    // In a real application, you would detect input events and call handleInput accordingly
}
```

## Key Concepts Demonstrated

1. **Screen Creation**: Creating separate screens for each display
2. **Widget Types**: Using various widget types (Label, Button) with different configurations
3. **Layout Management**: Positioning widgets based on display orientations
4. **Event Handling**: Setting up button callbacks
5. **Rendering Cycle**: Implementing a proper update and render cycle
6. **Dynamic Updates**: Updating widget content (counter value) dynamically

## Notes on Implementation

- The displays are initialized with different I2C addresses (0x3C and 0x3D)
- Display 1 is rotated 90 degrees (rotation value 3)
- The update method uses a time-based approach to limit refresh rate
- Widget callbacks use lambda functions for simplicity

For more details and best practices, please refer to the UI Framework Guide.
