#pragma once

#include <lvgl.h>

// Status screen overlay (connecting, connected messages)
void showStatusScreen(const char *message);
void hideStatusScreen();

// Connection status banner
void updateConnectionBanner(bool connected);
