#include "tab3_app_shortcuts.h"
#include "ui/ui.h"
#include <Arduino.h>

// Tab3 Button Event Handlers

void browser_btn_handler(lv_event_t *e)
{
    Serial.println("Browser button pressed");

    if (bleKeyboard.isConnected())
    {
        // Windows: Win+R, type "chrome" and Enter
        bleKeyboard.press(KEY_LEFT_GUI); // Windows key
        bleKeyboard.press('r');
        delay(100);
        bleKeyboard.releaseAll();
        delay(200);

        bleKeyboard.print("chrome");
        delay(100);
        bleKeyboard.press(KEY_RETURN);
        delay(100);
        bleKeyboard.releaseAll();
        Serial.println("Opened browser");
    }
    else
    {
        Serial.println("Not connected - cannot send browser shortcut");
    }
}

void discord_btn_handler(lv_event_t *e)
{
    Serial.println("Discord button pressed");

    if (bleKeyboard.isConnected())
    {
        // Windows: Win+R, then type "discord" and Enter
        bleKeyboard.press(KEY_LEFT_GUI); // Windows key
        bleKeyboard.press('r');
        delay(100);
        bleKeyboard.releaseAll();
        delay(200);

        bleKeyboard.print("https://discord.com/channels/@me");
        delay(200);
        bleKeyboard.press(KEY_RETURN);
        delay(100);
        bleKeyboard.releaseAll();
        Serial.println("Launched Discord");
    }
    else
    {
        Serial.println("Not connected - cannot launch Discord");
    }
}

void facebook_btn_handler(lv_event_t *e)
{
    Serial.println("Facebook button pressed");

    if (bleKeyboard.isConnected())
    {
        // Open browser first, then navigate to Facebook
        bleKeyboard.press(KEY_LEFT_GUI); // Windows key
        bleKeyboard.press('r');
        delay(100);
        bleKeyboard.releaseAll();
        delay(200);

        bleKeyboard.print("chrome facebook.com");
        delay(100);
        bleKeyboard.press(KEY_RETURN);
        delay(100);
        bleKeyboard.releaseAll();
        Serial.println("Opened Facebook");
    }
    else
    {
        Serial.println("Not connected - cannot open Facebook");
    }
}

void terminal_btn_handler(lv_event_t *e)
{
    Serial.println("Terminal button pressed");

    if (bleKeyboard.isConnected())
    {
        // Windows Terminal: Win+R, type "wt" and Enter
        bleKeyboard.press(KEY_LEFT_GUI); // Windows key
        bleKeyboard.press('r');
        delay(100);
        bleKeyboard.releaseAll();
        delay(200);

        bleKeyboard.print("wt");
        delay(100);
        bleKeyboard.press(KEY_RETURN);
        delay(100);
        bleKeyboard.releaseAll();
        Serial.println("Opened Windows Terminal");
    }
    else
    {
        Serial.println("Not connected - cannot open terminal");
    }
}

void vscode_btn_handler(lv_event_t *e)
{
    Serial.println("VS Code button pressed");

    if (bleKeyboard.isConnected())
    {
        // Windows: Win+R, type "code" and Enter
        bleKeyboard.press(KEY_LEFT_GUI); // Windows key
        bleKeyboard.press('r');
        delay(100);
        bleKeyboard.releaseAll();
        delay(200);

        bleKeyboard.print("code");
        delay(100);
        bleKeyboard.press(KEY_RETURN);
        delay(100);
        bleKeyboard.releaseAll();
        Serial.println("Opened VS Code");
    }
    else
    {
        Serial.println("Not connected - cannot open VS Code");
    }
}

void github_btn_handler(lv_event_t *e)
{
    Serial.println("GitHub button pressed");

    if (bleKeyboard.isConnected())
    {
        // Open browser first, then navigate to GitHub
        bleKeyboard.press(KEY_LEFT_GUI); // Windows key
        bleKeyboard.press('r');
        delay(100);
        bleKeyboard.releaseAll();
        delay(200);

        bleKeyboard.print("chrome github.com");
        delay(100);
        bleKeyboard.press(KEY_RETURN);
        delay(100);
        bleKeyboard.releaseAll();
        Serial.println("Opened GitHub");
    }
    else
    {
        Serial.println("Not connected - cannot open GitHub");
    }
}

// Initialization function - connects button handlers to UI elements
void tab3_init()
{
    // Connect button event handlers
    lv_obj_add_event_cb(ui_browserBtn, browser_btn_handler, LV_EVENT_CLICKED, NULL);
    lv_obj_add_event_cb(ui_discordBtn, discord_btn_handler, LV_EVENT_CLICKED, NULL);
    lv_obj_add_event_cb(ui_facebookBtn, facebook_btn_handler, LV_EVENT_CLICKED, NULL);
    lv_obj_add_event_cb(ui_trerminalBtn, terminal_btn_handler, LV_EVENT_CLICKED, NULL);
    lv_obj_add_event_cb(ui_vscodeBtn, vscode_btn_handler, LV_EVENT_CLICKED, NULL);
    lv_obj_add_event_cb(ui_githubBtn, github_btn_handler, LV_EVENT_CLICKED, NULL);

    Serial.println("Tab3 (App Shortcuts) button handlers connected");
}
