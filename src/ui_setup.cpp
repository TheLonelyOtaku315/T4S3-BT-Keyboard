#include "ui_setup.h"
#include <Arduino.h>
#include <BleKeyboard.h>

// Bluetooth keyboard object with improved stability settings
// Battery level set to 100 to prevent connection issues
BleKeyboard bleKeyboard("T4-S3 Keyboard", "LilyGO", 100);

lv_obj_t *tabview;
lv_obj_t *page1;
lv_obj_t *page2;
lv_obj_t *page3;

lv_obj_t *btn1;
lv_obj_t *btn2;
lv_obj_t *btn3;
lv_obj_t *btn4;

lv_obj_t *status_screen = NULL;
lv_obj_t *status_label = NULL;

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

void showConnectedPage()
{
    hideStatusScreen();
    lv_tabview_set_act(tabview, 2, LV_ANIM_ON); // Switch to page 3 (index 2)
}

// Browser button callback
void browser_btn_event_cb(lv_event_t *e)
{
    Serial.println("Opening browser...");

    if (bleKeyboard.isConnected())
    {
        // Send GUI+R to open Run dialog (Windows)
        // Or Command+Space on Mac
        bleKeyboard.press(KEY_LEFT_GUI);
        bleKeyboard.press('r');
        delay(100);
        bleKeyboard.releaseAll();
        delay(200);

        // Type browser command
        bleKeyboard.print("chrome"); // or "firefox", "msedge"
        delay(100);
        bleKeyboard.press(KEY_RETURN);
        bleKeyboard.releaseAll();

        Serial.println("Browser command sent!");
    }
    else
    {
        Serial.println("ERROR: Bluetooth not connected!");
    }
}

// Device connection button callback
void btn_event_cb(lv_event_t *e)
{
    const char *message = (const char *)lv_event_get_user_data(e);
    Serial.println(message);

    // Check which button was pressed
    lv_obj_t *btn = lv_event_get_target(e);

    if (btn == btn1 || btn == btn2)
    {
        // Check Bluetooth connection status
        if (bleKeyboard.isConnected())
        {
            Serial.println("Already connected!");
            showStatusScreen("Connected!");
            delay(1000);
            showConnectedPage();
        }
        else
        {
            // Still waiting for connection
            showStatusScreen("Connecting...");
            Serial.println("Waiting for Bluetooth connection...");
        }
    }
}

void setupPage1(lv_obj_t *parent)
{
    // 2x2 grid
    static lv_coord_t col_dsc[] = {LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST};
    static lv_coord_t row_dsc[] = {LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST};

    lv_obj_t *grid = lv_obj_create(parent);
    lv_obj_set_size(grid, LV_PCT(100), LV_PCT(100));
    lv_obj_center(grid);
    lv_obj_set_layout(grid, LV_LAYOUT_GRID);
    lv_obj_set_grid_dsc_array(grid, col_dsc, row_dsc);
    lv_obj_set_style_pad_column(grid, 10, 0);
    lv_obj_set_style_pad_row(grid, 10, 0);
    lv_obj_set_style_pad_all(grid, 10, 0);

    // Device 1
    btn1 = lv_btn_create(grid);
    lv_obj_set_grid_cell(btn1, LV_GRID_ALIGN_STRETCH, 0, 1, LV_GRID_ALIGN_STRETCH, 0, 1);
    lv_obj_add_event_cb(btn1, btn_event_cb, LV_EVENT_CLICKED, (void *)"Connecting to Device 1");
    lv_obj_set_style_bg_color(btn1, lv_color_make(0, 130, 252), 0);

    lv_obj_t *label1 = lv_label_create(btn1);
    lv_label_set_text(label1, "Device 1");
    lv_obj_set_style_text_font(label1, &lv_font_montserrat_24, 0);
    lv_obj_center(label1);

    // Device 2
    btn2 = lv_btn_create(grid);
    lv_obj_set_grid_cell(btn2, LV_GRID_ALIGN_STRETCH, 1, 1, LV_GRID_ALIGN_STRETCH, 0, 1);
    lv_obj_add_event_cb(btn2, btn_event_cb, LV_EVENT_CLICKED, (void *)"Connecting to Device 2");
    lv_obj_set_style_bg_color(btn2, lv_color_make(255, 130, 0), 0);

    lv_obj_t *label2 = lv_label_create(btn2);
    lv_label_set_text(label2, "Device 2");
    lv_obj_set_style_text_font(label2, &lv_font_montserrat_24, 0);
    lv_obj_center(label2);

    // Device 3
    btn3 = lv_btn_create(grid);
    lv_obj_set_grid_cell(btn3, LV_GRID_ALIGN_STRETCH, 0, 1, LV_GRID_ALIGN_STRETCH, 1, 1);
    lv_obj_add_event_cb(btn3, btn_event_cb, LV_EVENT_CLICKED, (void *)"Device 3 (Not implemented)");
    lv_obj_set_style_bg_color(btn3, lv_color_make(0, 200, 100), 0);

    lv_obj_t *label3 = lv_label_create(btn3);
    lv_label_set_text(label3, "Device 3");
    lv_obj_set_style_text_font(label3, &lv_font_montserrat_24, 0);
    lv_obj_center(label3);

    // Settings
    btn4 = lv_btn_create(grid);
    lv_obj_set_grid_cell(btn4, LV_GRID_ALIGN_STRETCH, 1, 1, LV_GRID_ALIGN_STRETCH, 1, 1);
    lv_obj_add_event_cb(btn4, btn_event_cb, LV_EVENT_CLICKED, (void *)"Settings");
    lv_obj_set_style_bg_color(btn4, lv_color_make(255, 0, 200), 0);

    lv_obj_t *label4 = lv_label_create(btn4);
    lv_label_set_text(label4, "Settings");
    lv_obj_set_style_text_font(label4, &lv_font_montserrat_24, 0);
    lv_obj_center(label4);
}

