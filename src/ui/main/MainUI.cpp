#include "MainUI.h"
#include "../MenuWidget.h"

// Stub implementations for new menu callbacks
void MainUI::onClockAppSelected(MenuWidget* menu) {
    if (instance) {
        instance->navigateToScreen(1, UI::CLOCK_SCREEN);
        instance->getAppTitle()->setTitle("CLOCK");
        instance->updateKeypadShortcuts(UI::CLOCK_SCREEN);
        instance->provideHapticFeedback(50);
    }
}
void MainUI::onTimerAppSelected(MenuWidget* menu) {
    if (instance) {
        instance->navigateToScreen(1, UI::TIMER_SCREEN);
        instance->getAppTitle()->setTitle("TIMER");
        instance->updateKeypadShortcuts(UI::TIMER_SCREEN);
        instance->provideHapticFeedback(50);
    }
}
void MainUI::onPomodoroAppSelected(MenuWidget* menu) {
    if (instance) {
        instance->navigateToScreen(1, UI::POMODORO_SCREEN);
        instance->getAppTitle()->setTitle("POMODORO");
        instance->updateKeypadShortcuts(UI::POMODORO_SCREEN);
        instance->provideHapticFeedback(50);
    }
}
void MainUI::onInputAppSelected(MenuWidget* menu) {
    if (instance) {
        instance->navigateToScreen(1, UI::INPUT_SCREEN);
        instance->getAppTitle()->setTitle("INPUT");
        instance->updateKeypadShortcuts(UI::INPUT_SCREEN);
        instance->provideHapticFeedback(50);
    }
}
void MainUI::onOutputAppSelected(MenuWidget* menu) {
    if (instance) {
        instance->navigateToScreen(1, UI::OUTPUT_SCREEN);
        instance->getAppTitle()->setTitle("OUTPUT");
        instance->updateKeypadShortcuts(UI::OUTPUT_SCREEN);
        instance->provideHapticFeedback(50);
    }
}
void MainUI::onSensorsAppSelected(MenuWidget* menu) {
    if (instance) {
        instance->navigateToScreen(1, UI::SENSORS_SCREEN);
        instance->getAppTitle()->setTitle("SENSORS");
        instance->updateKeypadShortcuts(UI::SENSORS_SCREEN);
        instance->provideHapticFeedback(50);
    }
}
void MainUI::onConnectedDevicesAppSelected(MenuWidget* menu) {
    if (instance) {
        instance->navigateToScreen(1, UI::CONNECTED_DEVICES_SCREEN);
        instance->getAppTitle()->setTitle("CONNECTED DEVICES");
        instance->updateKeypadShortcuts(UI::CONNECTED_DEVICES_SCREEN);
        instance->provideHapticFeedback(50);
    }
}
void MainUI::onNetworkStatusAppSelected(MenuWidget* menu) {
    if (instance) {
        instance->navigateToScreen(1, UI::NETWORK_STATUS_SCREEN);
        instance->getAppTitle()->setTitle("NETWORK STATUS");
        instance->updateKeypadShortcuts(UI::NETWORK_STATUS_SCREEN);
        instance->provideHapticFeedback(50);
    }
}
void MainUI::onNetworkConfigAppSelected(MenuWidget* menu) {
    if (instance) {
        instance->navigateToScreen(1, UI::NETWORK_CONFIG_SCREEN);
        instance->getAppTitle()->setTitle("NETWORK CONFIG");
        instance->updateKeypadShortcuts(UI::NETWORK_CONFIG_SCREEN);
        instance->provideHapticFeedback(50);
    }
}
void MainUI::onWiFiConfigAppSelected(MenuWidget* menu) {
    if (instance) {
        instance->navigateToScreen(1, UI::WIFI_CONFIG_SCREEN);
        instance->getAppTitle()->setTitle("WIFI CONFIG");
        instance->updateKeypadShortcuts(UI::WIFI_CONFIG_SCREEN);
        instance->provideHapticFeedback(50);
    }
}
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
    // Initialize UI with error handling
    try {
        initUIDisplays();
        initUIMenus();
        
        // Activate main screens
        if (!uiManager->setActiveScreen(0, UI::MAIN_SCREEN)) {
            REPORT_ERROR(ErrorCategory::UI, ErrorSeverity::ERROR, "Failed to set active screen for display 0");
        }
        
        if (!uiManager->setActiveScreen(1, UI::MAIN_SCREEN)) {
            REPORT_ERROR(ErrorCategory::UI, ErrorSeverity::ERROR, "Failed to set active screen for display 1");
        }
        
        // Configure initial shortcuts
        updateKeypadShortcuts(UI::MAIN_SCREEN);
        // Force an immediate update
        uiManager->update(0);
        REPORT_INFO("UI setup completed successfully");
        // Initialize menu stack with main menu
        menuStack.clear();
        menuStack.push_back(mainMenu);
        // Set menu selection callback for main menu and all sub-menus
        auto menuSelectHandler = [this](const MenuWidget::MenuItem& item) {
            if (item.subMenu) {
                // Sub-menu selected: push to stack, update screen, title, shortcuts
                menuStack.push_back(item.subMenu);
                BaseScreen* screen = uiManager->getScreen(1, uiManager->getActiveScreenId(1));
                screen->removeWidget(menuStack[menuStack.size()-2]);
                screen->addWidget(item.subMenu);
                if (appTitle) appTitle->setTitle(item.label);
                if (keypadShortcuts) {
                    keypadShortcuts->clearShortcuts();
                    keypadShortcuts->setShortcut('A', "Home");
                    keypadShortcuts->setShortcut('B', "Back");
                    keypadShortcuts->setShortcut('C', "Select");
                    keypadShortcuts->setShortcut('D', "Menu");
                }
            } else if (item.callback) {
                // App item selected: callback will handle screen/context
                item.callback(menuStack.back());
            }
            provideHapticFeedback(50);
        };
        mainMenu->setMenuItemSelectedCallback(menuSelectHandler);
        // Recursively set callback for all sub-menus
        std::function<void(MenuWidget*)> setCallbackRecursively = [&](MenuWidget* menu) {
            menu->setMenuItemSelectedCallback(menuSelectHandler);
            const auto& menuItems = menu->getItems();
            for (int i = 0; i < menuItems.size(); ++i) {
                if (menuItems[i].subMenu) {
                    setCallbackRecursively(menuItems[i].subMenu);
                }
            }
        };
        setCallbackRecursively(mainMenu);
    }
    catch (const std::exception& e) {
        REPORT_CRITICAL(ErrorCategory::UI, "Exception during UI setup: " + String(e.what()));
    }
    catch (...) {
        REPORT_CRITICAL(ErrorCategory::UI, "Unknown exception during UI setup");
    }
}

