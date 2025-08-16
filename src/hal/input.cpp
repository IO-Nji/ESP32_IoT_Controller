#include "input.h"
#include "hal_config.h"

// Define input hardware objects internally to the HAL
static char keys[KEYPAD_ROWS][KEYPAD_COLS] = {
    {'1','2','3','A'},
    {'4','5','6','B'},
    {'7','8','9','C'},
    {'*','0','#','D'}
};
static Keypad keypad = Keypad(makeKeymap(keys), const_cast<uint8_t*>(KEYPAD_ROW_PINS), 
                            const_cast<uint8_t*>(KEYPAD_COL_PINS), KEYPAD_ROWS, KEYPAD_COLS);
                            
// ESP32-specific rotary encoder implementation
static ESP32Encoder rotaryEncoder;

// For manual tracking of encoder position if needed
static volatile int lastEncoderValue = 0;

// Get access to the raw input device objects if needed
Keypad* hal_input_get_keypad() {
    return &keypad;
}

ESP32Encoder* hal_input_get_encoder() {
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
    
    // Configure encoder pins with explicit pullups
    pinMode(ENCODER_SW1_PIN, INPUT_PULLUP);
    pinMode(ENCODER_SW2_PIN, INPUT_PULLUP);
    pinMode(ENCODER_BTN_PIN, INPUT_PULLUP);
    
    // Configure the ESP32Encoder
    // Create a completely fresh configuration
    rotaryEncoder.clearCount();
    delay(10);
    
    // Attach in full quadrature mode for maximum responsiveness
    // This detects all edge transitions for maximum sensitivity
    rotaryEncoder.attachFullQuad(ENCODER_SW1_PIN, ENCODER_SW2_PIN);
    
    // Reset encoder position
    rotaryEncoder.setCount(0);
    lastEncoderValue = 0;
    
    Serial.println("Input hardware initialized with ESP32Encoder");
    Serial.println("Encoder configured in single edge mode");
    Serial.println("Encoder starts at position 0");
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
    // Read count directly from ESP32Encoder without division
    // for more responsive navigation
    return rotaryEncoder.getCount();
}

void hal_input_set_encoder(long position) {
    // Set the count directly
    rotaryEncoder.setCount(position);
    lastEncoderValue = position;
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
