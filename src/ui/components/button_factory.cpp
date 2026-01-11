#include "button_factory.h"
#include "../ui_config.h"
#include "../../app/media_controls.h"
#include <Arduino.h>
#include <BleKeyboard.h>

extern BleKeyboard bleKeyboard;

// ===== MEDIA CONTROL CALLBACKS =====

// Media button callback
void mediaBtnEventCb(lv_event_t *e)
{
    const char *action = (const char *)lv_event_get_user_data(e);
    Serial.print("Media control: ");
    Serial.println(action);

    if (!bleKeyboard.isConnected())
    {
        Serial.println("ERROR: Bluetooth not connected!");
        return;
    }

    // Send media keys based on action
    if (strcmp(action, "previous") == 0)
    {
        bleKeyboard.write(KEY_MEDIA_PREVIOUS_TRACK);
    }
    else if (strcmp(action, "play_pause") == 0)
    {
        bleKeyboard.write(KEY_MEDIA_PLAY_PAUSE);
    }
    else if (strcmp(action, "next") == 0)
    {
        bleKeyboard.write(KEY_MEDIA_NEXT_TRACK);
    }
    else if (strcmp(action, "volume_up") == 0)
    {
        bleKeyboard.write(KEY_MEDIA_VOLUME_UP);
    }
    else if (strcmp(action, "volume_down") == 0)
    {
        bleKeyboard.write(KEY_MEDIA_VOLUME_DOWN);
    }
    else if (strcmp(action, "mute") == 0)
    {
        bleKeyboard.write(KEY_MEDIA_MUTE);
    }
}

// ===== UTILITY BUTTON CALLBACKS =====

// Browser button callback
void browserButtonEventCallback(lv_event_t *e)
{
    if (!bleKeyboard.isConnected())
    {
        Serial.println("BLE not connected");
        return;
    }

    bleKeyboard.press(KEY_LEFT_GUI);
    bleKeyboard.press('r');
    vTaskDelay(pdMS_TO_TICKS(100));
    bleKeyboard.releaseAll();
    vTaskDelay(pdMS_TO_TICKS(100));
    bleKeyboard.print("https://google.com");
    vTaskDelay(pdMS_TO_TICKS(100));
    bleKeyboard.write(KEY_RETURN);

    Serial.println("Browser opened with URL");
}

// Discord button callback
void discordButtonEventCallback(lv_event_t *e)
{
    if (!bleKeyboard.isConnected())
    {
        Serial.println("BLE not connected");
        return;
    }

    bleKeyboard.press(KEY_LEFT_GUI);
    bleKeyboard.press('r');
    vTaskDelay(pdMS_TO_TICKS(100));
    bleKeyboard.releaseAll();
    vTaskDelay(pdMS_TO_TICKS(100));
    bleKeyboard.print("https://discord.com/channels/@me");
    vTaskDelay(pdMS_TO_TICKS(150));
    bleKeyboard.write(KEY_RETURN);

    Serial.println("Discord opened");
}

// Facebook button callback
void facebookButtonEventCallback(lv_event_t *e)
{
    if (!bleKeyboard.isConnected())
    {
        Serial.println("BLE not connected");
        return;
    }

    bleKeyboard.press(KEY_LEFT_GUI);
    bleKeyboard.press('r');
    vTaskDelay(pdMS_TO_TICKS(100));
    bleKeyboard.releaseAll();
    vTaskDelay(pdMS_TO_TICKS(100));
    bleKeyboard.print("https://www.facebook.com/");
    vTaskDelay(pdMS_TO_TICKS(100));
    bleKeyboard.write(KEY_RETURN);

    Serial.println("Facebook opened");
}

// Calculator button callback
void calculatorButtonEventCallback(lv_event_t *e)
{
    if (!bleKeyboard.isConnected())
    {
        Serial.println("BLE not connected");
        return;
    }

    bleKeyboard.press(KEY_LEFT_GUI);
    bleKeyboard.press('r');
    vTaskDelay(pdMS_TO_TICKS(100));
    bleKeyboard.releaseAll();
    vTaskDelay(pdMS_TO_TICKS(100));
    bleKeyboard.print("calc");
    vTaskDelay(pdMS_TO_TICKS(100));
    bleKeyboard.write(KEY_RETURN);

    Serial.println("Calculator opened");
}

// ===== BUTTON FACTORY FUNCTIONS =====

