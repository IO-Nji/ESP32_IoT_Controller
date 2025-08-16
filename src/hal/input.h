#ifndef HAL_INPUT_H
#define HAL_INPUT_H

#include <Arduino.h>
#include <Keypad.h>
#include <ESP32Encoder.h>

/**
 * @file input.h
 * @brief Input HAL interface
 * 
 * Hardware abstraction for all input devices (buttons, keypad, joystick, etc.)
 */

// Get access to the raw input device objects if needed
Keypad* hal_input_get_keypad();
ESP32Encoder* hal_input_get_encoder();

/**
 * Initialize all input peripherals (buttons, keypad, joystick, rotary encoder).
 */
void hal_input_init();

/**
 * Read the value from the 4x4 matrix keypad.
 * @return ASCII value of pressed key, or 0 if none.
 */
char hal_input_read_keypad();

/**
 * Read the state of a push button.
 * @param button_id Button number (1 or 2)
 * @return true if pressed, false otherwise.
 */
bool hal_input_read_button(int button_id);

/**
 * Read the analog value of the joystick X axis.
 * @return ADC value (0-4095)
 */
int hal_input_read_joystick_x();

/**
 * Read the analog value of the joystick Y axis.
 * @return ADC value (0-4095)
 */
int hal_input_read_joystick_y();

/**
 * Read the analog value of the rotary potentiometer.
 * @return ADC value (0-4095)
 */
int hal_input_read_pot();

/**
 * Read the position of the rotary encoder.
 * @return Encoder position (long)
 */
long hal_input_read_encoder();

/**
 * Set the rotary encoder position.
 * @param position New encoder position
 */
void hal_input_set_encoder(long position);

/**
 * Read the state of the rotary encoder button.
 * @return true if pressed, false otherwise.
 */
bool hal_input_read_encoder_btn();

/**
 * Get normalized values for controls (range -1.0 to 1.0)
 */
float hal_input_get_joystick_x_normalized();
float hal_input_get_joystick_y_normalized();
float hal_input_get_pot_normalized();

#endif // HAL_INPUT_H
