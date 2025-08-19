
#pragma once
#include <Arduino.h>
#include <vector>
#include "TimeService.h"

struct ScheduleEvent {
	uint16_t year;
	uint8_t month;
	uint8_t day;
	uint8_t hour;
	uint8_t minute;
	String label;
	bool triggered;
};

class ScheduleService {
public:
	ScheduleService(TimeService& timeService);
	void update();

	// Add a new event
	void addEvent(uint16_t year, uint8_t month, uint8_t day,
				  uint8_t hour, uint8_t minute, const String& label = "");
	// Remove event by index
	void removeEvent(size_t index);
	// Get all events
	const std::vector<ScheduleEvent>& getEvents() const;
	// Check if any event is triggered
	bool isEventTriggered(size_t& triggeredIndex) const;
	// Reset event trigger (after user acknowledges)
	void resetEvent(size_t index);

private:
	TimeService& _timeService;
	std::vector<ScheduleEvent> _events;
};
