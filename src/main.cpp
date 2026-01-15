#include <Arduino.h>
#include <lvgl.h>
#include "hardware/LilyGo_AMOLED.h"
#include "LV_Helper.h"
#include "ui/ui.h"

LilyGo_Class amoled;

// State tracking
int currentDevice = 0; // 0 = none, 1 = device1, 2 = device2, 3 = device3

// Button event handlers
void device1_btn_handler(lv_event_t *e)
{
    currentDevice = 1;
    lv_label_set_text(ui_heaterContent, "Device 1 Selected");
    Serial.println("Device 1 button pressed");
}

void device2_btn_handler(lv_event_t *e)
{
    currentDevice = 2;
    lv_label_set_text(ui_heaterContent, "Device 2 Selected");
    Serial.println("Device 2 button pressed");
}

void device3_btn_handler(lv_event_t *e)
{
    currentDevice = 3;
    lv_label_set_text(ui_heaterContent, "Device 3 Selected");
    Serial.println("Device 3 button pressed");
}

void disconnect_btn_handler(lv_event_t *e)
{
    currentDevice = 0;
    lv_label_set_text(ui_heaterContent, "Disconnected");
    Serial.println("Disconnect button pressed");
}

void reboot_btn_handler(lv_event_t *e)
{
    lv_label_set_text(ui_heaterContent, "Rebooting...");
    Serial.println("Reboot button pressed");
    delay(1000);
    ESP.restart();
}

void setting_btn_handler(lv_event_t *e)
{
    lv_label_set_text(ui_heaterContent, "Settings");
    Serial.println("Settings button pressed");
}

void setup()
{
    Serial.begin(115200);
    delay(2000);
    Serial.println("\n=== T4-S3 Bluetooth Macropad ===");

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

    // Initialize SquareLine Studio UI
    ui_init();
    Serial.println("UI loaded");

    // Connect button event handlers
    lv_obj_add_event_cb(ui_device1Btn, device1_btn_handler, LV_EVENT_CLICKED, NULL);
    lv_obj_add_event_cb(ui_device2Btn, device2_btn_handler, LV_EVENT_CLICKED, NULL);
    lv_obj_add_event_cb(ui_device3Btn, device3_btn_handler, LV_EVENT_CLICKED, NULL);
    lv_obj_add_event_cb(ui_disconnectBtn, disconnect_btn_handler, LV_EVENT_CLICKED, NULL);
    lv_obj_add_event_cb(ui_rebootBtn, reboot_btn_handler, LV_EVENT_CLICKED, NULL);
    lv_obj_add_event_cb(ui_settingBtn, setting_btn_handler, LV_EVENT_CLICKED, NULL);

    // Set initial label text
    lv_label_set_text(ui_heaterContent, "Ready");

    Serial.println("All button handlers connected - Ready!");
}

void loop()
{
    lv_task_handler();
    delay(5);
}