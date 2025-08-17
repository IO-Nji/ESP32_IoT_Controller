#include "AnimatedDashboardScreen.h"
#include "../LabelWidget.h"
#include "../ButtonWidget.h"
#include <cmath>

// Constants for animation
constexpr float TEMPERATURE_MIN = 15.0f;
constexpr float TEMPERATURE_MAX = 35.0f;
constexpr float HUMIDITY_MIN = 30.0f;
constexpr float HUMIDITY_MAX = 80.0f;
constexpr size_t HISTORY_MAX_POINTS = 24;

AnimatedDashboardScreen::AnimatedDashboardScreen(Adafruit_SSD1306& display)
    : display(display),
      width(display.width()),
      height(display.height()),
      titleWidget(nullptr),
      statusWidget(nullptr),
      menuButtonWidget(nullptr),
      animationPhase(0.0f),
      graphAnimationSpeed(1.0f),
      graphAnimationPaused(false),
      currentTemperature(25.0f),
      currentHumidity(50.0f),
      batteryLevel(85.0f)
{
    // Initialize sensor history with default values
    for (size_t i = 0; i < HISTORY_MAX_POINTS; i++) {
        temperatureHistory.push_back(25.0f);
        humidityHistory.push_back(50.0f);
    }
}

void AnimatedDashboardScreen::setup() {
    // Create retained mode widgets
    titleWidget = new LabelWidget(0, 0, width, 12, "System Dashboard");
    static_cast<LabelWidget*>(titleWidget)->setAlignment(LabelWidget::Alignment::CENTER);
    
    statusWidget = new LabelWidget(0, height - 10, width, 10, "Status: Normal");
    static_cast<LabelWidget*>(statusWidget)->setAlignment(LabelWidget::Alignment::LEFT);
    
    menuButtonWidget = new ButtonWidget(width - 40, 0, 40, 12, "Menu");
}

void AnimatedDashboardScreen::update(unsigned long deltaTime) {
    // Update animation phase
    float deltaTimeSeconds = deltaTime / 1000.0f;
    
    if (!graphAnimationPaused) {
        animationPhase += deltaTimeSeconds * graphAnimationSpeed;
        if (animationPhase > 1.0f) {
            animationPhase -= floor(animationPhase);
        }
    }
    
    // Update simulated sensor data
    updateSensorData(deltaTime);
    
    // Update retained mode widgets
    titleWidget->update(deltaTime);
    statusWidget->update(deltaTime);
    menuButtonWidget->update(deltaTime);
}

void AnimatedDashboardScreen::render() {
    display.clearDisplay();
    
    // Draw retained mode widgets
    titleWidget->draw(display);
    menuButtonWidget->draw(display);
    
    // Draw a separator line
    display.drawFastHLine(0, 13, width, SSD1306_WHITE);
    
    // Draw dynamic content using immediate mode UI
    renderSensorGraphs();
    renderStatusIndicators();
    renderAnimatedIcons();
    
    // Draw bottom status widget
    statusWidget->draw(display);
    
    // Update the display
    display.display();
}

bool AnimatedDashboardScreen::handleInput(uint8_t eventType, int32_t eventData) {
    // Handle input for retained mode widgets first
    if (menuButtonWidget->handleInput(eventType, eventData)) {
        // Menu button was pressed
        return true;
    }
    
    // Handle immediate mode UI interactions
    if (eventType == 1) { // Button press (example event type)
        // Toggle animation pause state on button press
        graphAnimationPaused = !graphAnimationPaused;
        return true;
    }
    else if (eventType == 2) { // Encoder rotation (example event type)
        // Adjust animation speed with encoder
        graphAnimationSpeed += (eventData / 10.0f);
        
        // Constrain to reasonable range
        if (graphAnimationSpeed < 0.1f) graphAnimationSpeed = 0.1f;
        if (graphAnimationSpeed > 3.0f) graphAnimationSpeed = 3.0f;
        
        return true;
    }
    
    return false;
}

