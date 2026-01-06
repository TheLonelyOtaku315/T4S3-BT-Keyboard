#pragma once

#include <lvgl.h>

// UI Theme Configuration - All styling constants in one place

// ========== COLORS ==========
namespace UIColors
{
    // Glass effect colors
    constexpr lv_color_t GLASS_BG = LV_COLOR_MAKE(20, 20, 40);
    constexpr lv_color_t GLASS_BORDER = LV_COLOR_MAKE(80, 80, 120);
    constexpr lv_color_t GLASS_BG_MINIMAL = LV_COLOR_MAKE(30, 30, 30);
    constexpr lv_color_t GLASS_BORDER_MINIMAL = LV_COLOR_MAKE(100, 100, 100);

    // Button colors
    constexpr lv_color_t BUTTON_BG = LV_COLOR_MAKE(40, 40, 40);
    constexpr lv_color_t BUTTON_BORDER = LV_COLOR_MAKE(120, 120, 120);

    // Text colors
    constexpr lv_color_t TEXT_PRIMARY = LV_COLOR_MAKE(220, 220, 220);
    constexpr lv_color_t TEXT_SECONDARY = LV_COLOR_MAKE(200, 200, 200);
    constexpr lv_color_t TEXT_TERTIARY = LV_COLOR_MAKE(180, 180, 180);

    // Icon colors
    constexpr lv_color_t ICON_COLOR = LV_COLOR_MAKE(200, 200, 200);

    // Status colors
    constexpr lv_color_t STATUS_CONNECTED = LV_COLOR_MAKE(0, 255, 0);
    constexpr lv_color_t STATUS_DISCONNECTED = LV_COLOR_MAKE(255, 0, 0);

    // Battery colors
    constexpr lv_color_t BATTERY_NORMAL = LV_COLOR_MAKE(200, 200, 200);
    constexpr lv_color_t BATTERY_WARNING = LV_COLOR_MAKE(180, 180, 180);
    constexpr lv_color_t BATTERY_LOW = LV_COLOR_MAKE(150, 150, 150);
}

// ========== OPACITY ==========
namespace UIOpacity
{
    constexpr lv_opa_t GLASS_BG = LV_OPA_30;
    constexpr lv_opa_t GLASS_BG_MINIMAL = LV_OPA_20;
    constexpr lv_opa_t GLASS_BORDER = LV_OPA_50;
    constexpr lv_opa_t GLASS_BORDER_MINIMAL = LV_OPA_30;
    constexpr lv_opa_t BUTTON_BG = LV_OPA_40;
    constexpr lv_opa_t BUTTON_BORDER = LV_OPA_40;
    constexpr lv_opa_t BUTTON_PRESSED = LV_OPA_60;
    constexpr lv_opa_t SHADOW = LV_OPA_40;
    constexpr lv_opa_t SHADOW_MINIMAL = LV_OPA_20;
    constexpr lv_opa_t STATUS_OVERLAY = LV_OPA_80;
    constexpr lv_opa_t STATUS_BANNER = LV_OPA_30;
}

// ========== DIMENSIONS ==========
namespace UIDimensions
{
    // Border widths
    constexpr lv_coord_t GLASS_BORDER = 2;
    constexpr lv_coord_t GLASS_BORDER_MINIMAL = 1;
    constexpr lv_coord_t BUTTON_BORDER = 1;

    // Corner radius
    constexpr lv_coord_t GLASS_RADIUS = 16;
    constexpr lv_coord_t GLASS_RADIUS_MINIMAL = 12;
    constexpr lv_coord_t BUTTON_RADIUS = 8;

    // Padding
    constexpr lv_coord_t GLASS_PADDING = 10;
    constexpr lv_coord_t BUTTON_PADDING = 8;
    constexpr lv_coord_t GRID_PADDING = 5;
    constexpr lv_coord_t GRID_SPACING = 10;

    // Shadow
    constexpr lv_coord_t SHADOW_WIDTH = 30;
    constexpr lv_coord_t SHADOW_WIDTH_MINIMAL = 20;

    // Info panel
    constexpr lv_coord_t INFO_PANEL_HEIGHT = 80;
    constexpr lv_coord_t INFO_PANEL_SPACING = 90;
}

