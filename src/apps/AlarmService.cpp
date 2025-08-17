#include "AlarmService.h"

AlarmService::AlarmService(TimeService& timeService) : _timeService(timeService) {}

void AlarmService::addAlarm(uint8_t hour, uint8_t minute, const String& label, bool repeatDaily) {
    Alarm alarm;
    alarm.hour = hour;
    alarm.minute = minute;
    alarm.enabled = true;
    alarm.label = label;
    alarm.repeatDaily = repeatDaily;
    alarm.triggered = false;
    _alarms.push_back(alarm);
}

void AlarmService::removeAlarm(size_t index) {
    if (index < _alarms.size()) {
        _alarms.erase(_alarms.begin() + index);
    }
}

void AlarmService::setAlarmEnabled(size_t index, bool enabled) {
    if (index < _alarms.size()) {
        _alarms[index].enabled = enabled;
    }
}

const std::vector<Alarm>& AlarmService::getAlarms() const {
    return _alarms;
}

bool AlarmService::isAlarmTriggered(size_t& triggeredIndex) const {
    uint8_t currentHour = _timeService.getHour();
    uint8_t currentMinute = _timeService.getMinute();
    for (size_t i = 0; i < _alarms.size(); ++i) {
        const Alarm& alarm = _alarms[i];
        if (alarm.enabled && !alarm.triggered &&
            alarm.hour == currentHour && alarm.minute == currentMinute) {
            triggeredIndex = i;
            return true;
        }
    }
    return false;
}

void AlarmService::resetAlarm(size_t index) {
    if (index < _alarms.size()) {
        _alarms[index].triggered = false;
        // If not repeating, disable after trigger
        if (!_alarms[index].repeatDaily) {
            _alarms[index].enabled = false;
        }
    }
}

void AlarmService::update() {
    uint8_t currentHour = _timeService.getHour();
    uint8_t currentMinute = _timeService.getMinute();
    for (size_t i = 0; i < _alarms.size(); ++i) {
        Alarm& alarm = _alarms[i];
        if (alarm.enabled && !alarm.triggered &&
            alarm.hour == currentHour && alarm.minute == currentMinute) {
            alarm.triggered = true;
        }
        // Reset trigger if time has passed
        if (alarm.triggered && (alarm.hour != currentHour || alarm.minute != currentMinute)) {
            alarm.triggered = false;
        }
    }
}
