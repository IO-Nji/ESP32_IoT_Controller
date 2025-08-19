#ifndef BASE_SCREEN_H
#define BASE_SCREEN_H

#include <Adafruit_SSD1306.h>
#include <vector>
#include "../Widget.h"
#include <ArduinoJson.h>

class BaseScreen {
public:
    BaseScreen(Adafruit_SSD1306& display, uint16_t width, uint16_t height);
    void addWidget(Widget* widget);
    bool removeWidget(Widget* widget);
    void render();
    void saveState(JsonDocument& state);
    void loadState(const JsonDocument& state);
    bool update(unsigned long deltaTime);
    bool handleInput(uint8_t eventType, int32_t eventData);
    uint16_t width;
    uint16_t height;
    std::vector<Widget*> widgets;
protected:
    Adafruit_SSD1306& display;
};

#endif // BASE_SCREEN_H
