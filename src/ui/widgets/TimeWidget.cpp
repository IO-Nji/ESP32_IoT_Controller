#include "TimeWidget.h"
#include <Adafruit_GFX.h>

TimeWidget::TimeWidget(int16_t x, int16_t y, int16_t w, int16_t h, TimeService* timeService)
    : Widget(x, y, w, h), _timeService(timeService) {}

void TimeWidget::draw(Adafruit_GFX& display) {
    if (!_timeService) return;
    char buf[6];
    sprintf(buf, "%02d:%02d", _timeService->getHour(), _timeService->getMinute());
    display.setTextSize(2);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(x, y);
    display.print(buf);
}

