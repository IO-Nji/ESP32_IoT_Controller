// Include the HAL interfaces
#include "src/hal/display.h"
#include "src/hal/input.h"
#include "src/hal/output.h"
#include "src/hal/sensor.h"
#include "src/hal/hal_config.h"
#include "src/tasks/tasks.h"
#include "src/ui/UIFramework.h"

// Screen layout constants
namespace UI {
  // Screen IDs
  enum ScreenId {
    MAIN_SCREEN = 0,
    APPS_MENU = 1,
    NETWORK_MENU = 2
  };
  
  // UI component dimensions
  namespace Layout {
    // Standard menu
    constexpr uint8_t MENU_X = 41;
    constexpr uint8_t MENU_Y = 4;
    constexpr uint8_t MENU_WIDTH = 95;
    constexpr uint8_t MENU_HEIGHT = 41;
    
    // Button labels
    constexpr uint8_t BUTTON_LABEL_HEIGHT = 14;
    constexpr uint8_t BUTTON_LABEL_Y = SCREEN_HEIGHT_DISP2 - BUTTON_LABEL_HEIGHT;
    constexpr uint8_t BUTTON_LABEL_WIDTH = 40;
    constexpr uint8_t HOME_BUTTON_X = 15;
    constexpr uint8_t BACK_BUTTON_X = 73;
  }
}

// Global UI components
UIManager* uiManager;
AppTitleWidget* appTitle;
KeypadShortcutsWidget* keypadShortcuts;
MenuWidget* mainMenu;
MenuWidget* appsMenu;
MenuWidget* networkMenu;

// Forward declarations
void initMainMenu();
void initAppsMenu();
void initNetworkMenu();
void provideHapticFeedback(int duration = 50);
void updateKeypadShortcuts(int screenId);
void processInput();

// Menu callback functions
void onAppsMenuSelected(MenuWidget* menu);
void onSystemMonitorSelected(MenuWidget* menu);
void onIoTControlSelected(MenuWidget* menu);
void onNetworkMenuSelected(MenuWidget* menu);
void onBackToMainSelected(MenuWidget* menu);

// Helper function to create a standard menu widget
MenuWidget* createStandardMenu() {
  return new MenuWidget(UI::Layout::MENU_X, UI::Layout::MENU_Y, 
                       UI::Layout::MENU_WIDTH, UI::Layout::MENU_HEIGHT, "");
}

// Helper function to add button labels to a screen
void addButtonLabelsToScreen(Screen* screen) {
  using namespace UI::Layout;
  
  screen->addWidget(new ButtonLabelWidget(
    HOME_BUTTON_X, BUTTON_LABEL_Y, BUTTON_LABEL_WIDTH, BUTTON_LABEL_HEIGHT,
    "Home", 1));
    
  screen->addWidget(new ButtonLabelWidget(
    BACK_BUTTON_X, BUTTON_LABEL_Y, BUTTON_LABEL_WIDTH, BUTTON_LABEL_HEIGHT,
    "Back", 1));
}

// Initialize the UI display elements
void initUIDisplays() {
  Adafruit_SSD1306* display1 = hal_display_get_display1();
  Adafruit_SSD1306* display2 = hal_display_get_display2();
  uiManager = new UIManager(*display1, *display2);
  
  // Setup display 1 (vertical 128x32)
  Screen* display1Screen = uiManager->createScreen(0, UI::MAIN_SCREEN);
  appTitle = new AppTitleWidget(0, 0, 32, 26, "ESP32 IOT");
  keypadShortcuts = new KeypadShortcutsWidget(0, 128 - 64, 32, 64);
  
  display1Screen->addWidget(appTitle);
  display1Screen->addWidget(keypadShortcuts);
  
  // Create all screens for display 2
  uiManager->createScreen(1, UI::MAIN_SCREEN);    // Main menu
  uiManager->createScreen(1, UI::APPS_MENU);      // Apps menu
  uiManager->createScreen(1, UI::NETWORK_MENU);   // Network menu
}

