#ifndef TAB5_SETTINGS_H
#define TAB5_SETTINGS_H

#include <lvgl.h>
#include <BleKeyboard.h>
#include "hardware/LilyGo_AMOLED.h"

// External references
extern BleKeyboard bleKeyboard;
extern LilyGo_Class amoled;

// Settings tab event handlers
void setting_btn_handler(lv_event_t *e);
void setting_close_btn_handler(lv_event_t *e);
void brightness_slider_handler(lv_event_t *e);

// Initialize settings tab
void tab5_init();

#endif // TAB5_SETTINGS_H