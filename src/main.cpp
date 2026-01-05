#include <Arduino.h>
#include <lvgl.h>
#include "LilyGo_AMOLED.h"
#include "LV_Helper.h"
#include "ui_setup.h"
#include <BleKeyboard.h>
#include <esp_wifi.h>
#include <esp_bt.h>
#include <esp_gap_ble_api.h>

extern BleKeyboard bleKeyboard;
LilyGo_Class amoled;

void setup()
{
    Serial.begin(115200);
    delay(2000);
    Serial.println("\n=== T4-S3 BT Keyboard ===");

    bool rslt = amoled.begin();
    if (!rslt)
    {
        Serial.println("ERROR: Display init failed!");
        while (1)
            delay(1000);
    }

    Serial.println("Display initialized successfully");
    delay(500);

    beginLvglHelper(amoled);
    Serial.println("LVGL initialized");

    setupUI();

    // Disable WiFi for BLE stability
    esp_wifi_stop();
    esp_wifi_deinit();

    // Release Classic BT memory
    esp_bt_controller_mem_release(ESP_BT_MODE_CLASSIC_BT);

    Serial.println("Ready - Press Device 1 or Device 2 to start");
}

void loop()
{
    lv_task_handler();
    delay(5);
}