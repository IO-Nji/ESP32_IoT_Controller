#pragma once
#include <Arduino.h>
#include <vector>
#include "TimeService.h"

struct Alarm {
    uint8_t hour;
    uint8_t minute;
    bool enabled;
    String label;
    bool repeatDaily;
    bool triggered;
};

class AlarmService {
public:
    AlarmService(TimeService& timeService);
    void update();
    void addAlarm(uint8_t hour, uint8_t minute, const String& label = "", bool repeatDaily = false);
    void removeAlarm(size_t index);
    void setAlarmEnabled(size_t index, bool enabled);
    const std::vector<Alarm>& getAlarms() const;
    bool isAlarmTriggered(size_t& triggeredIndex) const;
    void resetAlarm(size_t index);
private:
    TimeService& _timeService;
    std::vector<Alarm> _alarms;
};
