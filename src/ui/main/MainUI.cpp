#include "MainUI.h"

// Static instance for callbacks
MainUI* MainUI::instance = nullptr;

MainUI::MainUI(Adafruit_SSD1306& display1, Adafruit_SSD1306& display2) {
    // Create UI manager
    uiManager = new UIManager(display1, display2);
    
    // Initialize instance pointer for static callbacks
    instance = this;
}

void MainUI::setup() {
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

void MainUI::update(unsigned long deltaTime) {
    uiManager->update(deltaTime);
}

void MainUI::render() {
    uiManager->render();
}

void MainUI::navigateToScreen(uint8_t displayIndex, uint8_t screenId) {
    uiManager->setActiveScreen(displayIndex, screenId);
}

void MainUI::provideHapticFeedback(int duration) {
    hal_output_set_buzzer(true);
    delay(duration);
    hal_output_set_buzzer(false);
}

// Private methods

void MainUI::initUIDisplays() {
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

void MainUI::initUIMenus() {
    initMainMenu();
    initAppsMenu();
    initNetworkMenu();
}

void MainUI::initMainMenu() {
    Screen* mainMenuScreen = uiManager->getScreen(1, UI::MAIN_SCREEN);
    mainMenu = createStandardMenu();
    
    mainMenu->addItem("Apps", onAppsMenuSelected);
    mainMenu->addItem("SYS Monitor", onSystemMonitorSelected);
    mainMenu->addItem("IoT Control", onIoTControlSelected);
    mainMenu->addItem("Network", onNetworkMenuSelected);
    
    mainMenuScreen->addWidget(mainMenu);
    addButtonLabelsToScreen(mainMenuScreen);
}

void MainUI::initAppsMenu() {
    Screen* appsMenuScreen = uiManager->getScreen(1, UI::APPS_MENU);
    appsMenu = createStandardMenu();
    
    appsMenu->addItem("Clock", nullptr);
    appsMenu->addItem("Timer", nullptr);
    appsMenu->addItem("Pomodoro", nullptr);
    
    appsMenuScreen->addWidget(appsMenu);
    addButtonLabelsToScreen(appsMenuScreen);
}

void MainUI::initNetworkMenu() {
    Screen* networkMenuScreen = uiManager->getScreen(1, UI::NETWORK_MENU);
    networkMenu = createStandardMenu();
    
    networkMenu->addItem("NET Status", nullptr);
    networkMenu->addItem("NET Config", nullptr);
    networkMenu->addItem("WiFi Setup", nullptr);
    
    networkMenuScreen->addWidget(networkMenu);
    addButtonLabelsToScreen(networkMenuScreen);
}

MenuWidget* MainUI::createStandardMenu() {
    return new MenuWidget(UI::Layout::MENU_X, UI::Layout::MENU_Y, 
                         UI::Layout::MENU_WIDTH, UI::Layout::MENU_HEIGHT, "");
}

void MainUI::addButtonLabelsToScreen(Screen* screen) {
    using namespace UI::Layout;
    
    screen->addWidget(new ButtonLabelWidget(
        HOME_BUTTON_X, BUTTON_LABEL_Y, BUTTON_LABEL_WIDTH, BUTTON_LABEL_HEIGHT,
        "Home", 1));
        
    screen->addWidget(new ButtonLabelWidget(
        BACK_BUTTON_X, BUTTON_LABEL_Y, BUTTON_LABEL_WIDTH, BUTTON_LABEL_HEIGHT,
        "Back", 1));
}

void MainUI::updateKeypadShortcuts(int screenId) {
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

void MainUI::handleEncoderNavigation(long encoderValue, long lastEncoderValue) {
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
    }
}

void MainUI::handleButtonPress(int buttonId) {
    // Get active screen ID
    int activeScreenId = uiManager->getActiveScreenId(1);
    
    // Handle physical buttons
    if (buttonId == 1) {  // Home button
        if (activeScreenId != UI::MAIN_SCREEN) {
            onBackToMainSelected(nullptr);
        }
    } else if (buttonId == 2) {  // Back button
        if (activeScreenId != UI::MAIN_SCREEN) {
            onBackToMainSelected(nullptr);
        }
    }
}

// Static callback methods
void MainUI::onAppsMenuSelected(MenuWidget* menu) {
    if (instance) {
        instance->navigateToScreen(1, UI::APPS_MENU);
        instance->getAppTitle()->setTitle("APPS");
        instance->updateKeypadShortcuts(UI::APPS_MENU);
        instance->provideHapticFeedback(50);
    }
}

void MainUI::onSystemMonitorSelected(MenuWidget* menu) {
    if (instance) {
        instance->getAppTitle()->setTitle("MONITOR");
        instance->provideHapticFeedback(50);
    }
}

void MainUI::onIoTControlSelected(MenuWidget* menu) {
    if (instance) {
        instance->getAppTitle()->setTitle("IOT CTRL");
        instance->provideHapticFeedback(50);
    }
}

void MainUI::onNetworkMenuSelected(MenuWidget* menu) {
    if (instance) {
        instance->navigateToScreen(1, UI::NETWORK_MENU);
        instance->getAppTitle()->setTitle("NETWORK");
        instance->updateKeypadShortcuts(UI::NETWORK_MENU);
        instance->provideHapticFeedback(50);
    }
}

void MainUI::onBackToMainSelected(MenuWidget* menu) {
    if (instance) {
        instance->navigateToScreen(1, UI::MAIN_SCREEN);
        instance->getAppTitle()->setTitle("ESP32 IOT");
        instance->updateKeypadShortcuts(UI::MAIN_SCREEN);
        instance->provideHapticFeedback(50);
    }
}
