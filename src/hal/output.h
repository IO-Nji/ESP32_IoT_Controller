#ifndef HAL_OUTPUT_H
#define HAL_OUTPUT_H

// Output HAL interface
void hal_output_init();
void hal_output_set_buzzer(bool on);
void hal_output_set_led(int led_id, int r, int g, int b);

#endif // HAL_OUTPUT_H
