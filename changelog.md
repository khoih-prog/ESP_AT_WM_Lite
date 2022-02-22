## ESP_AT_WM_Lite (Light Weight Credentials / WiFi Manager for ESP8266 AT shields)

[![arduino-library-badge](https://www.ardu-badge.com/badge/ESP_AT_WM_Lite.svg?)](https://www.ardu-badge.com/ESP_AT_WM_Lite)
[![GitHub release](https://img.shields.io/github/release/khoih-prog/ESP_AT_WM_Lite.svg)](https://github.com/khoih-prog/ESP_AT_WM_Lite/releases)
[![GitHub](https://img.shields.io/github/license/mashape/apistatus.svg)](https://github.com/khoih-prog/ESP_AT_WM_Lite/blob/master/LICENSE)
[![contributions welcome](https://img.shields.io/badge/contributions-welcome-brightgreen.svg?style=flat)](#Contributing)
[![GitHub issues](https://img.shields.io/github/issues/khoih-prog/ESP_AT_WM_Lite.svg)](http://github.com/khoih-prog/ESP_AT_WM_Lite/issues)

---
---

## Table of Contents

* [Changelog](#changelog)
  * [Release v1.5.2](#release-v152)
  * [Release v1.5.1](#release-v151)
  * [Release v1.5.0](#release-v150)
  * [Release v1.4.1](#release-v141)
  * [Major Release v1.4.0](#major-release-v140)
  * [Major Release v1.3.0](#major-release-v130)
  * [Major Release v1.2.0](#major-release-v120)
  * [Major Release v1.1.0](#major-release-v110)
  * [Release v1.0.4](#release-v104)
  * [Release v1.0.3](#release-v103)
  * [Release v1.0.2](#release-v102)
  * [Release v1.0.1](#release-v101)
 
---
---

## Changelog

### Release v1.5.2

1. Optimize code by using passing by `reference` instead of by `value`
2. Optional `Board_Name` in Menu. Check [option to remove board name from web page #25](https://github.com/khoih-prog/WiFiManager_NINA_Lite/issues/25)
3. Add function `isConfigMode()` to signal system is in Config Portal mode.
4. Update `Packages' Patches`

### Release v1.5.1

1. Update to be compatible with new `FlashStorage_SAMD`
2. Use better `FlashStorage_STM32` or `FlashStorage_STM32F1` library for STM32
3. Add support to generic SAMD21 boards : `__SAMD21E1xA__`, `__SAMD21G1xA__` and `__SAMD21J1xA__`
4. Update `Packages' Patches`

### Release v1.5.0

1. Fix the blocking issue in loop(). Check [retries block the main loop #18](https://github.com/khoih-prog/WiFiManager_NINA_Lite/issues/18)
2. Configurable `WIFI_RECON_INTERVAL`. Check [retries block the main loop #18](https://github.com/khoih-prog/WiFiManager_NINA_Lite/issues/18#issuecomment-1006197561)
3. Update `Packages' Patches`

### Release v1.4.1

1. Update `platform.ini` and `library.json` to use original `khoih-prog` instead of `khoih.prog` after PIO fix
2. Update `Packages' Patches`

### Major Release v1.4.0

1. Add support to RP2040-based boards, such as **Nano_RP2040_Connect, RASPBERRY_PI_PICO, ADAFRUIT_FEATHER_RP2040 and GENERIC_RP2040**, using [**Arduino-mbed RP2040** core](https://github.com/arduino/ArduinoCore-mbed).

### Major Release v1.3.0

1. Add support to RP2040-based boards, such as **RASPBERRY_PI_PICO, ADAFRUIT_FEATHER_RP2040 and GENERIC_RP2040**, using [Earle Philhower's arduino-pico core](https://github.com/earlephilhower/arduino-pico) using LittleFS
2. Using new efficient features of [`FlashStorage_SAMD library v1.1.0+`](https://github.com/khoih-prog/FlashStorage_SAMD)
3. Optimize code and fix bug
4. Update examples with new features

### Major Release v1.2.0

1. Enable scan of WiFi networks for selection in Configuration Portal. Check [PR for v1.3.0 - Enable scan of WiFi networks #10](https://github.com/khoih-prog/WiFiManager_NINA_Lite/pull/10). Now you can select optional **SCAN_WIFI_NETWORKS**, **MANUAL_SSID_INPUT_ALLOWED** to be able to manually input SSID, not only from a scanned SSID lists and **MAX_SSID_IN_LIST** (from 2-6 for ESP8266-AT or 2-15 for other)
2. Minor enhancement to not display garbage when data is invalid
3. Tested with new [Arduino Core for STM32 v2.0.0](https://github.com/stm32duino/Arduino_Core_STM32) and add support to new STM32L5 boards.
4. Enhance MultiWiFi connection logic
5. Enhance debugging feature

### Major Release v1.1.0

1. Fix invalid "blank" Config Data treated as Valid. 
2. Permit optionally inputting one set of WiFi SSID/PWD by using `REQUIRE_ONE_SET_SSID_PW == true`
3. Enforce WiFi PWD minimum length of 8 chars
4. Optimize code
5. Use better FlashStorage_STM32. 
6. Add Table-of-Contents

### Release v1.0.4

1. Add support to **ESP32-AT WiFi shields.**
2. Add support to **WIS600-01S/W600-AT WiFi shields.**
3. Modify LOAD_DEFAULT_CONFIG_DATA logic.
4. Enhance MultiWiFi connection logic. 
5. Fix WiFi Status bug.

#### Release v1.0.3

1. Add support to **nRF52 (AdaFruit Feather nRF52832, nRF52840 Express, BlueFruit Sense, Itsy-Bitsy nRF52840 Express, Metro nRF52840 Express, NINA_B302_ublox, NINA_B112_ublox, etc.)**. Dynamic custom parameters to be saved **automatically in LittleFS**.
2. Add MultiWiFi features for WiFi
3. Add **DoubleResetDetector (DRD)** feature.
4. Restructure examples separate Credentials / Defines / Dynamic Params / Code.
5. Drop support to Teensy boards.

#### Release v1.0.2

1. Add support to **SAM51 (Itsy-Bitsy M4, Metro M4, Grand Central M4, Feather M4 Express, etc.) and SAM DUE**.
2. WiFi Password max length is 63, according to WPA2 standard.
3. Permit to input special chars such as **~,!,@,#,$,%,^,*,&** into data fields.
4. Fix bug

#### Release v1.0.1

1. New **powerful-yet-simple-to-use feature to enable adding dynamic custom parameters** from sketch and input using the same Config Portal. Config Portal will be auto-adjusted to match the number of dynamic parameters.
2. Dynamic custom parameters to be saved **automatically in EEPROM, SAMD EEPROM-emulated FlashStorage or SAM DUE DueFlashStorage**.



