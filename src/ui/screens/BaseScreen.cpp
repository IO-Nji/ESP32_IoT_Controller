#include "BaseScreen.h"
#include <ArduinoJson.h>

BaseScreen::BaseScreen(Adafruit_SSD1306& display, uint16_t width, uint16_t height)
    : display(display), width(width), height(height) {}

void BaseScreen::addWidget(BaseWidget* widget) {
        if (widget) {
            widgets.push_back(widget);
        }
}

bool BaseScreen::removeWidget(BaseWidget* widget) {
        for (auto it = widgets.begin(); it != widgets.end(); ++it) {
            if (*it == widget) {
                widgets.erase(it);
                return true;
            }
        }
        return false;
}

void BaseScreen::render() {
    display.clearDisplay();
        for (BaseWidget* widget : widgets) {
            widget->draw(display);
        }
        display.display();
}

void BaseScreen::saveState(JsonDocument& state) {
    for (auto* widget : widgets) {
        if (widget) widget->saveState(state);
    }
}

void BaseScreen::loadState(const JsonDocument& state) {
    for (auto* widget : widgets) {
        if (widget) widget->loadState(state);
    }
}

bool BaseScreen::update(unsigned long deltaTime) {
    bool success = true;
    try {
            for (BaseWidget* widget : widgets) {
                if (!widget->update(deltaTime)) {
                    success = false;
                }
            }
    } catch (...) {
        success = false;
    }
    return success;
}

bool BaseScreen::handleInput(uint8_t eventType, int32_t eventData) {
    for (auto it = widgets.rbegin(); it != widgets.rend(); ++it) {
        if ((*it)->handleInput(eventType, eventData)) {
            return true;
        }
    }
    return false;
}
