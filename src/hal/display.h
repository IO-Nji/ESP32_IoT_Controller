#ifndef HAL_DISPLAY_H
#define HAL_DISPLAY_H

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

/**
 * @file display.h
 * @brief Display HAL interface
 * 
 * Hardware abstraction for display operations.
 */

// Get display objects for direct access if needed
Adafruit_SSD1306* hal_display_get_display1();
Adafruit_SSD1306* hal_display_get_display2();

// Basic HAL functions
void hal_display_init();
void hal_display_clear(uint8_t display_id);
void hal_display_update(uint8_t display_id);
void hal_display_clear_all();
void hal_display_update_all();

// Drawing functions
void hal_display_draw_text(uint8_t display_id, const char* text, int16_t x, int16_t y, uint8_t text_size = 1);
void hal_display_set_text_color(uint8_t display_id, uint16_t color);
void hal_display_set_cursor(uint8_t display_id, int16_t x, int16_t y);
void hal_display_set_text_size(uint8_t display_id, uint8_t size);
void hal_display_set_rotation(uint8_t display_id, uint8_t rotation);
void hal_display_draw_pixel(uint8_t display_id, int16_t x, int16_t y, uint16_t color);
void hal_display_draw_line(uint8_t display_id, int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color);
void hal_display_draw_rect(uint8_t display_id, int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
void hal_display_fill_rect(uint8_t display_id, int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
void hal_display_draw_circle(uint8_t display_id, int16_t x0, int16_t y0, int16_t r, uint16_t color);

// Display properties
int16_t hal_display_get_width(uint8_t display_id);
int16_t hal_display_get_height(uint8_t display_id);

#endif // HAL_DISPLAY_H
