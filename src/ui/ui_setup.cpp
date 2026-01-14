#include "ui_setup.h"
#include "ui_config.h"
#include "components/status_screen.h"
#include "pages/connection_page.h"
#include "pages/info_page.h"
#include "pages/media_page.h"
#include "pages/utility_page.h"
#include "../hardware/LilyGo_AMOLED.h"
#include <Arduino.h>
#include <esp_gap_ble_api.h>

// External references
extern LilyGo_Class amoled;
extern BleKeyboard bleKeyboard; // Defined in main.cpp

// Global UI objects
lv_obj_t *tabview;
lv_obj_t *page1 = NULL;
lv_obj_t *page2 = NULL;
lv_obj_t *page3 = NULL;
lv_obj_t *page4 = NULL;

lv_obj_t *btn1 = NULL;
lv_obj_t *btn2 = NULL;
lv_obj_t *btn3 = NULL;
lv_obj_t *btn4 = NULL;

lv_obj_t *media_page = NULL;
lv_obj_t *connection_banner = NULL;
lv_obj_t *battery_label = NULL;
lv_obj_t *battery_label_util = NULL;

void startBluetoothWithName(const char *deviceName)
{
    // End previous connection if exists
    if (bleKeyboard.isConnected())
    {
        bleKeyboard.end();
        delay(500);
    }

    // Update device name and restart
    bleKeyboard.setName(deviceName);
    bleKeyboard.begin();

    // Configure "Just Works" pairing
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
    Serial.print("Bluetooth started as: ");
    Serial.println(deviceName);
}

void showMediaPage()
{
    hideStatusScreen();

    // Create page3 on first connection if it doesn't exist
    if (page3 == NULL)
    {
        page3 = lv_tabview_add_tab(tabview, "Connected");
        lv_obj_set_style_pad_all(page3, 0, 0);
        lv_obj_set_style_border_width(page3, 0, 0);
        lv_obj_set_style_bg_color(page3, lv_color_black(), 0);

        setupMediaControlsPage(page3);
        Serial.println("Media controls page created on connection!");
    }
    if (page4 == NULL)
    {
        page4 = lv_tabview_add_tab(tabview, "Utilities");
        lv_obj_set_style_pad_all(page4, 0, 0);
        lv_obj_set_style_border_width(page4, 0, 0);
        lv_obj_set_style_bg_color(page4, lv_color_black(), 0);

        setupUtilityPage(page4);
        Serial.println("Utilities page created on connection!");
    }

    lv_tabview_set_act(tabview, 2, LV_ANIM_ON);
}

void setupUI()
{
    // Remove padding from the screen itself
    lv_obj_t *screen = lv_scr_act();
    lv_obj_set_style_pad_all(screen, 0, 0);
    lv_obj_set_style_bg_color(screen, lv_color_black(), 0);
    lv_obj_set_style_border_width(screen, 0, 0);

    // Create tabview
    tabview = lv_tabview_create(screen, LV_DIR_TOP, 0);
    lv_obj_set_size(tabview, lv_disp_get_hor_res(NULL), lv_disp_get_ver_res(NULL));
    lv_obj_set_scrollbar_mode(tabview, LV_SCROLLBAR_MODE_OFF);

    // Remove all padding, borders, and radius from tabview
    lv_obj_set_style_pad_all(tabview, 0, 0);
    lv_obj_set_style_border_width(tabview, 0, 0);
    lv_obj_set_style_radius(tabview, 0, 0);
    lv_obj_set_style_bg_color(tabview, lv_color_black(), 0);

    // Get the tab content area and ensure it has no padding or borders
    lv_obj_t *tab_content = lv_tabview_get_content(tabview);
    lv_obj_set_style_pad_all(tab_content, 0, 0);
    lv_obj_set_style_border_width(tab_content, 0, 0);
    lv_obj_set_style_bg_color(tab_content, lv_color_black(), 0);

    // Create initial pages
    page1 = lv_tabview_add_tab(tabview, "Connections");
    page2 = lv_tabview_add_tab(tabview, "Board Info");

    // Remove padding and borders from pages
    lv_obj_set_style_pad_all(page1, 0, 0);
    lv_obj_set_style_border_width(page1, 0, 0);
    lv_obj_set_style_bg_color(page1, lv_color_black(), 0);

    lv_obj_set_style_pad_all(page2, 0, 0);
    lv_obj_set_style_border_width(page2, 0, 0);
    lv_obj_set_style_bg_color(page2, lv_color_black(), 0);

    // Setup pages
    setupConnectionPage(page1);
    setupInfoPage(page2);

    // Update battery display
    updateBatteryDisplay();

    Serial.println("UI ready! (Media page will appear on connection)");
}