// Initialize the UI menus
void initUIMenus() {
  initMainMenu();
  initAppsMenu();
  initNetworkMenu();
}

// Initialize the main menu
void initMainMenu() {
  Screen* mainMenuScreen = uiManager->getScreen(1, UI::MAIN_SCREEN);
  mainMenu = createStandardMenu();
  
  mainMenu->addItem("Apps", onAppsMenuSelected);
  mainMenu->addItem("SYS Monitor", onSystemMonitorSelected);
  mainMenu->addItem("IoT Control", onIoTControlSelected);
  mainMenu->addItem("Network", onNetworkMenuSelected);
  
  mainMenuScreen->addWidget(mainMenu);
  addButtonLabelsToScreen(mainMenuScreen);
}

// Initialize the apps menu
void initAppsMenu() {
  Screen* appsMenuScreen = uiManager->getScreen(1, UI::APPS_MENU);
  appsMenu = createStandardMenu();
  
  appsMenu->addItem("Clock", nullptr);
  appsMenu->addItem("Timer", nullptr);
  appsMenu->addItem("Pomodoro", nullptr);
  
  appsMenuScreen->addWidget(appsMenu);
  addButtonLabelsToScreen(appsMenuScreen);
}

// Initialize the network menu
void initNetworkMenu() {
  Screen* networkMenuScreen = uiManager->getScreen(1, UI::NETWORK_MENU);
  networkMenu = createStandardMenu();
  
  networkMenu->addItem("NET Status", nullptr);
  networkMenu->addItem("NET Config", nullptr);
  networkMenu->addItem("WiFi Setup", nullptr);
  
  networkMenuScreen->addWidget(networkMenu);
  addButtonLabelsToScreen(networkMenuScreen);
}

// Initialize the UI
void initUI() {
  initUIDisplays();
  initUIMenus();
  
  // Activate main screens
  uiManager->setActiveScreen(0, UI::MAIN_SCREEN);
  uiManager->setActiveScreen(1, UI::MAIN_SCREEN);
  
  // Configure initial shortcuts
  updateKeypadShortcuts(UI::MAIN_SCREEN);
  
  // Force an immediate update
  uiManager->update(0);
}

// Provide haptic feedback
void provideHapticFeedback(int duration) {
  hal_output_set_buzzer(true);
  delay(duration);
  hal_output_set_buzzer(false);
}

// Update keypad shortcuts based on current screen
void updateKeypadShortcuts(int screenId) {
  keypadShortcuts->clearShortcuts();
  
  // Common shortcuts
  keypadShortcuts->setShortcut('A', "Home");
  keypadShortcuts->setShortcut('B', "Back");
  
  // Screen-specific shortcuts
  switch(screenId) {
    case UI::MAIN_SCREEN:
      keypadShortcuts->setShortcut('C', "Menu");
      keypadShortcuts->setShortcut('D', "Select");
      keypadShortcuts->setShortcut('#', "Next");
      keypadShortcuts->setShortcut('*', "Prev");
      break;
    case UI::APPS_MENU:
      keypadShortcuts->setShortcut('C', "Run");
      keypadShortcuts->setShortcut('D', "Info");
      keypadShortcuts->setShortcut('#', "Next");
      keypadShortcuts->setShortcut('*', "Prev");
      break;
    case UI::NETWORK_MENU:
      keypadShortcuts->setShortcut('C', "Connect");
      keypadShortcuts->setShortcut('D', "Scan");
      keypadShortcuts->setShortcut('#', "Next");
      keypadShortcuts->setShortcut('*', "Refresh");
      break;
    default:
      keypadShortcuts->setShortcut('C', "Menu");
      keypadShortcuts->setShortcut('D', "Select");
      break;
  }
}

// Menu callbacks
void onAppsMenuSelected(MenuWidget* menu) {
  uiManager->setActiveScreen(1, UI::APPS_MENU);
  appTitle->setTitle("APPS");
  updateKeypadShortcuts(UI::APPS_MENU);
  provideHapticFeedback();
}