void AnimatedDashboardScreen::updateSensorData(unsigned long deltaTime) {
    // In a real implementation, you would read from actual sensors
    // Here we'll just simulate some dynamic values
    
    // Add some sine wave variation to temperature
    float tempVariation = sin(animationPhase * 2.0f * PI) * 5.0f;
    currentTemperature = 25.0f + tempVariation;
    
    // Add some cosine wave variation to humidity, with phase offset
    float humVariation = cos((animationPhase + 0.25f) * 2.0f * PI) * 15.0f;
    currentHumidity = 50.0f + humVariation;
    
    // Slowly drain and charge battery
    batteryLevel -= 0.01f;
    if (batteryLevel < 10.0f) {
        batteryLevel = 100.0f; // "Recharge"
    }
    
    // Update history every 250ms
    static unsigned long lastHistoryUpdate = 0;
    if (lastHistoryUpdate > deltaTime) {
        lastHistoryUpdate = 0;
    }
    
    lastHistoryUpdate += deltaTime;
    if (lastHistoryUpdate >= 250) {
        lastHistoryUpdate = 0;
        
        // Add current values to history and remove oldest if needed
        temperatureHistory.push_back(currentTemperature);
        humidityHistory.push_back(currentHumidity);
        
        if (temperatureHistory.size() > HISTORY_MAX_POINTS) {
            temperatureHistory.pop_front();
            humidityHistory.pop_front();
        }
    }
}

void AnimatedDashboardScreen::renderSensorGraphs() {
    // Draw temperature graph
    const int16_t graphY = 17;
    const int16_t graphHeight = 20;
    
    // Convert deque to array for ImUI::graph
    float tempArray[HISTORY_MAX_POINTS];
    float humArray[HISTORY_MAX_POINTS];
    
    for (size_t i = 0; i < temperatureHistory.size(); i++) {
        tempArray[i] = temperatureHistory[i];
        humArray[i] = humidityHistory[i];
    }
    
    // Draw temperature graph label
    display.setCursor(2, 17);
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.print("T:");
    display.print(currentTemperature, 1);
    display.print("C");
    
    // Draw temperature graph using immediate mode
    ImUI::graph(display, 38, graphY, width - 40, graphHeight,
               tempArray, HISTORY_MAX_POINTS, 
               TEMPERATURE_MIN, TEMPERATURE_MAX, false);
    
    // Draw humidity graph below
    const int16_t graph2Y = graphY + graphHeight + 5;
    
    // Draw humidity graph label
    display.setCursor(2, graph2Y);
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.print("H:");
    display.print(currentHumidity, 0);
    display.print("%");
    
    // Draw humidity graph using immediate mode
    ImUI::graph(display, 38, graph2Y, width - 40, graphHeight,
               humArray, HISTORY_MAX_POINTS,
               HUMIDITY_MIN, HUMIDITY_MAX, false);
}

void AnimatedDashboardScreen::renderStatusIndicators() {
    // Draw battery level indicator
    const int16_t batteryX = 2;
    const int16_t batteryY = height - 24;
    const int16_t batteryWidth = 14;
    const int16_t batteryHeight = 8;
    
    // Draw battery outline
    display.drawRect(batteryX, batteryY, batteryWidth, batteryHeight, SSD1306_WHITE);
    display.drawPixel(batteryX + batteryWidth, batteryY + 2, SSD1306_WHITE);
    display.drawPixel(batteryX + batteryWidth, batteryY + batteryHeight - 3, SSD1306_WHITE);
    
    // Draw battery level fill using immediate mode
    ImUI::progressBar(display, batteryX + 1, batteryY + 1, batteryWidth - 2, 
                     batteryHeight - 2, batteryLevel / 100.0f, false);
    
    // Draw connection status using immediate mode toggle
    const int16_t toggleX = 24;
    const int16_t toggleY = height - 25;
    const int16_t toggleWidth = 18;
    const int16_t toggleHeight = 10;
    
    // Connection status changes based on time
    bool connectionActive = (static_cast<int>(animationPhase * 10) % 3) != 0;
    
    ImUI::toggle(display, toggleX, toggleY, toggleWidth, toggleHeight, 
                connectionActive, true);
    
    // Draw "WiFi" label
    display.setCursor(toggleX + toggleWidth + 2, toggleY + 2);
    display.setTextSize(1);
    display.print("WiFi");
}

void AnimatedDashboardScreen::renderAnimatedIcons() {
    // Draw a spinner animation
    const int16_t spinnerX = width - 12;
    const int16_t spinnerY = height - 12;
    const int16_t spinnerRadius = 6;
    
    ImUI::spinner(display, spinnerX, spinnerY, spinnerRadius, animationPhase);
}