bool MainUI::update(unsigned long deltaTime) {
    try {
        bool result = uiManager->update(deltaTime);
        
        if (!result) {
            REPORT_ERROR(ErrorCategory::UI, ErrorSeverity::WARNING, "UI update returned failure");
        }
        
        return result;
    }
    catch (const std::exception& e) {
        REPORT_ERROR(ErrorCategory::UI, ErrorSeverity::ERROR, "Exception during UI update: " + String(e.what()));
        return false;
    }
    catch (...) {
        REPORT_ERROR(ErrorCategory::UI, ErrorSeverity::ERROR, "Unknown exception during UI update");
        return false;
    }
}

void MainUI::render() {
    try {
        uiManager->render();
    }
    catch (const std::exception& e) {
        REPORT_ERROR(ErrorCategory::UI, ErrorSeverity::ERROR, "Exception during UI render: " + String(e.what()));
    }
    catch (...) {
        REPORT_ERROR(ErrorCategory::UI, ErrorSeverity::ERROR, "Unknown exception during UI render");
    }
}

bool MainUI::navigateToScreen(uint8_t displayIndex, uint8_t screenId) {
    try {
        bool result = uiManager->setActiveScreen(displayIndex, screenId);
        
        if (!result) {
            REPORT_WARNING(ErrorCategory::UI, 
                        "Failed to navigate to screen " + String(screenId) + 
                        " on display " + String(displayIndex));
        }
        
        return result;
    }
    catch (const std::exception& e) {
        REPORT_ERROR(ErrorCategory::UI, ErrorSeverity::ERROR, 
                   "Exception during screen navigation: " + String(e.what()));
        return false;
    }
    catch (...) {
        REPORT_ERROR(ErrorCategory::UI, ErrorSeverity::ERROR, 
                   "Unknown exception during screen navigation");
        return false;
    }
}

void MainUI::showErrorMessage(const String& message, ErrorSeverity severity) {
    // In a real implementation, this would show the error on the display
    // For now, we just report it to the error handler
    ErrorHandler::displayError(message, severity);
}

void MainUI::provideHapticFeedback(int duration) {
    hal_output_set_buzzer(true);
    delay(duration);
    hal_output_set_buzzer(false);
}

// Private methods

