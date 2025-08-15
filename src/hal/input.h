#ifndef HAL_INPUT_H
#define HAL_INPUT_H

// Input HAL interface
void hal_input_init();
int hal_input_read_keypad();
bool hal_input_read_button(int button_id);

/**
 * Initialize all input peripherals (buttons, keypad, joystick, rotary encoder).
 */
void hal_input_init();

/**
 * Read the value from the 4x4 matrix keypad.
 * @return ASCII value of pressed key, or 0 if none.
 */
int hal_input_read_keypad();

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
 * Read the state of the rotary encoder button.
 * @return true if pressed, false otherwise.
 */
bool hal_input_read_encoder_btn();

#endif // HAL_INPUT_H
