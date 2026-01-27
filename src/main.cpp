#include <Arduino.h>
#include <lvgl.h>
#include <BleKeyboard.h>
#include "hardware/LilyGo_AMOLED.h"
#include "LV_Helper.h"
#include "ui/ui.h"
#include "tab1_device_control.h"
#include "tab2_media_control.h"
#include "tab3_app_shortcuts.h"

LilyGo_Class amoled;
BleKeyboard bleKeyboard("T4-S3 Keyboard", "LilyGo", 100);

// Disconnect timer variables (used by disconnect button handler)
unsigned long disconnectTime = 0;
bool showingDisconnected = false;
bool bleDisabled = false; // Track if BLE was manually disabled

struct BatteryPoint
{
    uint16_t voltage;
    uint8_t percent;
};

uint8_t getBatteryPercent(uint16_t voltage_mv)
{
    static const BatteryPoint curve[] = {
        {3300, 0},
        {3600, 5},
        {3700, 25},
        {3800, 40},
        {3900, 65},
        {4000, 80},
        {4100, 90},
        {4200, 100}};

    const uint8_t size = sizeof(curve) / sizeof(curve[0]);

    if (voltage_mv <= curve[0].voltage)
        return curve[0].percent;
    if (voltage_mv >= curve[size - 1].voltage)
        return curve[size - 1].percent;

    // Find the right segment and interpolate
    for (uint8_t i = 0; i < size - 1; i++)
    {
        if (voltage_mv >= curve[i].voltage && voltage_mv < curve[i + 1].voltage)
        {
            return curve[i].percent +
                   ((voltage_mv - curve[i].voltage) *
                    (curve[i + 1].percent - curve[i].percent)) /
                       (curve[i + 1].voltage - curve[i].voltage);
        }
    }

    return 0;
}

bool isCharging()
{
    uint16_t vbus = amoled.getVbusVoltage();
    uint16_t vbat = amoled.getBattVoltage();

    // USB connected AND battery not full
    return (vbus > 4500) && (vbat < 4200);
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
    amoled.setRotation(2); // Rotate screen 180 degrees

    // BLE will be started manually when Device 1 button is pressed
    bleDisabled = true;
    Serial.println("BLE is OFF - Press Device 1 to start Bluetooth");

    beginLvglHelper(amoled);
    Serial.println("LVGL initialized");

    // Initialize SquareLine Studio UI
    ui_init();
    Serial.println("UI loaded");

    // Hide tabs 2 and 3 initially since BLE is disabled
    lv_obj_add_flag(ui_mediaTab, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui_appTab, LV_OBJ_FLAG_HIDDEN);

    // Initialize Tab1, Tab2, and Tab3 event handlers
    tab1_init();
    tab2_init();
    tab3_init();

    Serial.println("All button handlers connected - Ready!");
}

void loop()
{
    lv_task_handler();

    // Update info tab live data every 1 second
    static unsigned long lastInfoUpdate = 0;
    if (millis() - lastInfoUpdate >= 1000)
    {
        lastInfoUpdate = millis();

        // Update temperature
        float temp = amoled.readCoreTemp();
        char tempStr[16];
        sprintf(tempStr, "%.1f°C", temp);
        lv_label_set_text(ui_Label18, tempStr);

        // Update free heap memory
        int freeHeap = ESP.getFreeHeap() / 1024; // Convert to KB
        char heapStr[16];
        sprintf(heapStr, "%d KB", freeHeap);
        lv_label_set_text(ui_Label17, heapStr);

        // Update battery info
        uint16_t vbat = amoled.getBattVoltage();
        uint8_t percent = getBatteryPercent(vbat);
        char battStr[8];
        sprintf(battStr, "%d%%", percent);
        lv_label_set_text(ui_batteryPourcentage, battStr);

        // Set battery icon
        if (isCharging())
        {
            lv_img_set_src(ui_batteryIcon, &ui_img_battery_android_frame_bolt_24dp_e3e3e3_fill0_wght400_grad0_opsz24_png);
        }
        else if (percent >= 100)
        {
            lv_img_set_src(ui_batteryIcon, &ui_img_battery_android_frame_full_24dp_e3e3e3_fill0_wght400_grad0_opsz24_png);
        }
        else if (percent >= 91)
        {
            lv_img_set_src(ui_batteryIcon, &ui_img_battery_android_frame_6_24dp_e3e3e3_fill0_wght400_grad0_opsz24_png);
        }
        else if (percent >= 81)
        {
            lv_img_set_src(ui_batteryIcon, &ui_img_battery_android_frame_5_24dp_e3e3e3_fill0_wght400_grad0_opsz24_png);
        }
        else if (percent >= 66)
        {
            lv_img_set_src(ui_batteryIcon, &ui_img_battery_android_frame_4_24dp_e3e3e3_fill0_wght400_grad0_opsz24_png);
        }
        else if (percent >= 41)
        {
            lv_img_set_src(ui_batteryIcon, &ui_img_battery_android_frame_3_24dp_e3e3e3_fill0_wght400_grad0_opsz24_png);
        }
        else if (percent >= 26)
        {
            lv_img_set_src(ui_batteryIcon, &ui_img_battery_android_frame_2_24dp_e3e3e3_fill0_wght400_grad0_opsz24_png);
        }
        else if (percent >= 10)
        {
            lv_img_set_src(ui_batteryIcon, &ui_img_battery_android_frame_1_24dp_e3e3e3_fill0_wght400_grad0_opsz24_png);
        }
        else
        {
            lv_img_set_src(ui_batteryIcon, &ui_img_battery_android_frame_alert_24dp_e3e3e3_fill0_wght400_grad0_opsz24_png);
        }
    }

    // Monitor connection status when BLE is active
    if (!bleDisabled)
    {
        static bool lastConnectionState = false;
        bool currentConnectionState = bleKeyboard.isConnected();

        if (currentConnectionState != lastConnectionState)
        {
            lastConnectionState = currentConnectionState;

            if (currentConnectionState)
            {
                // Just connected
                Serial.println("Device connected!");
                lv_label_set_text(ui_heaterContent, "Connected " LV_SYMBOL_OK);
                lv_obj_set_style_bg_color(ui_heater, lv_color_hex(0x2D8659), LV_PART_MAIN | LV_STATE_DEFAULT);
                lv_obj_clear_flag(ui_mediaTab, LV_OBJ_FLAG_HIDDEN);
                lv_obj_clear_flag(ui_appTab, LV_OBJ_FLAG_HIDDEN);
            }
            else
            {
                // Disconnected (but BLE still active)
                Serial.println("Device disconnected!");
                lv_label_set_text(ui_heaterContent, "Disconnected");
                lv_obj_set_style_bg_color(ui_heater, lv_color_hex(0xC42B1C), LV_PART_MAIN | LV_STATE_DEFAULT);
                lv_obj_add_flag(ui_mediaTab, LV_OBJ_FLAG_HIDDEN);
                lv_obj_add_flag(ui_appTab, LV_OBJ_FLAG_HIDDEN);
                disconnectTime = millis();
                showingDisconnected = true;
            }
        }
    }

    // After 4 seconds of showing Disconnected, change to Ready
    if (showingDisconnected && (millis() - disconnectTime >= 4000))
    {
        lv_label_set_text(ui_heaterContent, "T4-S3 Keyboard Ready");
        lv_obj_set_style_bg_color(ui_heater, lv_color_hex(0x2D2D30), LV_PART_MAIN | LV_STATE_DEFAULT); // Default surface color
        showingDisconnected = false;
    }

    delay(5);
}