void MainUI::initUIDisplays() {
    // Setup display 1 (vertical 128x32)
    BaseScreen* display1Screen = uiManager->createScreen(0, UI::MAIN_SCREEN);
    appTitle = new AppTitleWidget(0, 0, 32, 26, "ESP32 IOT");
    keypadShortcuts = new KeypadShortcutsWidget(0, 128 - 64, 32, 64);
    
    display1Screen->addWidget(appTitle);
    display1Screen->addWidget(keypadShortcuts);
    
    // Create all screens for display 2
    uiManager->createScreen(1, UI::MAIN_SCREEN);    // Main menu
    uiManager->createScreen(1, UI::APPS_MENU);      // Apps menu
    uiManager->createScreen(1, UI::NETWORK_MENU);   // Network menu

    // Create and add ClockScreen for display 2 (128x64)
    static TimeService timeService;
    static AlarmService alarmService(timeService);
    clockScreen = new ClockScreen(uiManager->getDisplay(1), timeService, alarmService);
    uiManager->addScreen(1, UI::CLOCK_SCREEN, clockScreen);
}

void MainUI::initUIMenus() {
    initMainMenu();
    initAppsMenu();
    initIoTDevMenu();
    initNetworkMenu();
}

void MainUI::initMainMenu() {
    BaseScreen* mainMenuScreen = uiManager->getScreen(1, UI::MAIN_SCREEN);
    mainMenu = createStandardMenu();
    // Apps (menu)
    MenuWidget* appsMenu = createStandardMenu();
    appsMenu->addItem("Clock", onClockAppSelected);
    appsMenu->addItem("Timer", onTimerAppSelected);
    appsMenu->addItem("Pomodoro Timer", onPomodoroAppSelected);
    mainMenu->addSubMenu("Apps", appsMenu);

    // SYS Info (application)
    mainMenu->addItem("SYS Info", onSystemMonitorSelected);

    // IoT DEV (menu)
    MenuWidget* iotDevMenu = createStandardMenu();
    // System (menu)
    MenuWidget* systemMenu = createStandardMenu();
    systemMenu->addItem("Input", onInputAppSelected);
    systemMenu->addItem("Output", onOutputAppSelected);
    systemMenu->addItem("Sensors", onSensorsAppSelected);
    iotDevMenu->addSubMenu("System", systemMenu);
    // Network (menu)
    MenuWidget* iotNetworkMenu = createStandardMenu();
    iotNetworkMenu->addItem("Connected Devices", onConnectedDevicesAppSelected);
    iotDevMenu->addSubMenu("Network", iotNetworkMenu);
    mainMenu->addSubMenu("IoT DEV", iotDevMenu);

    // Network (menu)
    MenuWidget* networkMenu = createStandardMenu();
    networkMenu->addItem("Status", onNetworkStatusAppSelected);
    networkMenu->addItem("Config", onNetworkConfigAppSelected);
    networkMenu->addItem("WiFi", onWiFiConfigAppSelected);
    mainMenu->addSubMenu("Network", networkMenu);

    mainMenuScreen->addWidget(mainMenu);
    addButtonLabelsToScreen(mainMenuScreen);
}

void MainUI::initAppsMenu() {
    BaseScreen* appsMenuScreen = uiManager->getScreen(1, UI::APPS_MENU);
    appsMenu = createStandardMenu();

    // Clock sub-menu
    MenuWidget* clockMenu = createStandardMenu();
    clockMenu->addItem("Clock", nullptr);
    clockMenu->addItem("Weather", nullptr);
    clockMenu->addItem("Alarm", nullptr);

    // Timer sub-menu
    MenuWidget* timerMenu = createStandardMenu();
    timerMenu->addItem("StopWatch", nullptr);
    timerMenu->addItem("CountDown", nullptr);
    appsMenu->addSubMenu("Timer", timerMenu);

    // Pomodoro Timer
    appsMenu->addItem("Pomodoro", nullptr);
    appsMenuScreen->addWidget(appsMenu);
    addButtonLabelsToScreen(appsMenuScreen);
}
// IoT DEV menu and sub-menus
void MainUI::initIoTDevMenu() {
    BaseScreen* iotDevMenuScreen = uiManager->createScreen(1, UI::IOT_DEV_MENU);
    MenuWidget* iotDevMenu = createStandardMenu();
    // System sub-menu
    MenuWidget* systemMenu = createStandardMenu();
    systemMenu->addItem("IMU Read", nullptr);
    systemMenu->addItem("RGB LED", nullptr);
    systemMenu->addItem("POT Read", nullptr);
    systemMenu->addItem("JoyStick", nullptr);
    iotDevMenu->addSubMenu("System", systemMenu);
    // Network sub-menu
    MenuWidget* iotNetworkMenu = createStandardMenu();
    iotNetworkMenu->addItem("Connected Devices", nullptr);
    iotDevMenu->addSubMenu("Network", iotNetworkMenu);
    iotDevMenuScreen->addWidget(iotDevMenu);
    addButtonLabelsToScreen(iotDevMenuScreen);
}

