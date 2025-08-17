#include "DateWidget.h"

DateWidget::DateWidget(int16_t x, int16_t y, int16_t w, int16_t h)
    : LabelWidget(x, y, w, h, "", 1) {}

void DateWidget::setDate(const String& dayOfWeek, const String& dayOfMonth, const String& month) {
    _dayOfWeek = dayOfWeek;
    _dayOfMonth = dayOfMonth;
    _month = month;
}

void DateWidget::draw(Adafruit_GFX& display) {
    display.setTextColor(SSD1306_WHITE);
    display.drawRoundRect(x, y, width, height, 2, SSD1306_WHITE); // Border with radius 2
    display.setTextSize(1);
    display.setCursor(x + 2, y + 2);
    display.print(_dayOfWeek);
    display.setTextSize(2);
    display.setCursor(x + 2, y + 12);
    display.print(_dayOfMonth);
    display.setTextSize(1);
    display.setCursor(x + 2, y + 28);
    display.print(_month);
}