// ========== FONTS ==========
namespace UIFonts
{
    // Main fonts (references to LVGL built-in fonts)
    constexpr const lv_font_t *ICON_LARGE = &lv_font_montserrat_48;
    constexpr const lv_font_t *TITLE_LARGE = &lv_font_montserrat_32;
    constexpr const lv_font_t *TITLE_MEDIUM = &lv_font_montserrat_28;
    constexpr const lv_font_t *INFO_LARGE = &lv_font_montserrat_24;
    constexpr const lv_font_t *INFO_MEDIUM = &lv_font_montserrat_20;
    constexpr const lv_font_t *INFO_SMALL = &lv_font_montserrat_16;
    constexpr const lv_font_t *LABEL = &lv_font_montserrat_14;
}

// ========== HELPER FUNCTIONS ==========
namespace UIHelpers
{
    // Apply glass container style
    inline void applyGlassStyle(lv_obj_t *obj, bool minimal = false)
    {
        if (minimal)
        {
            lv_obj_set_style_bg_color(obj, UIColors::GLASS_BG_MINIMAL, 0);
            lv_obj_set_style_bg_opa(obj, UIOpacity::GLASS_BG_MINIMAL, 0);
            lv_obj_set_style_border_color(obj, UIColors::GLASS_BORDER_MINIMAL, 0);
            lv_obj_set_style_border_width(obj, UIDimensions::GLASS_BORDER_MINIMAL, 0);
            lv_obj_set_style_border_opa(obj, UIOpacity::GLASS_BORDER_MINIMAL, 0);
            lv_obj_set_style_radius(obj, UIDimensions::GLASS_RADIUS_MINIMAL, 0);
            lv_obj_set_style_shadow_width(obj, UIDimensions::SHADOW_WIDTH_MINIMAL, 0);
            lv_obj_set_style_shadow_opa(obj, UIOpacity::SHADOW_MINIMAL, 0);
        }
        else
        {
            lv_obj_set_style_bg_color(obj, UIColors::GLASS_BG, 0);
            lv_obj_set_style_bg_opa(obj, UIOpacity::GLASS_BG, 0);
            lv_obj_set_style_border_color(obj, UIColors::GLASS_BORDER, 0);
            lv_obj_set_style_border_width(obj, UIDimensions::GLASS_BORDER, 0);
            lv_obj_set_style_border_opa(obj, UIOpacity::GLASS_BORDER, 0);
            lv_obj_set_style_radius(obj, UIDimensions::GLASS_RADIUS, 0);
            lv_obj_set_style_shadow_width(obj, UIDimensions::SHADOW_WIDTH, 0);
            lv_obj_set_style_shadow_opa(obj, UIOpacity::SHADOW, 0);
        }
        lv_obj_set_style_shadow_color(obj, lv_color_black(), 0);
        lv_obj_set_style_pad_all(obj, UIDimensions::GLASS_PADDING, 0);
    }

    // Apply button style
    inline void applyButtonStyle(lv_obj_t *btn)
    {
        lv_obj_set_style_bg_color(btn, UIColors::BUTTON_BG, 0);
        lv_obj_set_style_bg_opa(btn, UIOpacity::BUTTON_BG, 0);
        lv_obj_set_style_border_color(btn, UIColors::BUTTON_BORDER, 0);
        lv_obj_set_style_border_width(btn, UIDimensions::BUTTON_BORDER, 0);
        lv_obj_set_style_border_opa(btn, UIOpacity::BUTTON_BORDER, 0);
        lv_obj_set_style_radius(btn, UIDimensions::BUTTON_RADIUS, 0);
        lv_obj_set_style_bg_opa(btn, UIOpacity::BUTTON_PRESSED, LV_STATE_PRESSED);
        lv_obj_set_style_pad_all(btn, UIDimensions::BUTTON_PADDING, 0);
    }

    // Setup transparent container
    inline void makeTransparent(lv_obj_t *obj)
    {
        lv_obj_set_style_bg_opa(obj, LV_OPA_TRANSP, 0);
        lv_obj_set_style_border_width(obj, 0, 0);
    }
}
