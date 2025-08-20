#include "BaseWidget.h"

BaseWidget::BaseWidget(int16_t x, int16_t y, int16_t width, int16_t height, bool visible)
    : x(x), y(y), width(width), height(height), visible(visible), parent(nullptr) {
}

bool BaseWidget::update(unsigned long deltaTime) {
    // Base implementation does nothing and always succeeds
    // Derived classes can override for animations, etc.
    return true;
}

bool BaseWidget::handleInput(uint8_t eventType, int32_t eventData) {
    // Base implementation does not handle any inputs
    // Return false to indicate event was not handled
    return false;
}

bool BaseWidget::isVisible() const {
    return visible;
}

void BaseWidget::setVisible(bool visible) {
    this->visible = visible;
}

void BaseWidget::setPosition(int16_t x, int16_t y) {
    this->x = x;
    this->y = y;
}

void BaseWidget::setSize(int16_t width, int16_t height) {
    this->width = width;
    this->height = height;
}

bool BaseWidget::contains(int16_t testX, int16_t testY) const {
    int16_t absX = x;
    int16_t absY = y;
    const BaseWidget* currentParent = parent;
    while (currentParent) {
        absX += currentParent->x;
        absY += currentParent->y;
        currentParent = currentParent->parent;
    }
    return (testX >= absX && testX < absX + width &&
            testY >= absY && testY < absY + height);
}

void BaseWidget::getBounds(int16_t& outX, int16_t& outY, int16_t& outWidth, int16_t& outHeight) const {
    outX = x;
    outY = y;
    outWidth = width;
    outHeight = height;
}

void BaseWidget::setParent(BaseWidget* parent) {
    this->parent = parent;
}

BaseWidget* BaseWidget::getParent() const {
    return parent;
}
