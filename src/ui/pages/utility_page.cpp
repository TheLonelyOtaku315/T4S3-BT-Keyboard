#include "utility_page.h"
#include "../old_custom_ui/ui_config.h"
#include "../components/button_factory.h"
#include "../../assets/icons/brand_icons.h"
#include <Arduino.h>
#include <BleKeyboard.h>

extern BleKeyboard bleKeyboard;
extern lv_obj_t *battery_label_util;

void setupUtilityPage(lv_obj_t *parent)
{
    // Create glass container
    lv_obj_t *glass = lv_obj_create(parent);
    lv_obj_set_size(glass, LV_PCT(100), LV_PCT(100));
    UIHelpers::applyGlassStyle(glass, true);

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

    createUtilityButtonWithImage(grid, &facebook_icon, "FACEBOOK", facebookButtonEventCallback,
                                 lv_color_make(150, 255, 150), 0, 1);

    createUtilityButtonWithImage(grid, &discord_icon, "DISCORD", discordButtonEventCallback,
                                 lv_color_make(255, 150, 200), 1, 1);

    Serial.println("Utility page created!");
}
