#pragma once

#include <lvgl.h>
#include <BleKeyboard.h>

class LilyGo_AMOLED;

// Main UI setup function
void setupUI();

// Bluetooth management
void startBluetoothWithName(const char *deviceName);

// Page management
void showMediaPage();

// Global UI objects (accessed by pages)
extern BleKeyboard bleKeyboard;

extern lv_obj_t *tabview;
extern lv_obj_t *page1;
extern lv_obj_t *page2;
extern lv_obj_t *page3;
extern lv_obj_t *page4;

extern lv_obj_t *btn1;
extern lv_obj_t *btn2;
extern lv_obj_t *btn3;
extern lv_obj_t *btn4;

extern lv_obj_t *media_page;
extern lv_obj_t *connection_banner;
extern lv_obj_t *battery_label;
extern lv_obj_t *battery_label_util;
