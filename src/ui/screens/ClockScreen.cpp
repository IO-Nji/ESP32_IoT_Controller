#include "ClockScreen.h"
#include "../widgets/TimeWidget.h"
#include "../widgets/AlarmWidget.h"
#include <Arduino.h>
#include <TimeLib.h>

ClockScreen::ClockScreen(Adafruit_SSD1306& display, TimeService& timeService, AlarmService& alarmService)
    : Screen(display, 128, 64), _timeService(timeService), _alarmService(alarmService) {
    // Alarm widget at (96,1), textsize 1
    alarmWidget = new AlarmWidget(96, 1, 48, 10, &_alarmService); // Position (96,1), width 48, height 10
    // Date widget at (2,2), size 32x32
        dateWidget = new DateWidget(2, 2, 32, 32);
        dateWidget->setDate("SUN", "10", "NOV"); // Placeholder: 10 Nov 1985, Sunday
    // Time widget at (66,12), textsize 2
    timeWidget = new TimeWidget(66, 12, 48, 20, &_timeService);   // Position (66,12), width 48, height 20
    timeWidget->setText("12:43"); // Placeholder: 12:43
    // Button labels at bottom
    infoButton = new ButtonLabelWidget(0, 64-12, 32, 12, "info", 1);
    backButton = new ButtonLabelWidget(96, 64-12, 32, 12, "back", 1);
    // Add widgets
    addWidget(alarmWidget);
    addWidget(dateWidget);
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
    // Keypad D: update time from timeService
    if (eventType == 'D') {
        if (timeWidget) {
            timeWidget->setText("");
        }
        return true;
    }

    // Keypad B: enter alarm setting mode
    if (eventType == 'B' && !settingAlarm) {
        settingAlarm = true;
        // Start with current alarm or default
        const auto& alarms = _alarmService.getAlarms();
        if (!alarms.empty()) {
            alarmHour = alarms[0].hour;
            alarmMinute = alarms[0].minute;
        } else {
            alarmHour = 6;
            alarmMinute = 0;
        }
        editingHour = true;
        infoButton->setText("save");
        backButton->setText("cancel");
        return true;
    }

    // Rotary encoder: eventType 'E' for rotation, eventData +1/-1
    if (settingAlarm && eventType == 'E') {
        if (editingHour) {
            alarmHour = (alarmHour + eventData + 24) % 24;
        } else {
            alarmMinute = (alarmMinute + eventData + 60) % 60;
        }
        return true;
    }

    // Encoder button: eventType 'P' (press)
    if (settingAlarm && eventType == 'P') {
        editingHour = !editingHour;
        return true;
    }

    // BUTTON1_PIN (26): eventType 'BTN1'
    if (settingAlarm && eventType == 'BTN1') {
        _alarmService.addAlarm(alarmHour, alarmMinute);
        settingAlarm = false;
        infoButton->setText("info");
        backButton->setText("back");
        return true;
    }

    // BUTTON2_PIN (13): eventType 'BTN2'
    if (settingAlarm && eventType == 'BTN2') {
        settingAlarm = false;
        infoButton->setText("info");
        backButton->setText("back");
        return true;
    }

    return false;
}

void ClockScreen::updateWidgets() {
    // Time
    char timeStr[6];
    sprintf(timeStr, "%02d:%02d", _timeService.getHour(), _timeService.getMinute());
    // Alarm
    String alarmText;
    if (settingAlarm) {
        // Show editable alarm time, highlight field
        if (editingHour) {
            alarmText = String("[") + (alarmHour < 10 ? "0" : "") + String(alarmHour) + "]:" + (alarmMinute < 10 ? "0" : "") + String(alarmMinute);
        } else {
            alarmText = (alarmHour < 10 ? "0" : "") + String(alarmHour) + ":[" + (alarmMinute < 10 ? "0" : "") + String(alarmMinute) + "]";
        }
    } else {
        const auto& alarms = _alarmService.getAlarms();
        alarmText = alarms.empty() ? "--:--" : String(alarms[0].hour) + ":" + (alarms[0].minute < 10 ? "0" : "") + String(alarms[0].minute);
    }
    alarmWidget->setText(alarmText);
    // Date
    static const char* days[] = {"SUN","MON","TUE","WED","THU","FRI","SAT"};
    static const char* months[] = {"JAN","FEB","MAR","APR","MAY","JUN","JUL","AUG","SEP","OCT","NOV","DEC"};
    uint8_t dayOfWeek = weekday(_timeService.getUnixTime()) - 1; // Arduino weekday: 1=Sunday
    String dayStr = days[dayOfWeek];
    char dayNum[3];
    sprintf(dayNum, "%02d", _timeService.getDay());
    String monthStr = months[_timeService.getMonth()-1];
    dateWidget->setDate(dayStr, String(dayNum), monthStr);
}
