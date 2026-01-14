#include "media_page.h"
#include "../old_custom_ui/ui_config.h"
#include "../components/button_factory.h"
#include "../components/status_screen.h"
#include <Arduino.h>
#include <BleKeyboard.h>

extern BleKeyboard bleKeyboard;

void setupMediaControlsPage(lv_obj_t *parent)
{
    // Create glass container
    lv_obj_t *glass = lv_obj_create(parent);
    lv_obj_set_size(glass, LV_PCT(100), LV_PCT(100));
    UIHelpers::applyGlassStyle(glass, true);

    // Create grid container
    lv_obj_t *grid = lv_obj_create(glass);
    lv_obj_set_size(grid, LV_PCT(100), LV_PCT(100));
    lv_obj_align(grid, LV_ALIGN_BOTTOM_MID, 0, 0);
    UIHelpers::makeTransparent(grid);
    lv_obj_set_style_pad_top(grid, 30, 0);

    // Setup 3x2 grid layout
    static lv_coord_t col_dsc[] = {LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST};
    static lv_coord_t row_dsc[] = {LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST};

    lv_obj_set_layout(grid, LV_LAYOUT_GRID);
    lv_obj_set_grid_dsc_array(grid, col_dsc, row_dsc);
    lv_obj_set_style_pad_all(grid, UIDimensions::GRID_PADDING, 0);
    lv_obj_set_style_pad_column(grid, UIDimensions::GRID_SPACING, 0);
    lv_obj_set_style_pad_row(grid, UIDimensions::GRID_SPACING, 0);

    // Create buttons (3 columns x 2 rows)
    createMediaButton(grid, LV_SYMBOL_PREV, "PREVIOUS", "previous",
                      lv_color_make(100, 150, 255), 0, 0);

    createMediaButton(grid, LV_SYMBOL_PLAY, "PLAY/PAUSE", "play_pause",
                      lv_color_make(100, 255, 150), 1, 0);

    createMediaButton(grid, LV_SYMBOL_NEXT, "NEXT", "next",
                      lv_color_make(100, 150, 255), 2, 0);

    createMediaButton(grid, LV_SYMBOL_VOLUME_MAX, "VOLUME UP", "volume_up",
                      lv_color_make(255, 200, 100), 0, 1);

    createMediaButton(grid, LV_SYMBOL_VOLUME_MID, "VOLUME DOWN", "volume_down",
                      lv_color_make(255, 200, 100), 1, 1);

    createMediaButton(grid, LV_SYMBOL_MUTE, "MUTE", "mute",
                      lv_color_make(255, 100, 100), 2, 1);

    updateConnectionBanner(bleKeyboard.isConnected());

    Serial.println("Media controls page created!");
}
