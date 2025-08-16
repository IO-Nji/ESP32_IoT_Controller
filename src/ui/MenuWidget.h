#ifndef MENU_WIDGET_H
#define MENU_WIDGET_H

#include "Widget.h"
#include <vector>
#include <functional>

/**
 * @brief A widget for displaying a navigable menu
 * 
 * This widget displays a scrollable list of menu items that can be selected.
 * It supports navigation through items and selection callbacks.
 */
class MenuWidget : public Widget {
public:
    /**
     * @brief Menu item structure
     */
    struct MenuItem {
        String label;                                  ///< Text label for the menu item
        std::function<void(MenuWidget*)> callback;     ///< Callback function when item is selected
        bool enabled = true;                           ///< Whether the item is enabled
        
        MenuItem(const String& label, std::function<void(MenuWidget*)> callback, bool enabled = true)
            : label(label), callback(callback), enabled(enabled) {}
    };

    /**
     * @brief Construct a new Menu Widget
     * 
     * @param x X position
     * @param y Y position
     * @param width Width of the widget
     * @param height Height of the widget
     * @param title Optional title for the menu
     * @param visible Visibility flag
     */
    MenuWidget(int16_t x, int16_t y, int16_t width, int16_t height,
              const String& title = "", bool visible = true);
    
    /**
     * @brief Draw the menu widget on the display
     * 
     * @param display Display to draw on
     */
    virtual void draw(Adafruit_GFX& display) override;
    
    /**
     * @brief Update the menu state
     * 
     * @param deltaTime Time elapsed since last update (milliseconds)
     */
    virtual void update(unsigned long deltaTime) override;
    
    /**
     * @brief Add a menu item
     * 
     * @param label Text label for the menu item
     * @param callback Function to call when the item is selected
     * @param enabled Whether the item is enabled
     * @return int Index of the added item
     */
    int addItem(const String& label, std::function<void(MenuWidget*)> callback, bool enabled = true);
    
    /**
     * @brief Remove a menu item by index
     * 
     * @param index Index of the item to remove
     * @return true If successful
     * @return false If index is out of bounds
     */
    bool removeItem(int index);
    
    /**
     * @brief Set whether a menu item is enabled
     * 
     * @param index Index of the item
     * @param enabled Enabled state
     * @return true If successful
     * @return false If index is out of bounds
     */
    bool setItemEnabled(int index, bool enabled);
    
    /**
     * @brief Navigate to the next menu item
     * 
     * @return true If navigation was successful
     * @return false If there are no more items to navigate to
     */
    bool navigateNext();
    
    /**
     * @brief Navigate to the previous menu item
     * 
     * @return true If navigation was successful
     * @return false If there are no more items to navigate to
     */
    bool navigatePrevious();
    
    /**
     * @brief Select the currently highlighted menu item
     * 
     * @return true If selection was successful
     * @return false If the current item is disabled or no items exist
     */
    bool selectCurrentItem();
    
    /**
     * @brief Get the currently selected index
     * 
     * @return int Current index (-1 if no items)
     */
    int getCurrentIndex() const;
    
    /**
     * @brief Set the current index
     * 
     * @param index Index to set
     * @return true If successful
     * @return false If index is out of bounds
     */
    bool setCurrentIndex(int index);
    
    /**
     * @brief Set the menu title
     * 
     * @param title New title text
     */
    void setTitle(const String& title);
    
    /**
     * @brief Get the menu title
     * 
     * @return const String& Current title text
     */
    const String& getTitle() const;
    
    /**
     * @brief Set the visible items count
     * 
     * @param count Number of items to show at once
     */
    void setVisibleItemsCount(uint8_t count);
    
    /**
     * @brief Set whether to show a scrollbar
     * 
     * @param show Show scrollbar flag
     */
    void setShowScrollbar(bool show);

private:
    String title;                      ///< Menu title
    std::vector<MenuItem> items;       ///< Menu items
    int currentIndex = -1;             ///< Currently selected item index
    int topVisibleIndex = 0;           ///< Index of the top visible item
    uint8_t visibleItemsCount = 4;     ///< How many items to display at once
    bool showScrollbar = true;         ///< Whether to show the scrollbar
    unsigned long lastInputTime = 0;   ///< Time of last input
    unsigned long inputDelay = 200;    ///< Delay between inputs (ms)
    
    /**
     * @brief Calculate visible item indices
     * 
     * @param first Index to store first visible item
     * @param last Index to store last visible item
     */
    void calculateVisibleItems(int& first, int& last);
    
    /**
     * @brief Ensure current index is within the visible range
     */
    void adjustVisibleRange();
};

#endif // MENU_WIDGET_H
