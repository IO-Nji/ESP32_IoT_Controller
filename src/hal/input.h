#ifndef HAL_INPUT_H
#define HAL_INPUT_H

// Input HAL interface
void hal_input_init();
int hal_input_read_keypad();
bool hal_input_read_button(int button_id);

#endif // HAL_INPUT_H
