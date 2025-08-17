#include "TimeService.h"
#include <time.h>

TimeService::TimeService() : _currentTime(0) {}

void TimeService::begin() {
    // Optionally initialize RTC or NTP here
    _currentTime = time(nullptr);
}

void TimeService::update() {
    // For system time, just update from time(nullptr)
    _currentTime = time(nullptr);
}

uint8_t TimeService::getHour() const {
    struct tm* t = localtime(&_currentTime);
    return t ? t->tm_hour : 0;
}

uint8_t TimeService::getMinute() const {
    struct tm* t = localtime(&_currentTime);
    return t ? t->tm_min : 0;
}

uint8_t TimeService::getSecond() const {
    struct tm* t = localtime(&_currentTime);
    return t ? t->tm_sec : 0;
}

uint16_t TimeService::getYear() const {
    struct tm* t = localtime(&_currentTime);
    return t ? (t->tm_year + 1900) : 0;
}

uint8_t TimeService::getMonth() const {
    struct tm* t = localtime(&_currentTime);
    return t ? (t->tm_mon + 1) : 0;
}

uint8_t TimeService::getDay() const {
    struct tm* t = localtime(&_currentTime);
    return t ? t->tm_mday : 0;
}

void TimeService::setTime(uint16_t year, uint8_t month, uint8_t day,
                          uint8_t hour, uint8_t minute, uint8_t second) {
    struct tm t;
    t.tm_year = year - 1900;
    t.tm_mon = month - 1;
    t.tm_mday = day;
    t.tm_hour = hour;
    t.tm_min = minute;
    t.tm_sec = second;
    t.tm_isdst = -1;
    _currentTime = mktime(&t);
}

void TimeService::setUnixTime(uint32_t timestamp) {
    _currentTime = timestamp;
}

uint32_t TimeService::getUnixTime() const {
    return (uint32_t)_currentTime;
}

void TimeService::syncWithRTC() {
    // TODO: Implement RTC sync if hardware available
}

void TimeService::syncWithNTP() {
    // TODO: Implement NTP sync if network available
}
