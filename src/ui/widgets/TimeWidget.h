#pragma once
#include "../screens/Screen.h"
#include "../../apps/TimeService.h"

class TimeWidget : public Widget {
public:
    TimeWidget(int16_t x, int16_t y, int16_t w, int16_t h, TimeService* timeService);
    void draw(Adafruit_GFX& display) override;
private:
    TimeService* _timeService;
};
