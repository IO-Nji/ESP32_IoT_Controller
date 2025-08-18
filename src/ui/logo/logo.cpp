#include "logo.h"

const int FINAL_LOGO_DELAY = 5000;
const int LOGO_STEP_DELAY = 50;

const LogoRect logoRects[] = {
  {54, 14, 12, 6},      // .
  {40, 8, 12, 14},      // i
  {68, 8, 12, 14},      // i
  {26, 8, 12, 24},      // j
  {0, 0, 24, 22},       // n
  {8, 8, 4, 14},        // n detail
  {82, 8, 24, 14},      // O
  {94, 10, 6, 6}        // O detail
};

void logoDisplay(Adafruit_SSD1306& display, int startX, int startY) {
  display.clearDisplay();
  for (int i = 0; i < 6; ++i) {
    display.drawRect(logoRects[i].x + startX, logoRects[i].y + startY, logoRects[i].w, logoRects[i].h, WHITE);
    display.display();
    delay(LOGO_STEP_DELAY);
  }
  display.drawRect(logoRects[6].x + startX, logoRects[6].y + startY, logoRects[6].w, logoRects[6].h, WHITE);
  display.drawRect(logoRects[7].x + startX, logoRects[7].y + startY, logoRects[7].w, logoRects[7].h, WHITE);
  display.display();
  delay(FINAL_LOGO_DELAY);
}
