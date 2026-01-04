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

bool wasConnected = false;
unsigned long lastConnectionCheck = 0;
const unsigned long CONNECTION_CHECK_INTERVAL = 200; // Check every 200ms
int connectionStableCount = 0;
const int STABILITY_THRESHOLD = 5; // Require 5 consistent readings (1 second)

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

    // Start Bluetooth
    bleKeyboard.begin();

    // Configure "Just Works" pairing (automatic encryption, no PIN)
    esp_ble_auth_req_t auth_req = ESP_LE_AUTH_BOND;
    esp_ble_io_cap_t iocap = ESP_IO_CAP_NONE;
    uint8_t key_size = 16;
    uint8_t init_key = ESP_BLE_ENC_KEY_MASK | ESP_BLE_ID_KEY_MASK;
    uint8_t rsp_key = ESP_BLE_ENC_KEY_MASK | ESP_BLE_ID_KEY_MASK;
    uint8_t auth_option = ESP_BLE_ONLY_ACCEPT_SPECIFIED_AUTH_DISABLE;

    esp_ble_gap_set_security_param(ESP_BLE_SM_AUTHEN_REQ_MODE, &auth_req, sizeof(uint8_t));
    esp_ble_gap_set_security_param(ESP_BLE_SM_IOCAP_MODE, &iocap, sizeof(uint8_t));
    esp_ble_gap_set_security_param(ESP_BLE_SM_MAX_KEY_SIZE, &key_size, sizeof(uint8_t));
    esp_ble_gap_set_security_param(ESP_BLE_SM_ONLY_ACCEPT_SPECIFIED_SEC_AUTH, &auth_option, sizeof(uint8_t));
    esp_ble_gap_set_security_param(ESP_BLE_SM_SET_INIT_KEY, &init_key, sizeof(uint8_t));
    esp_ble_gap_set_security_param(ESP_BLE_SM_SET_RSP_KEY, &rsp_key, sizeof(uint8_t));

    delay(500);
    Serial.println("Bluetooth ready");
}

void loop()
{
    lv_task_handler();

    // Check Bluetooth connection status periodically (not every loop)
    unsigned long currentMillis = millis();
    if (currentMillis - lastConnectionCheck >= CONNECTION_CHECK_INTERVAL)
    {
        lastConnectionCheck = currentMillis;

        bool isConnected = bleKeyboard.isConnected();

        // Debounce connection state changes
        if (isConnected == wasConnected)
        {
            // State is stable, reset counter
            connectionStableCount = 0;
        }
        else
        {
            // State changed, increment counter
            connectionStableCount++;

            // Only accept state change if it's been stable for threshold
            if (connectionStableCount >= STABILITY_THRESHOLD)
            {
                if (isConnected && !wasConnected)
                {
                    Serial.println("Bluetooth connected!");
                    showStatusScreen("Connected!");
                    delay(1500);
                    showConnectedPage();
                    wasConnected = true;
                }
                else if (!isConnected && wasConnected)
                {
                    Serial.println("Bluetooth disconnected!");
                    wasConnected = false;
                }

                connectionStableCount = 0;
            }
        }
    }

    delay(5);
}