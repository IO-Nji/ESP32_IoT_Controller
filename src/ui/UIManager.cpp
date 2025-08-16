#include "UIManager.h"

UIManager::UIManager(Adafruit_SSD1306& display1, Adafruit_SSD1306& display2)
    : display1(display1), 
      display2(display2),
      display1Width(32),    // Vertical 128x32 display (rotated)
      display1Height(128),  // Vertical 128x32 display (rotated) 
      display2Width(128),   // Horizontal 128x64 display
      display2Height(64),   // Horizontal 128x64 display
      activeScreen1(255),   // Invalid screen ID
      activeScreen2(255) {  // Invalid screen ID
}

Screen* UIManager::createScreen(uint8_t displayIndex, uint8_t screenId) {
    if (displayIndex == 0) {
        auto screen = std::make_unique<Screen>(display1, display1Width, display1Height);
        Screen* screenPtr = screen.get();
        screens1[screenId] = std::move(screen);
        return screenPtr;
    } else if (displayIndex == 1) {
        auto screen = std::make_unique<Screen>(display2, display2Width, display2Height);
        Screen* screenPtr = screen.get();
        screens2[screenId] = std::move(screen);
        return screenPtr;
    }
    return nullptr;
}

bool UIManager::setActiveScreen(uint8_t displayIndex, uint8_t screenId) {
    if (displayIndex == 0) {
        auto it = screens1.find(screenId);
        if (it != screens1.end()) {
            activeScreen1 = screenId;
            return true;
        }
    } else if (displayIndex == 1) {
        auto it = screens2.find(screenId);
        if (it != screens2.end()) {
            activeScreen2 = screenId;
            return true;
        }
    }
    return false;
}

Screen* UIManager::getActiveScreen(uint8_t displayIndex) {
    if (displayIndex == 0 && activeScreen1 != 255) {
        auto it = screens1.find(activeScreen1);
        if (it != screens1.end()) {
            return it->second.get();
        }
    } else if (displayIndex == 1 && activeScreen2 != 255) {
        auto it = screens2.find(activeScreen2);
        if (it != screens2.end()) {
            return it->second.get();
        }
    }
    return nullptr;
}

uint8_t UIManager::getActiveScreenId(uint8_t displayIndex) {
    if (displayIndex == 0) {
        return activeScreen1;
    } else if (displayIndex == 1) {
        return activeScreen2;
    }
    return 255; // Invalid screen ID
}

void UIManager::update(unsigned long deltaTime) {
    // Update active screens
    Screen* screen1 = getActiveScreen(0);
    if (screen1) {
        screen1->update(deltaTime);
    }
    
    Screen* screen2 = getActiveScreen(1);
    if (screen2) {
        screen2->update(deltaTime);
    }
}

void UIManager::render() {
    // Render active screens
    Screen* screen1 = getActiveScreen(0);
    if (screen1) {
        screen1->render();
    }
    
    Screen* screen2 = getActiveScreen(1);
    if (screen2) {
        screen2->render();
    }
}

bool UIManager::handleInput(uint8_t displayIndex, uint8_t eventType, int32_t eventData) {
    Screen* screen = getActiveScreen(displayIndex);
    if (screen) {
        return screen->handleInput(eventType, eventData);
    }
    return false;
}
