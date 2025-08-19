// src/ui/screens/ScheduleScreen.h
#pragma once
#include "../UIFramework.h"
#include "../../services/ScheduleService.h"

class ScheduleScreen : public BaseScreen {
public:
    ScheduleScreen(Adafruit_SSD1306& display, ScheduleService& scheduleService);

    void render() override;
    bool handleInput(uint8_t eventType, int32_t eventData) override;
    void update(unsigned long deltaTime) override;

private:
    ScheduleService& _scheduleService;
    // Add pointers to widgets for event list, add/edit controls, etc.
};