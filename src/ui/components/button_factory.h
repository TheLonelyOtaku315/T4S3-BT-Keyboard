#pragma once

#include <lvgl.h>

// Factory functions for creating styled buttons

// Create a media control button
lv_obj_t *createMediaButton(lv_obj_t *parent, const char *icon, const char *label,
                            const char *action, lv_color_t color, uint8_t col, uint8_t row);

// Create a utility button (browser, calculator, etc.)
lv_obj_t *createUtilityButton(lv_obj_t *parent, const char *icon, const char *label,
                              lv_event_cb_t callback, lv_color_t color, uint8_t col, uint8_t row);

// Create a connection button (Bluetooth device, settings)
lv_obj_t *createConnectionButton(lv_obj_t *parent, const char *icon, const char *label,
                                 lv_event_cb_t callback, uint8_t col, uint8_t row);
