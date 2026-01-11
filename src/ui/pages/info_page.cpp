#include "info_page.h"
#include "../ui_config.h"
#include "../components/button_factory.h"
#include "../ui_setup.h"
#include "../../hardware/LilyGo_AMOLED.h"
#include <Arduino.h>

extern LilyGo_Class amoled;
extern lv_obj_t *battery_label;

static lv_obj_t *voltage_label = NULL;
static lv_obj_t *charging_label = NULL;

void setupInfoPage(lv_obj_t *parent)
{
    // Create glass container
    lv_obj_t *glass = lv_obj_create(parent);
    lv_obj_set_size(glass, LV_PCT(100), LV_PCT(100));
    UIHelpers::applyGlassStyle(glass, false);

    // Create content container
    lv_obj_t *content = lv_obj_create(glass);
    lv_obj_set_size(content, LV_PCT(90), LV_PCT(90));
    lv_obj_center(content);
    UIHelpers::makeTransparent(content);
    lv_obj_set_flex_flow(content, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(content, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_set_style_pad_row(content, 15, 0);
    lv_obj_set_style_pad_all(content, 20, 0);

    // Title
    lv_obj_t *title = lv_label_create(content);
    lv_label_set_text(title, "Board Information");
    lv_obj_set_style_text_font(title, UIFonts::TITLE_LARGE, 0);
    lv_obj_set_style_text_color(title, lv_color_white(), 0);

    // Divider
    lv_obj_t *divider = lv_obj_create(content);
    lv_obj_set_size(divider, LV_PCT(100), 2);
    lv_obj_set_style_bg_color(divider, UIColors::GLASS_BORDER, 0);
    lv_obj_set_style_border_width(divider, 0, 0);

    // Board Name
    lv_obj_t *board_name = lv_label_create(content);
    lv_label_set_text(board_name, LV_SYMBOL_SETTINGS " T-Display AMOLED");
    lv_obj_set_style_text_font(board_name, UIFonts::INFO_MEDIUM, 0);
    lv_obj_set_style_text_color(board_name, lv_color_white(), 0);

    // Chip Info
    lv_obj_t *chip_info = lv_label_create(content);
    lv_label_set_text(chip_info, LV_SYMBOL_CHARGE " ESP32-S3");
    lv_obj_set_style_text_font(chip_info, UIFonts::INFO_MEDIUM, 0);
    lv_obj_set_style_text_color(chip_info, lv_color_white(), 0);

    // Battery Section Title
    lv_obj_t *battery_title = lv_label_create(content);
    lv_label_set_text(battery_title, "\nBattery Status");
    lv_obj_set_style_text_font(battery_title, UIFonts::INFO_MEDIUM, 0);
    lv_obj_set_style_text_color(battery_title, UIColors::TEXT_PRIMARY, 0);

    // Battery Voltage
    voltage_label = lv_label_create(content);
    lv_label_set_text(voltage_label, "Voltage: -- mV");
    lv_obj_set_style_text_font(voltage_label, UIFonts::INFO_SMALL, 0);
    lv_obj_set_style_text_color(voltage_label, lv_color_white(), 0);

    // Charging Status
    charging_label = lv_label_create(content);
    lv_label_set_text(charging_label, "Status: --");
    lv_obj_set_style_text_font(charging_label, UIFonts::INFO_SMALL, 0);
    lv_obj_set_style_text_color(charging_label, lv_color_white(), 0);

    // Battery Percentage (shared with other pages)
    battery_label = lv_label_create(content);
    lv_label_set_text(battery_label, "Level: --%");
    lv_obj_set_style_text_font(battery_label, UIFonts::INFO_SMALL, 0);
    lv_obj_set_style_text_color(battery_label, UIColors::BATTERY_NORMAL, 0);
}

void updateBatteryDisplay()
{
    if (!battery_label && !voltage_label && !charging_label)
        return;

    // Get battery voltage
    uint16_t voltage = amoled.getBattVoltage();
    bool isCharging = amoled.isCharging();

    // Calculate approximate battery percentage based on voltage
    // LiPo typical voltage range: 3.0V (0%) to 4.2V (100%)
    int percent = 0;
    if (voltage >= 4200)
    {
        percent = 100;
    }
    else if (voltage <= 3000)
    {
        percent = 0;
    }
    else
    {
        percent = map(voltage, 3000, 4200, 0, 100);
    }

    // Update voltage label
    if (voltage_label)
    {
        char voltage_text[32];
        snprintf(voltage_text, sizeof(voltage_text), "Voltage: %d mV", voltage);
        lv_label_set_text(voltage_label, voltage_text);
    }

    // Update charging status
    if (charging_label)
    {
        if (isCharging)
        {
            lv_label_set_text(charging_label, LV_SYMBOL_CHARGE " Charging");
            lv_obj_set_style_text_color(charging_label, UIColors::TEXT_PRIMARY, 0);
        }
        else
        {
            lv_label_set_text(charging_label, "Status: Discharging");
            lv_obj_set_style_text_color(charging_label, lv_color_white(), 0);
        }
    }

    // Update battery percentage
    if (battery_label)
    {
        char battery_text[32];
        snprintf(battery_text, sizeof(battery_text), "Level: %d%%", percent);
        lv_label_set_text(battery_label, battery_text);

        // Set color based on battery level
        if (percent > 50)
        {
            lv_obj_set_style_text_color(battery_label, UIColors::BATTERY_NORMAL, 0);
        }
        else if (percent > 20)
        {
            lv_obj_set_style_text_color(battery_label, UIColors::BATTERY_WARNING, 0);
        }
        else
        {
            lv_obj_set_style_text_color(battery_label, UIColors::BATTERY_LOW, 0);
        }
    }
}
