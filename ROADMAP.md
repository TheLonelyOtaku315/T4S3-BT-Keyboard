# Development Roadmap

This document outlines the development progress and planned phases for the T4-S3 Bluetooth Keyboard Bridge project.

## Phase 1 - Core Infrastructure (Completed)

- [x] PlatformIO project setup and configuration
- [x] LilyGO T4-S3 AMOLED hardware initialization
- [x] 2.41" AMOLED display driver integration
- [x] Capacitive touch input handling
- [x] LVGL UI framework setup
- [x] Serial debugging interface
- [x] Hardware abstraction layer

## Phase 2 - Bluetooth HID Output (Completed)

- [x] Initialize NimBLE stack
- [x] Implement BLE HID keyboard profile
- [x] Handle pairing and connection management
- [x] Manual BLE start/stop functionality
- [x] Connection status monitoring and display
- [x] Device identification ("T4-S3 Keyboard")

## Phase 3 - User Interface (Completed)

- [x] Multi-tab interface design
- [x] Tab 1: Device control (BLE on/off, disconnect, reboot, settings)
- [x] Tab 2: Media controls (play/pause, previous, next, volume, mute)
- [x] Tab 3: App shortcuts (browser, Discord, Facebook, terminal, VS Code, GitHub)
- [x] Status display (connection state)
- [x] UI assets and icon integration
- [x] Dynamic tab visibility based on connection status

## Phase 4 - USB Host Support (In Progress)

- [ ] Implement USB Host mode configuration
- [ ] Add USB keyboard detection and enumeration
- [ ] Parse USB HID keyboard reports
- [ ] Handle modifier keys and special keys
- [ ] Forward USB keyboard input to BLE output

## Phase 5 - Bridge Functionality (Planned)

- [ ] Map USB input to Bluetooth output seamlessly
- [ ] Add key remapping capabilities
- [ ] Implement macro support
- [ ] Add latency optimization
- [ ] Support multiple keyboard layouts

## Future Ideas

Optional enhancements that may be implemented:

**Power & Hardware:**

- Battery voltage monitoring and percentage display
- Charging status indicator
- Sleep mode and auto-power-off timer
- Display brightness adjustment
- Low battery warning

**Connectivity:**

- Multiple device pairing (switch between devices)
- Wireless firmware updates (OTA)
- USB mouse passthrough support

**Configuration:**

- Save/load settings from flash storage
- Web-based configuration interface
- Custom keyboard layout editor
- Programmable macro editor via UI

**Platform Compatibility:**

- Windows HID support and testing
- macOS HID support and testing
- Linux HID support and testing
- Android/iOS device compatibility
- Chrome OS support
