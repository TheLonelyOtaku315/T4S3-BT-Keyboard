# UI Options - Choose Your Design

You have **two UI designs** preserved in this project:

## 1. Old Custom UI (Your Original Design)

**Location:** `old_custom_ui/`

**Features:**

- Media control buttons (Play/Pause, Next, Previous)
- Connection/pairing page
- Info page with device details
- Utility page with battery status
- Tabbed interface with 4 pages
- Complete BLE keyboard integration

**Files:**

- `old_custom_ui/ui_setup.cpp` - Main UI initialization
- `old_custom_ui/ui_setup.h` - Headers
- `old_custom_ui/ui_config.h` - Configuration
- `pages/` - All your custom pages (media, connection, info, utility)
- `components/` - Reusable components

## 2. SquareLine Studio UI (New Visual Design)

**Location:** `squareline_studio/`

**Features:**

- Visually designed in SquareLine Studio
- Screen1 (basic layout)
- Easy to redesign and export from SquareLine

**Files:**

- `squareline_studio/ui.c` - Main UI code
- `squareline_studio/ui.h` - Main header
- `squareline_studio/ui_Screen1.*` - Screen definition

---

## How to Switch Between UIs

### Use Old Custom UI (Current Original):

1. **Update `main.cpp`:**

   ```cpp
   // Replace:
   #include "ui/squareline_studio/ui.h"

   // With:
   #include "ui/old_custom_ui/ui_setup.h"
   ```

   ```cpp
   // Replace:
   ui_init();

   // With:
   setupUI();
   ```

2. **Rebuild:**
   ```powershell
   pio run -t upload
   ```

### Use SquareLine Studio UI (Current Active):

Already active! Files in `squareline_studio/` are being used.

---

## Combining Both UIs

You can also take the best of both:

1. **Design in SquareLine Studio** - Get the visual layout right
2. **Export and integrate** - Copy files to `squareline_studio/`
3. **Add your custom logic** - Use your `pages/` code for BLE functionality
4. **Wire them together** - Call your media control functions from SquareLine buttons

**Example:** Add button event in SquareLine that calls your existing media functions:

```cpp
// In ui_events.h (SquareLine)
void play_button_clicked(lv_event_t * e) {
    // Use your existing function from media_page.cpp
    if (bleKeyboard.isConnected()) {
        bleKeyboard.sendMediaPlayPause();
    }
}
```

---

## Recommendation

**Keep both!** They serve different purposes:

- **SquareLine Studio UI**: Fast iteration, visual design, easy to tweak layouts
- **Old Custom UI**: Your proven working code with full BLE integration

You can:

1. Design new screens in SquareLine
2. Copy the event handlers from your old custom UI
3. Get the best of both worlds!

---

## Need Help?

Check the files:

- Your old working code: `old_custom_ui/ui_setup.cpp`
- Your pages: `pages/*.cpp`
- Current active: `squareline_studio/ui.c`
