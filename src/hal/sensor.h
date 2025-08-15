#ifndef HAL_SENSOR_H
#define HAL_SENSOR_H

#include <Arduino.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>

/**
 * @file sensor.h
 * @brief Sensor HAL interface
 * 
 * Hardware abstraction for sensor devices (accelerometer, gyroscope, etc.)
 */

// Get access to raw sensor objects if needed
Adafruit_MPU6050* hal_sensor_get_mpu();

/**
 * Initialize all sensor devices.
 * @return True if initialization successful, false otherwise
 */
bool hal_sensor_init();

/**
 * Update all sensor readings.
 * Call this before reading values to ensure fresh data.
 */
void hal_sensor_update();

/**
 * Read the acceleration value on the X axis.
 * @return X acceleration in m/s^2
 */
float hal_sensor_read_accel_x();

/**
 * Read the acceleration value on the Y axis.
 * @return Y acceleration in m/s^2
 */
float hal_sensor_read_accel_y();

/**
 * Read the acceleration value on the Z axis.
 * @return Z acceleration in m/s^2
 */
float hal_sensor_read_accel_z();

/**
 * Read the gyroscope value on the X axis.
 * @return X gyroscope value in rad/s
 */
float hal_sensor_read_gyro_x();

/**
 * Read the gyroscope value on the Y axis.
 * @return Y gyroscope value in rad/s
 */
float hal_sensor_read_gyro_y();

/**
 * Read the gyroscope value on the Z axis.
 * @return Z gyroscope value in rad/s
 */
float hal_sensor_read_gyro_z();

/**
 * Read the temperature from the MPU6050.
 * @return Temperature in degrees Celsius
 */
float hal_sensor_read_temperature();

/**
 * Get the current orientation based on accelerometer data.
 * @return 0=flat, 1=portrait, 2=upside-down, 3=landscape, 4=landscape-reverse
 */
uint8_t hal_sensor_get_orientation();

#endif // HAL_SENSOR_H
