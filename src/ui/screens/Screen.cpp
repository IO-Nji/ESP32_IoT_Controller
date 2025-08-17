#include "Screen.h"
#include <ArduinoJson.h>

Screen::Screen(Adafruit_SSD1306& display, uint16_t width, uint16_t height)
    : display(display), width(width), height(height) {
}

void Screen::addWidget(Widget* widget) {
    if (widget) {
        widgets.push_back(widget);
    }
}

bool Screen::removeWidget(Widget* widget) {
    for (auto it = widgets.begin(); it != widgets.end(); ++it) {
        if (*it == widget) {
            widgets.erase(it);
            return true;
        }
    }
    return false;
}

void Screen::render() {
    // Clear the display
    display.clearDisplay();
    
    // Render all visible widgets
    for (Widget* widget : widgets) {
        widget->draw(display);
    }
    
    // Update the physical display
    display.display();
}

void Screen::saveState(JsonDocument& state) {
    // Save state for each widget (if supported)
    for (auto* widget : widgets) {
        if (widget) widget->saveState(state);
    }
}

void Screen::loadState(const JsonDocument& state) {
    // Restore state for each widget (if supported)
    for (auto* widget : widgets) {
        if (widget) widget->loadState(state);
    }
}

bool Screen::update(unsigned long deltaTime) {
    bool success = true;
    
    try {
        // Update all widgets
        for (Widget* widget : widgets) {
            if (!widget->update(deltaTime)) {
                success = false;
            }
        }
    }
    catch (...) {
        success = false;
    }
    
    return success;
}

bool Screen::handleInput(uint8_t eventType, int32_t eventData) {
    // Process input events in reverse order (top-most widget first)
    for (auto it = widgets.rbegin(); it != widgets.rend(); ++it) {
        if ((*it)->handleInput(eventType, eventData)) {
            // Event was handled by this widget
            return true;
        }
    }
    
    // No widget handled the event
    return false;
}
