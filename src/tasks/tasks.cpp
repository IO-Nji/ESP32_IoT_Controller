#include "tasks.h"

// External hardware objects (to be defined in .ino)
extern Adafruit_SSD1306 display1;
extern Adafruit_SSD1306 display2;
extern Keypad keypad;
extern Encoder rotaryEncoder;
extern Adafruit_MPU6050 mpu;
extern Adafruit_NeoPixel leds;
extern const uint8_t NUM_LEDS;
extern const uint8_t BUTTON1_PIN;

TaskHandle_t displayTaskHandle = NULL;
TaskHandle_t inputTaskHandle = NULL;
TaskHandle_t sensorTaskHandle = NULL;
TaskHandle_t outputTaskHandle = NULL;

void TaskDisplay(void *pvParameters) {
  for (;;) {
    display1.clearDisplay();
    display1.setCursor(0, 0);
    display1.print("System OK");
    display1.display();
    display2.clearDisplay();
    display2.setCursor(0, 0);
    display2.print("System OK");
    display2.display();
    vTaskDelay(500 / portTICK_PERIOD_MS);
  }
}

void TaskInput(void *pvParameters) {
  for (;;) {
    char key = keypad.getKey();
    long encoder = rotaryEncoder.read();
    bool btn1 = digitalRead(BUTTON1_PIN) == LOW;
    // ...handle input...
    vTaskDelay(50 / portTICK_PERIOD_MS);
  }
}

void TaskSensor(void *pvParameters) {
  for (;;) {
    sensors_event_t a, g, temp;
    mpu.getEvent(&a, &g, &temp);
    // ...process sensor data...
    vTaskDelay(100 / portTICK_PERIOD_MS);
  }
}

void TaskOutput(void *pvParameters) {
  static uint8_t hue = 0;
  for (;;) {
    for (int i = 0; i < NUM_LEDS; i++) {
      uint8_t led_hue = hue + (i * 20);
      uint32_t color = leds.ColorHSV(led_hue * 256, 255, 255);
      leds.setPixelColor(i, color);
    }
    leds.show();
    hue++;
    vTaskDelay(30 / portTICK_PERIOD_MS);
  }
}
