#include "status_screen.h"
#include "../ui_config.h"

static lv_obj_t *status_screen = NULL;
static lv_obj_t *status_label = NULL;
extern lv_obj_t *connection_banner;

void showStatusScreen(const char *message)
{
    if (status_screen == NULL)
    {
        status_screen = lv_obj_create(lv_scr_act());
        lv_obj_set_size(status_screen, LV_HOR_RES, LV_VER_RES);
        lv_obj_set_style_bg_color(status_screen, lv_color_black(), 0);
        lv_obj_set_style_bg_opa(status_screen, UIOpacity::STATUS_OVERLAY, 0);
        lv_obj_center(status_screen);

        status_label = lv_label_create(status_screen);
        lv_obj_set_style_text_font(status_label, UIFonts::TITLE_LARGE, 0);
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

void updateConnectionBanner(bool connected)
{
    if (connection_banner == NULL)
        return;

    if (connected)
    {
        lv_label_set_text(connection_banner, "CONNECTED");
        lv_obj_set_style_bg_color(connection_banner, UIColors::STATUS_CONNECTED, 0);
        lv_obj_set_style_bg_opa(connection_banner, UIOpacity::STATUS_BANNER, 0);
        lv_obj_set_style_border_color(connection_banner, UIColors::STATUS_CONNECTED, 0);
        lv_obj_set_style_text_color(connection_banner, UIColors::STATUS_CONNECTED, 0);
    }
    else
    {
        lv_label_set_text(connection_banner, "DISCONNECTED");
        lv_obj_set_style_bg_color(connection_banner, UIColors::STATUS_DISCONNECTED, 0);
        lv_obj_set_style_bg_opa(connection_banner, UIOpacity::STATUS_BANNER, 0);
        lv_obj_set_style_border_color(connection_banner, UIColors::STATUS_DISCONNECTED, 0);
        lv_obj_set_style_text_color(connection_banner, UIColors::STATUS_DISCONNECTED, 0);
    }
}