lv_obj_t *createMediaButton(lv_obj_t *parent, const char *icon, const char *label,
                            const char *action, lv_color_t color, uint8_t col, uint8_t row)
{
    lv_obj_t *btn = lv_btn_create(parent);
    lv_obj_set_grid_cell(btn, LV_GRID_ALIGN_STRETCH, col, 1, LV_GRID_ALIGN_STRETCH, row, 1);

    UIHelpers::applyButtonStyle(btn);

    // Container for icon and label (flex column)
    lv_obj_set_flex_flow(btn, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(btn, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);

    // Icon
    lv_obj_t *icon_label = lv_label_create(btn);
    lv_label_set_text(icon_label, icon);
    lv_obj_set_style_text_font(icon_label, UIFonts::ICON_LARGE, 0);
    lv_obj_set_style_text_color(icon_label, UIColors::ICON_COLOR, 0);

    // Text label
    lv_obj_t *text_label = lv_label_create(btn);
    lv_label_set_text(text_label, label);
    lv_obj_set_style_text_font(text_label, UIFonts::LABEL, 0);
    lv_obj_set_style_text_color(text_label, lv_color_white(), 0);
    lv_label_set_long_mode(text_label, LV_LABEL_LONG_WRAP);
    lv_obj_set_style_text_align(text_label, LV_TEXT_ALIGN_CENTER, 0);

    // Add callback
    lv_obj_add_event_cb(btn, mediaBtnEventCb, LV_EVENT_CLICKED, (void *)action);

    return btn;
}

lv_obj_t *createUtilityButton(lv_obj_t *parent, const char *icon, const char *label,
                              lv_event_cb_t callback, lv_color_t color, uint8_t col, uint8_t row)
{
    lv_obj_t *btn = lv_btn_create(parent);
    lv_obj_set_grid_cell(btn, LV_GRID_ALIGN_STRETCH, col, 1, LV_GRID_ALIGN_STRETCH, row, 1);

    UIHelpers::applyButtonStyle(btn);

    // Container for icon and label (flex column)
    lv_obj_set_flex_flow(btn, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(btn, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);

    // Icon
    lv_obj_t *icon_label = lv_label_create(btn);
    lv_label_set_text(icon_label, icon);
    lv_obj_set_style_text_font(icon_label, UIFonts::ICON_LARGE, 0);
    lv_obj_set_style_text_color(icon_label, UIColors::ICON_COLOR, 0);

    // Text label
    lv_obj_t *text_label = lv_label_create(btn);
    lv_label_set_text(text_label, label);
    lv_obj_set_style_text_font(text_label, UIFonts::LABEL, 0);
    lv_obj_set_style_text_color(text_label, lv_color_white(), 0);
    lv_label_set_long_mode(text_label, LV_LABEL_LONG_WRAP);
    lv_obj_set_style_text_align(text_label, LV_TEXT_ALIGN_CENTER, 0);

    // Add callback
    lv_obj_add_event_cb(btn, callback, LV_EVENT_CLICKED, NULL);

    return btn;
}

lv_obj_t *createUtilityButtonWithImage(lv_obj_t *parent, const lv_img_dsc_t *image, const char *label,
                                       lv_event_cb_t callback, lv_color_t color, uint8_t col, uint8_t row)
{
    lv_obj_t *btn = lv_btn_create(parent);
    lv_obj_set_grid_cell(btn, LV_GRID_ALIGN_STRETCH, col, 1, LV_GRID_ALIGN_STRETCH, row, 1);

    UIHelpers::applyButtonStyle(btn);

    // Container for icon and label (flex column)
    lv_obj_set_flex_flow(btn, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(btn, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);

    // Image icon
    lv_obj_t *img = lv_img_create(btn);
    lv_img_set_src(img, image);
    lv_obj_set_size(img, 48, 48);
    // Recolor the alpha mask to white
    lv_obj_set_style_img_recolor_opa(img, LV_OPA_COVER, 0);
    lv_obj_set_style_img_recolor(img, lv_color_white(), 0);

    // Text label
    lv_obj_t *text_label = lv_label_create(btn);
    lv_label_set_text(text_label, label);
    lv_obj_set_style_text_font(text_label, UIFonts::LABEL, 0);
    lv_obj_set_style_text_color(text_label, lv_color_white(), 0);
    lv_label_set_long_mode(text_label, LV_LABEL_LONG_WRAP);
    lv_obj_set_style_text_align(text_label, LV_TEXT_ALIGN_CENTER, 0);

    // Add callback
    lv_obj_add_event_cb(btn, callback, LV_EVENT_CLICKED, NULL);

    return btn;
}

lv_obj_t *createConnectionButton(lv_obj_t *parent, const char *icon, const char *label,
                                 lv_event_cb_t callback, uint8_t col, uint8_t row)
{
    lv_obj_t *btn = lv_btn_create(parent);
    lv_obj_set_grid_cell(btn, LV_GRID_ALIGN_STRETCH, col, 1, LV_GRID_ALIGN_STRETCH, row, 1);

    UIHelpers::applyButtonStyle(btn);

    // Container for icon and label (flex column)
    lv_obj_set_flex_flow(btn, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(btn, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);

    // Icon
    lv_obj_t *icon_label = lv_label_create(btn);
    lv_label_set_text(icon_label, icon);
    lv_obj_set_style_text_font(icon_label, UIFonts::ICON_LARGE, 0);
    lv_obj_set_style_text_color(icon_label, UIColors::ICON_COLOR, 0);

    // Text label
    lv_obj_t *text_label = lv_label_create(btn);
    lv_label_set_text(text_label, label);
    lv_obj_set_style_text_font(text_label, UIFonts::LABEL, 0);
    lv_obj_set_style_text_color(text_label, lv_color_white(), 0);
    lv_label_set_long_mode(text_label, LV_LABEL_LONG_WRAP);
    lv_obj_set_style_text_align(text_label, LV_TEXT_ALIGN_CENTER, 0);

    // Add callback
    lv_obj_add_event_cb(btn, callback, LV_EVENT_CLICKED, NULL);

    return btn;
}
