#ifndef HAL_DISPLAY_H
#define HAL_DISPLAY_H

// Display HAL interface
void hal_display_init();
void hal_display_clear();
void hal_display_draw_text(const char* text, int x, int y);

#endif // HAL_DISPLAY_H
