# ESP32 IoT Controller UI Framework Guide

*Last Updated: August 16, 2025*

## Overview

The ESP32 IoT Controller project uses a custom UI framework designed for managing dual OLED displays with different orientations. This document explains how to use the framework, its core components, and provides examples of implementation.

## Display Configuration

The controller has two OLED displays:

1. **Display 1**: 128x32 pixels, mounted in portrait orientation (rotated 90°)
   - Effectively becomes 32x128 pixels when accounting for rotation
   - Used for app titles, status information, and keypad shortcuts

2. **Display 2**: 128x64 pixels, mounted in standard landscape orientation
   - Used for menus, content, and button labels

## Core Components

### UIManager

The central class that manages screens and rendering for both displays.

```cpp
// Create a UI manager with both displays
UIManager* uiManager = new UIManager(*display1, *display2);

// Create screens for each display
Screen* screen1 = uiManager->createScreen(0, SCREEN_ID);  // For Display 1
Screen* screen2 = uiManager->createScreen(1, SCREEN_ID);  // For Display 2

// Set active screens
uiManager->setActiveScreen(0, SCREEN_ID);
uiManager->setActiveScreen(1, SCREEN_ID);

// Update UI and render
uiManager->update(deltaTime);
uiManager->render();
```

### Screen

Containers that hold widgets. Each display can have multiple screens, but only one active at a time.

```cpp
// Create a screen for display 0 with ID 1
Screen* myScreen = uiManager->createScreen(0, 1);

// Add widgets to the screen
myScreen->addWidget(myWidget);
```

### Widget Types

#### LabelWidget

Displays text with configurable alignment, font size, and position.

```cpp
// Create a centered label with font size 1
LabelWidget* label = new LabelWidget(x, y, width, height, "Text", 1, LabelWidget::Alignment::CENTER);
```

#### ButtonWidget

Displays a clickable button with optional callback function.

```cpp
// Create a button with callback
ButtonWidget* button = new ButtonWidget(x, y, width, height, "Button");
button->setOnPressCallback(myButtonCallback);
```

#### MenuWidget

Displays a vertical menu of selectable items.

```cpp
// Create menu with standard dimensions
MenuWidget* menu = new MenuWidget(x, y, width, height, "Menu Title");

// Add menu items with callbacks
menu->addItem("Option 1", option1Callback);
menu->addItem("Option 2", option2Callback);

// Navigate the menu
menu->navigateNext();     // Move selection down
menu->navigatePrevious(); // Move selection up
menu->selectCurrentItem(); // Execute selected item's callback
```

#### AppTitleWidget

Special widget for display 1 showing the application title and abbreviation.

```cpp
// Create app title at top of Display 1
AppTitleWidget* appTitle = new AppTitleWidget(0, 0, 32, 26, "Application Title");

// Change title programmatically
appTitle->setTitle("New Title");
```

#### KeypadShortcutsWidget

Displays keypad shortcuts at the bottom of Display 1.

```cpp
// Create keypad shortcuts widget
KeypadShortcutsWidget* shortcuts = new KeypadShortcutsWidget(0, 64, 32, 64);

// Set shortcuts
shortcuts->setShortcut('A', "Home");
shortcuts->setShortcut('B', "Back");
shortcuts->setShortcut('C', "Menu");
shortcuts->setShortcut('D', "Select");

// Clear shortcuts if needed
shortcuts->clearShortcuts();
```

#### ButtonLabelWidget

Shows labels for physical buttons at the bottom of Display 2.

```cpp
// Create button label
ButtonLabelWidget* btnLabel = new ButtonLabelWidget(x, y, width, height, "Label", 1);
```

## Input Handling

