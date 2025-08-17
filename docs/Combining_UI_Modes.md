# Combining Retained and Immediate Mode UI

This guide explains how to effectively combine retained mode widgets with immediate mode UI elements for dynamic applications in the ESP32 IoT Controller project.

## Understanding the Two UI Paradigms

### Retained Mode

- Widgets are created once and persist between frames
- Each widget maintains its own state
- Organized in a hierarchy (parent-child relationships)
- Good for stable UI elements that don't change frequently

### Immediate Mode

- UI elements are drawn on demand each frame
- No persistent state between frames (state managed by application)
- Flat drawing commands without hierarchy
- Good for dynamic elements that change frequently

## When to Use Each Approach

Use **Retained Mode** for:

- Static UI elements (titles, buttons, labels)
- Navigation elements
- Layout structure
- Elements with complex state and behavior

Use **Immediate Mode** for:

- Data visualizations (graphs, gauges)
- Animations and visual effects
- Elements that update every frame
- Lightweight UI elements that appear/disappear frequently

## Integration Patterns

### 1. Layer-Based Integration

Draw retained mode widgets first, then immediate mode elements on top:

```cpp
void render() {
    display.clearDisplay();
    
    // Draw retained mode widgets first
    for (auto* widget : widgets) {
        widget->draw(display);
    }
    
    // Draw immediate mode elements on top
    ImUI::graph(display, 10, 30, 108, 30, dataPoints, dataSize, 0, 100);
    ImUI::progressBar(display, 10, 70, 108, 10, progress, true);
    
    display.display();
}
```

### 2. Region-Based Integration

Reserve specific screen regions for each type:

```cpp
void render() {
    display.clearDisplay();
    
    // Top region: Retained mode header
    headerWidget->draw(display);
    
    // Middle region: Immediate mode content
    ImUI::graph(display, 0, 16, 128, 32, dataPoints, dataSize, 0, 100);
    
    // Bottom region: Retained mode footer
    footerWidget->draw(display);
    
    display.display();
}
```

### 3. Mixed Component Integration

Have retained mode widgets that internally use immediate mode for dynamic parts:

```cpp
class DynamicGraphWidget : public Widget {
public:
    // Standard widget constructor
    DynamicGraphWidget(int16_t x, int16_t y, int16_t w, int16_t h)
      : Widget(x, y, w, h) { }
    
    void draw(Adafruit_GFX& display) override {
        // Draw static parts using retained approach
        display.drawRect(x, y, width, height, SSD1306_WHITE);
        
        // Draw dynamic content using immediate mode
        ImUI::graph(display, x+2, y+2, width-4, height-4, 
                   dataPoints, dataPointCount, 
                   minValue, maxValue);
    }
    
    // Widget data management
    void addDataPoint(float value) {
        // Data management handled by widget
        dataPoints[dataIndex] = value;
        dataIndex = (dataIndex + 1) % MAX_DATA_POINTS;
        if (dataPointCount < MAX_DATA_POINTS)
            dataPointCount++;
    }
    
private:
    static constexpr size_t MAX_DATA_POINTS = 100;
    float dataPoints[MAX_DATA_POINTS] = {};
    size_t dataIndex = 0;
    size_t dataPointCount = 0;
    float minValue = 0.0f;
    float maxValue = 100.0f;
};
```

## Input Handling Strategy

When combining both UI modes, you need a clear input handling strategy:

1. **Prioritize retained mode widgets** for structured navigation
   ```cpp
   bool handleInput(uint8_t eventType, int32_t eventData) {
       // Give retained widgets first chance to handle input
       for (auto* widget : widgets) {
           if (widget->handleInput(eventType, eventData)) {
               return true; // Input handled
           }
       }
       
       // Handle immediate mode input if not handled by widgets
       return handleImmediateModeInput(eventType, eventData);
   }
   ```

2. **Use event types to route input** to appropriate handlers
   ```cpp
   bool handleImmediateModeInput(uint8_t eventType, int32_t eventData) {
       if (eventType == TOUCH_EVENT && 
           isPointInRect(eventData >> 16, eventData & 0xFFFF, 
                        graphX, graphY, graphWidth, graphHeight)) {
           // Handle graph interaction
           return true;
       }
       return false;
   }
   ```

## Animation Management

For animated elements using immediate mode:

