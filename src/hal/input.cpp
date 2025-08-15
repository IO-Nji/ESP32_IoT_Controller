// ...existing code...
#include "hal_includes.h"

#define BUTTON1_PIN 26
#define BUTTON2_PIN 13
#define JOYSTICK_X_PIN 35
#define JOYSTICK_Y_PIN 39
#define POT_PIN 36
#define ENCODER_SW1_PIN 34
#define ENCODER_SW2_PIN 33
#define ENCODER_BTN_PIN 14

// Keypad and encoder objects are defined in the main .ino file.

void hal_input_init() {
    pinMode(BUTTON1_PIN, INPUT_PULLUP);
    pinMode(BUTTON2_PIN, INPUT_PULLUP);
    pinMode(POT_PIN, INPUT);
    pinMode(JOYSTICK_X_PIN, INPUT);
    pinMode(JOYSTICK_Y_PIN, INPUT);
    pinMode(ENCODER_BTN_PIN, INPUT_PULLUP);
    // Keypad and encoder are initialized by their constructors
}

// Input logic is now implemented in the main .ino file.
