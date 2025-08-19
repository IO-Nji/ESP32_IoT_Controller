
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

	// Add a new alarm
	void addAlarm(uint8_t hour, uint8_t minute, const String& label = "", bool repeatDaily = false);
	// Remove alarm by index
	void removeAlarm(size_t index);
	// Enable/disable alarm
	void setAlarmEnabled(size_t index, bool enabled);
	// Get all alarms
	const std::vector<Alarm>& getAlarms() const;
	// Check if any alarm is triggered
	bool isAlarmTriggered(size_t& triggeredIndex) const;
	// Reset alarm trigger (after user acknowledges)
	void resetAlarm(size_t index);

private:
	TimeService& _timeService;
	std::vector<Alarm> _alarms;
};
