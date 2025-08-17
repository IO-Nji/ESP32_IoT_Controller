#include "AlarmWidget.h"
#include <Adafruit_GFX.h>

AlarmWidget::AlarmWidget(int16_t x, int16_t y, int16_t w, int16_t h, AlarmService* alarmService)
    : Widget(x, y, w, h), _alarmService(alarmService) {}

void AlarmWidget::draw(Adafruit_GFX& display) {
    String alarmText = "00:00";
    if (_alarmService) {
        const auto& alarms = _alarmService->getAlarms();
        if (!alarms.empty()) {
            char buf[6];
            sprintf(buf, "%02d:%02d", alarms[0].hour, alarms[0].minute);
            alarmText = String(buf);
        }
    }
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(x, y);
    display.print(alarmText);
}
