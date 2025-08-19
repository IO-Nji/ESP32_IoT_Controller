
#pragma once
#include <Arduino.h>

class TimeService {
public:
	TimeService();
	void begin();
	void update();

	// Get current time
	uint8_t getHour() const;
	uint8_t getMinute() const;
	uint8_t getSecond() const;
	uint16_t getYear() const;
	uint8_t getMonth() const;
	uint8_t getDay() const;

	// Set time manually
	void setTime(uint16_t year, uint8_t month, uint8_t day,
				 uint8_t hour, uint8_t minute, uint8_t second);

	// Set time from UNIX timestamp
	void setUnixTime(uint32_t timestamp);
	uint32_t getUnixTime() const;

	// Optionally: sync with RTC or NTP
	void syncWithRTC();
	void syncWithNTP();

private:
	time_t _currentTime;
	// Add RTC/NTP integration as needed
};
