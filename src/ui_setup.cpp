#include "ui_setup.h"
#include <Arduino.h>

lv_obj_t *btn1;
lv_obj_t *btn2;
lv_obj_t *btn3;
lv_obj_t *btn4;
lv_obj_t *label;

// Touch event callback
void btn_event_cb(lv_event_t *e)
{
    const char *message = (const char *)lv_event_get_user_data(e);
    Serial.println(message);
}

void setupUI()
{
    // Create a grid container for the buttons: 2 columns, 2 rows
    static lv_coord_t col_dsc[] = {LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST};
    static lv_coord_t row_dsc[] = {LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST};

    lv_obj_t *grid = lv_obj_create(lv_scr_act());
    lv_obj_set_size(grid, LV_HOR_RES, LV_VER_RES); // Full screen
    lv_obj_center(grid);
    lv_obj_set_layout(grid, LV_LAYOUT_GRID);
    lv_obj_set_grid_dsc_array(grid, col_dsc, row_dsc);

    // Set gap between cells
    lv_obj_set_style_pad_column(grid, 10, 0);
    lv_obj_set_style_pad_row(grid, 10, 0);
    lv_obj_set_style_pad_all(grid, 10, 0);

    // Button 1 - Top Left (col 0, row 0)
    btn1 = lv_btn_create(grid);
    lv_obj_set_grid_cell(btn1, LV_GRID_ALIGN_STRETCH, 0, 1, LV_GRID_ALIGN_STRETCH, 0, 1);
    lv_obj_add_event_cb(btn1, btn_event_cb, LV_EVENT_CLICKED, (void *)"Button 1 Pressed");
    lv_obj_set_style_bg_color(btn1, lv_color_make(0, 130, 252), 0);
    
    lv_obj_t *label1 = lv_label_create(btn1);
    lv_label_set_text(label1, "BT 1");
    lv_obj_set_style_text_font(label1, &lv_font_montserrat_24, 0);
    lv_obj_center(label1);

    // Button 2 - Top Right (col 1, row 0)
    btn2 = lv_btn_create(grid);
    lv_obj_set_grid_cell(btn2, LV_GRID_ALIGN_STRETCH, 1, 1, LV_GRID_ALIGN_STRETCH, 0, 1);
    lv_obj_add_event_cb(btn2, btn_event_cb, LV_EVENT_CLICKED, (void *)"Button 2 Pressed");
    lv_obj_set_style_bg_color(btn2, lv_color_make(255, 130, 0), 0);
    
    lv_obj_t *label2 = lv_label_create(btn2);
    lv_label_set_text(label2, "BT 2");
    lv_obj_set_style_text_font(label2, &lv_font_montserrat_24, 0);
    lv_obj_center(label2);

    // Button 3 - Bottom Left (col 0, row 1)
    btn3 = lv_btn_create(grid);
    lv_obj_set_grid_cell(btn3, LV_GRID_ALIGN_STRETCH, 0, 1, LV_GRID_ALIGN_STRETCH, 1, 1);
    lv_obj_add_event_cb(btn3, btn_event_cb, LV_EVENT_CLICKED, (void *)"Button 3 Pressed");
    lv_obj_set_style_bg_color(btn3, lv_color_make(0, 200, 100), 0);
    
    lv_obj_t *label3 = lv_label_create(btn3);
    lv_label_set_text(label3, "BT 3");
    lv_obj_set_style_text_font(label3, &lv_font_montserrat_24, 0);
    lv_obj_center(label3);

    // Button 4 - Bottom Right (col 1, row 1)
    btn4 = lv_btn_create(grid);
    lv_obj_set_grid_cell(btn4, LV_GRID_ALIGN_STRETCH, 1, 1, LV_GRID_ALIGN_STRETCH, 1, 1);
    lv_obj_add_event_cb(btn4, btn_event_cb, LV_EVENT_CLICKED, (void *)"Button 4 Pressed");
    lv_obj_set_style_bg_color(btn4, lv_color_make(255, 0, 200), 0);
    
    lv_obj_t *label4 = lv_label_create(btn4);
    lv_label_set_text(label4, "ESP32");
    lv_obj_set_style_text_font(label4, &lv_font_montserrat_24, 0);
    lv_obj_center(label4);
}