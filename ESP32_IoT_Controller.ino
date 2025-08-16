// Include the HAL interfaces
#include "src/hal/display.h"
#include "src/hal/input.h"
#include "src/hal/output.h"
#include "src/hal/sensor.h"
#include "src/hal/hal_config.h"
#include "src/tasks/tasks.h"

// Include the UI framework
#include "src/ui/UIFramework.h"

// UI Manager instance to control both displays
UIManager* uiManager;

// Screen IDs for different screens
#define MAIN_SCREEN_ID 0

// AppTitleWidget for Display 1 (vertical 128x32)
AppTitleWidget* appTitle;

// Initialize the UI widgets and screens
void initUI() {
  // Get raw display objects from HAL
  Adafruit_SSD1306* display1 = hal_display_get_display1();
  Adafruit_SSD1306* display2 = hal_display_get_display2();
  
  // Create UI manager with both displays
  uiManager = new UIManager(*display1, *display2);
  
  // Create screens for each display
  Screen* mainScreen1 = uiManager->createScreen(0, MAIN_SCREEN_ID); // Display 1, Main Screen
  Screen* mainScreen2 = uiManager->createScreen(1, MAIN_SCREEN_ID); // Display 2, Main Screen
  
  // Create AppTitleWidget for Display 1 (vertical 32x128)
  // The widget will display "ESP32 IOT" and an abbreviated "EI" below it
  appTitle = new AppTitleWidget(0, 10, 32, 50, "ESP32 IOT");
  
  // Add widget to screen 1
  mainScreen1->addWidget(appTitle);
  
  // Create a simple label for Display 2 to show it's working
  LabelWidget* infoLabel = new LabelWidget(0, 25, 128, 16, "AppTitleWidget Demo", 1, LabelWidget::Alignment::CENTER);
  
  // Add widget to screen 2
  mainScreen2->addWidget(infoLabel);
  
  // Activate the main screens
  uiManager->setActiveScreen(0, MAIN_SCREEN_ID);
  uiManager->setActiveScreen(1, MAIN_SCREEN_ID);
}

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
  
  // Display a startup message directly with HAL
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
  
  // Initialize UI components
  initUI();
  
  Serial.println("Initialization complete");
}

// Demo function to update the app title periodically
void updateAppTitle() {
  // Titles to cycle through for demonstration
  static const char* titles[] = {
    "ESP32 IOT", 
    "DASHBOARD", 
    "SETTINGS", 
    "SENSOR DATA"
  };
  static uint8_t titleIndex = 0;
  static unsigned long lastTitleChange = 0;
  
  // Change title every 3 seconds
  if (millis() - lastTitleChange > 3000) {
    titleIndex = (titleIndex + 1) % 4;
    appTitle->setTitle(titles[titleIndex]);
    
    // Provide feedback
    hal_output_set_buzzer(true);
    delay(50);
    hal_output_set_buzzer(false);
    
    lastTitleChange = millis();
  }
}

// Process user input for UI interactions
void processInput() {
  // Handle physical button inputs
  bool button1State = hal_input_read_button(1);
  bool button2State = hal_input_read_button(2);
  
  // Use buttons for LED color effects
  static uint8_t ledHue = 0;
  
  if (button1State) {
    ledHue = (ledHue + 10) % 256;
    
    // Provide feedback
    hal_output_set_buzzer(true);
    delay(50);
    hal_output_set_buzzer(false);
  }
  
  if (button2State) {
    // Update all LEDs with the current hue
    for (int i = 0; i < 6; i++) {
      hal_output_set_led_hsv(i, ledHue * 256, 255, 128);
    }
    hal_output_update_leds();
  }
}

void loop() {
  // Timing control
  static unsigned long lastUpdateTime = 0;
  static unsigned long lastUIRefreshTime = 0;
  unsigned long currentTime = millis();
  
  // Process hardware inputs and update widgets (run at 50Hz)
  if (currentTime - lastUpdateTime >= 20) {
    lastUpdateTime = currentTime;
    
    // Process UI input events
    processInput();
    
    // Update the app title periodically
    updateAppTitle();
    
    // Update sensor data
    hal_sensor_update();
  }
  
  // Update UI at 30Hz (separate from sensor/input processing)
  if (currentTime - lastUIRefreshTime >= 33) {
    lastUIRefreshTime = currentTime;
    
    // Calculate delta time for animations (if needed)
    unsigned long deltaTime = currentTime - lastUIRefreshTime;
    
    // Render both displays through UI manager
    uiManager->update(deltaTime);
  }
}
