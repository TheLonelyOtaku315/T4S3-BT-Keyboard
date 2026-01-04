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
    // Create a grid container for the buttons
    // 2 columns, 2 rows, with 10px gap between cells
    static lv_coord_t col_dsc[] = {LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST};
    static lv_coord_t row_dsc[] = {LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST};

    lv_obj_t *grid = lv_obj_create(lv_scr_act());
    lv_obj_set_size(grid, 350, 600); // Height for 2 rows
    lv_obj_set_pos(grid, 0, lv_obj_get_height(lv_scr_act()) - 610); // Position at bottom with 10px margin
    lv_obj_set_layout(grid, LV_LAYOUT_GRID);
    lv_obj_set_grid_dsc_array(grid, col_dsc, row_dsc);

    // Set gap between cells
    lv_obj_set_style_pad_column(grid, 10, 0);
    lv_obj_set_style_pad_row(grid, 10, 0);

    // Button 1
    btn1 = lv_btn_create(grid);
    lv_obj_set_grid_cell(btn1, LV_GRID_ALIGN_STRETCH, 0, 1, LV_GRID_ALIGN_STRETCH, 0, 1);
    lv_obj_add_event_cb(btn1, btn_event_cb, LV_EVENT_CLICKED, (void *)"Bluetooth Connection 1");
    lv_obj_set_style_bg_color(btn1, lv_color_make(0, 255, 0), 0);
    label = lv_label_create(btn1);
    lv_label_set_text(label, "Bluetooth Connection 1");
    lv_obj_set_style_text_font(label, &lv_font_montserrat_32, 0);
    lv_obj_center(label);

    // Button 2
    btn2 = lv_btn_create(grid);
    lv_obj_set_grid_cell(btn2, LV_GRID_ALIGN_STRETCH, 0, 1, LV_GRID_ALIGN_STRETCH, 1, 1);
    lv_obj_add_event_cb(btn2, btn_event_cb, LV_EVENT_CLICKED, (void *)"Bluetooth Connection 2");
    lv_obj_set_style_bg_color(btn2, lv_color_make(0, 200, 255), 0);
    lv_obj_t *label2 = lv_label_create(btn2);
    lv_label_set_text(label2, "Bluetooth Connection 2");
    lv_obj_set_style_text_font(label2, &lv_font_montserrat_32, 0);
    lv_obj_center(label2);

    // Button 3
    btn3 = lv_btn_create(grid);
    lv_obj_set_grid_cell(btn3, LV_GRID_ALIGN_STRETCH, 0, 1, LV_GRID_ALIGN_STRETCH, 2, 1);
    lv_obj_add_event_cb(btn3, btn_event_cb, LV_EVENT_CLICKED, (void *)"Bluetooth Connection 3");
    lv_obj_set_style_bg_color(btn3, lv_color_make(255, 200, 0), 0);
    lv_obj_t *label3 = lv_label_create(btn3);
    lv_label_set_text(label3, "Bluetooth Connection 3");
    lv_obj_set_style_text_font(label3, &lv_font_montserrat_32, 0);
    lv_obj_center(label3);

    // Button 4
    btn4 = lv_btn_create(grid);
    lv_obj_set_grid_cell(btn4, LV_GRID_ALIGN_STRETCH, 0, 1, LV_GRID_ALIGN_STRETCH, 3, 1);
    lv_obj_add_event_cb(btn4, btn_event_cb, LV_EVENT_CLICKED, (void *)"ESP32 Connection 4");
    lv_obj_set_style_bg_color(btn4, lv_color_make(255, 0, 200), 0);
    lv_obj_t *label4 = lv_label_create(btn4);
    lv_label_set_text(label4, "ESP32 Connection 4");
    lv_obj_set_style_text_font(label4, &lv_font_montserrat_32, 0);
    lv_obj_center(label4);
}