The framework supports input from:
- Rotary encoder (rotation and button press)
- Physical buttons (typically for Home and Back functions)
- Matrix keypad (A,B,C,D,*,#,0-9)

```cpp
// Handle encoder rotation for menu navigation
long encoderValue = hal_input_read_encoder();
if (encoderValue > lastEncoderValue) {
  menu->navigateNext();
} else if (encoderValue < lastEncoderValue) {
  menu->navigatePrevious();
}

// Handle encoder button press
if (hal_input_read_encoder_btn()) {
  menu->selectCurrentItem();
}

// Handle physical buttons
if (hal_input_read_button(1)) {  // Home button
  // Return to home screen
}
```

## Complete Example

Below is a complete example demonstrating how to set up a UI with screens, widgets, and input handling:

```cpp
// Initialize UI components
void initUI() {
  // Get display objects
  Adafruit_SSD1306* display1 = hal_display_get_display1();
  Adafruit_SSD1306* display2 = hal_display_get_display2();
  
  // Create UI manager
  uiManager = new UIManager(*display1, *display2);
  
  // Create screens for both displays
  Screen* display1Screen = uiManager->createScreen(0, MAIN_SCREEN_ID);
  Screen* mainMenuScreen = uiManager->createScreen(1, MAIN_SCREEN_ID);
  
  // Create app title for Display 1
  AppTitleWidget* appTitle = new AppTitleWidget(0, 0, 32, 26, "ESP32 IOT");
  display1Screen->addWidget(appTitle);
  
  // Create keypad shortcuts for Display 1
  KeypadShortcutsWidget* shortcuts = new KeypadShortcutsWidget(0, 128 - 64, 32, 64);
  shortcuts->setShortcut('A', "Home");
  shortcuts->setShortcut('B', "Back");
  display1Screen->addWidget(shortcuts);
  
  // Create main menu for Display 2
  MenuWidget* mainMenu = new MenuWidget(41, 4, 95, 41, "");
  mainMenu->addItem("Apps", onAppsMenuSelected);
  mainMenu->addItem("Network", onNetworkMenuSelected);
  mainMenuScreen->addWidget(mainMenu);
  
  // Add button labels to Display 2
  ButtonLabelWidget* homeButton = new ButtonLabelWidget(15, 50, 40, 14, "Home", 1);
  ButtonLabelWidget* backButton = new ButtonLabelWidget(73, 50, 40, 14, "Back", 1);
  mainMenuScreen->addWidget(homeButton);
  mainMenuScreen->addWidget(backButton);
  
  // Set active screens
  uiManager->setActiveScreen(0, MAIN_SCREEN_ID);
  uiManager->setActiveScreen(1, MAIN_SCREEN_ID);
  
  // Initial render
  uiManager->update(0);
  uiManager->render();
}

// Update loop
void loop() {
  unsigned long currentTime = millis();
  unsigned long deltaTime = currentTime - lastUpdateTime;
  
  if (deltaTime >= 33) {  // 30Hz refresh rate
    lastUpdateTime = currentTime;
    
    // Process input
    processInput();
    
    // Update UI
    uiManager->update(deltaTime);
    uiManager->render();
  }
}

// Process input events
void processInput() {
  // Handle encoder for navigation
  long encoderValue = hal_input_read_encoder();
  if (encoderValue != lastEncoderValue) {
    if (encoderValue > lastEncoderValue) {
      activeMenu->navigateNext();
    } else {
      activeMenu->navigatePrevious();
    }
    lastEncoderValue = encoderValue;
  }
  
  // Handle encoder button for selection
  if (hal_input_read_encoder_btn()) {
    activeMenu->selectCurrentItem();
  }
}
```

## Best Practices

1. **Widget Layout**:
   - Define constants for common dimensions and positions
   - Consider screen boundaries and rotation when positioning widgets
   - Use helper functions to create consistently styled widgets

2. **Screen Management**:
   - Use defined IDs for screens (e.g., `MAIN_SCREEN_ID`, `SETTINGS_SCREEN_ID`)
   - Update both displays when switching screens for a consistent experience

3. **Input Handling**:
   - Process input at a higher frequency than UI updates (e.g., 50Hz vs 30Hz)
   - Use debouncing for physical buttons (typically 150-200ms delay)
   - Provide feedback for user input (e.g., short buzzer beep)

4. **Performance**:
   - Only update the UI when needed, using a timer
   - Minimize drawing operations by checking if content has changed

5. **Memory Management**:
   - Use static widget allocation where possible
   - Be careful with dynamic memory allocation on the ESP32

## Integration with Hardware

The UI framework integrates with the Hardware Abstraction Layer (HAL):

```cpp
// Display access
Adafruit_SSD1306* display1 = hal_display_get_display1();
Adafruit_SSD1306* display2 = hal_display_get_display2();

// Input reading
long encoderValue = hal_input_read_encoder();
bool buttonPressed = hal_input_read_button(buttonNumber);

// Output feedback
hal_output_set_buzzer(true);  // Short beep
delay(50);
hal_output_set_buzzer(false);
```
