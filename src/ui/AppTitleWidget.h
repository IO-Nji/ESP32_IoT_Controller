#ifndef APP_TITLE_WIDGET_H
#define APP_TITLE_WIDGET_H

#include "Widget.h"

/**
 * @brief A widget for displaying an application title with abbreviated subtitle
 * 
 * This widget shows a title in all caps at the top, and a 2-character
 * abbreviated version of the title centered below it.
 */
class AppTitleWidget : public Widget {
public:
    /**
     * @brief Construct a new App Title Widget
     * 
     * @param x X position
     * @param y Y position
     * @param width Width of the widget
     * @param height Height of the widget
     * @param title Title text to display
     * @param visible Visibility flag
     */
    AppTitleWidget(int16_t x, int16_t y, int16_t width, int16_t height, 
                const String& title, bool visible = true);
    
    /**
     * @brief Draw the app title widget on the display
     * 
     * @param display Display to draw on
     */
    virtual void draw(Adafruit_GFX& display) override;
    
    /**
     * @brief Set the title text
     * 
     * @param title New title text
     */
    void setTitle(const String& title);
    
    /**
     * @brief Get the current title text
     * 
     * @return const String& Current title text
     */
    const String& getTitle() const;

private:
    String title;        ///< Title text
    String abbreviation; ///< 2-character abbreviation

    /**
     * @brief Generate the 2-character abbreviation from the title
     */
    void generateAbbreviation();
};

#endif // APP_TITLE_WIDGET_H
