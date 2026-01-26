#include "tab5_settings.h"
#include "ui/ui.h"
#include <Arduino.h>

// Settings tab event handlers
void setting_btn_handler(lv_event_t *e)
{
    // Check if button is checked or unchecked
    if (lv_obj_has_state(ui_settingBtn, LV_STATE_CHECKED))
    {
        // Button is checked - show settings tab and hide main tabs
        lv_obj_clear_flag(ui_settingTab, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(ui_main, LV_OBJ_FLAG_HIDDEN);
        Serial.println("Settings tab opened");
    }
    else
    {
        // Button is unchecked - hide settings tab and show main tabs
        lv_obj_add_flag(ui_settingTab, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(ui_main, LV_OBJ_FLAG_HIDDEN);
        Serial.println("Settings tab closed");
    }
}

void setting_close_btn_handler(lv_event_t *e)
{
    // Uncheck the settings button
    lv_obj_clear_state(ui_settingBtn, LV_STATE_CHECKED);

    // Hide the settings tab and show main tabs
    lv_obj_add_flag(ui_settingTab, LV_OBJ_FLAG_HIDDEN);
    lv_obj_clear_flag(ui_main, LV_OBJ_FLAG_HIDDEN);

    Serial.println("Settings tab closed via close button");
}

void brightness_slider_handler(lv_event_t *e)
{
    int value = lv_slider_get_value(ui_Slider1);
    amoled.setBrightness(value);

    // Update percentage label
    char percent[10];
    sprintf(percent, "%d%%", (value * 100) / 255);
    lv_label_set_text(ui_pourcentage, percent);

    Serial.print("Brightness set to: ");
    Serial.println(value);
}

// Initialize settings tab
void tab5_init()
{
    // Connect event handlers for settings tab elements
    lv_obj_add_event_cb(ui_Button2, setting_close_btn_handler, LV_EVENT_CLICKED, NULL);
    lv_obj_add_event_cb(ui_Slider1, brightness_slider_handler, LV_EVENT_VALUE_CHANGED, NULL);

    // Set slider range to 0-255 for brightness control
    lv_slider_set_range(ui_Slider1, 0, 255);

    // Initialize brightness slider to current brightness (128)
    lv_slider_set_value(ui_Slider1, 128, LV_ANIM_OFF);
    lv_label_set_text(ui_pourcentage, "50%"); // 128/255 ≈ 50%

    // Hide settings tab initially
    lv_obj_add_flag(ui_settingTab, LV_OBJ_FLAG_HIDDEN);

    Serial.println("Tab5 (Settings) handlers connected");
}