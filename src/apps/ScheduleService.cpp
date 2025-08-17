#include "ScheduleService.h"

ScheduleService::ScheduleService(TimeService& timeService) : _timeService(timeService) {}

void ScheduleService::addEvent(uint16_t year, uint8_t month, uint8_t day,
                               uint8_t hour, uint8_t minute, const String& label) {
    ScheduleEvent event;
    event.year = year;
    event.month = month;
    event.day = day;
    event.hour = hour;
    event.minute = minute;
    event.label = label;
    event.triggered = false;
    _events.push_back(event);
}

void ScheduleService::removeEvent(size_t index) {
    if (index < _events.size()) {
        _events.erase(_events.begin() + index);
    }
}

const std::vector<ScheduleEvent>& ScheduleService::getEvents() const {
    return _events;
}

bool ScheduleService::isEventTriggered(size_t& triggeredIndex) const {
    uint16_t currentYear = _timeService.getYear();
    uint8_t currentMonth = _timeService.getMonth();
    uint8_t currentDay = _timeService.getDay();
    uint8_t currentHour = _timeService.getHour();
    uint8_t currentMinute = _timeService.getMinute();
    for (size_t i = 0; i < _events.size(); ++i) {
        const ScheduleEvent& event = _events[i];
        if (!event.triggered &&
            event.year == currentYear && event.month == currentMonth && event.day == currentDay &&
            event.hour == currentHour && event.minute == currentMinute) {
            triggeredIndex = i;
            return true;
        }
    }
    return false;
}

void ScheduleService::resetEvent(size_t index) {
    if (index < _events.size()) {
        _events[index].triggered = false;
    }
}

void ScheduleService::update() {
    uint16_t currentYear = _timeService.getYear();
    uint8_t currentMonth = _timeService.getMonth();
    uint8_t currentDay = _timeService.getDay();
    uint8_t currentHour = _timeService.getHour();
    uint8_t currentMinute = _timeService.getMinute();
    for (size_t i = 0; i < _events.size(); ++i) {
        ScheduleEvent& event = _events[i];
        if (!event.triggered &&
            event.year == currentYear && event.month == currentMonth && event.day == currentDay &&
            event.hour == currentHour && event.minute == currentMinute) {
            event.triggered = true;
        }
        // Reset trigger if time has passed
        if (event.triggered &&
            (event.year != currentYear || event.month != currentMonth || event.day != currentDay ||
             event.hour != currentHour || event.minute != currentMinute)) {
            event.triggered = false;
        }
    }
}
