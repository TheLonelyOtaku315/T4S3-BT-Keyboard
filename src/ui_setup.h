#pragma once

#include <lvgl.h>

// Page objects
extern lv_obj_t *tabview;
extern lv_obj_t *connectionPage;
extern lv_obj_t *infoPage;
extern lv_obj_t *mediaPage; // Connected page

// Page 1 - Connection buttons
extern lv_obj_t *bluetooth1_btn;
extern lv_obj_t *bluetooth2_btn;
extern lv_obj_t *espnow_btn;
extern lv_obj_t *settings_btn;

// Status screen
extern lv_obj_t *status_screen;
extern lv_obj_t *status_label;

void setupUI();
void setupConnectionPage(lv_obj_t *parent);
void setupInfoPage(lv_obj_t *parent);
void setupMediaPage(lv_obj_t *parent); // Connected page
void showStatusScreen(const char *message);
void hideStatusScreen();
void showMediaPage();
void updateBatteryDisplay();
void buttonEventCallback(lv_event_t *e);
void browserButtonEventCallback(lv_event_t *e);