// Global flag for encoder button press
bool encoderButtonPressed = false;
/**
 * ESP32 IoT Controller
 * 
 * Main application file for the ESP32 IoT Controller.
 * This file initializes the hardware and starts the system tasks.
 */

// Include HAL interfaces
#include "src/hal/display.h"
#include "src/hal/input.h"
#include "src/hal/output.h"
#include "src/hal/sensor.h"
#include "src/hal/hal_config.h"

// Include refactored components
#include "src/ui/main/MainUI.h"
#include "src/tasks/TaskManager.h"
#include "src/ui/logo/logo.h"

// Main UI instance
MainUI* mainUI;

void setup() {
  // Initialize serial for debugging
  Serial.begin(115200);
  Serial.println("ESP32 IoT Controller initializing...");
  
  // Initialize hardware through HAL
  hal_display_init();
  hal_input_init();
  hal_output_init();
  hal_sensor_init();
  
  // Get display references
  Adafruit_SSD1306* display1 = hal_display_get_display1();
  Adafruit_SSD1306* display2 = hal_display_get_display2();

  // Display startup message on display1
  hal_display_clear(0);
  hal_display_draw_text(0, "ESP32", 0, 0, 1);
  hal_display_draw_text(0, "IoT Control", 0, 10, 1);
  hal_display_update(0);



  // Animated logo on display2
  logoDisplay(*display2,12,4);
  display2->setTextSize(1);
  display2->setTextColor(WHITE);
  display2->setCursor(10, 42);
  display2->print("System");
  display2->setCursor(10, 52);
  display2->print("Initialization");
  display2->display();
  delay(1000); // Show for 1 second
  
  // Initialize UI components
  mainUI = new MainUI(*display1, *display2);
  mainUI->setup();
  
  // Start task system
  TaskManager::init(mainUI);
  
  Serial.println("System initialization complete");
}

void loop() {
  // Main loop is mostly empty since tasks handle everything
  // This can be used for watchdog functionality or other system-level operations
  delay(1000);
}
