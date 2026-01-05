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

// Power management settings
#define NORMAL_BRIGHTNESS 128 // 50% brightness for power saving
#define DIM_BRIGHTNESS 30     // Dimmed brightness
#define DIM_TIMEOUT 30000     // Dim after 30 seconds
#define SLEEP_TIMEOUT 60000   // Turn off after 60 seconds

unsigned long lastActivityTime = 0;
bool isDisplayDimmed = false;
bool isDisplayOff = false;

void resetActivityTimer()
{
    lastActivityTime = millis();

    // Wake up display if it was off or dimmed
    if (isDisplayOff)
    {
        amoled.setBrightness(NORMAL_BRIGHTNESS);
        isDisplayOff = false;
        isDisplayDimmed = false;
        Serial.println("Display ON");
    }
    else if (isDisplayDimmed)
    {
        amoled.setBrightness(NORMAL_BRIGHTNESS);
        isDisplayDimmed = false;
        Serial.println("Display restored to normal brightness");
    }
}

void checkDisplayTimeout()
{
    unsigned long currentTime = millis();
    unsigned long inactiveTime = currentTime - lastActivityTime;

    // Turn off display after sleep timeout
    if (!isDisplayOff && inactiveTime >= SLEEP_TIMEOUT)
    {
        amoled.setBrightness(0); // True black on AMOLED = minimal power
        isDisplayOff = true;
        isDisplayDimmed = false;
        Serial.println("Display OFF (power saving)");
    }
    // Dim display after dim timeout
    else if (!isDisplayDimmed && !isDisplayOff && inactiveTime >= DIM_TIMEOUT)
    {
        amoled.setBrightness(DIM_BRIGHTNESS);
        isDisplayDimmed = true;
        Serial.println("Display dimmed");
    }
}

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

    // Set power-efficient brightness (50%)
    amoled.setBrightness(NORMAL_BRIGHTNESS);
    Serial.printf("Brightness set to %d (50%% for power saving)\n", NORMAL_BRIGHTNESS);

    // Initialize activity timer
    lastActivityTime = millis();

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

    // Check for touch activity to wake display
    if (amoled.isPressed())
    {
        resetActivityTimer();
    }

    // Check display timeout for auto-dim and sleep
    checkDisplayTimeout();

    delay(5);
}