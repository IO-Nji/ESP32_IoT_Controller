# Hardware Abstraction Layer (HAL) Documentation

## Overview

The Hardware Abstraction Layer (HAL) provides a clean interface between the application code and the physical hardware components of the ESP32 IoT Controller. This layer abstracts away hardware-specific details, allowing application code to work with logical functions rather than direct hardware manipulation.

## Design Philosophy

The HAL follows these key design principles:

1. **Separation of Concerns**: Hardware-specific code is isolated from application logic
2. **Uniform Interfaces**: Consistent function naming and parameter patterns
3. **Encapsulation**: Hardware objects are maintained internally within the HAL
4. **Testability**: Functions can be mocked for unit testing
5. **Portability**: Application code remains the same even if underlying hardware changes

## Directory Structure

```text
src/
├── hal/                     - Hardware Abstraction Layer
│   ├── hal_config.h         - Centralized hardware configuration
│   ├── hal_includes.h       - Common includes for all HAL modules
│   ├── display.h            - Display interface definitions
│   ├── display.cpp          - Display implementation
│   ├── input.h              - Input device interface definitions
│   ├── input.cpp            - Input device implementation
│   ├── output.h             - Output device interface definitions
│   ├── output.cpp           - Output device implementation
│   ├── sensor.h             - Sensor interface definitions
│   └── sensor.cpp           - Sensor implementation
└── ui/                      - User Interface Framework
    ├── Widget.h             - Base widget class
    ├── Widget.cpp           - Base widget implementation
    ├── LabelWidget.h        - Text display widget
    ├── LabelWidget.cpp      - Text widget implementation
    ├── ButtonWidget.h       - Interactive button widget
    ├── ButtonWidget.cpp     - Button implementation
    ├── GraphWidget.h        - Data visualization widget
    ├── GraphWidget.cpp      - Graph implementation
    ├── Screen.h             - Container for widgets
    ├── Screen.cpp           - Screen implementation
    ├── UIManager.h          - Manager for screens and displays
    ├── UIManager.cpp        - Manager implementation
    ├── UIFramework.h        - Common includes for UI components
    └── UIExample.*          - Example UI implementation
```

## HAL Modules

### Configuration (`hal_config.h`)

This file centralizes all hardware-specific constants, including:

- Pin assignments
- I2C/SPI addresses
- Display configurations
- LED configurations
- Keypad layouts

All hardware-related constants should be defined here rather than scattered throughout the codebase.

### Display Module (`display.h/cpp`)

Manages the two OLED displays (128x32 and 128x64) with functions for:

- Initialization and configuration
- Drawing text, shapes, and pixels
- Controlling display properties (rotation, text size, etc.)
- Clearing and updating the display buffers

The module maintains the display objects internally and provides functions to access them if needed for advanced operations.

### Input Module (`input.h/cpp`)

Handles all input devices with functions for:

- Reading button states
- Processing keypad input
- Reading joystick position
- Reading potentiometer value
- Reading rotary encoder position and button
- Normalizing analog values to standardized ranges

The module maintains input device objects internally (Keypad, Encoder) and provides consistent interfaces for all input types.

### Output Module (`output.h/cpp`)

Controls output devices with functions for:

- Managing the RGB LED strip (WS2812)
- Controlling the buzzer
- Playing tones and sound effects
- Setting LED colors via RGB, HSV, or packed color values
- Controlling LED brightness
- Updating all output devices

The module maintains output device objects internally and provides both low-level and high-level functions for controlling outputs.

### Sensor Module (`sensor.h/cpp`)

Manages sensor devices (primarily the MPU6050 accelerometer/gyroscope) with functions for:

- Initialization and configuration
- Reading acceleration values
- Reading gyroscope values
- Reading temperature
- Detecting orientation
- Updating sensor readings

The module maintains sensor objects internally and provides processed sensor data to the application.

## Usage Examples

### Display Example

```cpp
// Initialize displays
hal_display_init();

// Draw on display 1 (128x32 vertical)
hal_display_clear(0);
hal_display_draw_text(0, "Hello", 0, 0, 1);
hal_display_draw_rect(0, 0, 20, 32, 10, 1);
hal_display_update(0);

// Draw on display 2 (128x64 horizontal)
hal_display_clear(1);
hal_display_draw_text(1, "World", 40, 32, 2);
hal_display_update(1);
```

### Input Example