void onSystemMonitorSelected(MenuWidget* menu) {
  appTitle->setTitle("MONITOR");
  provideHapticFeedback();
}

void onIoTControlSelected(MenuWidget* menu) {
  appTitle->setTitle("IOT CTRL");
  provideHapticFeedback();
}

void onNetworkMenuSelected(MenuWidget* menu) {
  uiManager->setActiveScreen(1, UI::NETWORK_MENU);
  appTitle->setTitle("NETWORK");
  updateKeypadShortcuts(UI::NETWORK_MENU);
  provideHapticFeedback();
}

void onBackToMainSelected(MenuWidget* menu) {
  uiManager->setActiveScreen(1, UI::MAIN_SCREEN);
  appTitle->setTitle("ESP32 IOT");
  updateKeypadShortcuts(UI::MAIN_SCREEN);
  provideHapticFeedback();
}

// Process user input for UI
void processInput() {
  static long lastEncoderValue = 0;
  long encoderValue = hal_input_read_encoder();
  
  // Get active menu based on screen
  int activeScreenId = uiManager->getActiveScreenId(1);
  MenuWidget* activeMenu = mainMenu;
  
  if (activeScreenId == UI::APPS_MENU) {
    activeMenu = appsMenu;
  } else if (activeScreenId == UI::NETWORK_MENU) {
    activeMenu = networkMenu;
  }
  
  // Handle encoder rotation
  if (encoderValue != lastEncoderValue) {
    long diff = encoderValue - lastEncoderValue;
    
    if (diff > 0) {
      activeMenu->navigateNext();
      provideHapticFeedback(20);
    } else if (diff < 0) {
      activeMenu->navigatePrevious();
      provideHapticFeedback(20);
    }
    
    lastEncoderValue = encoderValue;
  }
  
  // Handle encoder button press
  if (hal_input_read_encoder_btn()) {
    activeMenu->selectCurrentItem();
    delay(200); // Debounce delay
  }
  
  // Handle physical buttons
  if (hal_input_read_button(1)) {  // Home button
    if (activeScreenId != UI::MAIN_SCREEN) {
      onBackToMainSelected(nullptr);
    }
    delay(200); // Debounce
  }
  
  if (hal_input_read_button(2)) {  // Back button
    if (activeScreenId != UI::MAIN_SCREEN) {
      onBackToMainSelected(nullptr);
    }
    delay(200); // Debounce
  }
}

void setup() {
  Serial.begin(115200);
  Serial.println("ESP32 IoT Controller initializing...");
  
  // Initialize hardware
  hal_display_init();
  hal_input_init();
  hal_output_init();
  hal_sensor_init();
  
  // Display startup message
  hal_display_clear(0);
  hal_display_draw_text(0, "ESP32", 0, 0, 1);
  hal_display_draw_text(0, "Controller", 0, 10, 1);
  hal_display_update(0);
  
  hal_display_clear(1);
  hal_display_draw_text(1, "System", 32, 20, 1);
  hal_display_draw_text(1, "Initializing", 32, 30, 1);
  hal_display_update(1);
  
  delay(1000);
  hal_input_set_encoder(0);
  
  // Initialize UI
  initUI();
  uiManager->render();
}

void loop() {
  static unsigned long lastUpdateTime = 0;
  static unsigned long lastUIRefreshTime = 0;
  unsigned long currentTime = millis();
  
  // Process inputs at 50Hz
  if (currentTime - lastUpdateTime >= 20) {
    lastUpdateTime = currentTime;
    processInput();
    hal_sensor_update();
  }
  
  // Update UI at 30Hz
  if (currentTime - lastUIRefreshTime >= 33) {
    unsigned long deltaTime = currentTime - lastUIRefreshTime;
    lastUIRefreshTime = currentTime;
    
    uiManager->update(deltaTime);
    uiManager->render();
  }
}
