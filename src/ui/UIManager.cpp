#include "UIManager.h"
#include <ArduinoJson.h>
#include <cstdint>
#include "screens/Screen.h"
#include <Adafruit_SSD1306.h>

UIManager::UIManager(Adafruit_SSD1306& d1, Adafruit_SSD1306& d2)
    : display1(d1), display2(d2),
      display1Width(32), display1Height(128),
      display2Width(128), display2Height(64),
      activeScreen1(255), activeScreen2(255) // 255 = invalid ID
{}

// State maps for persistence
// (no need to initialize std::map)
static std::map<uint8_t, JsonDocument> screenStates1;
static std::map<uint8_t, JsonDocument> screenStates2;

void UIManager::saveActiveScreenState(uint8_t displayIndex) {
    Screen* screen = getActiveScreen(displayIndex);
    uint8_t screenId = getActiveScreenId(displayIndex);
    if (screen) {
        JsonDocument state;
        screen->saveState(state);
        if (displayIndex == 0) {
            screenStates1[screenId] = state;
        } else {
            screenStates2[screenId] = state;
        }
    }
}

void UIManager::restoreScreenState(uint8_t displayIndex, uint8_t screenId) {
    Screen* screen = getScreen(displayIndex, screenId);
    if (screen) {
        if (displayIndex == 0 && screenStates1.count(screenId)) {
            screen->loadState(screenStates1[screenId]);
        } else if (displayIndex == 1 && screenStates2.count(screenId)) {
            screen->loadState(screenStates2[screenId]);
        }
    }
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
    // Save current screen state before switching
    saveActiveScreenState(displayIndex);

    bool found = false;
    if (displayIndex == 0) {
        auto it = screens1.find(screenId);
        if (it != screens1.end()) {
            activeScreen1 = screenId;
            found = true;
        }
    } else if (displayIndex == 1) {
        auto it = screens2.find(screenId);
        if (it != screens2.end()) {
            activeScreen2 = screenId;
            found = true;
        }
    }
    // Restore new screen state after switching
    if (found) {
        restoreScreenState(displayIndex, screenId);
    }
    return found;
}

Screen* UIManager::getActiveScreen(uint8_t displayIndex) {
    if (displayIndex == 0 && activeScreen1 != 255) {
        auto it = screens1.find(activeScreen1);
        if (it != screens1.end()) return it->second.get();
    } else if (displayIndex == 1 && activeScreen2 != 255) {
        auto it = screens2.find(activeScreen2);
        if (it != screens2.end()) return it->second.get();
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

Screen* UIManager::getScreen(uint8_t displayIndex, uint8_t screenId) {
    if (displayIndex == 0) {
        auto it = screens1.find(screenId);
        if (it != screens1.end()) {
            return it->second.get();
        }
    } else if (displayIndex == 1) {
        auto it = screens2.find(screenId);
        if (it != screens2.end()) {
            return it->second.get();
        }
    }
    return nullptr;
}

bool UIManager::update(unsigned long deltaTime) {
    bool success = true;
    
    // Update active screens
    Screen* screen1 = getActiveScreen(0);
    if (screen1) {
        if (!screen1->update(deltaTime)) {
            success = false;
        }
    }
    
    Screen* screen2 = getActiveScreen(1);
    if (screen2) {
        if (!screen2->update(deltaTime)) {
            success = false;
        }
    }
    
    return success;
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

void UIManager::addScreen(uint8_t displayIndex, uint8_t screenId, Screen* screen) {
    if (!screen) return;
    if (displayIndex == 0) {
        screens1[screenId] = std::unique_ptr<Screen>(screen);
    } else if (displayIndex == 1) {
        screens2[screenId] = std::unique_ptr<Screen>(screen);
    }
}

Adafruit_SSD1306& UIManager::getDisplay(uint8_t displayIndex) {
    return (displayIndex == 0) ? display1 : display2;
}
