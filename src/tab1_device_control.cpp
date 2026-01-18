#include "tab1_device_control.h"
#include "ui/ui.h"
#include <Arduino.h>
#include <esp_system.h>

// External variables for disconnect timer (defined in main.cpp)
extern unsigned long disconnectTime;
extern bool showingDisconnected;

// Tab1 Button Event Handlers
void device1_btn_handler(lv_event_t *e)
{
    Serial.println("Device 1 button pressed");

    if (bleKeyboard.isConnected())
    {
        lv_label_set_text(ui_heaterContent, "Connected " LV_SYMBOL_OK);
        lv_obj_set_style_bg_color(ui_heater, lv_color_hex(0x2D8659), LV_PART_MAIN | LV_STATE_DEFAULT); // Dark green
        lv_obj_clear_flag(ui_tab2, LV_OBJ_FLAG_HIDDEN);                                                // Show tab2
    }
    else
    {
        lv_label_set_text(ui_heaterContent, "Pair Now");
        lv_obj_set_style_bg_color(ui_heater, lv_color_hex(0xC97B3E), LV_PART_MAIN | LV_STATE_DEFAULT); // Burnt orange
        lv_obj_add_flag(ui_tab2, LV_OBJ_FLAG_HIDDEN);                                                  // Hide tab2
    }
}

void device2_btn_handler(lv_event_t *e)
{
    Serial.println("Device 2 - Not configured");
    lv_label_set_text(ui_heaterContent, "Device 2 - Not Configured");
}

void disconnect_btn_handler(lv_event_t *e)
{
    if (bleKeyboard.isConnected())
    {
        bleKeyboard.end();
        Serial.println("BLE Keyboard stopped");
    }

    // Hide tab2 when disconnected
    lv_obj_add_flag(ui_tab2, LV_OBJ_FLAG_HIDDEN);

    lv_label_set_text(ui_heaterContent, "Disconnected");
    lv_obj_set_style_bg_color(ui_heater, lv_color_hex(0xC42B1C), LV_PART_MAIN | LV_STATE_DEFAULT); // Red

    // Trigger 4-second timer
    disconnectTime = millis();
    showingDisconnected = true;

    Serial.println("Disconnect button pressed");
}

void reboot_btn_handler(lv_event_t *e)
{
    lv_label_set_text(ui_heaterContent, "Rebooting...");
    Serial.println("Reboot button pressed");
    delay(2000);
    ESP.restart();
}

void setting_btn_handler(lv_event_t *e)
{
    lv_label_set_text(ui_heaterContent, "Settings");
    Serial.println("Settings button pressed");
}

void info_btn_handler(lv_event_t *e)
{
    // Check if button is checked or unchecked
    if (lv_obj_has_state(ui_infoBtn, LV_STATE_CHECKED))
    {
        // Button is checked - show info tab and hide main tabs
        lv_obj_clear_flag(ui_infoTab, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(ui_main, LV_OBJ_FLAG_HIDDEN);
        Serial.println("Info tab opened");
    }
    else
    {
        // Button is unchecked - hide info tab and show main tabs
        lv_obj_add_flag(ui_infoTab, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(ui_main, LV_OBJ_FLAG_HIDDEN);
        Serial.println("Info tab closed");
    }
}

void info_close_btn_handler(lv_event_t *e)
{
    // Uncheck the info button
    lv_obj_clear_state(ui_infoBtn, LV_STATE_CHECKED);

    // Hide the info tab and show main tabs
    lv_obj_add_flag(ui_infoTab, LV_OBJ_FLAG_HIDDEN);
    lv_obj_clear_flag(ui_main, LV_OBJ_FLAG_HIDDEN);

    Serial.println("Info tab closed via close button");
}

void populate_info_tab_static()
{
    // Get and display MAC address
    uint8_t mac[6];
    esp_efuse_mac_get_default(mac);
    char macStr[18];
    sprintf(macStr, "%02X:%02X:%02X:%02X:%02X:%02X",
            mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
    lv_label_set_text(ui_Label20, macStr);

    // Get and display chip info
    char chipStr[32];
    sprintf(chipStr, "ESP32-S3 Rev %d", ESP.getChipRevision());
    lv_label_set_text(ui_Label19, chipStr);

    Serial.println("Info tab static data populated");
}

// Initialize Tab1 event handlers
void tab1_init()
{
    // Connect button event handlers for Tab1
    lv_obj_add_event_cb(ui_device1Btn, device1_btn_handler, LV_EVENT_CLICKED, NULL);

    lv_obj_add_event_cb(ui_device2Btn, device2_btn_handler, LV_EVENT_CLICKED, NULL);
    lv_obj_add_event_cb(ui_infoBtn, info_btn_handler, LV_EVENT_CLICKED, NULL);
    lv_obj_add_event_cb(ui_disconnectBtn, disconnect_btn_handler, LV_EVENT_CLICKED, NULL);
    lv_obj_add_event_cb(ui_rebootBtn, reboot_btn_handler, LV_EVENT_CLICKED, NULL);
    lv_obj_add_event_cb(ui_settingBtn, setting_btn_handler, LV_EVENT_CLICKED, NULL);
    lv_obj_add_event_cb(ui_Button1, info_close_btn_handler, LV_EVENT_CLICKED, NULL);

    // Set initial label text and default color
    lv_label_set_text(ui_heaterContent, "Ready");
    lv_obj_set_style_bg_color(ui_heater, lv_color_hex(0x2D2D30), LV_PART_MAIN | LV_STATE_DEFAULT); // Default surface color

    // Hide tab2 until Bluetooth is connected
    lv_obj_add_flag(ui_tab2, LV_OBJ_FLAG_HIDDEN);

    // Hide info tab initially
    lv_obj_add_flag(ui_infoTab, LV_OBJ_FLAG_HIDDEN);

    // Populate static info (MAC, Chip)
    populate_info_tab_static();

    Serial.println("Tab1 (Device Control) handlers connected");
}
