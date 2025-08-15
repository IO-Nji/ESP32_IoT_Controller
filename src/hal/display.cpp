#include "display.h"
#include "hal_config.h"
#include <Wire.h>

// Define display objects internally to the HAL
static Adafruit_SSD1306 display1(SCREEN_WIDTH, SCREEN_HEIGHT_DISP1, &Wire, -1);
static Adafruit_SSD1306 display2(SCREEN_WIDTH, SCREEN_HEIGHT_DISP2, &Wire, -1);

// Get display objects for direct access if needed
Adafruit_SSD1306* hal_display_get_display1() {
    return &display1;
}

Adafruit_SSD1306* hal_display_get_display2() {
    return &display2;
}

void hal_display_init() {
    // Initialize I2C bus
    Wire.begin(I2C_SDA_PIN, I2C_SCL_PIN);
    
    // Initialize display 1 (128x32)
    if (!display1.begin(SSD1306_SWITCHCAPVCC, SCREEN1_ADDR)) {
        // Handle error (could add a logging system later)
    }
    display1.setRotation(SCREEN1_ROTATION);
    display1.clearDisplay();
    display1.setTextColor(SSD1306_WHITE);
    display1.setTextSize(1);
    display1.display();
    
    // Initialize display 2 (128x64)
    if (!display2.begin(SSD1306_SWITCHCAPVCC, SCREEN2_ADDR)) {
        // Handle error (could add a logging system later)
    }
    display2.clearDisplay();
    display2.setTextColor(SSD1306_WHITE);
    display2.setTextSize(1);
    display2.display();
}

void hal_display_clear(uint8_t display_id) {
    if (display_id == 0) {
        display1.clearDisplay();
    } else if (display_id == 1) {
        display2.clearDisplay();
    }
}

void hal_display_update(uint8_t display_id) {
    if (display_id == 0) {
        display1.display();
    } else if (display_id == 1) {
        display2.display();
    }
}

void hal_display_clear_all() {
    display1.clearDisplay();
    display2.clearDisplay();
}

void hal_display_update_all() {
    display1.display();
    display2.display();
}

void hal_display_draw_text(uint8_t display_id, const char* text, int16_t x, int16_t y, uint8_t text_size) {
    if (display_id == 0) {
        display1.setTextSize(text_size);
        display1.setCursor(x, y);
        display1.print(text);
    } else if (display_id == 1) {
        display2.setTextSize(text_size);
        display2.setCursor(x, y);
        display2.print(text);
    }
}

void hal_display_set_text_color(uint8_t display_id, uint16_t color) {
    if (display_id == 0) {
        display1.setTextColor(color);
    } else if (display_id == 1) {
        display2.setTextColor(color);
    }
}

void hal_display_set_cursor(uint8_t display_id, int16_t x, int16_t y) {
    if (display_id == 0) {
        display1.setCursor(x, y);
    } else if (display_id == 1) {
        display2.setCursor(x, y);
    }
}

void hal_display_set_text_size(uint8_t display_id, uint8_t size) {
    if (display_id == 0) {
        display1.setTextSize(size);
    } else if (display_id == 1) {
        display2.setTextSize(size);
    }
}

void hal_display_set_rotation(uint8_t display_id, uint8_t rotation) {
    if (display_id == 0) {
        display1.setRotation(rotation);
    } else if (display_id == 1) {
        display2.setRotation(rotation);
    }
}

void hal_display_draw_pixel(uint8_t display_id, int16_t x, int16_t y, uint16_t color) {
    if (display_id == 0) {
        display1.drawPixel(x, y, color);
    } else if (display_id == 1) {
        display2.drawPixel(x, y, color);
    }
}

void hal_display_draw_line(uint8_t display_id, int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color) {
    if (display_id == 0) {
        display1.drawLine(x0, y0, x1, y1, color);
    } else if (display_id == 1) {
        display2.drawLine(x0, y0, x1, y1, color);
    }
}

void hal_display_draw_rect(uint8_t display_id, int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) {
    if (display_id == 0) {
        display1.drawRect(x, y, w, h, color);
    } else if (display_id == 1) {
        display2.drawRect(x, y, w, h, color);
    }
}

void hal_display_fill_rect(uint8_t display_id, int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) {
    if (display_id == 0) {
        display1.fillRect(x, y, w, h, color);
    } else if (display_id == 1) {
        display2.fillRect(x, y, w, h, color);
    }
}

void hal_display_draw_circle(uint8_t display_id, int16_t x0, int16_t y0, int16_t r, uint16_t color) {
    if (display_id == 0) {
        display1.drawCircle(x0, y0, r, color);
    } else if (display_id == 1) {
        display2.drawCircle(x0, y0, r, color);
    }
}

int16_t hal_display_get_width(uint8_t display_id) {
    if (display_id == 0) {
        return display1.width();
    } else if (display_id == 1) {
        return display2.width();
    }
    return 0;
}

int16_t hal_display_get_height(uint8_t display_id) {
    if (display_id == 0) {
        return display1.height();
    } else if (display_id == 1) {
        return display2.height();
    }
    return 0;
}
