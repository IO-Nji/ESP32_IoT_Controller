// Include the HAL interfaces
#include "src/hal/display.h"
#include "src/hal/input.h"
#include "src/hal/output.h"
#include "src/hal/sensor.h"
#include "src/hal/hal_config.h"
#include "src/tasks/tasks.h"

// Include the UI framework
#include "src/ui/UIFramework.h"

void setup() {
  // Serial for debugging
  Serial.begin(115200);
  Serial.println("ESP32 IoT Controller initializing...");
  
  // Initialize all hardware through the HAL
  hal_display_init();
  hal_input_init();
  hal_output_init();
  if (!hal_sensor_init()) {
    Serial.println("Failed to initialize MPU6050 sensor");
  }
  
  // Display a startup message
  hal_display_clear(0);
  hal_display_draw_text(0, "ESP32", 0, 0, 1);
  hal_display_draw_text(0, "Controller", 0, 10, 1);
  hal_display_update(0);
  
  hal_display_clear(1);
  hal_display_draw_text(1, "System", 32, 20, 1);
  hal_display_draw_text(1, "Initializing", 32, 30, 1);
  hal_display_update(1);
  
  // Flash the LEDs to indicate startup
  for (int i = 0; i < 3; i++) {
    hal_output_fill_leds(64, 64, 64);
    hal_output_update_leds();
    delay(100);
    hal_output_clear_leds();
    delay(100);
  }
  
  Serial.println("Initialization complete");
}

// Demo variables using HAL
long lastEncoder = 0;
uint8_t ledHue = 0;
uint8_t ledBrightness = 128;
unsigned long lastUpdateTime = 0;

void loop() {
  unsigned long currentTime = millis();
  unsigned long deltaTime = currentTime - lastUpdateTime;
  
  // Update only every 50ms to avoid excessive processing
  if (deltaTime >= 50) {
    lastUpdateTime = currentTime;
    
    // Read encoder through HAL
    long encoder = hal_input_read_encoder();
    if (encoder != lastEncoder) {
      ledHue = (encoder % 256);
      lastEncoder = encoder;
    }
  
    // Read keypad through HAL
    char key = hal_input_read_keypad();
    if (key) {
      if (key == '1') ledBrightness = 32;
      else if (key == '2') ledBrightness = 64;
      else if (key == '3') ledBrightness = 128;
      else if (key == '4') ledBrightness = 255;
      
      // Beep buzzer on keypress
      hal_output_set_buzzer(true);
      delay(50);
      hal_output_set_buzzer(false);
    }
  
    // Update LEDs through HAL
    for (int i = 0; i < 6; i++) {
      hal_output_set_led_hsv(i, (ledHue + i * 20) * 256, 255, ledBrightness);
    }
    hal_output_update_leds();
  
    // Update sensor data
    hal_sensor_update();
    
    // Update displays
    updateDisplays();
  }
}

void updateDisplays() {
  // Display 1 (small vertical display)
  hal_display_clear(0);
  
  // Draw encoder value
  hal_display_draw_text(0, "Enc:", 0, 0, 1);
  hal_display_draw_text(0, String(lastEncoder).c_str(), 0, 10, 1);
  
  // Draw pot value
  int potValue = hal_input_read_pot();
  hal_display_draw_text(0, "Pot:", 0, 30, 1);
  hal_display_draw_text(0, String(potValue).c_str(), 0, 40, 1);
  
  // Draw button states
  hal_display_draw_text(0, "Btn:", 0, 60, 1);
  hal_display_draw_text(0, hal_input_read_button(1) ? "1" : "0", 0, 70, 1);
  hal_display_draw_text(0, hal_input_read_button(2) ? "1" : "0", 10, 70, 1);
  
  hal_display_update(0);
  
  // Display 2 (large horizontal display)
  hal_display_clear(1);
  
  // Draw title
  hal_display_draw_text(1, "ESP32 IoT Controller", 10, 0, 1);
  hal_display_draw_line(1, 0, 10, 128, 10, 1);
  
  // Draw accelerometer values
  float accelX = hal_sensor_read_accel_x();
  float accelY = hal_sensor_read_accel_y();
  float accelZ = hal_sensor_read_accel_z();
  
  hal_display_draw_text(1, "Accel:", 0, 15, 1);
  hal_display_draw_text(1, ("X: " + String(accelX, 1)).c_str(), 50, 15, 1);
  hal_display_draw_text(1, ("Y: " + String(accelY, 1)).c_str(), 50, 25, 1);
  hal_display_draw_text(1, ("Z: " + String(accelZ, 1)).c_str(), 50, 35, 1);
  
  // Draw joystick values
  int joyX = hal_input_read_joystick_x();
  int joyY = hal_input_read_joystick_y();
  
  hal_display_draw_text(1, "Joy:", 0, 45, 1);
  hal_display_draw_text(1, ("X: " + String(joyX)).c_str(), 50, 45, 1);
  hal_display_draw_text(1, ("Y: " + String(joyY)).c_str(), 50, 55, 1);
  
  hal_display_update(1);
}
