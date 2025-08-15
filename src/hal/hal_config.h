#ifndef HAL_CONFIG_H
#define HAL_CONFIG_H

/**
 * @file hal_config.h
 * @brief Hardware configuration and pin assignments
 * 
 * This file contains all hardware-specific constants, pin assignments,
 * and configuration values for the ESP32 IoT Controller project.
 */

#include <Arduino.h>

// Display configurations
#define SCREEN1_ADDR 0x3C
#define SCREEN2_ADDR 0x3D
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT_DISP1 32    // First display is 128x32
#define SCREEN_HEIGHT_DISP2 64    // Second display is 128x64
#define SCREEN1_ROTATION 3        // Vertical orientation for first display

// Input pin definitions
#define BUTTON1_PIN 26
#define BUTTON2_PIN 13
#define JOYSTICK_X_PIN 35
#define JOYSTICK_Y_PIN 39
#define POT_PIN 36
#define ENCODER_SW1_PIN 34
#define ENCODER_SW2_PIN 33
#define ENCODER_BTN_PIN 14

// Keypad configuration
#define KEYPAD_ROWS 4
#define KEYPAD_COLS 4
const uint8_t KEYPAD_ROW_PINS[KEYPAD_ROWS] = {12, 32, 25, 27};
const uint8_t KEYPAD_COL_PINS[KEYPAD_COLS] = {15, 2, 0, 4};
const char KEYPAD_KEYS[KEYPAD_ROWS][KEYPAD_COLS] = {
    {'1','2','3','A'},
    {'4','5','6','B'},
    {'7','8','9','C'},
    {'*','0','#','D'}
};

// Output definitions
#define BUZZER_PIN 17
#define LED_PIN 16
#define NUM_LEDS 6

// I2C configuration
#define I2C_SDA_PIN 21
#define I2C_SCL_PIN 22

// SPI configuration (for future use)
#define SPI_MOSI_PIN 23
#define SPI_MISO_PIN 19
#define SPI_SCK_PIN 18
#define SPI_CS_PIN 5

// MPU6050 sensor configuration
#define MPU6050_ADDR 0x68

#endif // HAL_CONFIG_H
