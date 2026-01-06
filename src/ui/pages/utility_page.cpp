#include "utility_page.h"
#include "../ui_config.h"
#include "../components/button_factory.h"
#include <Arduino.h>
#include <BleKeyboard.h>

extern BleKeyboard bleKeyboard;
extern lv_obj_t *battery_label_util;

// Browser button callback
static void browserButtonEventCallback(lv_event_t *e)
{
    if (!bleKeyboard.isConnected())
    {
        Serial.println("BLE not connected");
        return;
    }

    bleKeyboard.press(KEY_LEFT_GUI);
    bleKeyboard.press('r');
    delay(50);
    bleKeyboard.releaseAll();
    delay(50);
    bleKeyboard.print("https://google.com");
    delay(50);
    bleKeyboard.write(KEY_RETURN);

    Serial.println("Browser opened with URL");
}

// Calculator button callback
static void calculatorButtonEventCallback(lv_event_t *e)
{
    if (!bleKeyboard.isConnected())
    {
        Serial.println("BLE not connected");
        return;
    }

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

void setupUtilityPage(lv_obj_t *parent)
{
    // Create glass container
    lv_obj_t *glass = lv_obj_create(parent);
    lv_obj_set_size(glass, LV_PCT(100), LV_PCT(100));
    UIHelpers::applyGlassStyle(glass, true);

    // Info panel at top
    lv_obj_t *info_panel = lv_obj_create(glass);
    lv_obj_set_size(info_panel, LV_PCT(100), UIDimensions::INFO_PANEL_HEIGHT);
    lv_obj_align(info_panel, LV_ALIGN_TOP_MID, 0, 0);
    lv_obj_set_style_bg_color(info_panel, UIColors::BUTTON_BG, 0);
    lv_obj_set_style_bg_opa(info_panel, UIOpacity::BUTTON_BG, 0);
    lv_obj_set_style_border_color(info_panel, UIColors::BUTTON_BORDER, 0);
    lv_obj_set_style_border_width(info_panel, UIDimensions::BUTTON_BORDER, 0);
    lv_obj_set_style_border_opa(info_panel, UIOpacity::BUTTON_BORDER, 0);
    lv_obj_set_style_radius(info_panel, UIDimensions::BUTTON_RADIUS, 0);
    lv_obj_set_flex_flow(info_panel, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(info_panel, LV_FLEX_ALIGN_SPACE_EVENLY, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);

    // Time display
    lv_obj_t *time_label = lv_label_create(info_panel);
    lv_label_set_text(time_label, "12:34");
    lv_obj_set_style_text_font(time_label, UIFonts::INFO_LARGE, 0);
    lv_obj_set_style_text_color(time_label, UIColors::TEXT_SECONDARY, 0);

    // Battery display
    battery_label_util = lv_label_create(info_panel);
    lv_label_set_text(battery_label_util, LV_SYMBOL_BATTERY_FULL " --");
    lv_obj_set_style_text_font(battery_label_util, UIFonts::INFO_LARGE, 0);
    lv_obj_set_style_text_color(battery_label_util, UIColors::TEXT_TERTIARY, 0);

    // Connection status
    lv_obj_t *conn_label = lv_label_create(info_panel);
    lv_label_set_text(conn_label, LV_SYMBOL_BLUETOOTH " ON");
    lv_obj_set_style_text_font(conn_label, UIFonts::INFO_MEDIUM, 0);
    lv_obj_set_style_text_color(conn_label, UIColors::TEXT_TERTIARY, 0);

    // Create grid container for buttons
    lv_obj_t *grid = lv_obj_create(glass);
    lv_obj_set_size(grid, LV_PCT(100), LV_PCT(100));
    lv_obj_align(grid, LV_ALIGN_BOTTOM_MID, 0, 0);
    UIHelpers::makeTransparent(grid);
    lv_obj_set_style_pad_top(grid, UIDimensions::INFO_PANEL_SPACING, 0);

    // Setup 2x2 grid layout
    static lv_coord_t col_dsc[] = {LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST};
    static lv_coord_t row_dsc[] = {LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST};

    lv_obj_set_layout(grid, LV_LAYOUT_GRID);
    lv_obj_set_grid_dsc_array(grid, col_dsc, row_dsc);
    lv_obj_set_style_pad_all(grid, UIDimensions::GRID_PADDING, 0);
    lv_obj_set_style_pad_column(grid, UIDimensions::GRID_SPACING, 0);
    lv_obj_set_style_pad_row(grid, UIDimensions::GRID_SPACING, 0);

    // Create buttons
    createUtilityButton(grid, LV_SYMBOL_IMAGE, "BROWSER", browserButtonEventCallback,
                        lv_color_make(100, 150, 255), 0, 0);

    createUtilityButton(grid, "#", "CALCULATOR", calculatorButtonEventCallback,
                        lv_color_make(255, 200, 100), 1, 0);

    createUtilityButton(grid, LV_SYMBOL_FILE, "NOTEPAD", browserButtonEventCallback,
                        lv_color_make(150, 255, 150), 0, 1);

    createUtilityButton(grid, LV_SYMBOL_SETTINGS, "SETTINGS", browserButtonEventCallback,
                        lv_color_make(255, 150, 200), 1, 1);

    Serial.println("Utility page created!");
}
