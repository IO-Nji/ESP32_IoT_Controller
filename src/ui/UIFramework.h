#ifndef UI_FRAMEWORK_H
#define UI_FRAMEWORK_H

/**
 * @brief UI Framework for ESP32 IoT Controller
 * 
 * This file includes all UI components needed for the project.
 */

// Retained mode widgets (persistent objects)
#include "Widget.h"
#include "LabelWidget.h"
#include "ButtonWidget.h"
#include "ButtonLabelWidget.h"
#include "AppTitleWidget.h"
#include "KeypadShortcutsWidget.h"
#include "MenuWidget.h"
#include "screens/Screen.h"
#include "screens/ClockScreen.h"
#include "UIManager.h"

// Immediate mode UI (for dynamic components)
#include "ImUI.h"

// Add additional widget includes here as they are created

#endif // UI_FRAMEWORK_H
