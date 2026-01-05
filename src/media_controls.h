#ifndef MEDIA_CONTROLS_H
#define MEDIA_CONTROLS_H

#include <lvgl.h>

// Media controls page setup
void setupMediaControlsPage(lv_obj_t *parent);

// Update connection status banner
void updateConnectionBanner(bool connected);

// Media button creation helper
lv_obj_t *create_media_button(lv_obj_t *parent, const char *icon, const char *label,
                              const char *action, lv_color_t color, uint8_t col, uint8_t row);

// Media control button callback
void media_btn_event_cb(lv_event_t *e);

// External references
extern lv_obj_t *media_page;
extern lv_obj_t *connection_banner;

#endif // MEDIA_CONTROLS_H