1. **Track animation state in your application**:
   ```cpp
   float animationPhase = 0.0f;
   
   void update(unsigned long deltaTime) {
       // Update animation phase (0.0 to 1.0)
       animationPhase += (deltaTime / 1000.0f);
       if (animationPhase > 1.0f) {
           animationPhase -= floor(animationPhase);
       }
   }
   ```

2. **Use animation state in immediate mode drawing**:
   ```cpp
   void render() {
       // Draw spinner using current animation phase
       ImUI::spinner(display, 64, 32, 10, animationPhase);
   }
   ```

## Performance Considerations

1. **Limit redraws** of static content by using dirty flags
   ```cpp
   bool contentDirty = true;
   
   void render() {
       if (contentDirty) {
           // Redraw static widgets
           for (auto* widget : staticWidgets) {
               widget->draw(display);
           }
           contentDirty = false;
       }
       
       // Always redraw dynamic content
       drawDynamicContent();
   }
   ```

2. **Balance update frequency** between different UI elements
   ```cpp
   void update(unsigned long deltaTime) {
       // Update high-frequency animations every frame
       updateHighFreqAnimations(deltaTime);
       
       // Update medium-frequency content (e.g., every 100ms)
       mediumUpdateAccumulator += deltaTime;
       if (mediumUpdateAccumulator >= 100) {
           updateMediumFreqContent(mediumUpdateAccumulator);
           mediumUpdateAccumulator = 0;
       }
       
       // Update low-frequency content (e.g., every 1000ms)
       lowUpdateAccumulator += deltaTime;
       if (lowUpdateAccumulator >= 1000) {
           updateLowFreqContent(lowUpdateAccumulator);
           lowUpdateAccumulator = 0;
       }
   }
   ```

## Example: Creating a Dashboard

The example below shows how to create a dashboard with both static UI elements (using retained mode) and dynamic visualizations (using immediate mode):

```cpp
void setupDashboard() {
    // Create retained mode widgets for stable UI parts
    Screen* screen = uiManager.createScreen(0, DASHBOARD_SCREEN_ID);
    
    // Add static widgets
    screen->addWidget(new LabelWidget(0, 0, 128, 12, "System Dashboard"));
    screen->addWidget(new ButtonWidget(2, 50, 40, 12, "Settings"));
    screen->addWidget(new ButtonWidget(86, 50, 40, 12, "Details"));
    
    // Initialize animation and data tracking
    animationPhase = 0.0f;
    for (int i = 0; i < 24; i++) {
        temperatureHistory[i] = 25.0f;
        humidityHistory[i] = 50.0f;
    }
}

void renderDashboard() {
    // Clear the display
    display.clearDisplay();
    
    // Render retained mode widgets
    screen->render();
    
    // Render immediate mode elements
    
    // 1. Temperature graph
    ImUI::graph(display, 2, 15, 124, 15, 
               temperatureHistory, 24, 
               15.0f, 35.0f, true);
               
    // 2. Humidity bar
    ImUI::progressBar(display, 42, 33, 84, 8, 
                     currentHumidity / 100.0f, true);
    
    // 3. Battery indicator with animation
    float batteryPulse = batteryLevel < 20.0f ? 
                        0.5f + 0.5f * sin(animationPhase * 2.0f * PI) : 1.0f;
    
    ImUI::progressBar(display, 2, 33, 36, 8, 
                     batteryLevel / 100.0f * batteryPulse, false);
                     
    // 4. Status indicator
    ImUI::spinner(display, 10, 55, 5, animationPhase);
    
    // Update the display
    display.display();
}
```

## Tips for Effective Integration

1. **Use immediate mode for items that need frame-by-frame updates**
   - Graphs with real-time data
   - Progress indicators that update frequently
   - Interactive sliders that respond to continuous input

2. **Keep retained mode for structured UI elements**
   - Navigation menus
   - Screen layouts and containers
   - Buttons and form controls

3. **Design your screen hierarchy first**, then add immediate mode elements
   - Plan which regions will contain dynamic content
   - Allocate appropriate memory for both types of UI elements

4. **Consider memory constraints**
   - Use immediate mode when memory is tight
   - For very complex UIs, prefer immediate mode with careful state management

5. **Maintain a consistent visual style**
   - Ensure retained and immediate mode elements follow the same visual language
   - Use consistent spacing, colors, and interaction patterns
