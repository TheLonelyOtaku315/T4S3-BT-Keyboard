#include <Arduino.h>
#include <lvgl.h>
#include "hardware/LilyGo_AMOLED.h"
#include "ui/LV_Helper.h"

LilyGo_Class amoled;

// Button object
lv_obj_t *btn;
lv_obj_t *label;

// Button click callback
void button_event_cb(lv_event_t *e)
{
    Serial.println("Button pressed");
}

void setup()
{
    Serial.begin(115200);
    delay(2000);
    Serial.println("\n=== Simple Button Test ===");

    bool rslt = amoled.begin();
    if (!rslt)
    {
        Serial.println("ERROR: Display init failed!");
        while (1)
            delay(1000);
    }

    Serial.println("Display initialized successfully");
    amoled.setBrightness(128);

    beginLvglHelper(amoled);
    Serial.println("LVGL initialized");

    // Create a simple button in the center of the screen
    btn = lv_btn_create(lv_scr_act());
    lv_obj_set_size(btn, 200, 80);
    lv_obj_center(btn);
    lv_obj_add_event_cb(btn, button_event_cb, LV_EVENT_CLICKED, NULL);

    // Add label to button
    label = lv_label_create(btn);
    lv_label_set_text(label, "Press Me");
    lv_obj_center(label);

    Serial.println("Button created - Ready!");
}

void loop()
{
    lv_task_handler();
    delay(5);
}