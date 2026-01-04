#pragma once

#include <lvgl.h>

extern lv_obj_t *btn1;
extern lv_obj_t *btn2;
extern lv_obj_t *btn3;
extern lv_obj_t *btn4;

extern lv_obj_t *label;

void setupUI();
void btn_event_cb(lv_event_t *e);