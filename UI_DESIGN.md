# UI Design with SquareLine Studio

## What is SquareLine Studio?

The **official LVGL visual editor** - design your UI with drag-and-drop, then export code to use in this project.

## Download & Install

**Free version available:** https://squareline.io/downloads

Windows, Mac, and Linux supported.

## Setup Your Project

1. **Launch SquareLine Studio**
2. **Create New Project:**

   - Click "Create"
   - Select "LVGL" project type

3. **Configure Board Settings:**

   - **Resolution Width:** `536`
   - **Resolution Height:** `240`
   - **Color Depth:** `16 bit`
   - **LVGL Version:** `8.3` or `8.4`
   - **Board:** Custom/Arduino

4. **Project Location:** Save anywhere (separate from this project)

## Design Your UI

- **Drag widgets** from the left panel
- **Configure properties** in the right panel
- **See live preview** in the center
- **Arrange layouts** with flexbox or absolute positioning
- **Add events/callbacks** for button clicks

### Widgets You'll Use:

- Buttons (lv_btn)
- Labels (lv_label)
- Tabview (lv_tabview)
- Containers (lv_obj)
- Sliders, switches, etc.

## Export to This Project

1. **In SquareLine:** Click **"Export"** → **"Export UI Files"**
2. **Copy generated files** to `src/ui/` folder:

   - `ui.c`
   - `ui.h`
   - `screens/` folder
   - `components/` folder (if any)

3. **Include in your code:**

   ```cpp
   #include "ui/ui.h"

   void setup() {
       // ... your setup code ...
       ui_init();  // Initialize the exported UI
   }
   ```

4. **Build and upload:**
   ```powershell
   pio run -t upload
   ```

## Workflow

1. **Design** in SquareLine Studio
2. **Export** code
3. **Copy** to `src/ui/`
4. **Upload** to board (~15 seconds)
5. **Test** on real hardware
6. **Iterate** - repeat as needed!

## Tips

- **Start simple:** Create one screen first, test it, then expand
- **Use flexbox layouts:** Easier than absolute positioning
- **Name widgets clearly:** Makes the code easier to work with
- **Export often:** Don't lose work!
- **Test on hardware:** Simulator colors may differ slightly from AMOLED

## Example: Media Control Button

In SquareLine Studio:

1. Drag a **Button** to canvas
2. Set size: 200x50
3. Add **Label** as child
4. Label text: "▶ Play/Pause"
5. Add event: `LV_EVENT_CLICKED` → `play_pause_handler`
6. Export!

In your code:

```cpp
void play_pause_handler(lv_event_t * e) {
    if (bleKeyboard.isConnected()) {
        bleKeyboard.sendMediaPlayPause();
    }
}
```

## Alternative: Quick Hardware Testing

Remember, uploading to your T4-S3 board only takes ~15 seconds:

```powershell
pio run -t upload && pio device monitor
```

For small UI tweaks, this can be faster than exporting from SquareLine!

## Resources

- [SquareLine Studio Docs](https://docs.squareline.io/)
- [LVGL Documentation](https://docs.lvgl.io/8.4/)
- [Widget Gallery](https://docs.lvgl.io/8.4/widgets/index.html)
