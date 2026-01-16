#include <Arduino.h>
#include <lvgl.h>
#include "hardware/LilyGo_AMOLED.h"
#include "LV_Helper.h"
#include "ui/ui.h"

LilyGo_Class amoled;

// State tracking
int currentDevice = 0; // 0 = none, 1 = device1, 2 = device2, 3 = device3

// Voltage calibration offset (PMU reads higher than actual)
// Multimeter: 3.8-3.9V, PMU reads: 4.2V, so subtract ~350mV
const int BATTERY_VOLTAGE_OFFSET = -350;

// Calculate battery percentage from voltage (LiPo battery curve)
int calculateBatteryPercent(uint16_t voltage)
{
    // Apply calibration offset
    voltage += BATTERY_VOLTAGE_OFFSET;

    // LiPo voltage curve: 3000mV=0%, 4200mV=100%
    const int table[11] = {
        3000, 3650, 3700, 3740, 3760, 3795,
        3840, 3910, 3980, 4070, 4150};

    if (voltage < table[0])
        return 0;
    if (voltage >= table[10])
        return 100;

    for (int i = 0; i < 11; i++)
    {
        if (voltage < table[i])
            return i * 10 - (10 * (int)(table[i] - voltage)) / (int)(table[i] - table[i - 1]);
    }
    return 100;
}

// Battery update function
void updateBattery()
{
    int batteryPercent = amoled.getBatteryPercent();
    bool isCharging = amoled.isCharging();
    bool isUsbConnected = amoled.isVbusIn();

    // If PMU gauge not calibrated, calculate from voltage
    if (batteryPercent < 0 && amoled.isBatteryConnect())
    {
        uint16_t voltage = amoled.getBattVoltage();
        batteryPercent = calculateBatteryPercent(voltage);
        Serial.printf("Calculated %d%% from %dmV\n", batteryPercent, voltage);
    }

    // Update battery percentage label
    char batteryText[32];
    if (isCharging && batteryPercent >= 0)
    {
        // Charging with battery percentage
        snprintf(batteryText, sizeof(batteryText), LV_SYMBOL_CHARGE " Charging %d%%", batteryPercent);
        lv_bar_set_value(ui_batteryIcon, batteryPercent, LV_ANIM_ON);
    }
    else if (isUsbConnected && batteryPercent < 0)
    {
        // USB connected but no battery
        snprintf(batteryText, sizeof(batteryText), "USB Connected");
        lv_bar_set_value(ui_batteryIcon, 100, LV_ANIM_ON);
    }
    else if (batteryPercent >= 0)
    {
        // On battery power
        snprintf(batteryText, sizeof(batteryText), "%d%%", batteryPercent);
        lv_bar_set_value(ui_batteryIcon, batteryPercent, LV_ANIM_ON);
    }
    else
    {
        // Unknown state
        snprintf(batteryText, sizeof(batteryText), "--");
        lv_bar_set_value(ui_batteryIcon, 0, LV_ANIM_ON);
    }

    Serial.printf("Setting battery text: '%s'\n", batteryText);
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

    // Update battery every 5 seconds
    static unsigned long lastBatteryUpdate = 0;
    if (millis() - lastBatteryUpdate > 5000)
    {
        updateBattery();
        lastBatteryUpdate = millis();
    }

    delay(5);
}