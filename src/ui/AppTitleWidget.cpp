#include "AppTitleWidget.h"
#include <Arduino.h>
#include "../hal/hal_config.h"

AppTitleWidget::AppTitleWidget(int16_t x, int16_t y, int16_t width, int16_t height,
                             const String& title, bool visible)
    : Widget(x, y, width, height, visible), title(title) {
    generateAbbreviation();
}

void AppTitleWidget::draw(Adafruit_GFX& display) {
    if (!isVisible()) {
        return;
    }
    
    // Calculate absolute coordinates
    int16_t absX, absY;
    getAbsolutePosition(absX, absY);
    
    // Save current text size and color
    display.setTextSize(1);
    
    // Convert title to uppercase
    String upperTitle = title;
    upperTitle.toUpperCase();
    
    // Truncate title if needed to fit in the space
    // For display1 with width 32 pixels, we can fit ~5 characters at size 1
    int maxChars = width / (6 * 1); // 6 pixels per character at text size 1
    if (upperTitle.length() > maxChars) {
        upperTitle = upperTitle.substring(0, maxChars);
    }
    
    // Calculate text position for true horizontal centering
    int16_t titleX1, titleY1;
    uint16_t titleWidth, titleHeight;
    display.getTextBounds(upperTitle, 0, 0, &titleX1, &titleY1, &titleWidth, &titleHeight);
    int16_t titleX = absX + (width - titleWidth) / 2;
    int16_t titleY = absY + 4 + titleHeight; // Small margin from top + account for baseline
    
    // Draw the title
    display.setCursor(titleX, titleY);
    display.print(upperTitle);
    
    // Draw the abbreviation with text size 2
    display.setTextSize(2);
    
    // Calculate abbreviation position for true horizontal centering
    int16_t abbrX1, abbrY1;
    uint16_t abbrWidth, abbrHeight;
    display.getTextBounds(abbreviation, 0, 0, &abbrX1, &abbrY1, &abbrWidth, &abbrHeight);
    int16_t abbrX = absX + (width - abbrWidth) / 2;
    
    // Calculate the vertical center between title and bottom of widget area
    // This will give proper vertical spacing in the box
    int16_t availableHeightForAbbr = height - (titleHeight + 8); // Height minus title and its margin
    int16_t abbrY = absY + titleHeight + 8 + (availableHeightForAbbr - abbrHeight) / 2 + abbrHeight;
    
    // Draw the abbreviation
    display.setCursor(abbrX, abbrY);
    display.print(abbreviation);
}

void AppTitleWidget::setTitle(const String& newTitle) {
    title = newTitle;
    generateAbbreviation();
}

const String& AppTitleWidget::getTitle() const {
    return title;
}

void AppTitleWidget::generateAbbreviation() {
    // Generate a 2-character abbreviation from the title
    abbreviation = "";
    
    // Split the title into words
    String temp = title;
    temp.trim();
    
    if (temp.length() == 0) {
        abbreviation = "??";
        return;
    }
    
    // For single-word titles, take first two characters
    int spaceIndex = temp.indexOf(' ');
    if (spaceIndex == -1) {
        abbreviation = temp.substring(0, (temp.length() >= 2) ? 2 : temp.length());
        abbreviation.toUpperCase();
        return;
    }
    
    // For multi-word titles, take first character of each of the first two words
    abbreviation += temp.charAt(0);
    
    // Find second word
    String remainingText = temp.substring(spaceIndex + 1);
    if (remainingText.length() > 0) {
        abbreviation += remainingText.charAt(0);
    } else {
        // If there's no valid second word, use the second character of the first word
        if (temp.length() > 1) {
            abbreviation += temp.charAt(1);
        } else {
            abbreviation += "?";
        }
    }
    
    abbreviation.toUpperCase();
}