void setupPage2(lv_obj_t *parent)
{
    lv_obj_t *container = lv_obj_create(parent);
    lv_obj_set_size(container, LV_PCT(100), LV_PCT(100));
    lv_obj_set_style_pad_all(container, 20, 0);
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

    lv_obj_t *hint = lv_label_create(container);
    lv_label_set_text(hint, "\n< Swipe to return");
    lv_obj_set_style_text_font(hint, &lv_font_montserrat_14, 0);
}

void setupPage3(lv_obj_t *parent)
{
    // Connected page with browser button
    static lv_coord_t col_dsc[] = {LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST};
    static lv_coord_t row_dsc[] = {LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST};

    lv_obj_t *grid = lv_obj_create(parent);
    lv_obj_set_size(grid, LV_PCT(100), LV_PCT(100));
    lv_obj_center(grid);
    lv_obj_set_layout(grid, LV_LAYOUT_GRID);
    lv_obj_set_grid_dsc_array(grid, col_dsc, row_dsc);
    lv_obj_set_style_pad_column(grid, 10, 0);
    lv_obj_set_style_pad_row(grid, 10, 0);
    lv_obj_set_style_pad_all(grid, 10, 0);

    // Browser button (top-left)
    lv_obj_t *browser_btn = lv_btn_create(grid);
    lv_obj_set_grid_cell(browser_btn, LV_GRID_ALIGN_STRETCH, 0, 1, LV_GRID_ALIGN_STRETCH, 0, 1);
    lv_obj_add_event_cb(browser_btn, browser_btn_event_cb, LV_EVENT_CLICKED, NULL);
    lv_obj_set_style_bg_color(browser_btn, lv_color_make(100, 200, 255), 0);

    lv_obj_t *browser_label = lv_label_create(browser_btn);
    lv_label_set_text(browser_label, "Browser");
    lv_obj_set_style_text_font(browser_label, &lv_font_montserrat_28, 0);
    lv_obj_center(browser_label);
}

void setupUI()
{
    tabview = lv_tabview_create(lv_scr_act(), LV_DIR_TOP, 0);

    page1 = lv_tabview_add_tab(tabview, "Connections");
    page2 = lv_tabview_add_tab(tabview, "Board Info");
    page3 = lv_tabview_add_tab(tabview, "Connected");

    setupPage1(page1);
    setupPage2(page2);
    setupPage3(page3);

    Serial.println("UI ready with 3 pages!");
}