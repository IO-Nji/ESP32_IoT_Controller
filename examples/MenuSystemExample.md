// Example implementation for the ESP32 IoT Controller menu system
// This file demonstrates how to create and use the MenuWidget

#include "src/ui/UIFramework.h"
#include "src/hal/display.h"
#include "src/hal/input.h"
#include "src/hal/output.h"
#include "src/hal/sensor.h"

// UI Manager instance
UIManager* uiManager;

// Screen IDs
#define MAIN_SCREEN_ID 0
#define SETTINGS_SCREEN_ID 1
#define SENSORS_SCREEN_ID 2

// Widgets
MenuWidget* mainMenu;
LabelWidget* statusLabel;

// Forward declarations of menu callbacks
void onSettingsSelected(MenuWidget* menu);
void onSensorsSelected(MenuWidget* menu);
void onWifiSelected(MenuWidget* menu);
void onLEDsSelected(MenuWidget* menu);
void onAboutSelected(MenuWidget* menu);

// Initialize the UI
void initUI() {
  // Get raw display objects from HAL
  Adafruit_SSD1306* display1 = hal_display_get_display1();
  Adafruit_SSD1306* display2 = hal_display_get_display2();
  
  // Create UI manager with both displays
  uiManager = new UIManager(*display1, *display2);
  
  // Create screens for each display
  Screen* mainScreen = uiManager->createScreen(0, MAIN_SCREEN_ID); // Display 1 (128x32 vertical)
  Screen* infoScreen = uiManager->createScreen(1, MAIN_SCREEN_ID); // Display 2 (128x64 horizontal)
  
  // Create a settings screen
  Screen* settingsScreen = uiManager->createScreen(1, SETTINGS_SCREEN_ID);
  
  // Create a sensors screen
  Screen* sensorsScreen = uiManager->createScreen(1, SENSORS_SCREEN_ID);
  
  // Create menu widget for vertical display (adjust position as needed)
  mainMenu = new MenuWidget(0, 0, 32, 128, "MENU");
  
  // Add menu items
  mainMenu->addItem("Settings", onSettingsSelected);
  mainMenu->addItem("Sensors", onSensorsSelected);
  mainMenu->addItem("WiFi", onWifiSelected);
  mainMenu->addItem("LEDs", onLEDsSelected);
  mainMenu->addItem("About", onAboutSelected);
  
  // Add menu to main screen
  mainScreen->addWidget(mainMenu);
  
  // Create a status label for the horizontal display
  statusLabel = new LabelWidget(0, 25, 128, 16, "Main Menu", 1, LabelWidget::Alignment::CENTER);
  infoScreen->addWidget(statusLabel);
  
  // Create settings screen widgets (example)
  LabelWidget* settingsTitle = new LabelWidget(0, 5, 128, 16, "Settings", 1, LabelWidget::Alignment::CENTER);
  LabelWidget* settingsInfo = new LabelWidget(5, 25, 118, 30, "Use encoder to adjust settings and buttons to select", 1, LabelWidget::Alignment::LEFT);
  settingsScreen->addWidget(settingsTitle);
  settingsScreen->addWidget(settingsInfo);
  
  // Create sensors screen widgets (example)
  LabelWidget* sensorsTitle = new LabelWidget(0, 5, 128, 16, "Sensors", 1, LabelWidget::Alignment::CENTER);
  LabelWidget* accelLabel = new LabelWidget(5, 25, 118, 10, "Accel: 0, 0, 0", 1, LabelWidget::Alignment::LEFT);
  LabelWidget* tempLabel = new LabelWidget(5, 40, 118, 10, "Temp: 25C", 1, LabelWidget::Alignment::LEFT);
  sensorsScreen->addWidget(sensorsTitle);
  sensorsScreen->addWidget(accelLabel);
  sensorsScreen->addWidget(tempLabel);
  
  // Activate the main screens initially
  uiManager->setActiveScreen(0, MAIN_SCREEN_ID);
  uiManager->setActiveScreen(1, MAIN_SCREEN_ID);
  
  // Force an immediate update of the displays
  uiManager->update(0);
  uiManager->render();
}

// Menu item callback functions
void onSettingsSelected(MenuWidget* menu) {
  uiManager->setActiveScreen(1, SETTINGS_SCREEN_ID);
  statusLabel->setText("Settings Menu");
  
  // Provide feedback
  hal_output_set_buzzer(true);
  delay(50);
  hal_output_set_buzzer(false);
}

