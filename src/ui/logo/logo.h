#ifndef UI_LOGO_LOGO_H
#define UI_LOGO_LOGO_H

#include <Adafruit_SSD1306.h>

struct LogoRect {
  int x, y, w, h;
};

extern const LogoRect logoRects[8];
extern const int FINAL_LOGO_DELAY;
extern const int LOGO_STEP_DELAY;

void logoDisplay(Adafruit_SSD1306& display, int startX, int startY);

#endif // UI_LOGO_LOGO_H
