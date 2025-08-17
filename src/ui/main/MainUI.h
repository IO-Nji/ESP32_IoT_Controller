#ifndef MAIN_UI_H
#define MAIN_UI_H

#include <Arduino.h>
#include <Adafruit_SSD1306.h>
#include "../UIFramework.h"
#include "../../config/UIConfig.h"
#include "../../hal/output.h"
#include "../../util/ErrorHandler.h"

class MainUI {
public:
    /**
     * @brief Constructor for MainUI
     * 
     * @param display1 First display (128x32 vertical)
     * @param display2 Second display (128x64 horizontal)
     */
    MainUI(Adafruit_SSD1306& display1, Adafruit_SSD1306& display2);
    
    /**
     * @brief Set up the UI components
     */
    void setup();
    
    /**
     * @brief Update the UI state
     * 
     * @param deltaTime Time since last update in milliseconds
     * @return true if update succeeded
     * @return false if update failed
     */
    bool update(unsigned long deltaTime);
    
    /**
     * @brief Render the UI to both displays
     */
    void render();
    
    /**
     * @brief Navigate to a specific screen
     * 
     * @param displayIndex Display index (0 or 1)
     * @param screenId Screen ID to navigate to
     * @return true if navigation succeeded
     * @return false if navigation failed
     */
    bool navigateToScreen(uint8_t displayIndex, uint8_t screenId);
    
    /**
     * @brief Display an error message on the UI
     * 
     * @param message Error message to display
     * @param severity Severity level of the error
     */
    void showErrorMessage(const String& message, ErrorSeverity severity = ErrorSeverity::ERROR);
    
    /**
     * @brief Update keypad shortcuts based on current screen
     * 
     * @param screenId Current screen ID
     */
    void updateKeypadShortcuts(int screenId);
    
    /**
     * @brief Process encoder input for menu navigation
     * 
     * @param encoderValue Current encoder value
     * @param lastEncoderValue Previous encoder value
     */
    void handleEncoderNavigation(long encoderValue, long lastEncoderValue);
    
    /**
     * @brief Process button presses
     * 
     * @param buttonId Button ID (1 for Home, 2 for Back)
     */
    void handleButtonPress(int buttonId);
    
    /**
     * @brief Provide haptic feedback
     * 
     * @param duration Duration of feedback in milliseconds
     */
    void provideHapticFeedback(int duration);
    
    // Getters
    UIManager* getUIManager() { return uiManager; }
    AppTitleWidget* getAppTitle() { return appTitle; }
    KeypadShortcutsWidget* getKeypadShortcuts() { return keypadShortcuts; }
    MenuWidget* getMainMenu() { return mainMenu; }
    MenuWidget* getAppsMenu() { return appsMenu; }
    MenuWidget* getNetworkMenu() { return networkMenu; }

private:
    // UI components
    UIManager* uiManager;
    AppTitleWidget* appTitle;
    KeypadShortcutsWidget* keypadShortcuts;
    ClockScreen* clockScreen;

    // Menu widgets
    MenuWidget* mainMenu;
    MenuWidget* appsMenu;
    MenuWidget* networkMenu;

    // Helper methods
    void initUIDisplays();
    void initUIMenus();
    void initMainMenu();
    void initAppsMenu();
    void initNetworkMenu();
    MenuWidget* createStandardMenu();
    void addButtonLabelsToScreen(Screen* screen);

    // Menu callbacks
    static void onAppsMenuSelected(MenuWidget* menu);
    static void onSystemMonitorSelected(MenuWidget* menu);
    static void onIoTControlSelected(MenuWidget* menu);
    static void onNetworkMenuSelected(MenuWidget* menu);
    static void onBackToMainSelected(MenuWidget* menu);
    static void onClockSelected(MenuWidget* menu);

    // Static reference for callbacks
    static MainUI* instance;
};

#endif // MAIN_UI_H
