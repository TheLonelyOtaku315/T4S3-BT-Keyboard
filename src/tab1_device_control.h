#ifndef TAB1_DEVICE_CONTROL_H
#define TAB1_DEVICE_CONTROL_H

#include <lvgl.h>
#include <BleKeyboard.h>
#include "hardware/LilyGo_AMOLED.h"

// External reference to BLE Keyboard from main.cpp
extern BleKeyboard bleKeyboard;
extern LilyGo_Class amoled;

// Initialize Tab1 event handlers
void tab1_init();

#endif // TAB1_DEVICE_CONTROL_H
