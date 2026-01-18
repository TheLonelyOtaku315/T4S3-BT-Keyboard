#include "tab2_media_control.h"
#include "ui/ui.h"
#include <Arduino.h>
#include <BleKeyboard.h>

// Tab2 Button Event Handlers (Media Controls)
void previous_btn_handler(lv_event_t *e)
{
    if (bleKeyboard.isConnected())
    {
        Serial.println("Previous track button pressed");
        bleKeyboard.write(KEY_MEDIA_PREVIOUS_TRACK);
    }
}

void play_pause_btn_handler(lv_event_t *e)
{
    if (bleKeyboard.isConnected())
    {
        Serial.println("Play/Pause button pressed");
        bleKeyboard.write(KEY_MEDIA_PLAY_PAUSE);
    }
}

void next_btn_handler(lv_event_t *e)
{
    if (bleKeyboard.isConnected())
    {
        Serial.println("Next track button pressed");
        bleKeyboard.write(KEY_MEDIA_NEXT_TRACK);
    }
}

void mute_btn_handler(lv_event_t *e)
{
    if (bleKeyboard.isConnected())
    {
        Serial.println("Mute button pressed");
        bleKeyboard.write(KEY_MEDIA_MUTE);
    }
}

void volume_down_btn_handler(lv_event_t *e)
{
    if (bleKeyboard.isConnected())
    {
        Serial.println("Volume Down button pressed");
        bleKeyboard.write(KEY_MEDIA_VOLUME_DOWN);
    }
}

void volume_up_btn_handler(lv_event_t *e)
{
    if (bleKeyboard.isConnected())
    {
        Serial.println("Volume Up button pressed");
        bleKeyboard.write(KEY_MEDIA_VOLUME_UP);
    }
}

// Initialize Tab2 event handlers
void tab2_init()
{
    // Connect button event handlers for Tab2 (Media Controls)
    lv_obj_add_event_cb(ui_device1Btn1, previous_btn_handler, LV_EVENT_CLICKED, NULL);
    lv_obj_add_event_cb(ui_pausePlayBtn, play_pause_btn_handler, LV_EVENT_CLICKED, NULL);
    lv_obj_add_event_cb(ui_device3Btn1, next_btn_handler, LV_EVENT_CLICKED, NULL);
    lv_obj_add_event_cb(ui_disconnectBtn1, mute_btn_handler, LV_EVENT_CLICKED, NULL);
    lv_obj_add_event_cb(ui_rebootBtn1, volume_down_btn_handler, LV_EVENT_CLICKED, NULL);
    lv_obj_add_event_cb(ui_settingBtn1, volume_up_btn_handler, LV_EVENT_CLICKED, NULL);

    Serial.println("Tab2 (Media Control) handlers connected");
}
