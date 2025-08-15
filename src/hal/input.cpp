#include "input.h"
#include "hal_config.h"

// Define input hardware objects internally to the HAL
static char keys[KEYPAD_ROWS][KEYPAD_COLS] = KEYPAD_KEYS;
static Keypad keypad = Keypad(makeKeymap(keys), const_cast<uint8_t*>(KEYPAD_ROW_PINS), 
                            const_cast<uint8_t*>(KEYPAD_COL_PINS), KEYPAD_ROWS, KEYPAD_COLS);
static Encoder rotaryEncoder(ENCODER_SW1_PIN, ENCODER_SW2_PIN);

// Get access to the raw input device objects if needed
Keypad* hal_input_get_keypad() {
    return &keypad;
}

Encoder* hal_input_get_encoder() {
    return &rotaryEncoder;
}

void hal_input_init() {
    // Configure button pins
    pinMode(BUTTON1_PIN, INPUT_PULLUP);
    pinMode(BUTTON2_PIN, INPUT_PULLUP);
    
    // Configure analog input pins
    pinMode(POT_PIN, INPUT);
    pinMode(JOYSTICK_X_PIN, INPUT);
    pinMode(JOYSTICK_Y_PIN, INPUT);
    
    // Configure encoder button pin
    pinMode(ENCODER_BTN_PIN, INPUT_PULLUP);
    
    // Keypad and encoder are initialized by their constructors
}

char hal_input_read_keypad() {
    return keypad.getKey();
}

bool hal_input_read_button(int button_id) {
    // Buttons are active LOW (pulled up and grounded when pressed)
    if (button_id == 1) {
        return digitalRead(BUTTON1_PIN) == LOW;
    } else if (button_id == 2) {
        return digitalRead(BUTTON2_PIN) == LOW;
    }
    return false;
}

int hal_input_read_joystick_x() {
    return analogRead(JOYSTICK_X_PIN);
}

int hal_input_read_joystick_y() {
    return analogRead(JOYSTICK_Y_PIN);
}

int hal_input_read_pot() {
    return analogRead(POT_PIN);
}

long hal_input_read_encoder() {
    return rotaryEncoder.read();
}

void hal_input_set_encoder(long position) {
    rotaryEncoder.write(position);
}

bool hal_input_read_encoder_btn() {
    return digitalRead(ENCODER_BTN_PIN) == LOW;  // Active LOW
}

float hal_input_get_joystick_x_normalized() {
    // ESP32 ADC range is 0-4095, convert to -1.0 to 1.0
    // Assuming joystick is centered at around 2048
    float raw = analogRead(JOYSTICK_X_PIN);
    return (raw - 2048.0f) / 2048.0f;
}

float hal_input_get_joystick_y_normalized() {
    // ESP32 ADC range is 0-4095, convert to -1.0 to 1.0
    // Assuming joystick is centered at around 2048
    float raw = analogRead(JOYSTICK_Y_PIN);
    return (raw - 2048.0f) / 2048.0f;
}

float hal_input_get_pot_normalized() {
    // ESP32 ADC range is 0-4095, convert to 0.0 to 1.0
    return analogRead(POT_PIN) / 4095.0f;
}
