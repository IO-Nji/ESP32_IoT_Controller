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

// Standard menu dimensions
// Start from X=33 and cover the rest of the screen width (128-33=95)
#define MENU_X 41
#define MENU_Y 4  // Start from y=4 as requested
#define MENU_WIDTH 95
#define MENU_HEIGHT 41

// Button label dimensions
#define BUTTON_LABEL_HEIGHT 14  // Increased to 14px as requested
#define BUTTON_LABEL_Y (SCREEN_HEIGHT_DISP2 - BUTTON_LABEL_HEIGHT) // Position from bottom (64-14=50)
#define BUTTON_LABEL_WIDTH 40   // Width of each button label
#define HOME_BUTTON_X 15        // Left button position
#define BACK_BUTTON_X 73        // Right button position

// Helper function to create a standard menu widget with consistent styling (33,4,95,41)
// No longer needs title parameter as titles are displayed separately
MenuWidget* createStandardMenu(const String& title = "") {
  // We still pass the title parameter to maintain API compatibility
  // but it won't be displayed in the MenuWidget anymore
  return new MenuWidget(MENU_X, MENU_Y, MENU_WIDTH, MENU_HEIGHT, title);
}

// Helper function to add the standard button labels to a screen
void addButtonLabelsToScreen(Screen* screen) {
  // Create "Home" button label on left side
  ButtonLabelWidget* homeButton = new ButtonLabelWidget(
    HOME_BUTTON_X, BUTTON_LABEL_Y, BUTTON_LABEL_WIDTH, BUTTON_LABEL_HEIGHT,
    "Home", 1);
  screen->addWidget(homeButton);
  
  // Create "Back" button label on right side
  ButtonLabelWidget* backButton = new ButtonLabelWidget(
    BACK_BUTTON_X, BUTTON_LABEL_Y, BUTTON_LABEL_WIDTH, BUTTON_LABEL_HEIGHT,
    "Back", 1);
  screen->addWidget(backButton);
}

// Screen IDs for different screens
#define MAIN_SCREEN_ID 0
#define APPS_MENU_ID 1
#define NETWORK_MENU_ID 2

// AppTitleWidget for Display 1 (vertical 128x32)
AppTitleWidget* appTitle;

// KeypadShortcutsWidget for Display 1 (vertical 128x32)
KeypadShortcutsWidget* keypadShortcuts;

// Helper function to update keypad shortcuts based on the current screen/mode
void updateKeypadShortcuts(int screenId) {
  // Clear existing shortcuts
  keypadShortcuts->clearShortcuts();
  
  // Set default shortcuts that are common across screens
  keypadShortcuts->setShortcut('A', "Home");
  keypadShortcuts->setShortcut('B', "Back");
  
  // Set screen-specific shortcuts
  switch(screenId) {
    case MAIN_SCREEN_ID:
      keypadShortcuts->setShortcut('C', "Menu");
      keypadShortcuts->setShortcut('D', "Select");
      keypadShortcuts->setShortcut('#', "Next");
      keypadShortcuts->setShortcut('*', "Prev");
      break;
    case APPS_MENU_ID:
      keypadShortcuts->setShortcut('C', "Run");
      keypadShortcuts->setShortcut('D', "Info");
      keypadShortcuts->setShortcut('#', "Next");
      keypadShortcuts->setShortcut('*', "Prev");
      break;
    case NETWORK_MENU_ID:
      keypadShortcuts->setShortcut('C', "Connect");
      keypadShortcuts->setShortcut('D', "Scan");
      keypadShortcuts->setShortcut('#', "Next");
      keypadShortcuts->setShortcut('*', "Refresh");
      break;
    default:
      // Default shortcuts for unknown screens
      keypadShortcuts->setShortcut('C', "Menu");
      keypadShortcuts->setShortcut('D', "Select");
      break;
  }
}

// Menu widgets for hierarchical navigation
MenuWidget* mainMenu;
MenuWidget* appsMenu;
MenuWidget* networkMenu;

// Forward declarations of menu callback functions
void onAppsMenuSelected(MenuWidget* menu);
void onSystemMonitorSelected(MenuWidget* menu);
void onIoTControlSelected(MenuWidget* menu);
void onNetworkMenuSelected(MenuWidget* menu);
void onBackToMainSelected(MenuWidget* menu);