```cpp
// Initialize input devices
hal_input_init();

// Read various inputs
bool button1Pressed = hal_input_read_button(1);
char key = hal_input_read_keypad();
long encoderPosition = hal_input_read_encoder();
float potValue = hal_input_get_pot_normalized();
```

### Output Example

```cpp
// Initialize output devices
hal_output_init();

// Control buzzer
hal_output_set_buzzer(true);
delay(100);
hal_output_set_buzzer(false);

// Control LEDs
hal_output_set_led(0, 255, 0, 0);  // Red
hal_output_set_led_hsv(1, 21845, 255, 255);  // Green
hal_output_set_led_brightness(128);
hal_output_update_leds();
```

### Sensor Example

```cpp
// Initialize sensor
hal_sensor_init();

// Read sensor data
hal_sensor_update();
float accelX = hal_sensor_read_accel_x();
float gyroZ = hal_sensor_read_gyro_z();
uint8_t orientation = hal_sensor_get_orientation();
```

## Direct Hardware Access

While the HAL provides abstracted functions, there may be cases where direct access to hardware objects is needed for advanced operations. Each module provides getter functions to access the underlying hardware objects:

```cpp
Adafruit_SSD1306* display1 = hal_display_get_display1();
Keypad* keypad = hal_input_get_keypad();
Adafruit_NeoPixel* leds = hal_output_get_leds();
Adafruit_MPU6050* mpu = hal_sensor_get_mpu();
```

This should be used sparingly and only when necessary for advanced operations not covered by the HAL interface.

## Future Improvements

### HAL Enhancements

Potential enhancements for the HAL:

1. Error handling and status reporting
2. Power management functions
3. Self-test capabilities for hardware components
4. Calibration functions for sensors
5. Advanced input debouncing and filtering
6. Expanded drawing capabilities for displays
7. More complex LED pattern generation
8. Data logging and telemetry

### UI Framework Enhancements

The UI framework could be extended with:

1. **Layout Management**: Automatic positioning of widgets
2. **Animation Support**: Frame-based or tween animations
3. **Additional Widgets**: Gauges, progress bars, menus, etc.
4. **Touch Support**: For future touch-enabled displays
5. **Theming**: Consistent visual styles across widgets
6. **Event System**: More sophisticated event routing

## Integration with UI Framework

The HAL is designed to work seamlessly with the UI framework. The UI framework accesses hardware through the HAL's functions rather than directly manipulating hardware objects, providing a clean separation between UI logic and hardware details.

### UI Framework Architecture

The UI framework consists of:

1. **Widget Classes**: Base classes for UI elements (LabelWidget, ButtonWidget, GraphWidget)
2. **Screen Management**: Container for organizing widgets
3. **UIManager**: Coordinates multiple screens and displays

### How UI Components Use HAL

```cpp
// UIManager initialization
UIManager::UIManager(Adafruit_SSD1306& display1, Adafruit_SSD1306& display2) {
    // Stores references to display objects provided by the HAL
}

// Widget rendering
void LabelWidget::draw(Adafruit_GFX& display) {
    // UI code draws on the display object without knowing hardware-specific details
    display.setCursor(x, y);
    display.print(text);
}

// Input handling via HAL
void processInput() {
    // Read physical buttons through HAL
    bool button1State = hal_input_read_button(1);
    
    // Map to UI actions
    if (button1State) {
        button->simulatePress();
    }
}

// Main application
void setup() {
    // Initialize hardware via HAL
    hal_display_init();
    hal_input_init();
    
    // Set up UI with HAL-provided display objects
    uiManager = new UIManager(*hal_display_get_display1(), *hal_display_get_display2());
    
    // Create screens and widgets
    // ...
}
```

### Benefits of UI Framework with HAL

1. **Separation of Concerns**: UI code focuses on presentation, HAL on hardware control
2. **Widget Reusability**: UI elements are independent of specific hardware
3. **Testability**: UI can be tested with mock display objects
4. **Flexibility**: Easily swap hardware without changing UI code

## Best Practices

When working with the HAL:

1. **Always use HAL functions** rather than direct hardware access when possible
2. **Add new hardware-specific constants** to `hal_config.h`
3. **Extend HAL interfaces** when adding new hardware rather than bypassing the HAL
4. **Use getter functions sparingly** and only for operations not covered by the HAL
5. **Keep hardware objects inside the HAL** rather than exposing them globally

## Implementation Details

### Static Object Pattern

The HAL uses a "static object pattern" for hardware components, where device objects are declared as `static` variables inside the implementation files:

