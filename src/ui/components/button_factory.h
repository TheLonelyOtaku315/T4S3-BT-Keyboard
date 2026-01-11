#pragma once

#include <lvgl.h>

// ===== BUTTON CALLBACKS =====

// Media control callbacks
void mediaBtnEventCb(lv_event_t *e);

// Utility button callbacks
void browserButtonEventCallback(lv_event_t *e);
void discordButtonEventCallback(lv_event_t *e);
void facebookButtonEventCallback(lv_event_t *e);
void calculatorButtonEventCallback(lv_event_t *e);

// ===== BUTTON FACTORY FUNCTIONS =====

// Create a media control button
lv_obj_t *createMediaButton(lv_obj_t *parent, const char *icon, const char *label,
                            const char *action, lv_color_t color, uint8_t col, uint8_t row);

// Create a utility button (browser, calculator, etc.)
lv_obj_t *createUtilityButton(lv_obj_t *parent, const char *icon, const char *label,
                              lv_event_cb_t callback, lv_color_t color, uint8_t col, uint8_t row);

// Create a utility button with image icon
lv_obj_t *createUtilityButtonWithImage(lv_obj_t *parent, const lv_img_dsc_t *image, const char *label,
                                       lv_event_cb_t callback, lv_color_t color, uint8_t col, uint8_t row);

// Create a connection button (Bluetooth device, settings)
lv_obj_t *createConnectionButton(lv_obj_t *parent, const char *icon, const char *label,
                                 lv_event_cb_t callback, uint8_t col, uint8_t row);
