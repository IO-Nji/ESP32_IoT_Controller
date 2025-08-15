#ifndef HAL_SENSOR_H
#define HAL_SENSOR_H

// Sensor HAL interface
void hal_sensor_init();
float hal_sensor_read_accel_x();
float hal_sensor_read_accel_y();
float hal_sensor_read_accel_z();
float hal_sensor_read_gyro_x();
float hal_sensor_read_gyro_y();
float hal_sensor_read_gyro_z();

#endif // HAL_SENSOR_H
