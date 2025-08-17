#include "sensor.h"
#include "hal_config.h"

// Define sensor objects internally to the HAL
static Adafruit_MPU6050 mpu;

// Cached sensor readings
static sensors_event_t accel;
static sensors_event_t gyro;
static sensors_event_t temp;

// Get access to raw sensor objects if needed
Adafruit_MPU6050* hal_sensor_get_mpu() {
    return &mpu;
}

bool hal_sensor_init() {
    // Initialize MPU6050
    if (!mpu.begin(MPU6050_ADDR, &Wire)) {
        return false;
    }
    
    // Configure motion detection
    mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
    mpu.setGyroRange(MPU6050_RANGE_500_DEG);
    mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
    
    // Get initial readings
    hal_sensor_update();
    
    return true;
}

bool hal_sensor_update() {
    try {
        // Get fresh readings from the sensor
        mpu.getEvent(&accel, &gyro, &temp);
        return true;
    } 
    catch (...) {
        // Handle any sensor errors
        return false;
    }
}

float hal_sensor_read_accel_x() {
    return accel.acceleration.x;
}

float hal_sensor_read_accel_y() {
    return accel.acceleration.y;
}

float hal_sensor_read_accel_z() {
    return accel.acceleration.z;
}

float hal_sensor_read_gyro_x() {
    return gyro.gyro.x;
}

float hal_sensor_read_gyro_y() {
    return gyro.gyro.y;
}

float hal_sensor_read_gyro_z() {
    return gyro.gyro.z;
}

float hal_sensor_read_temperature() {
    return temp.temperature;
}

uint8_t hal_sensor_get_orientation() {
    // Simple orientation detection based on acceleration values
    // Assuming the MPU6050 is mounted with X pointing right, Y pointing forward, Z pointing up
    
    float x = accel.acceleration.x;
    float y = accel.acceleration.y;
    float z = accel.acceleration.z;
    
    // Check the predominant axis of gravity
    if (abs(z) > abs(x) && abs(z) > abs(y)) {
        // Flat orientation (Z is dominant)
        if (z > 0) {
            return 0; // Flat, right side up
        } else {
            return 2; // Upside-down
        }
    } else if (abs(y) > abs(x)) {
        // Portrait orientation (Y is dominant)
        if (y > 0) {
            return 1; // Portrait
        } else {
            return 3; // Portrait upside-down
        }
    } else {
        // Landscape orientation (X is dominant)
        if (x > 0) {
            return 4; // Landscape
        } else {
            return 5; // Landscape reverse
        }
    }
}
