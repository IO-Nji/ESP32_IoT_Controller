#pragma once
#include <Arduino.h>
#include "../ui/UIManager.h"

class BaseApplication {
public:
    virtual ~BaseApplication() = default;

    // Called once when the app is started
    virtual void setup(UIManager& uiManager) = 0;

    // Called every frame
    virtual void update(unsigned long deltaTime) = 0;

    // Called to render the app's UI
    virtual void render() = 0;

    // Called to handle input events
    virtual bool handleInput(uint8_t eventType, int32_t eventData) = 0;

    // Called when the app is stopped or switched out
    virtual void teardown() {}
};