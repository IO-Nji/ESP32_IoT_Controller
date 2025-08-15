#ifndef HAL_OUTPUT_H
#define HAL_OUTPUT_H

#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

/**
 * @file output.h
 * @brief Output HAL interface
 * 
 * Hardware abstraction for output devices (buzzer, LEDs, etc.)
 */

// Get access to raw output objects if needed
Adafruit_NeoPixel* hal_output_get_leds();

/**
 * Initialize all output devices (buzzer, LEDs).
 */
void hal_output_init();

/**
 * Set the state of the buzzer.
 * @param on True to turn on, false to turn off
 */
void hal_output_set_buzzer(bool on);

/**
 * Set the PWM duty cycle for the buzzer (for tones).
 * @param duty_cycle PWM duty cycle (0-255)
 */
void hal_output_set_buzzer_duty(uint8_t duty_cycle);

/**
 * Play a tone on the buzzer for a duration.
 * @param frequency Frequency in Hz
 * @param duration Duration in milliseconds
 */
void hal_output_play_tone(unsigned int frequency, unsigned long duration);

/**
 * Set the color of a specific RGB LED.
 * @param led_id LED index (0 to NUM_LEDS-1)
 * @param r Red component (0-255)
 * @param g Green component (0-255)
 * @param b Blue component (0-255)
 */
void hal_output_set_led(uint8_t led_id, uint8_t r, uint8_t g, uint8_t b);

/**
 * Set the color of a specific RGB LED using an RGB packed value.
 * @param led_id LED index (0 to NUM_LEDS-1)
 * @param color Packed color value (0xRRGGBB)
 */
void hal_output_set_led_color(uint8_t led_id, uint32_t color);

/**
 * Set the color of a specific RGB LED using HSV values.
 * @param led_id LED index (0 to NUM_LEDS-1)
 * @param hue Hue (0-65535)
 * @param saturation Saturation (0-255)
 * @param value Brightness (0-255)
 */
void hal_output_set_led_hsv(uint8_t led_id, uint16_t hue, uint8_t saturation, uint8_t value);

/**
 * Set the brightness for all LEDs.
 * @param brightness Brightness value (0-255)
 */
void hal_output_set_led_brightness(uint8_t brightness);

/**
 * Fill all LEDs with the same color.
 * @param r Red component (0-255)
 * @param g Green component (0-255)
 * @param b Blue component (0-255)
 */
void hal_output_fill_leds(uint8_t r, uint8_t g, uint8_t b);

/**
 * Update the LED strip (must be called after setting colors).
 */
void hal_output_update_leds();

/**
 * Clear all LEDs (turn off).
 */
void hal_output_clear_leds();

#endif // HAL_OUTPUT_H
