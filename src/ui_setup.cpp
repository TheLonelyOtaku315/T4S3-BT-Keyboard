#include "ui_setup.h"
#include "media_controls.h"
#include <Arduino.h>
#include <BleKeyboard.h>
#include <esp_gap_ble_api.h>
#include "LilyGo_AMOLED.h"

// External references
extern LilyGo_Class amoled;

// Bluetooth keyboard object
BleKeyboard bleKeyboard("T4-S3 Keyboard", "LilyGO", 100);

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

lv_obj_t *tabview;
lv_obj_t *page1;
lv_obj_t *page2;
lv_obj_t *page3;
lv_obj_t *page4;

lv_obj_t *btn1;
lv_obj_t *btn2;
lv_obj_t *btn3;
lv_obj_t *btn4;

lv_obj_t *status_screen = NULL;
lv_obj_t *status_label = NULL;

lv_obj_t *media_page = NULL;
lv_obj_t *connection_banner = NULL;
lv_obj_t *battery_label = NULL;
lv_obj_t *battery_label_util = NULL;

// Show status overlay (Connecting...)
void showStatusScreen(const char *message)
{
    if (status_screen == NULL)
    {
        status_screen = lv_obj_create(lv_scr_act());
        lv_obj_set_size(status_screen, LV_HOR_RES, LV_VER_RES);
        lv_obj_set_style_bg_color(status_screen, lv_color_black(), 0);
        lv_obj_set_style_bg_opa(status_screen, LV_OPA_80, 0);
        lv_obj_center(status_screen);

        status_label = lv_label_create(status_screen);
        lv_obj_set_style_text_font(status_label, &lv_font_montserrat_32, 0);
        lv_obj_set_style_text_color(status_label, lv_color_white(), 0);
        lv_obj_center(status_label);
    }

    lv_label_set_text(status_label, message);
    lv_obj_center(status_label);
    lv_obj_clear_flag(status_screen, LV_OBJ_FLAG_HIDDEN);
}

void hideStatusScreen()
{
    if (status_screen != NULL)
    {
        lv_obj_add_flag(status_screen, LV_OBJ_FLAG_HIDDEN);
    }
}

void showMediaPage()
{
    hideStatusScreen();

    // Create page3 on first connection if it doesn't exist
    if (page3 == NULL)
    {
        page3 = lv_tabview_add_tab(tabview, "Connected");
        setupMediaControlsPage(page3);
        Serial.println("Media controls page created on connection!");
    }
    if (page4 == NULL)
    {
        page4 = lv_tabview_add_tab(tabview, "Utilities");
        Serial.println("Utilities page created on connection!");
    }

    lv_tabview_set_act(tabview, 2, LV_ANIM_ON); // Switch to page 3 (index 2)
}

// Update connection status banner
void updateConnectionBanner(bool connected)
{
    if (connection_banner == NULL)
        return;

    if (connected)
    {
        lv_label_set_text(connection_banner, "CONNECTED");
        lv_obj_set_style_bg_color(connection_banner, lv_color_make(0, 255, 0), 0);
        lv_obj_set_style_bg_opa(connection_banner, LV_OPA_30, 0);
        lv_obj_set_style_border_color(connection_banner, lv_color_make(0, 255, 0), 0);
        lv_obj_set_style_text_color(connection_banner, lv_color_make(0, 255, 0), 0);
    }
    else
    {
        lv_label_set_text(connection_banner, "DISCONNECTED");
        lv_obj_set_style_bg_color(connection_banner, lv_color_make(255, 0, 0), 0);
        lv_obj_set_style_bg_opa(connection_banner, LV_OPA_30, 0);
        lv_obj_set_style_border_color(connection_banner, lv_color_make(255, 0, 0), 0);
        lv_obj_set_style_text_color(connection_banner, lv_color_make(255, 0, 0), 0);
    }
}