```cpp
// Inside display.cpp
static Adafruit_SSD1306 display1(SCREEN_WIDTH, SCREEN_HEIGHT_DISP1, &Wire, -1);
static Adafruit_SSD1306 display2(SCREEN_WIDTH, SCREEN_HEIGHT_DISP2, &Wire, -1);
```

Benefits of this approach:

- Objects are encapsulated within the HAL module
- No global namespace pollution
- Objects are initialized only once
- Clear ownership of hardware resources

### Parameter Consistency

HAL functions follow consistent parameter patterns:

1. **Device selection first**: For functions that operate on multiple devices, the device identifier is always the first parameter:

    ```cpp
    void hal_display_draw_text(uint8_t display_id, const char* text, int16_t x, int16_t y);
    ```

2. **Status return values**: Functions that can fail return a boolean or status code:

    ```cpp
    bool hal_sensor_init();
    ```

3. **Normalized values**: Analog inputs provide both raw and normalized values:

    ```cpp
    int hal_input_read_pot();              // Raw ADC value (0-4095)
    float hal_input_get_pot_normalized();  // Normalized value (0.0-1.0)
    ```

### Update-then-Read Pattern

For sensors and other devices requiring a refresh cycle, the HAL implements an "update-then-read" pattern:

```cpp
// Update sensor data
hal_sensor_update();

// Then read cached values
float accelX = hal_sensor_read_accel_x();
float accelY = hal_sensor_read_accel_y();
```

This improves efficiency by:

- Minimizing redundant I2C/SPI transactions
- Ensuring consistent readings across multiple calls
- Allowing for future background updating in a task

### Display Buffer Management

The display HAL separates clearing, drawing, and updating operations:

```cpp
hal_display_clear(0);         // Clear the buffer
hal_display_draw_text(...);   // Multiple drawing operations
hal_display_update(0);        // Update physical display
```

This allows multiple drawing operations to be batched before updating the physical display, improving efficiency and reducing flicker.

## Design Decisions

### Why Indices vs. Enums for Device Selection

The HAL uses simple indices (e.g., `display_id = 0` or `1`) rather than enums for device selection:

```cpp
hal_display_draw_text(0, "Text on Display 1", 0, 0);
hal_display_draw_text(1, "Text on Display 2", 0, 0);
```

This was chosen because:

1. It's more compact and readable for this application
2. The number of devices per type is small and fixed
3. It maps naturally to array indices for internal implementation
4. It avoids complex type-checking overhead

### Direct Access vs. Complete Abstraction

While the HAL aims for complete hardware abstraction, it also provides "escape hatch" functions to access underlying objects:

```cpp
Adafruit_SSD1306* display = hal_display_get_display1();
```

This design decision balances:

- **Abstraction**: Most code should use HAL functions
- **Flexibility**: Advanced operations can access the raw objects
- **Pragmatism**: No need to wrap every single library function

### Naming Conventions

The HAL uses a consistent naming pattern: `hal_<module>_<action>`:

```cpp
hal_display_draw_text()
hal_input_read_button()
hal_output_set_led()
hal_sensor_read_accel_x()
```

This convention:

- Clearly identifies HAL functions in code
- Groups functions by module for better organization
- Describes actions in a verb-noun format
- Prevents name collisions with other code

### Arduino vs. ESP-IDF

The HAL is designed to work with Arduino libraries rather than the lower-level ESP-IDF framework. This decision was made because:

1. Better compatibility with existing Arduino libraries
2. Simpler API for rapid development
3. Sufficient performance for this application
4. Easier to find community support and examples

The HAL structure allows for a future transition to ESP-IDF if needed, by changing only the implementation files while keeping the interfaces intact.

### Memory Management Strategy

The HAL uses static allocation for all hardware objects to avoid dynamic memory issues:

1. No use of `new`/`delete` or `malloc`/`free` in the HAL
2. Fixed-size buffers for any string operations
3. Static object instances for all hardware components

This ensures:

- No heap fragmentation from HAL operations
- Predictable memory usage
- Higher reliability for long-running operations

### Concurrency and Task Safety

The current HAL and UI implementation is not inherently thread-safe. When using with FreeRTOS tasks:

1. Display operations should be confined to a single task or protected by a mutex
2. Sensor readings should follow the update-then-read pattern within a single task
3. Input reading is generally safe across tasks (hardware-level atomic operations)
4. Output operations should be confined to a single task or protected by a mutex
5. UI updates should happen in a single task to prevent rendering artifacts

A future enhancement could add explicit mutex protection within the HAL and UI framework.
