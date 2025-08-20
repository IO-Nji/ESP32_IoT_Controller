#include "DateWidget.h"
#include <Adafruit_SSD1306.h>

DateWidget::DateWidget(int16_t x, int16_t y, int16_t w, int16_t h)
    : LabelWidget(x, y, w, h, "", 1) {}

void DateWidget::setDate(const String& dayOfWeek, const String& dayOfMonth, const String& month) {
    _dayOfWeek = dayOfWeek;
    _dayOfMonth = dayOfMonth;
    _month = month;
}

void DateWidget::draw(Adafruit_GFX& display) {
    display.setTextColor(SSD1306_WHITE);
    display.drawRoundRect(this->getX(), this->getY(), this->getWidth(), 42, 2, SSD1306_WHITE); // Border with radius 2, height 40

    // Calculate heights for each line
    int16_t x1, y1;
    uint16_t w1, h1;
    display.setTextSize(1);
    display.getTextBounds(_dayOfWeek, this->getX(), this->getY(), &x1, &y1, &w1, &h1);
    uint16_t hDayOfWeek = h1;
    display.setTextSize(2);
    display.getTextBounds(_dayOfMonth, this->getX(), this->getY(), &x1, &y1, &w1, &h1);
    uint16_t hDayOfMonth = h1;
    display.setTextSize(1);
    display.getTextBounds(_month, this->getX(), this->getY(), &x1, &y1, &w1, &h1);
    uint16_t hMonth = h1;

    uint16_t totalTextHeight = hDayOfWeek + hDayOfMonth + hMonth;
    uint16_t boxHeight = 40;
    int16_t yStart = this->getY() + (boxHeight - totalTextHeight) / 2;

    // Draw day of week
    display.setTextSize(1);
    display.getTextBounds(_dayOfWeek, this->getX(), this->getY(), &x1, &y1, &w1, &h1);
    display.setCursor(this->getX() + (this->getWidth() - w1) / 2, yStart);
    display.print(_dayOfWeek);

    // Draw day of month with 2px spacing
    display.setTextSize(2);
    display.getTextBounds(_dayOfMonth, this->getX(), this->getY(), &x1, &y1, &w1, &h1);
    display.setCursor(this->getX() + (this->getWidth() - w1) / 2, yStart + hDayOfWeek + 2);
    display.print(_dayOfMonth);

    // Draw month with 2px spacing after day of month
    display.setTextSize(1);
    display.getTextBounds(_month, this->getX(), this->getY(), &x1, &y1, &w1, &h1);
    display.setCursor(this->getX() + (this->getWidth() - w1) / 2, yStart + hDayOfWeek + hDayOfMonth + 4);
    display.print(_month);
}
