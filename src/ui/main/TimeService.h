#pragma once
#include <Arduino.h>

class TimeService {
public:
    TimeService();
    void begin();
    void update();

    uint8_t getHour() const;
    uint8_t getMinute() const;
    uint8_t getSecond() const;
    uint16_t getYear() const;
    uint8_t getMonth() const;
    uint8_t getDay() const;

    void setTime(uint16_t year, uint8_t month, uint8_t day,
                 uint8_t hour, uint8_t minute, uint8_t second);
};
