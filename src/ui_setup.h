#pragma once

#include <lvgl.h>

// Page objects
extern lv_obj_t *tabview;
extern lv_obj_t *page1;
extern lv_obj_t *page2;
extern lv_obj_t *page3;  // Connected page

// Page 1 - Connection buttons
extern lv_obj_t *btn1;
extern lv_obj_t *btn2;
extern lv_obj_t *btn3;
extern lv_obj_t *btn4;

// Status screen
extern lv_obj_t *status_screen;
extern lv_obj_t *status_label;

void setupUI();
void setupPage1(lv_obj_t *parent);
void setupPage2(lv_obj_t *parent);
void setupPage3(lv_obj_t *parent);  // Connected page
void showStatusScreen(const char *message);
void hideStatusScreen();
void showConnectedPage();
void btn_event_cb(lv_event_t *e);
void browser_btn_event_cb(lv_event_t *e);