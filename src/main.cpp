#include <Arduino.h>
#include <lvgl.h>
#include "hardware/LilyGo_AMOLED.h"
#include "LV_Helper.h"
#include "ui/ui.h"

LilyGo_Class amoled;

// State tracking
int currentDevice = 0; // 0 = none, 1 = device1, 2 = device2, 3 = device3

// Battery percentage based on typical LiPo discharge curve
int calculateBatteryPercent(uint16_t voltage)
{
    // LiPo voltage ranges:
    // 4.2V = 100% (fully charged)
    // 3.7V = ~50% (nominal voltage)
    // 3.3V = 0% (empty, cut-off)

    if (voltage >= 4200)
        return 100;
    if (voltage <= 3300)
        return 0;

    // Linear approximation from 3.3V to 4.2V
    // Range: 900mV (4200 - 3300)
    return ((voltage - 3300) * 100) / 900;
}
// Battery update function
void updateBattery()
{
    // Get raw voltage - this is the most reliable reading
    uint16_t voltage = amoled.getBattVoltage();
    bool isUsbConnected = amoled.isVbusIn();

    // Calculate percentage from voltage (most reliable method for T4-S3)
    int batteryPercent = 0;
    bool isBatteryConnected = (voltage > 3000);

    if (isBatteryConnected)
    {
        batteryPercent = calculateBatteryPercent(voltage);
    }

    // Determine charging state: USB connected AND battery present AND voltage increasing
    // Note: isCharging() is unreliable, so we use USB + battery presence instead
    bool isCharging = isUsbConnected && isBatteryConnected;

    // Update battery percentage label
    char batteryText[32];

    if (!isBatteryConnected && isUsbConnected)
    {
        // USB connected but no battery
        snprintf(batteryText, sizeof(batteryText), "USB Only");
        lv_bar_set_value(ui_batteryIcon, 100, LV_ANIM_ON);
    }
    else if (!isBatteryConnected)
    {
        // No battery and no USB
        snprintf(batteryText, sizeof(batteryText), "No Battery");
        lv_bar_set_value(ui_batteryIcon, 0, LV_ANIM_ON);
    }
    else if (isCharging)
    {
        // Charging with battery present
        snprintf(batteryText, sizeof(batteryText), LV_SYMBOL_CHARGE " Charging %d%%", batteryPercent);
        lv_bar_set_value(ui_batteryIcon, batteryPercent, LV_ANIM_ON);
    }
    else
    {
        // On battery power
        snprintf(batteryText, sizeof(batteryText), "%d%%", batteryPercent);
        lv_bar_set_value(ui_batteryIcon, batteryPercent, LV_ANIM_ON);
    }

    Serial.printf("Voltage: %dmV, Percent: %d%%, USB: %d, Text: '%s'\n",
                  voltage, batteryPercent, isUsbConnected, batteryText);
    lv_label_set_text(ui_batteryPourcentage, batteryText);
}

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

    // Set initial battery display
    updateBattery();

    Serial.println("All button handlers connected - Ready!");
}

void loop()
{
    lv_task_handler();

    // Debug: Print battery info every 2 seconds
    static unsigned long lastDebugPrint = 0;
    if (millis() - lastDebugPrint > 2000)
    {
        int batteryPercent = amoled.getBatteryPercent();
        bool isCharging = amoled.isCharging();
        bool isUsbConnected = amoled.isVbusIn();
        uint16_t battVoltage = amoled.getBattVoltage();

        Serial.printf("Batt: %d%%, Voltage: %d mV, Charging: %d, USB: %d\n",
                      batteryPercent, battVoltage, isCharging, isUsbConnected);
        lastDebugPrint = millis();
    }

    // Update battery every 2 seconds
    static unsigned long lastBatteryUpdate = 0;
    if (millis() - lastBatteryUpdate > 2000)
    {
        updateBattery();
        lastBatteryUpdate = millis();
    }

    delay(5);
}