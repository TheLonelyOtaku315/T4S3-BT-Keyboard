#include "connection_page.h"
#include "../old_custom_ui/ui_config.h"
#include "../components/button_factory.h"
#include "../components/status_screen.h"
#include "../old_custom_ui/ui_setup.h"
#include <Arduino.h>
#include <BleKeyboard.h>

extern lv_obj_t *btn1, *btn2, *btn3, *btn4;
extern lv_obj_t *tabview;
extern lv_obj_t *page3, *page4;

// Device connection button callback
static void buttonEventCallback(lv_event_t *e)
{
    const char *message = (const char *)lv_event_get_user_data(e);
    Serial.println(message);

    lv_obj_t *btn = lv_event_get_target(e);

    if (btn == btn1)
    {
        showStatusScreen("Connecting...");
        startBluetoothWithName("T4-S3 Keyboard 1");

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
    // Create glass container
    lv_obj_t *glass = lv_obj_create(parent);
    lv_obj_set_size(glass, LV_PCT(100), LV_PCT(100));
    UIHelpers::applyGlassStyle(glass, false);

    // 2x2 grid
    static lv_coord_t col_dsc[] = {LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST};
    static lv_coord_t row_dsc[] = {LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST};

    lv_obj_t *grid = lv_obj_create(glass);
    lv_obj_set_size(grid, LV_PCT(100), LV_PCT(100));
    lv_obj_set_layout(grid, LV_LAYOUT_GRID);
    lv_obj_set_grid_dsc_array(grid, col_dsc, row_dsc);
    lv_obj_set_style_pad_column(grid, UIDimensions::GRID_SPACING, 0);
    lv_obj_set_style_pad_row(grid, UIDimensions::GRID_SPACING, 0);
    lv_obj_set_style_pad_all(grid, UIDimensions::GRID_PADDING, 0);
    UIHelpers::makeTransparent(grid);

    // Device 1
    btn1 = createConnectionButton(grid, LV_SYMBOL_BLUETOOTH, "DEVICE 1",
                                  buttonEventCallback, 0, 0);
    lv_obj_set_user_data(btn1, (void *)"Connecting to Device 1");

    // Device 2
    btn2 = createConnectionButton(grid, LV_SYMBOL_BLUETOOTH, "DEVICE 2",
                                  buttonEventCallback, 1, 0);
    lv_obj_set_user_data(btn2, (void *)"Connecting to Device 2");

    // Device 3
    btn3 = createConnectionButton(grid, LV_SYMBOL_BLUETOOTH, "DEVICE 3",
                                  buttonEventCallback, 0, 1);
    lv_obj_set_user_data(btn3, (void *)"Device 3 (Not implemented)");

    // Settings
    btn4 = createConnectionButton(grid, LV_SYMBOL_SETTINGS, "SETTINGS",
                                  buttonEventCallback, 1, 1);
    lv_obj_set_user_data(btn4, (void *)"Settings");
}
