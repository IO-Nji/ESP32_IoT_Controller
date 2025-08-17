#include "Widget.h"

Widget::Widget(int16_t x, int16_t y, int16_t width, int16_t height, bool visible)
    : x(x), y(y), width(width), height(height), visible(visible), parent(nullptr) {
}

bool Widget::update(unsigned long deltaTime) {
    // Base implementation does nothing and always succeeds
    // Derived classes can override for animations, etc.
    return true;
}

bool Widget::handleInput(uint8_t eventType, int32_t eventData) {
    // Base implementation does not handle any inputs
    // Return false to indicate event was not handled
    return false;
}

void Widget::setVisible(bool visible) {
    this->visible = visible;
}

bool Widget::isVisible() const {
    return visible;
}

void Widget::setPosition(int16_t x, int16_t y) {
    this->x = x;
    this->y = y;
}

void Widget::setSize(int16_t width, int16_t height) {
    this->width = width;
    this->height = height;
}

bool Widget::contains(int16_t testX, int16_t testY) const {
    // Calculate absolute coordinates if we have a parent
    int16_t absX = x;
    int16_t absY = y;
    const Widget* currentParent = parent;
    
    while (currentParent) {
        absX += currentParent->x;
        absY += currentParent->y;
        currentParent = currentParent->parent;
    }
    
    return (testX >= absX && testX < absX + width &&
            testY >= absY && testY < absY + height);
}

void Widget::getBounds(int16_t& outX, int16_t& outY, int16_t& outWidth, int16_t& outHeight) const {
    outX = x;
    outY = y;
    outWidth = width;
    outHeight = height;
}

void Widget::setParent(Widget* parent) {
    this->parent = parent;
}

Widget* Widget::getParent() const {
    return parent;
}
