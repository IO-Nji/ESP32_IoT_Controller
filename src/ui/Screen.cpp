#include "Screen.h"

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

void Screen::update(unsigned long deltaTime) {
    // Update all widgets
    for (Widget* widget : widgets) {
        widget->update(deltaTime);
    }
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
