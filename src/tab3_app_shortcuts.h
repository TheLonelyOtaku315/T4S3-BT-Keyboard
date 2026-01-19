#ifndef TAB3_APP_SHORTCUTS_H
#define TAB3_APP_SHORTCUTS_H

#include <lvgl.h>
#include <BleKeyboard.h>

// External BLE Keyboard instance
extern BleKeyboard bleKeyboard;

// Tab3 Button Event Handlers
void browser_btn_handler(lv_event_t *e);
void discord_btn_handler(lv_event_t *e);
void facebook_btn_handler(lv_event_t *e);
void terminal_btn_handler(lv_event_t *e);
void vscode_btn_handler(lv_event_t *e);
void github_btn_handler(lv_event_t *e);

// Initialization function
void tab3_init();

#endif // TAB3_APP_SHORTCUTS_H