// Media control button callback
void media_btn_event_cb(lv_event_t *e)
{
    const char *action = (const char *)lv_event_get_user_data(e);
    Serial.print("Media control: ");
    Serial.println(action);

    if (!bleKeyboard.isConnected())
    {
        Serial.println("ERROR: Bluetooth not connected!");
        return;
    }

    // Send media keys based on action
    if (strcmp(action, "previous") == 0)
    {
        bleKeyboard.write(KEY_MEDIA_PREVIOUS_TRACK);
    }
    else if (strcmp(action, "play_pause") == 0)
    {
        bleKeyboard.write(KEY_MEDIA_PLAY_PAUSE);
    }
    else if (strcmp(action, "next") == 0)
    {
        bleKeyboard.write(KEY_MEDIA_NEXT_TRACK);
    }
    else if (strcmp(action, "volume_up") == 0)
    {
        bleKeyboard.write(KEY_MEDIA_VOLUME_UP);
    }
    else if (strcmp(action, "volume_down") == 0)
    {
        bleKeyboard.write(KEY_MEDIA_VOLUME_DOWN);
    }
    else if (strcmp(action, "mute") == 0)
    {
        bleKeyboard.write(KEY_MEDIA_MUTE);
    }
}

lv_obj_t *create_media_button(lv_obj_t *parent, const char *icon, const char *label, const char *action,
                              lv_color_t color, uint8_t col, uint8_t row)
{
    lv_obj_t *btn = lv_btn_create(parent);
    lv_obj_set_grid_cell(btn, LV_GRID_ALIGN_STRETCH, col, 1, LV_GRID_ALIGN_STRETCH, row, 1);

    // Glass effect styling
    lv_obj_set_style_bg_color(btn, lv_color_make(40, 40, 60), 0);
    lv_obj_set_style_bg_opa(btn, LV_OPA_50, 0);
    lv_obj_set_style_border_color(btn, lv_color_make(80, 80, 120), 0);
    lv_obj_set_style_border_width(btn, 1, 0);
    lv_obj_set_style_border_opa(btn, LV_OPA_60, 0);
    lv_obj_set_style_radius(btn, 8, 0);

    // Hover effect (pressed state)
    lv_obj_set_style_bg_opa(btn, LV_OPA_70, LV_STATE_PRESSED);

    // Container for icon and label (flex column)
    lv_obj_set_flex_flow(btn, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(btn, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_set_style_pad_all(btn, 8, 0);

    // Icon (using Unicode symbols)
    lv_obj_t *icon_label = lv_label_create(btn);
    lv_label_set_text(icon_label, icon);
    lv_obj_set_style_text_font(icon_label, &lv_font_montserrat_48, 0);
    lv_obj_set_style_text_color(icon_label, color, 0);

    // Text label - UPPERCASE and bold style
    lv_obj_t *text_label = lv_label_create(btn);
    lv_label_set_text(text_label, label);
    lv_obj_set_style_text_font(text_label, &lv_font_montserrat_14, 0);
    lv_obj_set_style_text_color(text_label, lv_color_white(), 0);
    lv_label_set_long_mode(text_label, LV_LABEL_LONG_WRAP);
    lv_obj_set_style_text_align(text_label, LV_TEXT_ALIGN_CENTER, 0);

    // Add callback
    lv_obj_add_event_cb(btn, media_btn_event_cb, LV_EVENT_CLICKED, (void *)action);

    return btn;
}

// Browser button callback
void browserButtonEventCallback(lv_event_t *e)
{
    if (!bleKeyboard.isConnected())
    {
        Serial.println("BLE not connected");
        return;
    }

    // Open Run dialog (Win+R), then launch URL in default browser
    bleKeyboard.press(KEY_LEFT_GUI); // Windows key
    bleKeyboard.press('r');          // R key
    delay(50);
    bleKeyboard.releaseAll();
    delay(50); // Wait for Run dialog to open

    bleKeyboard.print("https://google.com");
    delay(50);
    bleKeyboard.write(KEY_RETURN);

    Serial.println("Browser opened with URL");
}

// Calculator button callback
void calculatorButtonEventCallback(lv_event_t *e)
{
    if (!bleKeyboard.isConnected())
    {
        Serial.println("BLE not connected");
        return;
    }

    // Send Windows + R to open Run dialog, then type calc and press Enter
    bleKeyboard.press(KEY_LEFT_GUI);
    bleKeyboard.press('r');
    delay(50);
    bleKeyboard.releaseAll();
    delay(50);
    bleKeyboard.print("calc");
    delay(50);
    bleKeyboard.write(KEY_RETURN);

    Serial.println("Calculator opened");
}

// Device connection button callback
void buttonEventCallback(lv_event_t *e)
{
    const char *message = (const char *)lv_event_get_user_data(e);
    Serial.println(message);

    lv_obj_t *btn = lv_event_get_target(e);

    if (btn == btn1)
    {
        showStatusScreen("Connecting...");
        startBluetoothWithName("T4-S3 Keyboard 1");

        // Wait briefly then check connection
        delay(2000);
        if (bleKeyboard.isConnected())
        {
            showStatusScreen("Connected!");
            delay(1000);
            showMediaPage();
        }
    }
    else if (btn == btn2)
    {
        showStatusScreen("Connecting...");
        startBluetoothWithName("T4-S3 Keyboard 2");

        // Wait briefly then check connection
        delay(2000);
        if (bleKeyboard.isConnected())
        {
            showStatusScreen("Connected!");
            delay(1000);
            showMediaPage();
        }
    }
}

void setupConnectionPage(lv_obj_t *parent)
{
    // 2x2 grid
    static lv_coord_t col_dsc[] = {LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST};
    static lv_coord_t row_dsc[] = {LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST};

    lv_obj_t *grid = lv_obj_create(parent);
    lv_obj_set_size(grid, LV_PCT(100), LV_PCT(100));
    lv_obj_set_layout(grid, LV_LAYOUT_GRID);
    lv_obj_set_grid_dsc_array(grid, col_dsc, row_dsc);
    lv_obj_set_style_pad_column(grid, 10, 0);
    lv_obj_set_style_pad_row(grid, 10, 0);
    lv_obj_set_style_pad_all(grid, 10, 0);
    lv_obj_set_style_bg_opa(grid, LV_OPA_TRANSP, 0);
    lv_obj_set_style_border_width(grid, 0, 0);

    // Device 1
    btn1 = lv_btn_create(grid);
    lv_obj_set_grid_cell(btn1, LV_GRID_ALIGN_STRETCH, 0, 1, LV_GRID_ALIGN_STRETCH, 0, 1);
    lv_obj_add_event_cb(btn1, buttonEventCallback, LV_EVENT_CLICKED, (void *)"Connecting to Device 1");
    lv_obj_set_style_bg_color(btn1, lv_color_make(0, 130, 252), 0);

    lv_obj_t *label1 = lv_label_create(btn1);
    lv_label_set_text(label1, "Device 1");
    lv_obj_set_style_text_font(label1, &lv_font_montserrat_24, 0);
    lv_obj_center(label1);

    // Device 2
    btn2 = lv_btn_create(grid);
    lv_obj_set_grid_cell(btn2, LV_GRID_ALIGN_STRETCH, 1, 1, LV_GRID_ALIGN_STRETCH, 0, 1);
    lv_obj_add_event_cb(btn2, buttonEventCallback, LV_EVENT_CLICKED, (void *)"Connecting to Device 2");
    lv_obj_set_style_bg_color(btn2, lv_color_make(255, 130, 0), 0);

    lv_obj_t *label2 = lv_label_create(btn2);
    lv_label_set_text(label2, "Device 2");
    lv_obj_set_style_text_font(label2, &lv_font_montserrat_24, 0);
    lv_obj_center(label2);

    // Device 3
    btn3 = lv_btn_create(grid);
    lv_obj_set_grid_cell(btn3, LV_GRID_ALIGN_STRETCH, 0, 1, LV_GRID_ALIGN_STRETCH, 1, 1);
    lv_obj_add_event_cb(btn3, buttonEventCallback, LV_EVENT_CLICKED, (void *)"Device 3 (Not implemented)");
    lv_obj_set_style_bg_color(btn3, lv_color_make(0, 200, 100), 0);

    lv_obj_t *label3 = lv_label_create(btn3);
    lv_label_set_text(label3, "Device 3");
    lv_obj_set_style_text_font(label3, &lv_font_montserrat_24, 0);
    lv_obj_center(label3);

    // Settings
    btn4 = lv_btn_create(grid);
    lv_obj_set_grid_cell(btn4, LV_GRID_ALIGN_STRETCH, 1, 1, LV_GRID_ALIGN_STRETCH, 1, 1);
    lv_obj_add_event_cb(btn4, buttonEventCallback, LV_EVENT_CLICKED, (void *)"Settings");
    lv_obj_set_style_bg_color(btn4, lv_color_make(255, 0, 200), 0);

    lv_obj_t *label4 = lv_label_create(btn4);
    lv_label_set_text(label4, "Settings");
    lv_obj_set_style_text_font(label4, &lv_font_montserrat_24, 0);
    lv_obj_center(label4);
}

void setupInfoPage(lv_obj_t *parent)
{
    lv_obj_t *container = lv_obj_create(parent);
    lv_obj_set_size(container, LV_PCT(100), LV_PCT(100));
    lv_obj_set_style_pad_all(container, 10, 0);
    lv_obj_set_style_bg_opa(container, LV_OPA_TRANSP, 0);
    lv_obj_set_style_border_width(container, 0, 0);
    lv_obj_set_flex_flow(container, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(container, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);

    lv_obj_t *title = lv_label_create(container);
    lv_label_set_text(title, "Board Information");
    lv_obj_set_style_text_font(title, &lv_font_montserrat_28, 0);
    lv_obj_set_style_text_color(title, lv_color_make(0, 200, 255), 0);

    lv_obj_t *model = lv_label_create(container);
    lv_label_set_text(model, "Model: T4-S3 AMOLED");
    lv_obj_set_style_text_font(model, &lv_font_montserrat_20, 0);

    lv_obj_t *display = lv_label_create(container);
    lv_label_set_text(display, "Display: 2.41\" AMOLED");
    lv_obj_set_style_text_font(display, &lv_font_montserrat_20, 0);

    lv_obj_t *chip = lv_label_create(container);
    lv_label_set_text(chip, "Chip: ESP32-S3");
    lv_obj_set_style_text_font(chip, &lv_font_montserrat_20, 0);

    String macStr = "MAC: " + String((uint32_t)ESP.getEfuseMac(), HEX);
    lv_obj_t *mac = lv_label_create(container);
    lv_label_set_text(mac, macStr.c_str());
    lv_obj_set_style_text_font(mac, &lv_font_montserrat_16, 0);

    // Battery info (store reference globally)
    battery_label = lv_label_create(container);
    lv_label_set_text(battery_label, "Battery: Checking...");
    lv_obj_set_style_text_font(battery_label, &lv_font_montserrat_20, 0);
    lv_obj_set_style_text_color(battery_label, lv_color_make(100, 255, 150), 0);

    lv_obj_t *hint = lv_label_create(container);
    lv_label_set_text(hint, "\n< Swipe to return");
    lv_obj_set_style_text_font(hint, &lv_font_montserrat_14, 0);
}

void setupMediaControlsPage(lv_obj_t *parent)
{
    // Create glass container
    lv_obj_t *glass = lv_obj_create(parent);
    lv_obj_set_size(glass, LV_PCT(100), LV_PCT(100));

    // Glass effect background
    lv_obj_set_style_bg_color(glass, lv_color_make(20, 20, 40), 0);
    lv_obj_set_style_bg_opa(glass, LV_OPA_30, 0);
    lv_obj_set_style_border_color(glass, lv_color_make(80, 80, 120), 0);
    lv_obj_set_style_border_width(glass, 2, 0);
    lv_obj_set_style_border_opa(glass, LV_OPA_50, 0);
    lv_obj_set_style_radius(glass, 16, 0);
    lv_obj_set_style_shadow_width(glass, 30, 0);
    lv_obj_set_style_shadow_color(glass, lv_color_black(), 0);
    lv_obj_set_style_shadow_opa(glass, LV_OPA_40, 0);
    lv_obj_set_style_pad_all(glass, 10, 0);

    // Create grid container (below banner)
    lv_obj_t *grid = lv_obj_create(glass);
    lv_obj_set_size(grid, LV_PCT(100), LV_PCT(100));
    lv_obj_align(grid, LV_ALIGN_BOTTOM_MID, 0, 0);
    lv_obj_set_style_bg_opa(grid, LV_OPA_TRANSP, 0);
    lv_obj_set_style_border_width(grid, 0, 0);
    lv_obj_set_style_pad_top(grid, 30, 0); // Space for banner

    // Setup 3x2 grid layout
    static lv_coord_t col_dsc[] = {LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST};
    static lv_coord_t row_dsc[] = {LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST};

    lv_obj_set_layout(grid, LV_LAYOUT_GRID);
    lv_obj_set_grid_dsc_array(grid, col_dsc, row_dsc);
    lv_obj_set_style_pad_all(grid, 5, 0);
    lv_obj_set_style_pad_column(grid, 10, 0);
    lv_obj_set_style_pad_row(grid, 10, 0);

    // Create buttons (3 columns x 2 rows)
    // Row 0
    create_media_button(grid, LV_SYMBOL_PREV, "PREVIOUS", "previous",
                        lv_color_make(100, 150, 255), 0, 0);

    create_media_button(grid, LV_SYMBOL_PLAY, "PLAY/PAUSE", "play_pause",
                        lv_color_make(100, 255, 150), 1, 0);

    create_media_button(grid, LV_SYMBOL_NEXT, "NEXT", "next",
                        lv_color_make(100, 150, 255), 2, 0);

    // Row 1
    create_media_button(grid, LV_SYMBOL_VOLUME_MAX, "VOLUME UP", "volume_up",
                        lv_color_make(255, 200, 100), 0, 1);

    create_media_button(grid, LV_SYMBOL_VOLUME_MID, "VOLUME DOWN", "volume_down",
                        lv_color_make(255, 200, 100), 1, 1);

    create_media_button(grid, LV_SYMBOL_MUTE, "MUTE", "mute",
                        lv_color_make(255, 100, 100), 2, 1);

    // Set initial connection status
    updateConnectionBanner(bleKeyboard.isConnected());

    Serial.println("Media controls page created with status banner!");
}

// Create utility button (for browser, calculator, etc.)
lv_obj_t *create_utility_button(lv_obj_t *parent, const char *icon, const char *label,
                                lv_event_cb_t callback, lv_color_t color, uint8_t col, uint8_t row)
{
    lv_obj_t *btn = lv_btn_create(parent);
    lv_obj_set_grid_cell(btn, LV_GRID_ALIGN_STRETCH, col, 1, LV_GRID_ALIGN_STRETCH, row, 1);

    // Glass effect styling
    lv_obj_set_style_bg_color(btn, lv_color_make(40, 40, 60), 0);
    lv_obj_set_style_bg_opa(btn, LV_OPA_50, 0);
    lv_obj_set_style_border_color(btn, lv_color_make(80, 80, 120), 0);
    lv_obj_set_style_border_width(btn, 1, 0);
    lv_obj_set_style_border_opa(btn, LV_OPA_60, 0);
    lv_obj_set_style_radius(btn, 8, 0);

    // Hover effect (pressed state)
    lv_obj_set_style_bg_opa(btn, LV_OPA_70, LV_STATE_PRESSED);

    // Container for icon and label (flex column)
    lv_obj_set_flex_flow(btn, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(btn, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_set_style_pad_all(btn, 8, 0);

    // Icon (using Unicode symbols)
    lv_obj_t *icon_label = lv_label_create(btn);
    lv_label_set_text(icon_label, icon);
    lv_obj_set_style_text_font(icon_label, &lv_font_montserrat_48, 0);
    lv_obj_set_style_text_color(icon_label, color, 0);

    // Text label - UPPERCASE and bold style
    lv_obj_t *text_label = lv_label_create(btn);
    lv_label_set_text(text_label, label);
    lv_obj_set_style_text_font(text_label, &lv_font_montserrat_14, 0);
    lv_obj_set_style_text_color(text_label, lv_color_white(), 0);
    lv_label_set_long_mode(text_label, LV_LABEL_LONG_WRAP);
    lv_obj_set_style_text_align(text_label, LV_TEXT_ALIGN_CENTER, 0);

    // Add callback
    lv_obj_add_event_cb(btn, callback, LV_EVENT_CLICKED, NULL);

    return btn;
}

void setupUtilityPage(lv_obj_t *parent)
{
    // Create glass container
    lv_obj_t *glass = lv_obj_create(parent);
    lv_obj_set_size(glass, LV_PCT(100), LV_PCT(100));

    // Glass effect background
    lv_obj_set_style_bg_color(glass, lv_color_make(20, 20, 40), 0);
    lv_obj_set_style_bg_opa(glass, LV_OPA_30, 0);
    lv_obj_set_style_border_color(glass, lv_color_make(80, 80, 120), 0);
    lv_obj_set_style_border_width(glass, 2, 0);
    lv_obj_set_style_border_opa(glass, LV_OPA_50, 0);
    lv_obj_set_style_radius(glass, 16, 0);
    lv_obj_set_style_shadow_width(glass, 30, 0);
    lv_obj_set_style_shadow_color(glass, lv_color_black(), 0);
    lv_obj_set_style_shadow_opa(glass, LV_OPA_40, 0);
    lv_obj_set_style_pad_all(glass, 10, 0);

    // Info panel at top
    lv_obj_t *info_panel = lv_obj_create(glass);
    lv_obj_set_size(info_panel, LV_PCT(100), 80);
    lv_obj_align(info_panel, LV_ALIGN_TOP_MID, 0, 0);
    lv_obj_set_style_bg_color(info_panel, lv_color_make(40, 40, 60), 0);
    lv_obj_set_style_bg_opa(info_panel, LV_OPA_50, 0);
    lv_obj_set_style_border_color(info_panel, lv_color_make(80, 80, 120), 0);
    lv_obj_set_style_border_width(info_panel, 1, 0);
    lv_obj_set_style_border_opa(info_panel, LV_OPA_60, 0);
    lv_obj_set_style_radius(info_panel, 8, 0);
    lv_obj_set_flex_flow(info_panel, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(info_panel, LV_FLEX_ALIGN_SPACE_EVENLY, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);

    // Time display
    lv_obj_t *time_label = lv_label_create(info_panel);
    lv_label_set_text(time_label, "12:34");
    lv_obj_set_style_text_font(time_label, &lv_font_montserrat_24, 0);
    lv_obj_set_style_text_color(time_label, lv_color_make(100, 200, 255), 0);

    // Battery display (store reference globally)
    battery_label_util = lv_label_create(info_panel);
    lv_label_set_text(battery_label_util, LV_SYMBOL_BATTERY_FULL " --");
    lv_obj_set_style_text_font(battery_label_util, &lv_font_montserrat_24, 0);
    lv_obj_set_style_text_color(battery_label_util, lv_color_make(100, 255, 150), 0);

    // Connection status
    lv_obj_t *conn_label = lv_label_create(info_panel);
    lv_label_set_text(conn_label, LV_SYMBOL_BLUETOOTH " ON");
    lv_obj_set_style_text_font(conn_label, &lv_font_montserrat_20, 0);
    lv_obj_set_style_text_color(conn_label, lv_color_make(150, 150, 255), 0);

    // Create grid container for buttons
    lv_obj_t *grid = lv_obj_create(glass);
    lv_obj_set_size(grid, LV_PCT(100), LV_PCT(100));
    lv_obj_align(grid, LV_ALIGN_BOTTOM_MID, 0, 0);
    lv_obj_set_style_bg_opa(grid, LV_OPA_TRANSP, 0);
    lv_obj_set_style_border_width(grid, 0, 0);
    lv_obj_set_style_pad_top(grid, 90, 0); // Space for info panel

    // Setup 2x2 grid layout
    static lv_coord_t col_dsc[] = {LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST};
    static lv_coord_t row_dsc[] = {LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST};

    lv_obj_set_layout(grid, LV_LAYOUT_GRID);
    lv_obj_set_grid_dsc_array(grid, col_dsc, row_dsc);
    lv_obj_set_style_pad_all(grid, 5, 0);
    lv_obj_set_style_pad_column(grid, 10, 0);
    lv_obj_set_style_pad_row(grid, 10, 0);

    // Create buttons (2 columns x 2 rows)
    // Row 0
    create_utility_button(grid, LV_SYMBOL_IMAGE, "BROWSER", browserButtonEventCallback,
                          lv_color_make(100, 150, 255), 0, 0);

    create_utility_button(grid, "#", "CALCULATOR", calculatorButtonEventCallback,
                          lv_color_make(255, 200, 100), 1, 0);

    // Row 1 - Add more utility buttons here
    create_utility_button(grid, LV_SYMBOL_FILE, "NOTEPAD", browserButtonEventCallback,
                          lv_color_make(150, 255, 150), 0, 1);

    create_utility_button(grid, LV_SYMBOL_SETTINGS, "SETTINGS", browserButtonEventCallback,
                          lv_color_make(255, 150, 200), 1, 1);

    Serial.println("Utility page created with info panel!");
}

// Battery monitoring function
void updateBatteryDisplay()
{
    static char battText[32];
    uint16_t battVoltage = amoled.getBattVoltage();
    bool isCharging = amoled.isVbusIn();

    if (battVoltage > 0)
    {
        // Calculate battery percentage based on voltage
        // 4.2V = 100%, 3.7V = 50%, 3.3V = 0%
        int percentage = 0;
        if (battVoltage >= 4100)
        {
            percentage = 100;
        }
        else if (battVoltage >= 3700)
        {
            percentage = 50 + ((battVoltage - 3700) * 50) / 400;
        }
        else if (battVoltage >= 3300)
        {
            percentage = ((battVoltage - 3300) * 50) / 400;
        }

        // Choose battery icon based on percentage
        const char *icon;
        if (percentage >= 90)
            icon = LV_SYMBOL_BATTERY_FULL;
        else if (percentage >= 60)
            icon = LV_SYMBOL_BATTERY_3;
        else if (percentage >= 30)
            icon = LV_SYMBOL_BATTERY_2;
        else if (percentage >= 10)
            icon = LV_SYMBOL_BATTERY_1;
        else
            icon = LV_SYMBOL_BATTERY_EMPTY;

        if (isCharging)
        {
            snprintf(battText, sizeof(battText), "%s %d%% " LV_SYMBOL_CHARGE, icon, percentage);
        }
        else
        {
            snprintf(battText, sizeof(battText), "%s %d%%", icon, percentage);
        }

        // Update color based on battery level
        lv_color_t color;
        if (percentage >= 30)
        {
            color = lv_color_make(100, 255, 150); // Green
        }
        else if (percentage >= 15)
        {
            color = lv_color_make(255, 200, 100); // Orange
        }
        else
        {
            color = lv_color_make(255, 100, 100); // Red
        }

        // Update all battery labels
        if (battery_label != NULL)
        {
            lv_label_set_text(battery_label, battText);
            lv_obj_set_style_text_color(battery_label, color, 0);
        }
        if (battery_label_util != NULL)
        {
            lv_label_set_text(battery_label_util, battText);
            lv_obj_set_style_text_color(battery_label_util, color, 0);
        }
    }
    else
    {
        // No battery detected
        const char *noBattText = LV_SYMBOL_BATTERY_EMPTY " USB";
        if (battery_label != NULL)
        {
            lv_label_set_text(battery_label, noBattText);
        }
        if (battery_label_util != NULL)
        {
            lv_label_set_text(battery_label_util, noBattText);
        }
    }
}

void setupUI()
{
    tabview = lv_tabview_create(lv_scr_act(), LV_DIR_TOP, 0);
    lv_obj_set_size(tabview, LV_PCT(100), LV_PCT(100));

    page1 = lv_tabview_add_tab(tabview, "Connections");
    page2 = lv_tabview_add_tab(tabview, "Board Info");
    // page4 = lv_tabview_add_tab(tabview, "Utilities");
    // page3 will be created dynamically when first connected

    setupConnectionPage(page1);
    setupInfoPage(page2);
    setupUtilityPage(page4);

    // Initial battery update
    updateBatteryDisplay();

    Serial.println("UI ready with 3 pages! (Media page will appear on connection)");
}