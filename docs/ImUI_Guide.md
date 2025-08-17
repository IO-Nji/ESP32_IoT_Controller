# Immediate Mode UI Components

This document describes how to use the immediate mode UI components in the ESP32 IoT Controller project.

## Overview

Unlike retained mode widgets that are created once and persist between frames, immediate mode UI components are drawn on-demand each frame. This makes them ideal for:

1. Dynamic UI elements that change frequently
2. UI elements that need to respond directly to input without state tracking
3. Lightweight UI components for memory-constrained areas
4. Data visualization and animated elements

## Available Components

The `ImUI` class provides several immediate mode UI components:

- `button`: Simple button with label
- `slider`: Adjustable slider for selecting values
- `progressBar`: Bar showing progress percentage
- `toggle`: On/off toggle switch
- `textField`: Text input field
- `graph`: Line graph for data visualization
- `spinner`: Animated loading indicator

## Usage Pattern

The key pattern for using immediate mode UI is:

1. Track any necessary state in your application code (not in the UI component)
2. Call the ImUI drawing functions each frame
3. Handle return values to update your application state

## Example

```cpp
// State variables in your application
int32_t volumeLevel = 50;
bool powerOn = false;
float sensorData[10] = {/* ... */};

void renderUI() {
    // Draw slider for volume
    bool volumeActive = ImUI::slider(display, 10, 20, 100, 15, 
                                    volumeLevel, 0, 100, isVolumeSliderActive());
    
    // Update application state based on UI interaction
    if (volumeActive) {
        // User is adjusting volume, maybe play a sample tone
        playTone(volumeLevel);
    }
    
    // Draw toggle for power
    powerOn = ImUI::toggle(display, 10, 40, 30, 15, powerOn);
    
    // Draw a graph of sensor readings
    ImUI::graph(display, 10, 60, 108, 50, sensorData, 10, 0, 100, true);
}
```

## Comparison with Retained Mode

| Immediate Mode | Retained Mode |
|----------------|---------------|
| Created and drawn every frame | Created once, drawn when needed |
| No persistent state | Maintains state between frames |
| Lower memory usage | Higher memory usage |
| Better for frequently changing UIs | Better for stable UIs |
| More responsive to input | More structured object model |

## When to Use Immediate Mode

Use immediate mode UI components when:

1. You need a simple, lightweight UI element
2. The UI changes frequently based on real-time data
3. You're creating animations or transitions
4. You need to implement custom UI behaviors
5. Memory usage is a concern

Use retained mode widgets when:

1. You need structured UI with complex layouts
2. The UI is relatively stable
3. You need screen management and navigation
4. You need consistent styling across components

## Integration with Retained Mode

You can combine both approaches in the same UI:

```cpp
void setupUI() {
    // Create retained mode widgets for stable parts of the UI
    Screen* screen = uiManager.createScreen(0, MAIN_SCREEN_ID);
    screen->addWidget(new LabelWidget(5, 5, "Main Screen"));
    screen->addWidget(new ButtonWidget(5, 20, 50, 15, "Settings"));
}

void updateUI() {
    // Draw retained mode widgets
    screen->render();
    
    // Draw immediate mode elements for dynamic content
    ImUI::progressBar(display, 5, 40, 118, 10, batteryLevel / 100.0f);
    ImUI::graph(display, 5, 55, 118, 40, temperatureHistory, 24, 0, 50);
}
```

## Performance Considerations

While immediate mode UI is lightweight for memory, it can be CPU-intensive since everything is redrawn each frame. Some tips:

1. Limit the number of immediate mode elements in a single frame
2. Only update at the rate needed (e.g., 10fps instead of 60fps for slow-changing data)
3. Use simple shapes and avoid complex calculations during drawing
4. Consider caching complex drawing operations in a buffer