// Initialize the UI widgets and screens
void initUI() {
  // Get raw display objects from HAL
  Adafruit_SSD1306* display1 = hal_display_get_display1();
  Adafruit_SSD1306* display2 = hal_display_get_display2();
  
  // Create UI manager with both displays
  uiManager = new UIManager(*display1, *display2);
  
  // Create main screens
  Screen* display1Screen = uiManager->createScreen(0, MAIN_SCREEN_ID);  // Display 1 (vertical 128x32)
  Screen* mainMenuScreen = uiManager->createScreen(1, MAIN_SCREEN_ID);  // Display 2 (horizontal 128x64) - Main Menu
  
  // Create submenu screens
  Screen* appsMenuScreen = uiManager->createScreen(1, APPS_MENU_ID);    // Apps Menu
  Screen* networkMenuScreen = uiManager->createScreen(1, NETWORK_MENU_ID); // Network Menu
  
  // Create AppTitleWidget for Display 1 (vertical 128x32)
  // With rotation 3 (portrait), the display is effectively 32 wide x 128 high
  // Position at top center (0,0) with full width and appropriate height
  appTitle = new AppTitleWidget(0, 0, 32, 26, "ESP32 IOT");
  display1Screen->addWidget(appTitle);
  
  // Create KeypadShortcutsWidget for Display 1
  // Position at bottom with full width and 64px height
  keypadShortcuts = new KeypadShortcutsWidget(0, 128 - 64, 32, 64);
  // Set some initial shortcuts
  keypadShortcuts->setShortcut('A', "Home");
  keypadShortcuts->setShortcut('B', "Back");
  keypadShortcuts->setShortcut('C', "Menu");
  keypadShortcuts->setShortcut('D', "Select");
  keypadShortcuts->setShortcut('#', "Next");
  keypadShortcuts->setShortcut('*', "Previous");
  display1Screen->addWidget(keypadShortcuts);
  
  // Main menu widget with standard dimensions (40,4,95,41)
  // No labels shown while in menus as requested
  mainMenu = createStandardMenu("");
  
  // Add main menu items with plain text (no icons)
  mainMenu->addItem("Apps", onAppsMenuSelected);
  mainMenu->addItem("SYS Monitor", onSystemMonitorSelected);
  mainMenu->addItem("IoT Control", onIoTControlSelected);
  mainMenu->addItem("Network", onNetworkMenuSelected);
  
  // Add main menu to main screen
  mainMenuScreen->addWidget(mainMenu);
  
  // Add button labels to help users understand the physical button functions
  addButtonLabelsToScreen(mainMenuScreen);
  
  // --- Apps Menu Screen ---
  // No labels shown while in menus as requested
  // Apps menu widget with standard dimensions (33,4,95,41)
  appsMenu = createStandardMenu("");

  // Add apps menu items (plain text, no icons)
  appsMenu->addItem("Clock", nullptr);
  appsMenu->addItem("Timer", nullptr);
  appsMenu->addItem("Pomodoro", nullptr);
  
  // Add apps menu to its screen
  appsMenuScreen->addWidget(appsMenu);
  
  // Add button labels to help users understand the physical button functions
  addButtonLabelsToScreen(appsMenuScreen);
  
    // --- Network Menu Screen ---
  // No labels shown while in menus as requested
  // Network menu widget with standard dimensions (33,4,95,41)
  networkMenu = createStandardMenu("");

  // Add network menu items (plain text, no icons)
  networkMenu->addItem("NET Status", nullptr);
  networkMenu->addItem("NET Config", nullptr);
  networkMenu->addItem("WiFi Setup", nullptr);
  
  // Add network menu to its screen
  networkMenuScreen->addWidget(networkMenu);
  
  // Add button labels to help users understand the physical button functions
  addButtonLabelsToScreen(networkMenuScreen);
  
  // Activate the main screens
  uiManager->setActiveScreen(0, MAIN_SCREEN_ID);
  uiManager->setActiveScreen(1, MAIN_SCREEN_ID);
  
  // Force an immediate update of the displays
  uiManager->update(0);
}

void setup() {
  // Serial for debugging
  Serial.begin(115200);
  Serial.println("ESP32 IoT Controller initializing...");
  
  // Initialize all hardware through the HAL
  hal_display_init();
  hal_input_init();
  hal_output_init();
  hal_sensor_init();
  
  // Display a startup message directly with HAL
  hal_display_clear(0);
  hal_display_draw_text(0, "ESP32", 0, 0, 1);
  hal_display_draw_text(0, "Controller", 0, 10, 1);
  hal_display_update(0);
  
  hal_display_clear(1);
  hal_display_draw_text(1, "System", 32, 20, 1);
  hal_display_draw_text(1, "Initializing", 32, 30, 1);
  hal_display_update(1);
  
  delay(1000); // Show startup message for a short time
  
  // Reset encoder position
  hal_input_set_encoder(0);
  
  // Initialize UI components
  initUI();
  
  // Initialize keypad shortcuts for main screen
  updateKeypadShortcuts(MAIN_SCREEN_ID);
  
  // Force initial rendering to make sure widgets appear
  uiManager->update(0);
  uiManager->render();
}

