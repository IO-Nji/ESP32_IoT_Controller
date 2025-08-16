#include "output.h"
#include "hal_config.h"

// No need to include ESP32-specific headers here
// We'll use a platform-independent approach

// Define output hardware objects internally to the HAL
static Adafruit_NeoPixel leds(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);

// Get access to raw output objects if needed
Adafruit_NeoPixel* hal_output_get_leds() {
    return &leds;
}

void hal_output_init() {
    // Initialize buzzer pin
    pinMode(BUZZER_PIN, OUTPUT);
    digitalWrite(BUZZER_PIN, LOW);
    
    // Initialize LED strip
    leds.begin();
    leds.setBrightness(128); // Default brightness
    leds.clear();
    leds.show();
}

void hal_output_set_buzzer(bool on) {
    digitalWrite(BUZZER_PIN, on ? HIGH : LOW);
}

void hal_output_set_buzzer_duty(uint8_t duty_cycle) {
    // Using analogWrite which works on most Arduino platforms
    // For ESP32, this will be mapped to the appropriate PWM function
    analogWrite(BUZZER_PIN, duty_cycle);
}

void hal_output_play_tone(unsigned int frequency, unsigned long duration) {
    // Use generic Arduino tone function
    // This works on most platforms including ESP32 with the Arduino framework
    tone(BUZZER_PIN, frequency, duration);
    
    // If duration is 0, tone runs until stopped
    if (duration > 0) {
        delay(duration);
        noTone(BUZZER_PIN);
    }
}

void hal_output_set_led(uint8_t led_id, uint8_t r, uint8_t g, uint8_t b) {
    if (led_id < NUM_LEDS) {
        leds.setPixelColor(led_id, r, g, b);
    }
}

void hal_output_set_led_color(uint8_t led_id, uint32_t color) {
    if (led_id < NUM_LEDS) {
        leds.setPixelColor(led_id, color);
    }
}

void hal_output_set_led_hsv(uint8_t led_id, uint16_t hue, uint8_t saturation, uint8_t value) {
    if (led_id < NUM_LEDS) {
        leds.setPixelColor(led_id, leds.ColorHSV(hue, saturation, value));
    }
}

void hal_output_set_led_brightness(uint8_t brightness) {
    leds.setBrightness(brightness);
}

void hal_output_fill_leds(uint8_t r, uint8_t g, uint8_t b) {
    leds.fill(leds.Color(r, g, b), 0, NUM_LEDS);
}

void hal_output_update_leds() {
    leds.show();
}

void hal_output_clear_leds() {
    leds.clear();
    leds.show();
}
