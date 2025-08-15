#ifndef TASKS_H
#define TASKS_H

#include <Arduino.h>
#include <Adafruit_SSD1306.h>
#include <Keypad.h>
#include <Encoder.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_NeoPixel.h>

// Task handles
extern TaskHandle_t displayTaskHandle;
extern TaskHandle_t inputTaskHandle;
extern TaskHandle_t sensorTaskHandle;
extern TaskHandle_t outputTaskHandle;

// Task function prototypes
void TaskDisplay(void *pvParameters);
void TaskInput(void *pvParameters);
void TaskSensor(void *pvParameters);
void TaskOutput(void *pvParameters);

#endif // TASKS_H
