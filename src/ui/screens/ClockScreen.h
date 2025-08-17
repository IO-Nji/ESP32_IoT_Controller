// src/ui/screens/ClockScreen.h
#pragma once
#include "../UIFramework.h"
#include "../../apps/TimeService.h"
#include "../widgets/TimeWidget.h"
#include "../widgets/AlarmWidget.h"
#include "../../apps/AlarmService.h"
#include "../LabelWidget.h"
#include "../ButtonLabelWidget.h"

class ClockScreen : public Screen {
public:
    ClockScreen(Adafruit_SSD1306& display, TimeService& timeService, AlarmService& alarmService);
    bool update(unsigned long deltaTime);
    void render();
    bool handleInput(uint8_t eventType, int32_t eventData);

private:
    TimeService& _timeService;
    AlarmService& _alarmService;
    TimeWidget* timeWidget;
    AlarmWidget* alarmWidget;
    LabelWidget* dateWidget;
    ButtonLabelWidget* infoButton;
    ButtonLabelWidget* backButton;
    void updateWidgets();
};
