// Project hardware includes
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Keypad.h>
#include <Encoder.h>
#include <Adafruit_NeoPixel.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include "src/tasks/tasks.h"

// OLED display addresses and dimensions
#define SCREEN1_ADDR 0x3C
#define SCREEN2_ADDR 0x3D
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display1(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
Adafruit_SSD1306 display2(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// Input pin definitions
extern const uint8_t BUTTON1_PIN = 26;
#define BUTTON2_PIN 13
#define JOYSTICK_X_PIN 35
#define JOYSTICK_Y_PIN 39
#define POT_PIN 36
#define ENCODER_SW1_PIN 34
#define ENCODER_SW2_PIN 33
#define ENCODER_BTN_PIN 14

// Keypad configuration
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

// IMU sensor
Adafruit_MPU6050 mpu;

// Output definitions
#define BUZZER_PIN 17
#define LED_PIN 16
extern const uint8_t NUM_LEDS = 6;
Adafruit_NeoPixel leds(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);
// Include the tasks header
#include "src/tasks/tasks.h"

void setup() {
  // Initialize hardware peripherals
  Wire.begin();
  display1.begin(SSD1306_SWITCHCAPVCC, SCREEN1_ADDR);
  display2.begin(SSD1306_SWITCHCAPVCC, SCREEN2_ADDR);
  display1.setRotation(3); // Set vertical orientation for 128x32 display
  display1.clearDisplay();
  display2.clearDisplay();
  pinMode(BUTTON1_PIN, INPUT_PULLUP);
  pinMode(BUTTON2_PIN, INPUT_PULLUP);
  pinMode(POT_PIN, INPUT);
  pinMode(JOYSTICK_X_PIN, INPUT);
  pinMode(JOYSTICK_Y_PIN, INPUT);
  pinMode(ENCODER_BTN_PIN, INPUT_PULLUP);
  mpu.begin();
  pinMode(BUZZER_PIN, OUTPUT);
  leds.begin();
  leds.show();
}

// Demo: Encoder changes LED hue, keypad changes LED brightness, displays show values
long lastEncoder = 0;
uint8_t ledHue = 0;
uint8_t ledBrightness = 128;

void loop() {
  // Read encoder
  long encoder = rotaryEncoder.read();
  if (encoder != lastEncoder) {
    ledHue = (encoder % 256);
    lastEncoder = encoder;
  }

  // Read keypad
  char key = keypad.getKey();
  if (key) {
    if (key == '1') ledBrightness = 32;
    else if (key == '2') ledBrightness = 64;
    else if (key == '3') ledBrightness = 128;
    else if (key == '4') ledBrightness = 255;
  }

  // Update LEDs
  for (int i = 0; i < NUM_LEDS; i++) {
    uint32_t color = leds.ColorHSV((ledHue + i * 20) * 256, 255, ledBrightness);
    leds.setPixelColor(i, color);
  }
  leds.show();

  // Update displays
  display1.clearDisplay();
  display1.setTextSize(1);
  display1.setTextColor(SSD1306_WHITE);
  display1.setCursor(0, 0);
  display1.print("Encoder Hue: ");
  display1.print(ledHue);
  display1.setCursor(0, 16);
  display1.print("Brightness: ");
  display1.print(ledBrightness);
  display1.display();

  display2.clearDisplay();
  display2.setTextSize(1);
  display2.setTextColor(SSD1306_WHITE);
  display2.setCursor(0, 0);
  display2.print("Keypad: 1-4");
  display2.setCursor(0, 16);
  display2.print("Brt: ");
  display2.print(ledBrightness);
  display2.display();

  delay(50);
}
