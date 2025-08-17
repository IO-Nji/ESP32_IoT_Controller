#include "MenuWidget.h"
#include <Arduino.h>

MenuWidget::MenuWidget(int16_t x, int16_t y, int16_t width, int16_t height,
                       const String& title, bool visible)
    : Widget(x, y, width, height, visible), title(title) {
    // Force exactly 4 visible items for all menus
    visibleItemsCount = 4;
    
    // We use the full height for menu items (no space reserved for title)
    int availableHeight = height;
}

void MenuWidget::draw(Adafruit_GFX& display) {
    if (!isVisible()) {
        return;
    }
    
    // Calculate absolute coordinates
    int16_t absX, absY;
    getAbsolutePosition(absX, absY);
    
    // No border drawing - removed border code
    // No title drawing - removed title code to use full space for menu items
    
    // If no items, show a message
    if (items.empty()) {
        display.setTextSize(1);
        display.setCursor(absX, absY);
        display.print("No items");
        return;
    }
    
    // Calculate visible items range
    int first, last;
    calculateVisibleItems(first, last);
    
    // Item dimensions - fixed 10 pixel line height
    int itemHeight = 10;
    int itemY = absY;
    
    // Calculate the exact space needed for 4 items (10px each)
    int maxY = absY + (4 * 10); // 4 items * 10px per item
    
    // Draw exactly 4 items or as many as available
    // Make sure we're drawing at most 4 items
    // Cast both arguments to int to avoid type mismatch
    int itemsToDraw = min((int)visibleItemsCount, (last - first + 1));
    
    for (int i = 0; i < itemsToDraw; i++) {
        int itemIndex = first + i;
        
        display.setTextSize(1);
        display.setTextColor(SSD1306_WHITE);
        
        // Draw item text with number prefix
        int itemNumber = itemIndex + 1; // 1-based numbering
        int numberOffset = (itemNumber >= 10) ? 26 : 20; // Wider space for double-digit numbers
        
        if (itemIndex == currentIndex) {
            // For highlighted item, add ">" prefix before the number
            display.setCursor(absX, itemY);
            display.print(">");
            display.setCursor(absX + 8, itemY);
            
            // Add item number (1-based)
            display.print(String(itemNumber) + ".");
            display.setCursor(absX + numberOffset, itemY); // Adjust space based on number width
        } else {
            // For non-highlighted items, add just the number
            display.setCursor(absX + 8, itemY);
            
            // Add item number (1-based)
            display.print(String(itemNumber) + ".");
            display.setCursor(absX + numberOffset, itemY); // Adjust space based on number width
        }
        
        // Draw different for disabled items
        if (!items[itemIndex].enabled) {
            // For disabled items, use a different pattern (dashed text)
            for (size_t j = 0; j < items[itemIndex].label.length(); j++) {
                if (j % 2 == 0) {
                    display.print(items[itemIndex].label[j]);
                } else {
                    display.print(' ');
                }
            }
        } else {
            // Remove any icons from the text by checking for emoji characters
            String labelText = items[itemIndex].label;
            if (labelText.length() > 2 && (uint8_t)labelText[0] >= 0xE0) {
                // Skip emoji/icon characters (typically UTF-8 multi-byte)
                labelText = labelText.substring(3);
                // Remove any spaces after the icon
                if (labelText.length() > 0 && labelText[0] == ' ') {
                    labelText = labelText.substring(1);
                }
            }
            display.print(labelText);
        }
        
        itemY += itemHeight; // Move to next item position with exact 10px spacing
    }
    
    // Show scroll indicators and position info
    if (items.size() > visibleItemsCount) {
        // Position indicator in top-right corner of menu area
        String countText = String(currentIndex + 1) + "/" + String(items.size());
        int16_t textWidth = countText.length() * 6; // Approximate text width
        display.setCursor(absX + width - textWidth, absY - 10); // Move counter above the menu items
        display.setTextSize(1);
        display.print(countText);
        
        // Draw up/down arrows if there are more items above or below
        if (first > 0) {
            // More items above - draw up arrow at top left
            display.setCursor(absX, absY - 10); // Move up arrow above the menu items
            display.print("^");
        }
        
        if (last < items.size() - 1) {
            // More items below - draw down arrow at bottom left
            display.setCursor(absX, absY + itemHeight * visibleItemsCount);
            display.print("v");
        }
    }
}

