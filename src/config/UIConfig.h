#ifndef UI_CONFIG_H
#define UI_CONFIG_H

#include "../hal/hal_config.h"

namespace UI {
    // Screen IDs
    enum ScreenId {
        MAIN_SCREEN = 0,
        APPS_MENU = 1,
        NETWORK_MENU = 2,
        CLOCK_SCREEN = 3
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

#endif // UI_CONFIG_H
