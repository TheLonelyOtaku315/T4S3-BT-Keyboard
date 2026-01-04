#include <Arduino.h>
#include "LilyGo_AMOLED.h"
#include "LV_Helper.h"

LilyGo_Class amoled;
lv_obj_t *btn;
lv_obj_t *label;

// Touch event callback
void btn_event_cb(lv_event_t *e) {
    Serial.println("Button Pressed!");
}

void setup() {
    Serial.begin(115200);
    delay(2000);
    Serial.println("\n=== T4-S3 Touch Button Test ===");
    
    // Initialize display with LVGL
    bool rslt = amoled.begin();
    if (!rslt) {
        Serial.println("ERROR: Display init failed!");
        while (1) {
            delay(1000);
        }
    }
    
    Serial.println("Display initialized successfully");
    
    // Wait a bit for everything to settle
    delay(500);
    
    // Initialize LVGL helper
    beginLvglHelper(amoled);
    
    Serial.println("LVGL initialized");
    
    // Create a button
    btn = lv_btn_create(lv_scr_act());
    if (btn == NULL) {
        Serial.println("ERROR: Failed to create button!");
        while(1) delay(1000);
    }
    
    lv_obj_set_size(btn, 300, 120);
    lv_obj_center(btn);
    lv_obj_add_event_cb(btn, btn_event_cb, LV_EVENT_CLICKED, NULL);
    
    // Style the button - make it bright green
    lv_obj_set_style_bg_color(btn, lv_color_make(0, 255, 0), 0);
    
    // Add label to button
    label = lv_label_create(btn);
    lv_label_set_text(label, "PRESS ME!");
    lv_obj_set_style_text_font(label, &lv_font_montserrat_32, 0);
    lv_obj_center(label);
    
    Serial.println("Touch button created!");
    Serial.println("Touch the green button on screen to test.");
}

void loop() {
    lv_timer_handler();
    delay(5);
}