bool MenuWidget::update(unsigned long deltaTime) {
    // Check if enough time has passed since last input
    if (millis() - lastInputTime < inputDelay) {
        return true; // Early exit is still a success
    }
    
    // Example of handling physical input (would need to be modified for your system)
    // This could be replaced with calls from your main program based on encoder or buttons
    
    // For demonstration purposes, we'll use a simple input handling method
    // In real usage, you'd call navigateNext(), navigatePrevious(), and selectCurrentItem() 
    // from your main program's input handling
    
    return true; // Update succeeded
}

int MenuWidget::addItem(const String& label, std::function<void(MenuWidget*)> callback, bool enabled) {
    items.push_back(MenuItem(label, callback, enabled));
    
    // If this is the first item, select it
    if (items.size() == 1) {
        currentIndex = 0;
    }
    
    return items.size() - 1;
}

bool MenuWidget::removeItem(int index) {
    if (index < 0 || index >= items.size()) {
        return false;
    }
    
    items.erase(items.begin() + index);
    
    // Adjust current index if needed
    if (items.empty()) {
        currentIndex = -1;
    } else if (currentIndex >= items.size()) {
        currentIndex = items.size() - 1;
    }
    
    adjustVisibleRange();
    return true;
}

bool MenuWidget::setItemEnabled(int index, bool enabled) {
    if (index < 0 || index >= items.size()) {
        return false;
    }
    
    items[index].enabled = enabled;
    return true;
}

bool MenuWidget::navigateNext() {
    if (items.empty() || currentIndex >= items.size() - 1) {
        return false;
    }
    
    currentIndex++;
    adjustVisibleRange();
    lastInputTime = millis();
    return true;
}

bool MenuWidget::navigatePrevious() {
    if (items.empty() || currentIndex <= 0) {
        return false;
    }
    
    currentIndex--;
    adjustVisibleRange();
    lastInputTime = millis();
    return true;
}

bool MenuWidget::selectCurrentItem() {
    if (items.empty() || currentIndex < 0 || currentIndex >= items.size()) {
        return false;
    }
    
    if (!items[currentIndex].enabled) {
        return false; // Can't select disabled items
    }
    
    // Call the item's callback function if available
    if (items[currentIndex].callback) {
        items[currentIndex].callback(this);
    }
    
    lastInputTime = millis();
    return true;
}

int MenuWidget::getCurrentIndex() const {
    return currentIndex;
}

bool MenuWidget::setCurrentIndex(int index) {
    if (index < -1 || index >= items.size()) {
        return false;
    }
    
    currentIndex = index;
    adjustVisibleRange();
    return true;
}

void MenuWidget::setTitle(const String& newTitle) {
    title = newTitle;
}

const String& MenuWidget::getTitle() const {
    return title;
}

void MenuWidget::setVisibleItemsCount(uint8_t count) {
    // Always set to exactly 4 items for consistency
    visibleItemsCount = 4;
    
    adjustVisibleRange();
}

void MenuWidget::setShowScrollbar(bool show) {
    showScrollbar = show;
}

void MenuWidget::calculateVisibleItems(int& first, int& last) {
    if (items.empty()) {
        first = -1;
        last = -1;
        return;
    }
    
    first = topVisibleIndex;
    last = min((int)items.size() - 1, (int)(topVisibleIndex + visibleItemsCount - 1));
}

void MenuWidget::adjustVisibleRange() {
    if (items.empty()) {
        topVisibleIndex = 0;
        return;
    }
    
    // Always display exactly 4 items when possible
    
    // If the current item is above the visible area, scroll up
    if (currentIndex < topVisibleIndex) {
        // When scrolling up, position the selection at the top
        topVisibleIndex = currentIndex;
    } 
    // If the current item is below the visible area, scroll down
    else if (currentIndex >= topVisibleIndex + visibleItemsCount) {
        // When scrolling down, position the selection at the bottom
        topVisibleIndex = currentIndex - visibleItemsCount + 1;
    }
    
    // Special case: if we have 4 or fewer items total
    if (items.size() <= visibleItemsCount) {
        topVisibleIndex = 0; // Always show from the beginning
    }
    // If we have more than 4 items but are near the end
    else if (items.size() - topVisibleIndex < visibleItemsCount) {
        // Adjust so we always show exactly 4 items (or as many as we have)
        topVisibleIndex = max(0, (int)items.size() - (int)visibleItemsCount);
    }
}
