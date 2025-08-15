#include "UIExample.h"

// Initialize static members
unsigned long UIExample::lastUpdateTime = 0;
int UIExample::counterValue = 0;

UIExample::UIExample(Adafruit_SSD1306& display1, Adafruit_SSD1306& display2)
    : uiManager(display1, display2) {
}

void UIExample::setup() {
    // Create screens for both displays
    Screen* screen1 = uiManager.createScreen(0, 0); // Display 1, Screen ID 0
    Screen* screen2 = uiManager.createScreen(1, 0); // Display 2, Screen ID 0
    
    if (screen1) {
        // Add widgets to display 1 (vertical 32x128)
        // Note: Coordinates are relative to the rotated display
        
        // Status label at top
        LabelWidget* statusLabel = new LabelWidget(0, 0, 32, 20, "STATUS", 1, LabelWidget::Alignment::CENTER);
        screen1->addWidget(statusLabel);
        
        // Value display in middle
        LabelWidget* valueLabel = new LabelWidget(0, 40, 32, 20, "0", 2, LabelWidget::Alignment::CENTER);
        screen1->addWidget(valueLabel);
        
        // Button at bottom
        ButtonWidget* menuButton = new ButtonWidget(4, 100, 24, 20, "MENU");
        menuButton->setOnPressCallback(onButton1Pressed);
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
        button1->setOnPressCallback(onButton1Pressed);
        screen2->addWidget(button1);
        
        ButtonWidget* button2 = new ButtonWidget(68, 44, 50, 16, "BTN 2");
        button2->setOnPressCallback(onButton2Pressed);
        screen2->addWidget(button2);
    }
    
    // Set active screens
    uiManager.setActiveScreen(0, 0);
    uiManager.setActiveScreen(1, 0);
    
    // Initial render
    uiManager.render();
}

void UIExample::update() {
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

void UIExample::handleInput(uint8_t displayIndex, uint8_t eventType, int32_t eventData) {
    uiManager.handleInput(displayIndex, eventType, eventData);
}

// Example button callbacks
void UIExample::onButton1Pressed() {
    counterValue++;
}

void UIExample::onButton2Pressed() {
    counterValue--;
    if (counterValue < 0) {
        counterValue = 0;
    }
}
