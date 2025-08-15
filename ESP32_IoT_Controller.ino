// ...existing code...

// ...existing code...
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Keypad.h>
#include <Encoder.h>
#include <Adafruit_NeoPixel.h>
// ...existing code...
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>

#define SCREEN1_ADDR 0x3C
#define SCREEN2_ADDR 0x3D
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display1(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
Adafruit_SSD1306 display2(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

#define BUTTON1_PIN 26
#define BUTTON2_PIN 13
#define JOYSTICK_X_PIN 35
#define JOYSTICK_Y_PIN 39
#define POT_PIN 36
#define ENCODER_SW1_PIN 34
#define ENCODER_SW2_PIN 33
#define ENCODER_BTN_PIN 14

const byte ROWS = 4; 
const byte COLS = 4; 
byte rowPins[ROWS] = {12, 32, 25, 27};
byte colPins[COLS] = {15, 2, 0, 4};
char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);
Encoder rotaryEncoder(ENCODER_SW1_PIN, ENCODER_SW2_PIN);

Adafruit_MPU6050 mpu;

#define BUZZER_PIN 17
#define LED_PIN 16
#define NUM_LEDS 6
Adafruit_NeoPixel leds(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);
// ...existing code...
// Only one setup() and loop() definition should exist. Remove duplicates if present.
void setup() {
  Wire.begin();
  display1.begin(SSD1306_SWITCHCAPVCC, SCREEN1_ADDR);
  display2.begin(SSD1306_SWITCHCAPVCC, SCREEN2_ADDR);
  display1.clearDisplay();
  display1.setTextSize(2);
  display1.setTextColor(SSD1306_WHITE);
  display1.setCursor(0, 0);
  display1.print("Hello World");
  display1.display();

  display2.clearDisplay();
  display2.setTextSize(2);
  display2.setTextColor(SSD1306_WHITE);
  display2.setCursor(0, 0);
  display2.print("Hello World");
  display2.display();
}

void loop() {
  // Animate WS2812 RGB LED strip (6 LEDs)
  static uint8_t hue = 0;
  for (int i = 0; i < NUM_LEDS; i++) {
    uint8_t led_hue = hue + (i * 20);
    uint32_t color = leds.ColorHSV(led_hue * 256, 255, 255);
    leds.setPixelColor(i, color);
  }
  leds.show();
  hue++;
  delay(30);
}