// Menu callback functions
void onAppsMenuSelected(MenuWidget* menu) {
  // Switch to the Apps submenu
  uiManager->setActiveScreen(1, APPS_MENU_ID);
  appTitle->setTitle("APPS");
  
  // Update keypad shortcuts for this screen
  updateKeypadShortcuts(APPS_MENU_ID);
  
  // Provide feedback
  hal_output_set_buzzer(true);
  delay(50);
  hal_output_set_buzzer(false);
}

void onSystemMonitorSelected(MenuWidget* menu) {
  // For now, just update the title
  appTitle->setTitle("MONITOR");
  
  // Provide feedback
  hal_output_set_buzzer(true);
  delay(50);
  hal_output_set_buzzer(false);
}

void onIoTControlSelected(MenuWidget* menu) {
  // For now, just update the title
  appTitle->setTitle("IOT CTRL");
  
  // Provide feedback
  hal_output_set_buzzer(true);
  delay(50);
  hal_output_set_buzzer(false);
}

void onNetworkMenuSelected(MenuWidget* menu) {
  // Switch to the Network submenu
  uiManager->setActiveScreen(1, NETWORK_MENU_ID);
  appTitle->setTitle("NETWORK");
  
  // Update keypad shortcuts for this screen
  updateKeypadShortcuts(NETWORK_MENU_ID);
  
  // Provide feedback
  hal_output_set_buzzer(true);
  delay(50);
  hal_output_set_buzzer(false);
}

void onBackToMainSelected(MenuWidget* menu) {
  // Return to the main menu
  uiManager->setActiveScreen(1, MAIN_SCREEN_ID);
  appTitle->setTitle("ESP32 IOT");
  
  // Update keypad shortcuts for this screen
  updateKeypadShortcuts(MAIN_SCREEN_ID);
  
  // Provide feedback
  hal_output_set_buzzer(true);
  delay(50);
  hal_output_set_buzzer(false);
}

// Process user input for UI interactions
void processInput() {
  // Read encoder value for menu navigation
  static long lastEncoderValue = 0;
  
  // Get the encoder value
  long encoderValue = hal_input_read_encoder();
  
  // Get the active screen ID to determine which menu to control
  int activeScreenId = uiManager->getActiveScreenId(1);
  MenuWidget* activeMenu = mainMenu; // Default to main menu
  
  // Select the appropriate menu based on active screen
  if (activeScreenId == APPS_MENU_ID) {
    activeMenu = appsMenu;
  } else if (activeScreenId == NETWORK_MENU_ID) {
    activeMenu = networkMenu;
  }
  
  // Check if encoder value has changed
  if (encoderValue != lastEncoderValue) {
    // Handle encoder rotation for menu navigation
    long diff = encoderValue - lastEncoderValue;
    
    if (diff > 0) {
      // Positive change - navigate down
      activeMenu->navigateNext();
      
      // Provide subtle feedback
      hal_output_set_buzzer(true);
      delay(20);
      hal_output_set_buzzer(false);
    } 
    else if (diff < 0) {
      // Negative change - navigate up
      activeMenu->navigatePrevious();
      
      // Provide subtle feedback
      hal_output_set_buzzer(true);
      delay(20);
      hal_output_set_buzzer(false);
    }
    
    // Update last encoder value
    lastEncoderValue = encoderValue;
  }
  
  // Handle encoder button press for menu selection
  if (hal_input_read_encoder_btn()) {
    activeMenu->selectCurrentItem();
    delay(200); // Debounce delay
  }
  
  // Handle physical button inputs for navigation
  bool homeButtonState = hal_input_read_button(1);  // Left button - "Home"
  bool backButtonState = hal_input_read_button(2);  // Right button - "Back"
  
  if (homeButtonState) {
    // Home button always returns to main menu
    if (activeScreenId != MAIN_SCREEN_ID) {
      onBackToMainSelected(nullptr);
    }
    delay(200); // Debounce delay
  }
  
  if (backButtonState) {
    // Back button goes up one level in menu hierarchy
    // For now, it also returns to main menu from submenus
    if (activeScreenId != MAIN_SCREEN_ID) {
      onBackToMainSelected(nullptr);
    }
    delay(200); // Debounce delay
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
    
    // Process UI input events for menu navigation
    processInput();
    
    // Update sensor data
    hal_sensor_update();
  }
  
  // Update UI at 30Hz (separate from sensor/input processing)
  if (currentTime - lastUIRefreshTime >= 33) {
    lastUIRefreshTime = currentTime;
    
    // Calculate delta time for animations (if needed)
    unsigned long deltaTime = currentTime - lastUIRefreshTime;
    
    // First update widget states
    uiManager->update(deltaTime);
    
    // Then render the widgets to the displays
    uiManager->render();
  }
}