void MainUI::initNetworkMenu() {
    BaseScreen* networkMenuScreen = uiManager->getScreen(1, UI::NETWORK_MENU);
    networkMenu = createStandardMenu();
    networkMenu->addItem("Status", nullptr);
    networkMenu->addItem("Config", nullptr);
    networkMenu->addItem("WiFi", nullptr);
    networkMenuScreen->addWidget(networkMenu);
    addButtonLabelsToScreen(networkMenuScreen);
}

MenuWidget* MainUI::createStandardMenu() {
    return new MenuWidget(UI::Layout::MENU_X, UI::Layout::MENU_Y, 
                         UI::Layout::MENU_WIDTH, UI::Layout::MENU_HEIGHT, "");
}

void MainUI::addButtonLabelsToScreen(BaseScreen* screen) {
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
            keypadShortcuts->setShortcut('C', "Con");
            keypadShortcuts->setShortcut('D', "Scan");
            keypadShortcuts->setShortcut('#', "Next");
            keypadShortcuts->setShortcut('*', "Refresh");
            break;
        case UI::CLOCK_SCREEN:
            keypadShortcuts->setShortcut('A', "SET");
            keypadShortcuts->setShortcut('B', "ALR");
            keypadShortcuts->setShortcut('C', "WTR");
            keypadShortcuts->setShortcut('D', "UPD");
            break;
        default:
            keypadShortcuts->setShortcut('C', "Menu");
            keypadShortcuts->setShortcut('D', "Select");
            break;
    }
}

void MainUI::handleEncoderNavigation(long encoderValue, long lastEncoderValue) {
    MenuWidget* activeMenu = menuStack.empty() ? nullptr : menuStack.back();
    if (!activeMenu) return;
    if (encoderValue != lastEncoderValue) {
        long diff = encoderValue - lastEncoderValue;
        bool handled = uiManager->handleInput(1, 'E', diff);
        if (!handled) {
            if (diff > 0) {
                activeMenu->navigateNext();
                provideHapticFeedback(20);
            } else if (diff < 0) {
                activeMenu->navigatePrevious();
                provideHapticFeedback(20);
            }
        }
    }
    extern bool encoderButtonPressed;
    if (encoderButtonPressed) {
        activeMenu->selectCurrentItem();
        encoderButtonPressed = false;
    }
}

void MainUI::handleButtonPress(int buttonId) {
    // Handle Home and Back buttons for menu stack navigation
    if (buttonId == 1) {  // Home button
        // Go to root menu
        if (menuStack.size() > 1) {
            // Remove all menu widgets from display2
            BaseScreen* screen = uiManager->getScreen(1, uiManager->getActiveScreenId(1));
            for (auto it = menuStack.rbegin(); it != menuStack.rend(); ++it) {
                screen->removeWidget(*it);
            }
            menuStack.clear();
            menuStack.push_back(mainMenu);
            screen->addWidget(mainMenu);
        }
        onBackToMainSelected(nullptr);
    } else if (buttonId == 2) {  // Back button
        // Pop menu stack if possible
        if (menuStack.size() > 1) {
            BaseScreen* screen = uiManager->getScreen(1, uiManager->getActiveScreenId(1));
            screen->removeWidget(menuStack.back());
            menuStack.pop_back();
            screen->addWidget(menuStack.back());
            if (appTitle) appTitle->setTitle(menuStack.back()->getTitle());
        } else {
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
        instance->getAppTitle()->setTitle("SYSTEM");
        instance->provideHapticFeedback(50);
    }
}

void MainUI::onIoTControlSelected(MenuWidget* menu) {
    if (instance) {
        instance->getAppTitle()->setTitle("IOT DEV");
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

void MainUI::onClockSelected(MenuWidget* menu) {
    if (instance) {
    instance->navigateToScreen(1, UI::CLOCK_SCREEN); // Switch display 2 to clock screen
        instance->getAppTitle()->setTitle("CLOCK");
        instance->updateKeypadShortcuts(UI::CLOCK_SCREEN);
        instance->provideHapticFeedback(50);
    }
}


