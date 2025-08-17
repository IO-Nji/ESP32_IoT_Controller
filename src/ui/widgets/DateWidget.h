#pragma once
#include "../LabelWidget.h"
#include <Arduino.h>

class DateWidget : public LabelWidget {
public:
    DateWidget(int16_t x, int16_t y, int16_t w, int16_t h);
    void setDate(const String& dayOfWeek, const String& dayOfMonth, const String& month);
    void draw(Adafruit_GFX& display) override;
private:
    String _dayOfWeek;
    String _dayOfMonth;
    String _month;
};
