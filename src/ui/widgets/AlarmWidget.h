#pragma once
#include "../Widget.h"
#include "../../services/AlarmService.h"

class AlarmWidget : public Widget {
public:
    AlarmWidget(int16_t x, int16_t y, int16_t w, int16_t h, AlarmService* alarmService);
        void setText(const String& text);
    void draw(Adafruit_GFX& display) override;
private:
    AlarmService* _alarmService;
};
