#include "info_page.h"
#include "../ui_config.h"
#include "../../hardware/LilyGo_AMOLED.h"
#include <Arduino.h>

extern LilyGo_Class amoled;
extern lv_obj_t *battery_label;
extern lv_obj_t *battery_label_util;

void setupInfoPage(lv_obj_t *parent)
{
    // Create glass container
    lv_obj_t *glass = lv_obj_create(parent);
    lv_obj_set_size(glass, LV_PCT(100), LV_PCT(100));
    UIHelpers::applyGlassStyle(glass, false);

    lv_obj_t *container = lv_obj_create(glass);
    lv_obj_set_size(container, LV_PCT(100), LV_PCT(100));
    lv_obj_set_style_pad_all(container, UIDimensions::GLASS_PADDING, 0);
    UIHelpers::makeTransparent(container);
    lv_obj_set_flex_flow(container, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(container, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);

    lv_obj_t *title = lv_label_create(container);
    lv_label_set_text(title, "Board Information");
    lv_obj_set_style_text_font(title, UIFonts::TITLE_MEDIUM, 0);
    lv_obj_set_style_text_color(title, UIColors::TEXT_PRIMARY, 0);

    lv_obj_t *model = lv_label_create(container);
    lv_label_set_text(model, "Model: T4-S3 AMOLED");
    lv_obj_set_style_text_font(model, UIFonts::INFO_MEDIUM, 0);

    lv_obj_t *display = lv_label_create(container);
    lv_label_set_text(display, "Display: 2.41\" AMOLED");
    lv_obj_set_style_text_font(display, UIFonts::INFO_MEDIUM, 0);

    lv_obj_t *chip = lv_label_create(container);
    lv_label_set_text(chip, "Chip: ESP32-S3");
    lv_obj_set_style_text_font(chip, UIFonts::INFO_MEDIUM, 0);

    String macStr = "MAC: " + String((uint32_t)ESP.getEfuseMac(), HEX);
    lv_obj_t *mac = lv_label_create(container);
    lv_label_set_text(mac, macStr.c_str());
    lv_obj_set_style_text_font(mac, UIFonts::INFO_SMALL, 0);

    // Battery info
    battery_label = lv_label_create(container);
    lv_label_set_text(battery_label, "Battery: Checking...");
    lv_obj_set_style_text_font(battery_label, UIFonts::INFO_MEDIUM, 0);
    lv_obj_set_style_text_color(battery_label, UIColors::TEXT_TERTIARY, 0);

    lv_obj_t *hint = lv_label_create(container);
    lv_label_set_text(hint, "\n< Swipe to return");
    lv_obj_set_style_text_font(hint, UIFonts::LABEL, 0);
}

void updateBatteryDisplay()
{
    static char battText[32];
    uint16_t battVoltage = amoled.getBattVoltage();
    bool isCharging = amoled.isVbusIn();

    if (battVoltage > 0)
    {
        // Calculate battery percentage
        int percentage = 0;
        if (battVoltage >= 4100)
            percentage = 100;
        else if (battVoltage >= 3700)
            percentage = 50 + ((battVoltage - 3700) * 50) / 400;
        else if (battVoltage >= 3300)
            percentage = ((battVoltage - 3300) * 50) / 400;

        // Choose battery icon
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
            snprintf(battText, sizeof(battText), "%s %d%% " LV_SYMBOL_CHARGE, icon, percentage);
        else
            snprintf(battText, sizeof(battText), "%s %d%%", icon, percentage);

        // Update color based on level
        lv_color_t color;
        if (percentage >= 30)
            color = UIColors::BATTERY_NORMAL;
        else if (percentage >= 15)
            color = UIColors::BATTERY_WARNING;
        else
            color = UIColors::BATTERY_LOW;

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
        const char *noBattText = LV_SYMBOL_BATTERY_EMPTY " USB";
        if (battery_label != NULL)
            lv_label_set_text(battery_label, noBattText);
        if (battery_label_util != NULL)
            lv_label_set_text(battery_label_util, noBattText);
    }
}
