#include "TimeWidget.h"
#include <Adafruit_GFX.h>

TimeWidget::TimeWidget(int16_t x, int16_t y, int16_t w, int16_t h, TimeService* timeService)
    : BaseWidget(x, y, w, h), _timeService(timeService) {}

void TimeWidget::draw(Adafruit_GFX& display) {
    display.setTextSize(2);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(this->getX(), this->getY());
    if (_text.length() > 0) {
        display.print(_text);
    } else if (_timeService) {
        char buf[6];
        sprintf(buf, "%02d:%02d", _timeService->getHour(), _timeService->getMinute());
        display.print(buf);
    }
}

void TimeWidget::setText(const String& text) {
    _text = text;
}

