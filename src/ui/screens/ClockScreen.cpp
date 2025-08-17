#include "ClockScreen.h"
#include "../widgets/TimeWidget.h"
#include <Arduino.h>
#include <TimeLib.h>

ClockScreen::ClockScreen(Adafruit_SSD1306& display, TimeService& timeService, AlarmService& alarmService)
    : Screen(display, 128, 64), _timeService(timeService), _alarmService(alarmService) {
    // Time widget at (40,12), textsize 2
    timeWidget = new TimeWidget(40, 12, 48, 20, &_timeService);   // Position (40,12), width 48, height 20
    // AlarmWidget and DateWidget are not implemented yet
    // alarmWidget = new AlarmWidget(10, 52, 108, 10, &_alarmService); // Example position/size/args
    // dateWidget = new DateWidget(10, 35, 108, 15, &_timeService);   // Example position/size/args
    // Button labels at bottom
    infoButton = new ButtonLabelWidget(0, 64-12, 32, 12, "info", 1);
    backButton = new ButtonLabelWidget(96, 64-12, 32, 12, "back", 1);
    // Add widgets
    addWidget(timeWidget);
    addWidget(infoButton);
    addWidget(backButton);
}

bool ClockScreen::update(unsigned long deltaTime) {
    _timeService.update();
    _alarmService.update();
    updateWidgets();
    return true;
}

void ClockScreen::render() {
    Screen::render();
}

bool ClockScreen::handleInput(uint8_t eventType, int32_t eventData) {
    // Handle input for setting time, alarm, etc. (stub)
    return false;
}

void ClockScreen::updateWidgets() {
    // Time
    char timeStr[6];
    sprintf(timeStr, "%02d:%02d", _timeService.getHour(), _timeService.getMinute());
    // Alarm
    const auto& alarms = _alarmService.getAlarms();
    String alarmText = alarms.empty() ? "--:--" : String(alarms[0].hour) + ":" + (alarms[0].minute < 10 ? "0" : "") + String(alarms[0].minute);
    alarmWidget->setText(alarmText);
    // Date
    static const char* days[] = {"SUN","MON","TUE","WED","THU","FRI","SAT"};
    static const char* months[] = {"JAN","FEB","MAR","APR","MAY","JUN","JUL","AUG","SEP","OCT","NOV","DEC"};
    uint8_t dayOfWeek = weekday(_timeService.getUnixTime()) - 1; // Arduino weekday: 1=Sunday
    String dateText = String(days[dayOfWeek]) + "\n";
    char dayStr[3];
    sprintf(dayStr, "%02d", _timeService.getDay());
    dateText += String(dayStr) + "\n";
    dateText += String(months[_timeService.getMonth()-1]);
    dateWidget->setText(dateText);
}