void onSensorsSelected(MenuWidget* menu) {
  uiManager->setActiveScreen(1, SENSORS_SCREEN_ID);
  statusLabel->setText("Sensor Readings");
  
  // Provide feedback
  hal_output_set_buzzer(true);
  delay(50);
  hal_output_set_buzzer(false);
}

void onWifiSelected(MenuWidget* menu) {
  statusLabel->setText("WiFi Selected");
  
  // Provide feedback
  hal_output_set_buzzer(true);
  delay(50);
  hal_output_set_buzzer(false);
}

void onLEDsSelected(MenuWidget* menu) {
  statusLabel->setText("LEDs Selected");
  
  // Provide feedback
  hal_output_set_buzzer(true);
  delay(50);
  hal_output_set_buzzer(false);
  
  // Example: Cycle through some LED patterns
  static uint8_t ledPattern = 0;
  ledPattern = (ledPattern + 1) % 4;
  
  switch(ledPattern) {
    case 0:
      for (int i = 0; i < 6; i++) {
        hal_output_set_led_hsv(i, i * 40 * 256, 255, 128);
      }
      break;
    case 1:
      for (int i = 0; i < 6; i++) {
        hal_output_set_led_hsv(i, 0 * 256, 255, (i * 40) + 20);
      }
      break;
    case 2:
      for (int i = 0; i < 6; i++) {
        hal_output_set_led_hsv(i, 240 * 256, 255, 128);
      }
      break;
    case 3:
      for (int i = 0; i < 6; i++) {
        hal_output_set_led_hsv(i, i % 2 ? 0 : 100 * 256, 255, 128);
      }
      break;
  }
  hal_output_update_leds();
}

void onAboutSelected(MenuWidget* menu) {
  statusLabel->setText("ESP32 IoT v1.0");
  
  // Provide feedback
  hal_output_set_buzzer(true);
  delay(50);
  hal_output_set_buzzer(false);
}

// Process encoder input for menu navigation
void processMenuInput() {
  // Read encoder change since last time
  static long lastEncoderValue = 0;
  long encoderValue = hal_input_read_encoder();
  
  // If encoder changed, navigate the menu
  if (encoderValue > lastEncoderValue) {
    mainMenu->navigateNext();
    lastEncoderValue = encoderValue;
  } else if (encoderValue < lastEncoderValue) {
    mainMenu->navigatePrevious();
    lastEncoderValue = encoderValue;
  }
  
  // Check if encoder button was pressed
  if (hal_input_read_encoder_btn()) {  // Note: Changed to correct function name
    mainMenu->selectCurrentItem();
    delay(200); // Debounce
  }
  
  // Check if a button was pressed to return to the main menu
  if (hal_input_read_button(1)) {
    uiManager->setActiveScreen(1, MAIN_SCREEN_ID);
    statusLabel->setText("Main Menu");
    delay(200); // Debounce
  }
}

void setup() {
  // Serial for debugging
  Serial.begin(115200);
  Serial.println("ESP32 IoT Controller with Menu initializing...");
  
  // Initialize all hardware through the HAL
  hal_display_init();
  hal_input_init();
  hal_output_init();
  hal_sensor_init();
  
  // Display a startup message
  hal_display_clear(0);
  hal_display_draw_text(0, "ESP32", 0, 0, 1);
  hal_display_draw_text(0, "Controller", 0, 10, 1);
  hal_display_update(0);
  
  hal_display_clear(1);
  hal_display_draw_text(1, "System", 32, 20, 1);
  hal_display_draw_text(1, "Initializing", 32, 30, 1);
  hal_display_update(1);
  
  delay(1000); // Show startup message briefly
  
  // Initialize UI components
  initUI();
  
  Serial.println("Menu system initialized");
}

void loop() {
  // Timing control
  static unsigned long lastUpdateTime = 0;
  static unsigned long lastUIRefreshTime = 0;
  unsigned long currentTime = millis();
  
  // Process inputs and update UI at 50Hz
  if (currentTime - lastUpdateTime >= 20) {
    lastUpdateTime = currentTime;
    
    // Process menu navigation input
    processMenuInput();
    
    // Update sensor data if needed
    hal_sensor_update();
  }
  
  // Render UI at 30Hz
  if (currentTime - lastUIRefreshTime >= 33) {
    lastUIRefreshTime = currentTime;
    
    // Calculate delta time for animations (if needed)
    unsigned long deltaTime = currentTime - lastUIRefreshTime;
    
    // Update widget states
    uiManager->update(deltaTime);
    
    // Render the displays
    uiManager->render();
  }
}
