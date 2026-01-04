#include <Arduino.h>
#include "LilyGo_AMOLED.h"
#include "LV_Helper.h"
#include "ui_setup.h"

LilyGo_Class amoled;

void setup()
{
    Serial.begin(115200);
    delay(2000);
    Serial.println("\n=== T4-S3 Touch Button Test ===");

    // Initialize display with LVGL
    bool rslt = amoled.begin();
    if (!rslt)
    {
        Serial.println("ERROR: Display init failed!");
        while (1)
        {
            delay(1000);
        }
    }

    Serial.println("Display initialized successfully");

    // Wait a bit for everything to settle
    delay(500);

    // Initialize LVGL helper
    beginLvglHelper(amoled);

    Serial.println("LVGL initialized");

    setupUI();
}

void loop()
{
    lv_timer_handler();
    delay(5);
}