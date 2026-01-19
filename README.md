# T4-S3 Bluetooth Keyboard Bridge

ESP32-S3 project that bridges USB keyboard input to Bluetooth HID output using the LilyGO T4-S3 AMOLED development board with 2.41" touch display.

## Overview

This project transforms the LilyGO T4-S3 AMOLED board into a USB to Bluetooth keyboard adapter. It receives keyboard input via USB Host and transmits the keystrokes over Bluetooth HID, allowing wired keyboards to connect wirelessly to devices.

## Hardware

- **Board**: LilyGO T4-S3 AMOLED (2.41" touch version)
- **MCU**: ESP32-S3 with dual-core processor
- **Display**: 2.41" AMOLED touchscreen (536x240 resolution)
- **Connectivity**: USB-C port (supports USB Host and device modes)
- **Power**: USB-C powered with built-in battery management

## Current Features

**Display & UI:**

- AMOLED display initialization and control
- Capacitive touch input handling
- LVGL UI framework integration
- Multi-tab interface (Device Control, Media Control, App Shortcuts)

**System:**

- Serial debugging interface
- PlatformIO build system configured
- Hardware abstraction layer for LilyGO board

## Project Structure

```
src/
├── main.cpp                    # Main application entry point
├── tab1_device_control.cpp/h   # Device control tab UI
├── tab2_media_control.cpp/h    # Media control tab UI
├── tab3_app_shortcuts.cpp/h    # Application shortcuts tab UI
├── hardware/                   # Hardware drivers and initialization
└── ui/                         # LVGL UI components and screens

lib/
└── LilyGo-AMOLED-Series-Touch/ # Hardware support library

boards/
└── T-Display-AMOLED.json       # Board definition for PlatformIO
```

## Setup

### Prerequisites

- PlatformIO IDE (VS Code extension recommended)
- USB-C cable
- LilyGO T4-S3 AMOLED board

### Installation

1. Install PlatformIO:
   - Install VS Code
   - Add the PlatformIO IDE extension

2. Clone this repository with submodules:

   ```bash
   git clone --recursive https://github.com/TheLonelyOtaku315/T4S3-BT-Keyboard.git
   cd T4S3-BT-Keyboard
   ```

3. Open project in VS Code with PlatformIO

4. Build the project:

   ```bash
   pio run
   ```

5. Upload to board:

   ```bash
   pio run --target upload
   ```

6. Monitor serial output (optional):
   ```bash
   pio device monitor
   ```

## Configuration

Project configuration is managed through `platformio.ini`. Key settings:

- Platform: ESP32 (Espressif32)
- Board: esp32-s3-devkitc-1
- Framework: Arduino
- Monitor speed: 115200 baud

## Development Roadmap

See [ROADMAP.md](ROADMAP.md) for detailed development phases and planned features.

## Troubleshooting

**Upload fails:**

- Hold BOOT button while connecting USB
- Ensure correct COM port is selected
- Try reducing upload speed in platformio.ini

**Display not working:**

- Check display connector is seated properly
- Verify board variant matches T4-S3 AMOLED
- Review serial output for initialization errors

**Touch not responding:**

- Calibrate touch in firmware if needed
- Check for conflicting pin definitions

## Resources

- [LilyGO T4-S3 Documentation](https://github.com/Xinyuan-LilyGO/LilyGo-AMOLED-Series)
- [ESP32-S3 Datasheet](https://www.espressif.com/sites/default/files/documentation/esp32-s3_datasheet_en.pdf)
- [LVGL Documentation](https://docs.lvgl.io/)
- [PlatformIO Documentation](https://docs.platformio.org/)

## License

MIT License - see LICENSE file for details

## Author

TheLonelyOtaku315
