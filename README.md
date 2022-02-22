## ESP_AT_WM_Lite (Light Weight Credentials / WiFi Manager for ESP8266 AT shields)

[![arduino-library-badge](https://www.ardu-badge.com/badge/ESP_AT_WM_Lite.svg?)](https://www.ardu-badge.com/ESP_AT_WM_Lite)
[![GitHub release](https://img.shields.io/github/release/khoih-prog/ESP_AT_WM_Lite.svg)](https://github.com/khoih-prog/ESP_AT_WM_Lite/releases)
[![GitHub](https://img.shields.io/github/license/mashape/apistatus.svg)](https://github.com/khoih-prog/ESP_AT_WM_Lite/blob/master/LICENSE)
[![contributions welcome](https://img.shields.io/badge/contributions-welcome-brightgreen.svg?style=flat)](#Contributing)
[![GitHub issues](https://img.shields.io/github/issues/khoih-prog/ESP_AT_WM_Lite.svg)](http://github.com/khoih-prog/ESP_AT_WM_Lite/issues)

<a href="https://www.buymeacoffee.com/khoihprog6" title="Donate to my libraries using BuyMeACoffee"><img src="https://cdn.buymeacoffee.com/buttons/v2/default-yellow.png" alt="Donate to my libraries using BuyMeACoffee" style="height: 50px !important;width: 181px !important;" ></a>
<a href="https://www.buymeacoffee.com/khoihprog6" title="Donate to my libraries using BuyMeACoffee"><img src="https://img.shields.io/badge/buy%20me%20a%20coffee-donate-orange.svg?logo=buy-me-a-coffee&logoColor=FFDD00" style="height: 20px !important;width: 200px !important;" ></a>

---
---

## Table of Contents

* [Why do we need this ESP_AT_WM_Lite library](#why-do-we-need-this-esp_at_wm_lite-library)
  * [Features](#features)
  * [Currently supported Boards](#currently-supported-boards)
* [Changelog](changelog.md)
* [Prerequisites](#prerequisites)
* [Important Notes about AT Firmwares](#important-notes-about-at-firmwares)
  * [1. Firmwares tested OK with ESP8266 AT shields](#1-firmwares-tested-ok-with-esp8266-at-shields)
  * [2. Firmwares tested OK with ESP32 AT shields](#2-firmwares-tested-ok-with-esp32-at-shields)
* [Installation](#installation)
  * [Use Arduino Library Manager](#use-arduino-library-manager)
  * [Manual Install](#manual-install)
  * [VS Code & PlatformIO](#vs-code--platformio)
* [Packages' Patches](#packages-patches)
  * [1. For Adafruit nRF52840 and nRF52832 boards](#1-for-adafruit-nRF52840-and-nRF52832-boards)
  * [2. For Teensy boards](#2-for-teensy-boards)
  * [3. For Arduino SAM DUE boards](#3-for-arduino-sam-due-boards)
  * [4. For Arduino SAMD boards](#4-for-arduino-samd-boards)
      * [For core version v1.8.10+](#for-core-version-v1810)
      * [For core version v1.8.9-](#for-core-version-v189-)
  * [5. For Adafruit SAMD boards](#5-for-adafruit-samd-boards)
  * [6. For Seeeduino SAMD boards](#6-for-seeeduino-samd-boards)
  * [7. For STM32 boards](#7-for-stm32-boards) 
    * [7.1. For STM32 boards to use LAN8720](#71-for-stm32-boards-to-use-lan8720)
    * [7.2. For STM32 boards to use Serial1](#72-for-stm32-boards-to-use-serial1)
  * [8. For RP2040-based boards using Earle Philhower arduino-pico core](#8-for-rp2040-based-boards-using-earle-philhower-arduino-pico-core)
    * [8.1. To use BOARD_NAME](#81-to-use-board_name)
    * [8.2. To avoid compile error relating to microsecondsToClockCycles](#82-to-avoid-compile-error-relating-to-microsecondstoclockcycles)
  * [9. For Portenta_H7 boards using Arduino IDE in Linux](#9-for-portenta_h7-boards-using-arduino-ide-in-linux) 
* [How It Works](#how-it-works)
* [How to use](#how-to-use)
  * [1. Basic usage](#1-basic-usage)
  * [2. Add custom parameters](#2-add-custom-parameters)
  * [3. Not using custom parameters](#3-not-using-custom-parameters)
  * [4. To open Config Portal](#4-to-open-config-portal)
  * [5. To use different AP WiFi Channel](#5-to-use-different-ap-wifi-channel)
  * [6. To use different static AP IP from default](#6-to-use-different-static-ap-ip-from-default)
  * [7. To use and input only one set of WiFi SSID and PWD](#7-to-use-and-input-only-one-set-of-wifi-ssid-and-pwd)
    * [7.1 If you need to use and input only one set of WiFi SSID/PWD](#71-if-you-need-to-use-and-input-only-one-set-of-wifi-ssidpwd)
    * [7.2 If you need to use both sets of WiFi SSID/PWD](#72-if-you-need-to-use-both-sets-of-wifi-ssidpwd)
  * [8. To enable auto-scan of WiFi networks for selection in Configuration Portal](#8-to-enable-auto-scan-of-wifi-networks-for-selection-in-configuration-portal)
    * [8.1 Enable auto-scan of WiFi networks for selection in Configuration Portal](#81-enable-auto-scan-of-wifi-networks-for-selection-in-configuration-portal)
    * [8.2 Disable manually input SSIDs](#82-disable-manually-input-ssids)
    * [8.3 Select maximum number of SSIDs in the list](#83-select-maximum-number-of-ssids-in-the-list)
  * [9. To avoid blocking in loop when WiFi is lost](#9-To-avoid-blocking-in-loop-when-wifi-is-lost)
    * [9.1 Max times to try WiFi per loop](#91-max-times-to-try-wifi-per-loop)
    * [9.2 Interval between reconnection WiFi if lost](#92-interval-between-reconnection-wifi-if-lost)
  * [10. Not using Board_Name on Config_Portal](#10-Not-using-Board_Name-on-Config_Portal)
* [Important Notes for using Dynamic Parameters' ids](#important-notes-for-using-dynamic-parameters-ids)
* [Examples](#examples)
  * [ 1. Mega_ESP8266Shield](examples/Mega_ESP8266Shield)
  * [ 2. SAMD_ESP8266Shield](examples/SAMD_ESP8266Shield)
  * [ 3. SAM_DUE_ESP8266Shield](examples/SAM_DUE_ESP8266Shield)
  * [ 4. STM32_ESP8266Shield](examples/STM32_ESP8266Shield)
  * [ 5. nRF52_ESP8266Shield](examples/nRF52_ESP8266Shield)
  * [ 6. RPi_Pico_ESP8266Shield](examples/RPi_Pico_ESP8266Shield) **New**
* [So, how it works?](#so-how-it-works)
  * [1. Without SCAN_WIFI_NETWORKS](#1-without-scan_wifi_networks)
  * [2. With SCAN_WIFI_NETWORKS](#2-with-scan_wifi_networks)
* [Important Notes](#important-notes)
* [How to use default Credentials and have them pre-loaded onto Config Portal](#how-to-use-default-credentials-and-have-them-pre-loaded-onto-config-portal)
  * [1. To always load Default Credentials and override Config Portal data](#1-to-always-load-default-credentials-and-override-config-portal-data)
  * [2. To load Default Credentials when there is no valid Credentials](#2-to-load-default-credentials-when-there-is-no-valid-credentials)
  * [3. Example of Default Credentials](#3-example-of-default-credentials)
* [How to add dynamic parameters from sketch](#how-to-add-dynamic-parameters-from-sketch)
* [Important Notes for using Dynamic Parameters' ids](#important-notes-for-using-dynamic-parameters-ids)
* [Example nRF52_ESP8266Shield](#example-nrf52_esp8266shield)
  * [1. File nRF52_ESP8266Shield.ino](#1-file-nrf52_esp8266shieldino)
  * [2. File defines.h](#2-file-definesh)
  * [3. File Credentials.h](#3-file-credentialsh)
  * [4. File dynamicParams.h](#4-file-dynamicparamsh)
* [Debug Terminal Output Samples](#debug-terminal-output-samples)
  * [1. nRF52_ESP8266Shield on Adafruit ItsyBitsy NRF52840 Feather Express](#1-nrf52_esp8266shield-on-adafruit-itsybitsy-nrf52840-feather-express)
    * [1.1 Open Config Portal](#11-open-config-portal)
    * [1.2 Got valid Credential from Config Portal, then connected to WiFi](#12-got-valid-credential-from-config-portal-then-connected-to-wifi)
    * [1.3 Lost a WiFi and autoconnect to another WiFi AP](#13-lost-a-wifi-and-autoconnect-to-another-wifi-ap)
  * [2. SAMD_ESP8266Shield on Adafruit SAMD51 ITSYBITSY_M4](#2-samd_esp8266shield-on-adafruit-samd51-itsybitsy_m4)
    * [2.1 Open Config Portal](#21-open-config-portal)
    * [2.2 Got valid Credential from Config Portal, then connected to WiFi](#22-got-valid-credential-from-config-portal-then-connected-to-wifi)
    * [2.3 Lost a WiFi and autoconnect to another WiFi AP](#23-lost-a-wifi-and-autoconnect-to-another-wifi-ap)
  * [3. RPi_Pico_ESP8266Shield on RASPBERRY_PI_PICO](#3-rpi_pico_esp8266shield-on-raspberry_pi_pico)
    * [3.1 Open Config Portal](#31-open-config-portal)
    * [3.2 Got valid Credential from Config Portal, then connected to WiFi](#32-got-valid-credential-from-config-portal-then-connected-to-wifi)
  * [4. RPi_Pico_ESP8266Shield on MBED RASPBERRY_PI_PICO](#4-rpi_pico_esp8266shield-on-mbed-raspberry_pi_pico)
    * [4.1 Open Config Portal](#41-open-config-portal)
    * [4.2 Got valid Credential from Config Portal, then connected to WiFi](#42-got-valid-credential-from-config-portal-then-connected-to-wifi)
* [Debug](#debug)
* [Troubleshooting](#troubleshooting)
* [Issues](#issues)
* [TO DO](#to-do)
* [DONE](#done)
* [Contributions and Thanks](#contributions-and-thanks)
* [Contributing](#contributing)
* [License](#license)
* [Copyright](#copyright)

---
---

### Why do we need this [ESP_AT_WM_Lite library](https://github.com/khoih-prog/ESP_AT_WM_Lite)

#### Features

This library is a Light Weight Credentials / WiFi Manager for ESP8266/ESP32-AT shields, specially designed to support **AVR Mega, SAM DUE, SAMD21, SAMD51, nRF52, STM32F/L/H/G/WB/MP1, etc. boards running ESP8266/ESP32-AT-command shields.** with smaller memory (64+K bytes)

The **RP2040-based boards, such as Nano_RP1040_Connect, RASPBERRY_PI_PICO**, are currently supported using [**Earle Philhower's arduino-pico** core](https://github.com/earlephilhower/arduino-pico) or [**Arduino-mbed RP2040** core](https://github.com/arduino/ArduinoCore-mbed) with Blynk/WiFiManager features thanks to **LittleFS** support.

Other AVR-family boards (UNO, Nano, etc.) are **not supported** as they don't have enough memory to run Config Portal WebServer.

This [ESP_AT_WM_Lite library](https://github.com/khoih-prog/ESP_AT_WM_Lite) is a Credentials / WiFi Connection Manager, permitting the addition of custom parameters to be configured in Config Portal. The parameters then will be saved automatically, **without the complicated callback functions** to handle data saving / retrieving.

If you have used one of the full-fledge WiFiManagers such as :

1. [`Tzapu WiFiManager`](https://github.com/tzapu/WiFiManager)
2. [`Ken Taylor WiFiManager`](https://github.com/kentaylor/WiFiManager)
3. [`Khoi Hoang ESP_WiFiManager`](https://github.com/khoih-prog/ESP_WiFiManager)

and have to write **complicated callback functions** to save custom parameters in SPIFFS/LittleFS/EEPROM, you'd appreciate the simplicity of this Light-Weight Credentials / WiFiManager.

You can also specify static AP and STA IP. Use much less memory compared to full-fledge WiFiManager. Config Portal will be auto-adjusted to match the number of dynamic custom parameters. Credentials are saved in EEPROM, [`FlashStorage_SAMD`](https://github.com/khoih-prog/FlashStorage_SAMD), [`FlashStorage_STM32`](https://github.com/khoih-prog/FlashStorage_STM32), [`DueFlashStorage`](https://github.com/sebnil/DueFlashStorage) or nRF52 LittleFS.

The web configuration portal, served from the `ESP32/ESP8266-AT WiFi` is operating as an access point (AP) with configurable static IP address or use default IP Address of 192.168.4.1

New recent features:

- **MultiWiFi** feature for configuring/auto(re)connecting **ESP32/ESP8266-AT WiFi** to the available MultiWiFi APs at runtime.
- **DoubleDetectDetector** feature to force Config Portal when double reset is detected within predetermined time, default 10s.
- **Powerful-yet-simple-to-use feature to enable adding dynamic custom parameters** from sketch and input using the same Config Portal. Config Portal will be auto-adjusted to match the number of dynamic parameters.
- Optional default **Credentials as well as Dynamic parameters to be optionally autoloaded into Config Portal** to use or change instead of manually input.
- Dynamic custom parameters to be saved **automatically in non-volatile memory, such as EEPROM, FlashStorage_SAMD, FlashStorage_STM32, DueFlashStorage or nRF52 LittleFS.**.
- Configurable **Config Portal Title** to be either BoardName or default undistinguishable names.
- Examples are designed to separate Credentials / Defines / Dynamic Params / Code, so that you can change Credentials / Dynamic Params quickly for each device.
- To permit autoreset after configurable timeout if DRD/MRD or non-persistent forced-CP
- Use new nRF52 LittleFS, SAMD FlashStorage_SAMD and STM32F/L/H/G/WB/MP1 FlashStorage_STM32 features
- **Scan WiFi networks** for selection in Configuration Portal

#### Currently Supported Boards

This [**ESP_AT_WM_Lite** library](https://github.com/khoih-prog/ESP_AT_WM_Lite) currently supports these following boards:

 1. **nRF52 boards**, such as **AdaFruit Feather nRF52832, nRF52840 Express, BlueFruit Sense, Itsy-Bitsy nRF52840 Express, Metro nRF52840 Express, NINA_B302_ublox, NINA_B112_ublox, etc.**
 
 2. **SAM DUE**
 
 3. **SAMD21**
  - Arduino SAMD21: ZERO, MKRs, NANO_33_IOT, etc.
  - Adafruit SAMD21 (M0): ItsyBitsy M0, Feather M0, Feather M0 Express, Metro M0 Express, Circuit Playground Express, Trinket M0, PIRkey, Hallowing M0, Crickit M0, etc.
  - Seeeduino: LoRaWAN, Zero, Femto M0, XIAO M0, Wio GPS Board, etc.
  
 4. **SAMD51**
  - Adafruit SAMD51 (M4): Metro M4, Grand Central M4, ItsyBitsy M4, Feather M4 Express, Trellis M4, Metro M4 AirLift Lite, MONSTER M4SK Express, Hallowing M4, etc.
  - Seeeduino: Wio Terminal, Grove UI Wireless
  
 5. **Teensy (4.1, 4.0, 3.6, 3.5, 3,2, 3.1, 3.0, LC)**
 
 6. **STM32F/L/H/G/WB/MP1 boards (with 64+K Flash)**

- Nucleo-144
- Nucleo-64
- Discovery
- Generic STM32F0, STM32F1, STM32F2, STM32F3, STM32F4, STM32F7 (with 64+K Flash): x8 and up
- STM32L0, STM32L1, STM32L4, **STM32L5**
- STM32G0, STM32G4
- STM32H7
- STM32WB
- STM32MP1
- LoRa boards
- 3-D printer boards
- Generic Flight Controllers
- Midatronics boards
 
 7. RP2040-based boards, such as **Nano RP2040 Connect**, using [**Arduino mbed OS for Nano boards**](https://github.com/arduino/ArduinoCore-mbed).

 8. RP2040-based boards, such as **RASPBERRY_PI_PICO, ADAFRUIT_FEATHER_RP2040 and GENERIC_RP2040**, using [**Arduino-mbed RP2040** core](https://github.com/arduino/ArduinoCore-mbed) or [**Earle Philhower's arduino-pico** core](https://github.com/earlephilhower/arduino-pico).
 
---
---

## Prerequisites

 1. [`Arduino IDE 1.8.19+` for Arduino](https://github.com/arduino/Arduino). [![GitHub release](https://img.shields.io/github/release/arduino/Arduino.svg)](https://github.com/arduino/Arduino/releases/latest)
 2. [`Arduino AVR core 1.8.5+`](https://github.com/arduino/ArduinoCore-avr) for Arduino (Use Arduino Board Manager) for AVR boards. [![GitHub release](https://img.shields.io/github/release/arduino/ArduinoCore-avr.svg)](https://github.com/arduino/ArduinoCore-avr/releases/latest)
 3. [`Arduino Core for STM32 v2.2.0+`](https://github.com/stm32duino/Arduino_Core_STM32) for STM32F/L/H/G/WB/MP1 boards (Nucleo-144 NUCLEO_F767ZI, Nucleo-64 NUCLEO_L053R8, etc.). [![GitHub release](https://img.shields.io/github/release/stm32duino/Arduino_Core_STM32.svg)](https://github.com/stm32duino/Arduino_Core_STM32/releases/latest)
 4. [`Teensy core 1.56+`](https://www.pjrc.com/teensy/td_download.html) for Teensy (4.1, 4.0, 3.6, 3.5, 3,2, 3.1, 3.0, LC) boards
 5. [`Arduino SAM DUE core v1.6.12+`](https://github.com/arduino/ArduinoCore-sam) for SAM DUE ARM Cortex-M3 boards.
 6. [`Arduino SAMD core 1.8.12+`](https://github.com/arduino/ArduinoCore-samd) for SAMD ARM Cortex-M0+ boards. [![GitHub release](https://img.shields.io/github/release/arduino/ArduinoCore-samd.svg)](https://github.com/arduino/ArduinoCore-samd/releases/latest)
 7. [`Adafruit SAMD core 1.7.9+`](https://github.com/adafruit/ArduinoCore-samd) for SAMD ARM Cortex-M0+ and M4 boards (Nano 33 IoT, etc.). [![GitHub release](https://img.shields.io/github/release/adafruit/ArduinoCore-samd.svg)](https://github.com/adafruit/ArduinoCore-samd/releases/latest)
 8. [`Seeeduino SAMD core 1.8.2+`](https://github.com/Seeed-Studio/ArduinoCore-samd) for SAMD21/SAMD51 boards (XIAO M0, Wio Terminal, etc.). [![Latest release](https://img.shields.io/github/release/Seeed-Studio/ArduinoCore-samd.svg)](https://github.com/Seeed-Studio/ArduinoCore-samd/releases/latest/)
 9. [`Adafruit nRF52 v1.3.0+`](https://github.com/adafruit/Adafruit_nRF52_Arduino) for nRF52 boards such as Adafruit NRF52840_FEATHER, NRF52832_FEATHER, NRF52840_FEATHER_SENSE, NRF52840_ITSYBITSY, NRF52840_CIRCUITPLAY, NRF52840_CLUE, NRF52840_METRO, NRF52840_PCA10056, PARTICLE_XENON, **NINA_B302_ublox**, etc. [![GitHub release](https://img.shields.io/github/release/adafruit/Adafruit_nRF52_Arduino.svg)](https://github.com/adafruit/Adafruit_nRF52_Arduino/releases/latest)
10. [`Arduino mbed_rp2040 core 2.7.2+`](https://github.com/arduino/ArduinoCore-mbed) for Arduino RP2040-based boards, such as **Arduino Nano RP2040 Connect, RASPBERRY_PI_PICO, etc.**. [![GitHub release](https://img.shields.io/github/release/arduino/ArduinoCore-mbed.svg)](https://github.com/arduino/ArduinoCore-mbed/releases/latest)
11. [`Earle Philhower's arduino-pico core v1.12.0+`](https://github.com/earlephilhower/arduino-pico) for RP2040-based boards such as **RASPBERRY_PI_PICO, ADAFRUIT_FEATHER_RP2040 and GENERIC_RP2040**, etc. [![GitHub release](https://img.shields.io/github/release/earlephilhower/arduino-pico.svg)](https://github.com/earlephilhower/arduino-pico/releases/latest)
12. [`ESP8266_AT_WebServer library v1.5.3+`](https://github.com/khoih-prog/ESP8266_AT_WebServer) to be able to support ESP32-AT shields. To install, check [![arduino-library-badge](https://www.ardu-badge.com/badge/ESP8266_AT_WebServer.svg?)](https://www.ardu-badge.com/ESP8266_AT_WebServer)
13. [`FlashStorage_SAMD library v1.3.2+`](https://github.com/khoih-prog/FlashStorage_SAMD) for SAMD21 and SAMD51 boards (ZERO, MKR, NANO_33_IOT, M0, M0 Pro, AdaFruit Itsy-Bitsy M4, etc.). To install, check [![arduino-library-badge](https://www.ardu-badge.com/badge/FlashStorage_SAMD.svg?)](https://www.ardu-badge.com/FlashStorage_SAMD)
14. [`FlashStorage_STM32F1 library v1.1.0+`](https://github.com/khoih-prog/FlashStorage_STM32F1) for STM32F1/F3 boards. To install. check [![arduino-library-badge](https://www.ardu-badge.com/badge/FlashStorage_STM32F1.svg?)](https://www.ardu-badge.com/FlashStorage_STM32F1)
15. [`FlashStorage_STM32 library v1.2.0+`](https://github.com/khoih-prog/FlashStorage_STM32) for STM32F/L/H/G/WB/MP1 boards. To install. check [![arduino-library-badge](https://www.ardu-badge.com/badge/FlashStorage_STM32.svg?)](https://www.ardu-badge.com/FlashStorage_STM32) 
16. [`DueFlashStorage library v1.0.0+`](https://github.com/sebnil/DueFlashStorage) for SAM DUE. To install, check [![arduino-library-badge](https://www.ardu-badge.com/badge/DueFlashStorage.svg?)](https://www.ardu-badge.com/DueFlashStorage)
17. [`Adafruit's LittleFS/InternalFS`](www.adafruit.com) for nRF52
18. [`DoubleResetDetector_Generic v1.8.0+`](https://github.com/khoih-prog/DoubleResetDetector_Generic). To install. check [![arduino-library-badge](https://www.ardu-badge.com/badge/DoubleResetDetector_Generic.svg?)](https://www.ardu-badge.com/DoubleResetDetector_Generic)
19. [`Ai-Thinker AT Firmware v1.5.4`](https://github.com/khoih-prog/ESP8266_AT_WebServer/blob/master/AT_Firmwares/At_firmware_bin1.54.zip) or [`AT Firmware v1.7.4.0`](https://github.com/khoih-prog/ESP8266_AT_WebServer/blob/master/AT_Firmwares/AT_Firmware_bin_1.7.4.0.zip) for ESP8266-AT WiFi shields.
20. [`AT version_2.1.0.0_dev`](https://github.com/khoih-prog/ESP8266_AT_WebServer/blob/master/AT_Firmwares/AT_version_2.1.0.0_dev.zip) for ESP32-AT WiFi shields.
21. `AT version_1.1.4` for WIS600-01S and W600-AT WiFi shields.

---

### Important Notes about AT Firmwares

### 1. Firmwares tested OK with ESP8266 AT shields

  - [`Ai-Thinker AT Firmware v1.5.4`](https://github.com/khoih-prog/ESP8266_AT_WebServer/blob/master/AT_Firmwares/At_firmware_bin1.54.zip)
  
    ```
    AT version:1.1.0.0(May 11 2016 18:09:56)
    SDK version:1.5.4(baaeaebb)
    Ai-Thinker Technology Co. Ltd.
    Jun 13 2016 11:29:20
    ```
  - [`AT Firmware v1.7.4.0`](https://github.com/khoih-prog/ESP8266_AT_WebServer/blob/master/AT_Firmwares/AT_Firmware_bin_1.7.4.0.zip)
  
    ```
    AT version:1.7.4.0(May 11 2020 19:13:04)
    SDK version:3.0.4(9532ceb)
    compile time:May 27 2020 10:12:17
    Bin version(Wroom 02):1.7.4
    ```
    
  - [`WIS600-01S`](https://www.aliexpress.com/item/32956707239.html) and [`W600`](https://www.seeedstudio.com/w600.html) using either ESP8266 or ESP32-AT commands and stock firmware
  
    ```
    AT version:1.1.4(Dec 05 2018 11:06:45)
    SDK version:3.0.0
    Dec 05 2018 11:06:45
    ```
  
  
### 2. Firmwares tested OK with ESP32 AT shields

  - [`AT version_2.1.0.0_dev`](https://github.com/khoih-prog/ESP8266_AT_WebServer/blob/master/AT_Firmwares/AT_version_2.1.0.0_dev.zip)
    
    ```
    AT version:2.1.0.0-dev(4f6b92c - Jun 10 2020 10:36:54)
    SDK version:v4.0.1-193-ge7ac221b4
    compile time(b85a8df):Jun 18 2020 14:00:59
    Bin version:2.0.0(WROOM-32)
    ```
    
    See instructions at [AT Command Core](https://github.com/espressif/esp-at) and [ESP_AT_Get_Started](https://github.com/espressif/esp-at/blob/master/docs/en/get-started/ESP_AT_Get_Started.md)
  
3. Upload [`AT Firmware v1.7.4.0`](https://github.com/khoih-prog/ESP8266_AT_WebServer/blob/master/AT_Firmwares/AT_Firmware_bin_1.7.4.0.zip) bin files to correct locations as follows:

```
# BOOT MODE

### Flash size 8Mbit: 512KB+512KB
    boot_v1.2+.bin              0x00000
    user1.1024.new.2.bin        0x01000
    esp_init_data_default.bin   0xfc000
    blank.bin                   0x7e000 & 0xfe000


### Flash size 16Mbit-C1: 1024KB+1024KB
    boot_v1.2+.bin              0x00000
    user1.2048.new.5.bin        0x01000
    esp_init_data_default.bin   0x1fc000
    blank.bin                   0xfe000 & 0x1fe000
```

4. Test before using different AT-Firmware Version at your own risks. Just use any simple example to verify if the AT-firmware is OK.
5. Compatible AT-Firmare version will be updated. Check for all supported AT Firmwares and download them from [AT_Firmwares](https://github.com/khoih-prog/ESP8266_AT_WebServer/tree/master/AT_Firmwares).
6. Support to ESP32-AT-command shields will be added by using new library [ESP_AT_Lib](https://github.com/khoih-prog/ESP_AT_Lib) to replace [Blynk's BlynkESP8266_Lib](https://github.com/blynkkk/blynk-library/releases/Blynk_Release_v0.6.1.zip).

---

## Installation

### Use Arduino Library Manager

The best and easiest way is to use `Arduino Library Manager`. Search for `ESP_AT_WM_Lite`, then select / install the latest version.
You can also use this link [![arduino-library-badge](https://www.ardu-badge.com/badge/ESP_AT_WM_Lite.svg?)](https://www.ardu-badge.com/ESP_AT_WM_Lite) for more detailed instructions.

### Manual Install

1. Navigate to [ESP_AT_WM_Lite](https://github.com/khoih-prog/ESP_AT_WM_Lite) page.
2. Download the latest release `ESP_AT_WM_Lite-master.zip`.
3. Extract the zip file to `ESP_AT_WM_Lite-master` directory 
4. Copy whole 
  - `ESP_AT_WM_Lite-master` folder to Arduino libraries' directory such as `~/Arduino/libraries/`.

### VS Code & PlatformIO:

1. Install [VS Code](https://code.visualstudio.com/)
2. Install [PlatformIO](https://platformio.org/platformio-ide)
3. Install [**ESP_AT_WM_Lite** library](https://platformio.org/lib/show/7131/ESP_AT_WM_Lite) by using [Library Manager](https://platformio.org/lib/show/7131/ESP_AT_WM_Lite/installation). Search for **ESP_AT_WM_Lite** in [Platform.io Author's Libraries](https://platformio.org/lib/search?query=author:%22Khoi%20Hoang%22)
4. Use included [platformio.ini](platformio/platformio.ini) file from examples to ensure that all dependent libraries will installed automatically. Please visit documentation for the other options and examples at [Project Configuration File](https://docs.platformio.org/page/projectconf.html)

---
---

### Packages' Patches

#### 1. For Adafruit nRF52840 and nRF52832 boards

**To be able to compile, run and automatically detect and display BOARD_NAME on nRF52840/nRF52832 boards**, you have to copy the whole [nRF52 Packages_Patches](Packages_Patches/adafruit/hardware/nrf52/1.3.0) directory into Adafruit nRF52 directory (~/.arduino15/packages/adafruit/hardware/nrf52/1.3.0). 

Supposing the Adafruit nRF52 version is 1.3.0. These files must be copied into the directory:
- `~/.arduino15/packages/adafruit/hardware/nrf52/1.3.0/platform.txt`
- `~/.arduino15/packages/adafruit/hardware/nrf52/1.3.0/boards.txt`
- `~/.arduino15/packages/adafruit/hardware/nrf52/1.3.0/cores/nRF5/Udp.h`
- `~/.arduino15/packages/adafruit/hardware/nrf52/1.3.0/cores/nRF5/Print.h`
- `~/.arduino15/packages/adafruit/hardware/nrf52/1.3.0/cores/nRF5/Print.cpp`
- `~/.arduino15/packages/adafruit/hardware/nrf52/1.3.0/variants/NINA_B302_ublox/variant.h`
- `~/.arduino15/packages/adafruit/hardware/nrf52/1.3.0/variants/NINA_B302_ublox/variant.cpp`
- `~/.arduino15/packages/adafruit/hardware/nrf52/1.3.0/variants/NINA_B112_ublox/variant.h`
- `~/.arduino15/packages/adafruit/hardware/nrf52/1.3.0/variants/NINA_B112_ublox/variant.cpp`
- **`~/.arduino15/packages/adafruit/hardware/nrf52/1.3.0/cores/nRF5/Udp.h`**

Whenever a new version is installed, remember to copy these files into the new version directory. For example, new version is x.yy.z
These files must be copied into the directory:

- `~/.arduino15/packages/adafruit/hardware/nrf52/x.yy.z/platform.txt`
- `~/.arduino15/packages/adafruit/hardware/nrf52/x.yy.z/boards.txt`
- `~/.arduino15/packages/adafruit/hardware/nrf52/x.yy.z/cores/nRF5/Udp.h`
- `~/.arduino15/packages/adafruit/hardware/nrf52/x.yy.z/cores/nRF5/Print.h`
- `~/.arduino15/packages/adafruit/hardware/nrf52/x.yy.z/cores/nRF5/Print.cpp`
- `~/.arduino15/packages/adafruit/hardware/nrf52/x.yy.z/variants/NINA_B302_ublox/variant.h`
- `~/.arduino15/packages/adafruit/hardware/nrf52/x.yy.z/variants/NINA_B302_ublox/variant.cpp`
- `~/.arduino15/packages/adafruit/hardware/nrf52/x.yy.z/variants/NINA_B112_ublox/variant.h`
- `~/.arduino15/packages/adafruit/hardware/nrf52/x.yy.z/variants/NINA_B112_ublox/variant.cpp`
- **`~/.arduino15/packages/adafruit/hardware/nrf52/x.yy.z/cores/nRF5/Udp.h`**

#### 2. For Teensy boards
 
 **To be able to compile and run on Teensy boards**, you have to copy the files in [**Packages_Patches for Teensy directory**](Packages_Patches/hardware/teensy/avr) into Teensy hardware directory (./arduino-1.8.19/hardware/teensy/avr/boards.txt). 

Supposing the Arduino version is 1.8.19. These files must be copied into the directory:

- `./arduino-1.8.19/hardware/teensy/avr/boards.txt`
- `./arduino-1.8.19/hardware/teensy/avr/cores/teensy/Stream.h`
- `./arduino-1.8.19/hardware/teensy/avr/cores/teensy3/Stream.h`
- `./arduino-1.8.19/hardware/teensy/avr/cores/teensy4/Stream.h`

Whenever a new version is installed, remember to copy this file into the new version directory. For example, new version is x.yy.zz
These files must be copied into the directory:

- `./arduino-x.yy.zz/hardware/teensy/avr/boards.txt`
- `./arduino-x.yy.zz/hardware/teensy/avr/cores/teensy/Stream.h`
- `./arduino-x.yy.zz/hardware/teensy/avr/cores/teensy3/Stream.h`
- `./arduino-x.yy.zz/hardware/teensy/avr/cores/teensy4/Stream.h`

#### 3. For Arduino SAM DUE boards
 
 **To be able to compile and run on SAM DUE boards**, you have to copy the whole [SAM DUE](Packages_Patches/arduino/hardware/sam/1.6.12) directory into Arduino sam directory (~/.arduino15/packages/arduino/hardware/sam/1.6.12). 

Supposing the Arduino SAM core version is 1.6.12. This file must be copied into the directory:

- `~/.arduino15/packages/arduino/hardware/sam/1.6.12/platform.txt`

Whenever a new version is installed, remember to copy this file into the new version directory. For example, new version is x.yy.zz
This file must be copied into the directory:

- `~/.arduino15/packages/arduino/hardware/sam/x.yy.zz/platform.txt`

#### 4. For Arduino SAMD boards
 
 ***To be able to compile, run and automatically detect and display BOARD_NAME on Arduino SAMD (Nano-33-IoT, etc) boards***, you have to copy the whole [Arduino SAMD Packages_Patches](Packages_Patches/arduino/hardware/samd/1.8.12) directory into Arduino SAMD directory (~/.arduino15/packages/arduino/hardware/samd/1.8.12).
 
#### For core version v1.8.10+

Supposing the Arduino SAMD version is 1.8.12. Now only one file must be copied into the directory:

- `~/.arduino15/packages/arduino/hardware/samd/1.8.12/platform.txt`

Whenever a new version is installed, remember to copy this files into the new version directory. For example, new version is x.yy.zz

This file must be copied into the directory:

- `~/.arduino15/packages/arduino/hardware/samd/x.yy.zz/platform.txt`
 
#### For core version v1.8.9-

Supposing the Arduino SAMD version is 1.8.9. These files must be copied into the directory:

- `~/.arduino15/packages/arduino/hardware/samd/1.8.9/platform.txt`
- ***`~/.arduino15/packages/arduino/hardware/samd/1.8.9/cores/arduino/Arduino.h`***

Whenever a new version is installed, remember to copy these files into the new version directory. For example, new version is x.yy.z

These files must be copied into the directory:

- `~/.arduino15/packages/arduino/hardware/samd/x.yy.z/platform.txt`
- ***`~/.arduino15/packages/arduino/hardware/samd/x.yy.z/cores/arduino/Arduino.h`***
 
 This is mandatory to fix the ***notorious Arduino SAMD compiler error***. See [Improve Arduino compatibility with the STL (min and max macro)](https://github.com/arduino/ArduinoCore-samd/pull/399)
 
```
 ...\arm-none-eabi\include\c++\7.2.1\bits\stl_algobase.h:243:56: error: macro "min" passed 3 arguments, but takes just 2
     min(const _Tp& __a, const _Tp& __b, _Compare __comp)
```

Whenever the above-mentioned compiler error issue is fixed with the new Arduino SAMD release, you don't need to copy the `Arduino.h` file anymore.

#### 5. For Adafruit SAMD boards
 
 ***To be able to compile, run and automatically detect and display BOARD_NAME on Adafruit SAMD (Itsy-Bitsy M4, etc) boards***, you have to copy the whole [Adafruit SAMD Packages_Patches](Packages_Patches/adafruit/hardware/samd/1.7.9) directory into Adafruit samd directory (~/.arduino15/packages/adafruit/hardware/samd/1.7.9). 

Supposing the Adafruit SAMD core version is 1.7.9. This file must be copied into the directory:

- `~/.arduino15/packages/adafruit/hardware/samd/1.7.9/platform.txt`
- `~/.arduino15/packages/adafruit/hardware/samd/1.7.9/cores/arduino/Print.h`
- `~/.arduino15/packages/adafruit/hardware/samd/1.7.9/cores/arduino/Print.cpp`

Whenever a new version is installed, remember to copy this file into the new version directory. For example, new version is x.yy.zz
This file must be copied into the directory:

- `~/.arduino15/packages/adafruit/hardware/samd/x.yy.zz/platform.txt`
- `~/.arduino15/packages/adafruit/hardware/samd/x.yy.zz/cores/arduino/Print.h`
- `~/.arduino15/packages/adafruit/hardware/samd/x.yy.zz/cores/arduino/Print.cpp`

#### 6. For Seeeduino SAMD boards
 
 ***To be able to compile, run and automatically detect and display BOARD_NAME on Seeeduino SAMD (XIAO M0, Wio Terminal, etc) boards***, you have to copy the whole [Seeeduino SAMD Packages_Patches](Packages_Patches/Seeeduino/hardware/samd/1.8.2) directory into Seeeduino samd directory (~/.arduino15/packages/Seeeduino/hardware/samd/1.8.2). 

Supposing the Seeeduino SAMD core version is 1.8.2. This file must be copied into the directory:

- `~/.arduino15/packages/Seeeduino/hardware/samd/1.8.2/platform.txt`
- `~/.arduino15/packages/Seeeduino/hardware/samd/1.8.2/cores/arduino/Arduino.h`
- `~/.arduino15/packages/Seeeduino/hardware/samd/1.8.2/cores/arduino/Print.h`
- `~/.arduino15/packages/Seeeduino/hardware/samd/1.8.2/cores/arduino/Print.cpp`

Whenever a new version is installed, remember to copy this file into the new version directory. For example, new version is x.yy.zz
This file must be copied into the directory:

- `~/.arduino15/packages/Seeeduino/hardware/samd/x.yy.zz/platform.txt`
- `~/.arduino15/packages/Seeeduino/hardware/samd/x.yy.zz/cores/arduino/Arduino.h`
- `~/.arduino15/packages/Seeeduino/hardware/samd/x.yy.zz/cores/arduino/Print.h`
- `~/.arduino15/packages/Seeeduino/hardware/samd/x.yy.zz/cores/arduino/Print.cpp`

#### 7. For STM32 boards

#### 7.1 For STM32 boards to use LAN8720

To use LAN8720 on some STM32 boards 

- **Nucleo-144 (F429ZI, NUCLEO_F746NG, NUCLEO_F746ZG, NUCLEO_F756ZG)**
- **Discovery (DISCO_F746NG)**
- **STM32F4 boards (BLACK_F407VE, BLACK_F407VG, BLACK_F407ZE, BLACK_F407ZG, BLACK_F407VE_Mini, DIYMORE_F407VGT, FK407M1)**

you have to copy the files [stm32f4xx_hal_conf_default.h](Packages_Patches/STM32/hardware/stm32/2.2.0/system/STM32F4xx) and [stm32f7xx_hal_conf_default.h](Packages_Patches/STM32/hardware/stm32/2.2.0/system/STM32F7xx) into STM32 stm32 directory (~/.arduino15/packages/STM32/hardware/stm32/2.2.0/system) to overwrite the old files.

Supposing the STM32 stm32 core version is 2.2.0. These files must be copied into the directory:

- `~/.arduino15/packages/STM32/hardware/stm32/2.2.0/system/STM32F4xx/stm32f4xx_hal_conf_default.h` for STM32F4.
- `~/.arduino15/packages/STM32/hardware/stm32/2.2.0/system/STM32F7xx/stm32f7xx_hal_conf_default.h` for Nucleo-144 STM32F7.

Whenever a new version is installed, remember to copy this file into the new version directory. For example, new version is x.yy.zz,
theses files must be copied into the corresponding directory:

- `~/.arduino15/packages/STM32/hardware/stm32/x.yy.zz/system/STM32F4xx/stm32f4xx_hal_conf_default.h`
- `~/.arduino15/packages/STM32/hardware/stm32/x.yy.zz/system/STM32F7xx/stm32f7xx_hal_conf_default.h


#### 7.2 For STM32 boards to use Serial1

**To use Serial1 on some STM32 boards without Serial1 definition (Nucleo-144 NUCLEO_F767ZI, Nucleo-64 NUCLEO_L053R8, etc.) boards**, you have to copy the files [STM32 variant.h](Packages_Patches/STM32/hardware/stm32/2.2.0) into STM32 stm32 directory (~/.arduino15/packages/STM32/hardware/stm32/2.2.0). You have to modify the files corresponding to your boards, this is just an illustration how to do.

Supposing the STM32 stm32 core version is 2.2.0. These files must be copied into the directory:

- `~/.arduino15/packages/STM32/hardware/stm32/2.2.0/variants/STM32F7xx/F765Z(G-I)T_F767Z(G-I)T_F777ZIT/NUCLEO_F767ZI/variant.h` for Nucleo-144 NUCLEO_F767ZI.
- `~/.arduino15/packages/STM32/hardware/stm32/2.2.0/variants/STM32L0xx/L052R(6-8)T_L053R(6-8)T_L063R8T/NUCLEO_L053R8/variant.h` for Nucleo-64 NUCLEO_L053R8.

Whenever a new version is installed, remember to copy this file into the new version directory. For example, new version is x.yy.zz,
theses files must be copied into the corresponding directory:

- `~/.arduino15/packages/STM32/hardware/stm32/x.yy.zz/variants/STM32F7xx/F765Z(G-I)T_F767Z(G-I)T_F777ZIT/NUCLEO_F767ZI/variant.h`
- `~/.arduino15/packages/STM32/hardware/stm32/x.yy.zz/variants/STM32L0xx/L052R(6-8)T_L053R(6-8)T_L063R8T/NUCLEO_L053R8/variant.h`

#### 8. For RP2040-based boards using [Earle Philhower arduino-pico core](https://github.com/earlephilhower/arduino-pico)

#### 8.1 To use BOARD_NAME

 **To be able to automatically detect and display BOARD_NAME on RP2040-based boards (RASPBERRY_PI_PICO, ADAFRUIT_FEATHER_RP2040, GENERIC_RP2040, etc) boards**, you have to copy the file [RP2040 platform.txt](Packages_Patches/rp2040/hardware/rp2040/1.4.0) into rp2040 directory (~/.arduino15/packages/rp2040/hardware/rp2040/1.4.0). 

Supposing the rp2040 core version is 1.4.0. This file must be copied into the directory:

- `~/.arduino15/packages/rp2040/hardware/rp2040/1.4.0/platform.txt`

Whenever a new version is installed, remember to copy this file into the new version directory. For example, new version is x.yy.zz
This file must be copied into the directory:

- `~/.arduino15/packages/rp2040/hardware/rp2040/x.yy.zz/platform.txt`

With core after v1.5.0, this step is not necessary anymore thanks to the PR [Add -DBOARD_NAME="{build.board}" #136](https://github.com/earlephilhower/arduino-pico/pull/136).

#### 8.2 To avoid compile error relating to microsecondsToClockCycles

Some libraries, such as [Adafruit DHT-sensor-library](https://github.com/adafruit/DHT-sensor-library), require the definition of microsecondsToClockCycles(). **To be able to compile and run on RP2040-based boards**, you have to copy the files in [**RP2040 Arduino.h**](Packages_Patches/rp2040/hardware/rp2040/1.4.0/cores/rp2040/Arduino.h) into rp2040 directory (~/.arduino15/packages/rp2040/hardware/rp2040/1.4.0).

Supposing the rp2040 core version is 1.4.0. This file must be copied to replace:

- `~/.arduino15/packages/rp2040/hardware/rp2040/1.4.0/cores/rp2040/Arduino.h`

Whenever a new version is installed, remember to copy this file into the new version directory. For example, new version is x.yy.zz
This file must be copied to replace:

- `~/.arduino15/packages/rp2040/hardware/rp2040/x.yy.zz/cores/rp2040/Arduino.h`

With core after v1.5.0, this step is not necessary anymore thanks to the PR [Add defs for compatibility #142](https://github.com/earlephilhower/arduino-pico/pull/142).


#### 9. For Portenta_H7 boards using Arduino IDE in Linux

  **To be able to upload firmware to Portenta_H7 using Arduino IDE in Linux (Ubuntu, etc.)**, you have to copy the file [portenta_post_install.sh](Packages_Patches/arduino/hardware/mbed_portenta/2.7.2/portenta_post_install.sh) into mbed_portenta directory (~/.arduino15/packages/arduino/hardware/mbed_portenta/2.7.2/portenta_post_install.sh). 
  
  Then run the following command using `sudo`
  
```
$ cd ~/.arduino15/packages/arduino/hardware/mbed_portenta/2.7.2
$ chmod 755 portenta_post_install.sh
$ sudo ./portenta_post_install.sh
```

This will create the file `/etc/udev/rules.d/49-portenta_h7.rules` as follows:

```
# Portenta H7 bootloader mode UDEV rules

SUBSYSTEMS=="usb", ATTRS{idVendor}=="2341", ATTRS{idProduct}=="035b", GROUP="plugdev", MODE="0666"
```

Supposing the ArduinoCore-mbed core version is 2.7.2. Now only one file must be copied into the directory:

- `~/.arduino15/packages/arduino/hardware/mbed_portenta/2.7.2/portenta_post_install.sh`

Whenever a new version is installed, remember to copy this files into the new version directory. For example, new version is x.yy.zz

This file must be copied into the directory:

- `~/.arduino15/packages/arduino/hardware/mbed_portenta/x.yy.zz/portenta_post_install.sh`

---
---

## How It Works

- The [nRF52_ESP8266Shield](examples/nRF52_ESP8266Shield) example shows how it works and should be used as the basis for a sketch that uses this library.
- The concept of [nRF52_ESP8266Shield](examples/nRF52_ESP8266Shield) is that a new `ESP8266 AT shield` will start a WiFi configuration portal when powered up, but has no valid stored Credentials. 
- There are **maximum 3 more custom parameters** added in the sketch which you can use in your program later. In the example, they are: Blynk Server, Token and Blynk Port.
- Using any WiFi enabled device with a browser (computer, phone, tablet) connect to the newly created AP and type in the configurable AP IP address (default 192.168.4.1). The Config Portal AP channel (default 10) is also configurable to **avoid conflict** with other APs.
- The Config Portal is auto-adjusted to fix the 2 static parameters (WiFi SSID/PWD) as well as 6 more dynamic custom parameters.
- After the custom data entered, and `Save` button pressed, the configuration data will be saved in host's non-volatile memory, then the board reboots.
- If there is valid stored Credentials, it'll go directly to connect to WiFi without starting / using the Config Portal.
- `ESP8266 AT shield` will try to connect. If successful, the dynamic DHCP or configured static IP address will be displayed in the configuration portal. 
- The `ESP8266 AT shield` WiFi Config Portal network and Web Server will shutdown to return control to the sketch code.

---
---

### How to use

#### 1. Basic usage

- Include in your sketch

```cpp

// Select depending on board
#define EspSerial Serial1

#include <Esp8266_AT_WM_Lite_nRF52.h>

#define HOST_NAME   "nRF52-ESP_AT"

// SSID and PW for Config Portal
String portal_ssid      = "nRF52-CfgPrtl-SSID";
String portal_password  = "nRF52-CfgPrtl-PW";

ESP_AT_WiFiManager_Lite* ESP_AT_WiFiManager; 

// Your nRF52 <-> ESP8266 baud rate:
#define ESP8266_BAUD 115200
```

#### 2. Add custom parameters

- To add custom parameters, just modify from the example below

```
#define USE_DYNAMIC_PARAMETERS      true

/////////////// Start dynamic Credentials ///////////////

//Defined in <Esp8266_AT_WM_Lite_nRF52.h>
/**************************************
  #define MAX_ID_LEN                5
  #define MAX_DISPLAY_NAME_LEN      16

  typedef struct
  {
  char id             [MAX_ID_LEN + 1];
  char displayName    [MAX_DISPLAY_NAME_LEN + 1];
  char *pdata;
  uint8_t maxlen;
  } MenuItem;
**************************************/

#if USE_DYNAMIC_PARAMETERS

#define MAX_MQTT_SERVER_LEN      34
char MQTT_Server  [MAX_MQTT_SERVER_LEN + 1]   = "mqtt-server";

#define MAX_MQTT_PORT_LEN        6
char MQTT_Port   [MAX_MQTT_PORT_LEN + 1]  = "1883";


MenuItem myMenuItems [] =
{
  { "mqtt", "MQTT Server",      MQTT_Server,      MAX_MQTT_SERVER_LEN },
  { "mqpt", "Port",             MQTT_Port,        MAX_MQTT_PORT_LEN   },
};

// Due to notorious 2K buffer limitation of ESO8266-AT shield, the NUM_MENU_ITEMS is limited to max 3
// to avoid WebServer not working due to HTML data larger than 2K can't be sent successfully
// The items with index larger than 3 will be ignored

uint16_t NUM_MENU_ITEMS = min( 3, sizeof(myMenuItems) / sizeof(MenuItem));  //MenuItemSize;

#else

MenuItem myMenuItems [] = {};

uint16_t NUM_MENU_ITEMS = 0;
#endif

/////// // End dynamic Credentials ///////////

```

#### 3. Not using custom parameters

- If you don't need to add dynamic parameters, use in sketch

```
#define USE_DYNAMIC_PARAMETERS      false

```

#### 4. To open Config Portal

- When you want to open a config portal, just add

```cpp
ESP_AT_WiFiManager = new ESP_AT_WiFiManager_Lite(&EspSerial, ESP8266_BAUD);

// Optional to change default AP IP(192.168.4.1)
//ESP_AT_WiFiManager->setConfigPortalIP(IPAddress(192, 168, 220, 1));

// Use channel(0) for random AP WiFi channel
ESP_AT_WiFiManager->setConfigPortalChannel(1);

// Personalized portal_ssid and password, e.g. CfgPrtl-SSID and CfgPrtl-PW
ESP_AT_WiFiManager->setConfigPortal(portal_ssid, portal_password);

ESP_AT_WiFiManager->begin();
```

#### 5. To use different AP WiFi Channel

- To not use default AP WiFi Channel 10 to avoid conflict with other WiFi APs, call 

```cpp
ESP_AT_WiFiManager->setConfigPortalChannel(newChannel);
```

- To use random AP WiFi Channel to avoid conflict with other WiFi APs : 

```cpp
ESP_AT_WiFiManager->setConfigPortalChannel(0);
```

#### 6. To use different static AP IP from default

- To use different static AP IP (not use default `192.168.4.1`), call 
```cpp
ESP_AT_WiFiManager->setConfigPortalIP(IPAddress(xxx,xxx,xxx,xxx));
```

While in AP mode, connect to it using its `SSID` (Personalized SSID or "ESP_AT_XXXXXX") / `Password` (Personalized PW or "MyESP_AT_XXXXXX"), then connect a Web-Browser to the Portal AP IP, default `192.168.4.1`, configure wifi and dynamic Credentials, then click `Save`. The Credentials / WiFi connection information will be saved in non-volatile memory. It will then autoconnect to one of the configured WiFi APs.


Once Credentials / WiFi network information is saved in the host non-volatile memory, it will try to autoconnect to WiFi every time it is started, without requiring any function calls in the sketch.

#### 7. To use and input only one set of WiFi SSID and PWD

#### 7.1 If you need to use and input only one set of WiFi SSID/PWD.

```
// Permit input only one set of WiFi SSID/PWD. The other can be "NULL or "blank"
// Default is false (if not defined) => must input 2 sets of SSID/PWD
#define REQUIRE_ONE_SET_SSID_PW       true
```
But it's always advisable to use and input both sets for reliability.
 
#### 7.2 If you need to use both sets of WiFi SSID/PWD

```
// Permit input only one set of WiFi SSID/PWD. The other can be "NULL or "blank"
// Default is false (if not defined) => must input 2 sets of SSID/PWD
#define REQUIRE_ONE_SET_SSID_PW       false
```

#### 8. To enable auto-scan of WiFi networks for selection in Configuration Portal

#### 8.1 Enable auto-scan of WiFi networks for selection in Configuration Portal


```
#define SCAN_WIFI_NETWORKS                  true
```

The manual input of SSIDs is default enabled, so that users can input arbitrary SSID, not only from the scanned list. This is for the sample use-cases in which users can input the known SSIDs of another place, then send the boards to that place. The boards can connect to WiFi without users entering Config Portal to re-configure.

#### 8.2 Disable manually input SSIDs

```
// To disable manually input SSID, only from a scanned SSID lists
#define MANUAL_SSID_INPUT_ALLOWED           false
```

This is for normal use-cases in which users can only select an SSID from a scanned list of SSIDs to avoid typo mistakes and/or security.

#### 8.3 Select maximum number of SSIDs in the list

The maximum number of SSIDs in the list is selectable from 2 to 6 for ESP8266/ESP32-AT. If invalid number of SSIDs is selected, the default number of 6 will be used.


```
// From 2-15
#define MAX_SSID_IN_LIST                    8
```

#### 9. To avoid blocking in loop when WiFi is lost


#### 9.1 Max times to try WiFi per loop

To define max times to try WiFi per loop() iteration. To avoid blocking issue in loop()

Default is 1 if not defined, and minimum is forced to be 1.

To use, uncomment in `defines.h`. 

Check [retries block the main loop #18](https://github.com/khoih-prog/WiFiManager_NINA_Lite/issues/18#issue-1094004380)

```
#define MAX_NUM_WIFI_RECON_TRIES_PER_LOOP     2
```

#### 9.2 Interval between reconnection WiFi if lost

Default is no interval between reconnection WiFi times if lost WiFi. Max permitted interval will be 10mins.

Uncomment to use. Be careful, WiFi reconnection will be delayed if using this method.

Only use whenever urgent tasks in loop() can't be delayed. But if so, it's better you have to rewrite your code, e.g. using higher priority tasks.

Check [retries block the main loop #18](https://github.com/khoih-prog/WiFiManager_NINA_Lite/issues/18#issuecomment-1006197561)

```
#define WIFI_RECON_INTERVAL                   30000     // 30s
```


#### 10. Not using Board_Name on Config_Portal

Default is `true`. Just change to `false` to not using `Board_Name` on Config_Portal


https://github.com/khoih-prog/ESP_AT_WM_Lite/blob/83964d54bd43fce6d8a7167b08e45d506ced8d27/examples/RPi_Pico_ESP8266Shield/defines.h#L126-L131

---
---

### Important Notes for using Dynamic Parameters' ids

1. These ids (such as "mqtt" in example) must be **unique**.

Please be noted that the following **reserved names are already used in library**:

```
"id"    for WiFi SSID
"pw"    for WiFi PW
"id1"   for WiFi1 SSID
"pw1"   for WiFi1 PW
"nm"    for Board Name
```

---
---

### Examples:

1. [Mega_ESP8266Shield](examples/Mega_ESP8266Shield)
2. [SAMD_ESP8266Shield](examples/SAMD_ESP8266Shield)
3. [SAM_DUE_ESP8266Shield](examples/SAM_DUE_ESP8266Shield)
4. [STM32_ESP8266Shield](examples/STM32_ESP8266Shield)
5. [nRF52_ESP8266Shield](examples/nRF52_ESP8266Shield)
6. [RPi_Pico_ESP8266Shield](examples/RPi_Pico_ESP8266Shield). **New**

---

## So, how it works?

In `Configuration Portal Mode`, it starts an AP named `ESP_AT_XXXXXX`. Connect to it using the `configurable password` you can define in the code. For example, `MyESP_AT_XXXXXX` (see examples):

<p align="center">
    <img src="https://github.com/khoih-prog/ESP_AT_WM_Lite/blob/master/pics/PortalAuth.png">
</p>


```cpp
// SSID and PW for Config Portal
String ssid = "ESP_AT_" + String(0x1ABCDEF, HEX);
const char* password = "ESP_AT_PW";
```

After you connected, please, go to http://192.168.4.1 or newly configured AP IP, The following Config Portal screen will appear:

<p align="center">
    <img src="https://github.com/khoih-prog/ESP_AT_WM_Lite/blob/master/pics/Main.png">
</p>

Enter your WiFi credentials,

### 1. Without SCAN_WIFI_NETWORKS


<p align="center">
    <img src="https://github.com/khoih-prog/ESP_AT_WM_Lite/blob/master/pics/Input.png">
</p>


### 2. With SCAN_WIFI_NETWORKS


<p align="center">
    <img src="https://github.com/khoih-prog/ESP_AT_WM_Lite/blob/master/pics/ESP_AT_Input_With_Scan.png">
</p>


then click `Save`. 

<p align="center">
    <img src="https://github.com/khoih-prog/ESP_AT_WM_Lite/blob/master/pics/Save.png">
</p>

The WiFi Credentials will be saved and the board connect to the selected WiFi AP.

If you're already connected to a listed WiFi AP and don't want to change anything, just select `Exit` from the `Main` page to reboot the board and connect to the previously-stored AP. The WiFi Credentials are still intact.

---

### Important Notes

1. Now you can use special chars such as **~, !, @, #, $, %, ^, &, _, -, space,etc.** thanks to [brondolin](https://github.com/brondolin) to provide the amazing fix in [**Blynk_WM**](https://github.com/khoih-prog/Blynk_WM) to permit input special chars such as **%** and **#** into data fields. See [Issue 3](https://github.com/khoih-prog/Blynk_WM/issues/3).
2. The SSIDs, Passwords must be input (or to make them different from **blank**). Otherwise, the Config Portal will re-open until those fields have been changed. If you don't need any field, just input anything or use duplicated data from similar field.
3. WiFi password max length now is 63 chars according to WPA2 standard.

---
---

### How to use default Credentials and have them pre-loaded onto Config Portal

See this example and modify as necessary

#### 1. To always load [Default Credentials](examples/SAMD_ESP8266Shield/Credentials.h) and override Config Portal data

```
// Used mostly for development and debugging. FORCES default values to be loaded each run.
// Config Portal data input will be ignored and overridden by DEFAULT_CONFIG_DATA
bool LOAD_DEFAULT_CONFIG_DATA = true;
```

#### 2. To load [Default Credentials](examples/SAMD_ESP8266Shield/Credentials.h) when there is no valid Credentials.

```
// Used mostly once debugged. Assumes good data already saved in device.
// Config Portal data input will be override DEFAULT_CONFIG_DATA
bool LOAD_DEFAULT_CONFIG_DATA = false;
```

#### 3. Example of [Default Credentials](examples/SAMD_ESP8266Shield/Credentials.h)

```cpp
/// Start Default Config Data //////////////////

/*
#define SSID_MAX_LEN      32
//From v1.0.3, WPA2 passwords can be up to 63 characters long.
#define PASS_MAX_LEN      64

typedef struct
{
  char wifi_ssid[SSID_MAX_LEN];
  char wifi_pw  [PASS_MAX_LEN];
}  WiFi_Credentials;

#define NUM_WIFI_CREDENTIALS      2

// Configurable items besides fixed Header, just add board_name 
#define NUM_CONFIGURABLE_ITEMS    ( ( 2 * NUM_WIFI_CREDENTIALS ) + 1 )
////////////////

typedef struct Configuration
{
  char header         [16];
  WiFi_Credentials  WiFi_Creds  [NUM_WIFI_CREDENTIALS];
  char board_name     [24];
  int  checkSum;
} ESP8266_AT_Configuration;
*/

#define TO_LOAD_DEFAULT_CONFIG_DATA      true

#if TO_LOAD_DEFAULT_CONFIG_DATA

// This feature is primarily used in development to force a known set of values as Config Data
// It will NOT force the Config Portal to activate. Use DRD or erase Config Data with Blynk.clearConfigData()

// Used mostly for development and debugging. FORCES default values to be loaded each run.
// Config Portal data input will be ignored and overridden by DEFAULT_CONFIG_DATA
//bool LOAD_DEFAULT_CONFIG_DATA = true;

// Used mostly once debugged. Assumes good data already saved in device.
// Config Portal data input will override DEFAULT_CONFIG_DATA
bool LOAD_DEFAULT_CONFIG_DATA = false;

ESP8266_AT_Configuration defaultConfig =
{
  //char header[16], dummy, not used
  "SHD_ESP8266",
  // WiFi_Credentials  WiFi_Creds  [NUM_WIFI_CREDENTIALS];
  // WiFi_Credentials.wifi_ssid and WiFi_Credentials.wifi_pw
  "SSID1",  "password1",
  "SSID2",  "password2",
  //char board_name     [24];
  "SAMD-ESP_AT",
  // terminate the list
  //int  checkSum, dummy, not used
  0
  /////////// End Default Config Data /////////////
};

#else

bool LOAD_DEFAULT_CONFIG_DATA = false;

ESP8266_AT_Configuration defaultConfig;

#endif    // TO_LOAD_DEFAULT_CONFIG_DATA

/////////// End Default Config Data /////////////
```

---

### How to add dynamic parameters from sketch

Example of [Default dynamicParams](examples/SAMD_ESP8266Shield/dynamicParams.h)

- To add custom parameters, just modify the example below

```
#ifndef dynamicParams_h
#define dynamicParams_h

#include "defines.h"

// USE_DYNAMIC_PARAMETERS defined in defined.h
//#define USE_DYNAMIC_PARAMETERS      true

/////////////// Start dynamic Credentials ///////////////

//Defined in <Esp8266_AT_WM_Lite_SAMD.h>
/**************************************
  #define MAX_ID_LEN                5
  #define MAX_DISPLAY_NAME_LEN      16

  typedef struct
  {
  char id             [MAX_ID_LEN + 1];
  char displayName    [MAX_DISPLAY_NAME_LEN + 1];
  char *pdata;
  uint8_t maxlen;
  } MenuItem;
**************************************/

#if USE_DYNAMIC_PARAMETERS

#define MAX_MQTT_SERVER_LEN      34
char MQTT_Server  [MAX_MQTT_SERVER_LEN + 1]   = "mqtt-server";

#define MAX_MQTT_PORT_LEN        6
char MQTT_Port   [MAX_MQTT_PORT_LEN + 1]  = "1883";


MenuItem myMenuItems [] =
{
  { "mqtt", "MQTT Server",      MQTT_Server,      MAX_MQTT_SERVER_LEN },
  { "mqpt", "Port",             MQTT_Port,        MAX_MQTT_PORT_LEN   },
};

// Due to notorious 2K buffer limitation of ESO8266-AT shield, the NUM_MENU_ITEMS is limited to max 3
// to avoid WebServer not working due to HTML data larger than 2K can't be sent successfully
// The items with index larger than 3 will be ignored

uint16_t NUM_MENU_ITEMS = min( 3, sizeof(myMenuItems) / sizeof(MenuItem));  //MenuItemSize;

#else

MenuItem myMenuItems [] = {};

uint16_t NUM_MENU_ITEMS = 0;
#endif

/////// // End dynamic Credentials ///////////


#endif      //dynamicParams_h

```
- If you don't need to add dynamic parameters, use the following in sketch

```
#define USE_DYNAMIC_PARAMETERS     false
```

or

```
/////////////// Start dynamic Credentials ///////////////

MenuItem myMenuItems [] = {};

uint16_t NUM_MENU_ITEMS = 0;
/////// // End dynamic Credentials ///////////

```

---
---

### Important Notes for using Dynamic Parameters' ids

1. These ids (such as "mqtt" in example) must be **unique**.

Please be noted that the following **reserved names are already used in library**:

```
"id"    for WiFi SSID
"pw"    for WiFi PW
"id1"   for WiFi1 SSID
"pw1"   for WiFi1 PW
"nm"    for Board Name
```

---
---

### Example [nRF52_ESP8266Shield](examples/nRF52_ESP8266Shield)


#### 1. File [nRF52_ESP8266Shield.ino](examples/nRF52_ESP8266Shield/nRF52_ESP8266Shield.ino)

https://github.com/khoih-prog/ESP_AT_WM_Lite/blob/83964d54bd43fce6d8a7167b08e45d506ced8d27/examples/nRF52_ESP8266Shield/nRF52_ESP8266Shield.ino#L13-L130



#### 2. File [defines.h](examples/nRF52_ESP8266Shield/defines.h)

https://github.com/khoih-prog/ESP_AT_WM_Lite/blob/83964d54bd43fce6d8a7167b08e45d506ced8d27/examples/nRF52_ESP8266Shield/defines.h#L13-L149



#### 3. File [Credentials.h](examples/nRF52_ESP8266Shield/Credentials.h)

https://github.com/khoih-prog/ESP_AT_WM_Lite/blob/83964d54bd43fce6d8a7167b08e45d506ced8d27/examples/nRF52_ESP8266Shield/Credentials.h#L13-L89


#### 4. File [dynamicParams.h](examples/nRF52_ESP8266Shield/dynamicParams.h)


https://github.com/khoih-prog/ESP_AT_WM_Lite/blob/83964d54bd43fce6d8a7167b08e45d506ced8d27/examples/nRF52_ESP8266Shield/dynamicParams.h#L13-L68



---
---


### Debug Terminal output Samples

#### 1. nRF52_ESP8266Shield on Adafruit ItsyBitsy NRF52840 Feather Express

This is the terminal output when running [nRF52_ESP8266Shield](examples/nRF52_ESP8266Shield) example on **Adafruit ItsyBitsy NRF52840 Express** and DRD is detected:

#### 1.1 Open Config Portal

```
Start nRF52_ESP8266Shield on NRF52840_FEATHER
ESP_AT_WM_Lite v1.5.2
Debug Level = 3
[ESP_AT] Use ES8266-AT Command
LittleFS Flag read = 0xd0d01234
Flag read = 0xd0d01234
doubleResetDetected
Saving to DRD file : 0xd0d04321
Saving DRD file OK
LittleFS Flag read = 0xd0d04321
ClearFlag write = 0xd0d04321
[ESP_AT] LoadCfgFile 
[ESP_AT] OK
[ESP_AT] CCSum=0x1327,RCSum=0x1327
[ESP_AT] Valid Stored Dynamic Data
[ESP_AT] ======= Start Stored Config Data =======
[ESP_AT] b:StayInCfgPortal:DRD
[ESP_AT] Scanning Network
[ESP_AT] scanWifiNetworks: Done, Scanned Networks n = 8
[ESP_AT] WiFi networks found:
[ESP_AT] 1: dragino-1ed63c, -25dB
[ESP_AT] 2: HueNet, -27dB
[ESP_AT] 3: HueNetTek, -90dB
[ESP_AT] 4: HueNet1, -54dB
[ESP_AT] 5: HueNet2, -70dB
[ESP_AT] 6: bacau, -70dB
[ESP_AT] 7: guest_24, -25dB
[ESP_AT] 8: FishBowl, -13dB
[ESP_AT] SSID=CfgPrtl-SSID,PW=CfgPrtl-PW
[ESP_AT] IP=192.168.4.1,CH=1
C
Stored Dynamic Params:
MQTT Server = mqtt-server
Port = 1883
CCCC
```

#### 1.2 Got valid Credential from Config Portal, then connected to WiFi

```
Start nRF52_ESP8266Shield on NRF52840_FEATHER
ESP_AT_WM_Lite v1.5.2
Debug Level = 3
[ESP_AT] Use ES8266-AT Command
LittleFS Flag read = 0xd0d04321
Flag read = 0xd0d04321
No doubleResetDetected
Saving DOUBLERESETDETECTOR_FLAG to DRD file : 0xd0d01234
Saving DRD file OK
SetFlag write = 0xd0d01234
[ESP_AT] LoadCfgFile 
[ESP_AT] OK
[ESP_AT] CCSum=0x1327,RCSum=0x1327
[ESP_AT] Valid Stored Dynamic Data
[ESP_AT] ======= Start Stored Config Data =======
[ESP_AT] ConMultiWifi
[ESP_AT] First connection, Using index=0
[ESP_AT] con2WF:SSID=HueNet1,PW=password
[ESP_AT] Remaining retry_time=3
[ESP_AT] WOK, lastConnectedIndex=0
[ESP_AT] con2WF:OK
Stop doubleResetDetecting
Saving to DRD file : 0xd0d04321
Saving DRD file OK
LittleFS Flag read = 0xd0d04321
ClearFlag write = 0xd0d04321
H
Stored Dynamic Params:
MQTT Server = mqtt-server
Port = 1883
HHHHHHHHH HHHHHHHHHH HHHHHHHHHH HHHHHHHHHH 
```

#### 1.3 Lost a WiFi and autoconnect to another WiFi AP

```
Start nRF52_ESP8266Shield on NRF52840_FEATHER
ESP_AT_WM_Lite v1.5.2
Debug Level = 3
[ESP_AT] Use ES8266-AT Command
LittleFS Flag read = 0xd0d04321
Flag read = 0xd0d04321
No doubleResetDetected
Saving DOUBLERESETDETECTOR_FLAG to DRD file : 0xd0d01234
Saving DRD file OK
SetFlag write = 0xd0d01234
[ESP_AT] CCSum=0x12c9,RCSum=0x12c9
[ESP_AT] Valid Stored Dynamic Data
[ESP_AT] ======= Start Stored Config Data =======
[ESP_AT] ConMultiWifi
[ESP_AT] First connection, Using index=0
[ESP_AT] con2WF:SSID=HueNet1,PW=password
[ESP_AT] Remaining retry_time=3
[ESP_AT] WOK, lastConnectedIndex=0
[ESP_AT] con2WF:OK
Stop doubleResetDetecting
ClearFlag write = 0xd0d04321
H
Stored Dynamic Params:
MQTT Server = mqtt-server
Port = 1883
HH[ESP_AT] ConMultiWifi                 <== Lost Primary WiFi AP
[ESP_AT] Using index=1, lastConnectedIndex=0
[ESP_AT] con2WF:SSID=HueNet2,PW=password
[ESP_AT] Remaining retry_time=2
[ESP_AT] WOK, lastConnectedIndex=1     <== Reconnect to Secondary WiFi AP
[ESP_AT] con2WF:OK
HHHHH
```

---


#### 2. SAMD_ESP8266Shield on Adafruit SAMD51 ITSYBITSY_M4

This is the terminal output when running [SAMD_ESP8266Shield](examples/SAMD_ESP8266Shield) example on **Adafruit SAMD51 ItsyBitsy_M4** :

#### 2.1 Open Config Portal

```
Start SAMD_ESP8266Shield on ITSYBITSY_M4
ESP_AT_WM_Lite v1.5.2
Debug Level = 3
[ESP_AT] Use ES8266-AT Command
Flag read = 0xffffffff
No doubleResetDetected
SetFlag write = 0xd0d01234
[ESP_AT] CCSum=0x72de,RCSum=0x10200754
[ESP_AT] Invalid Stored Dynamic Data. Load default from Sketch
[ESP_AT] SaveEEPROM,sz=1024,Datasz=520,CSum=4156
[ESP_AT] b:StayInCfgPortal:NoCfgDat
[ESP_AT] Scanning Network
[ESP_AT] scanWifiNetworks: Done, Scanned Networks n = 9
[ESP_AT] WiFi networks found:
[ESP_AT] 1: HueNet1, -30dB
[ESP_AT] 2: HueNet, -88dB
[ESP_AT] 3: HueNetTek, -91dB
[ESP_AT] 4: dragino-1ed63c, -24dB
[ESP_AT] 5: HueNet2, -50dB
[ESP_AT] 6: Rogers 786, -26dB
[ESP_AT] 7: Access 2.0, -41dB
[ESP_AT] 8: VIRGIN874, -78dB
[ESP_AT] 9: Jasmine, -90dB
[ESP_AT] SSID=SAMD-CfgPrtl-SSID,PW=SAMD-CfgPrtl-PW
[ESP_AT] IP=192.168.4.1,CH=6
Stop doubleResetDetecting
ClearFlag write = 0xd0d04321
C
Stored Dynamic Params:
MQTT Server = mqtt-server
Port = 1883

FFFF[ESP_AT] h:UpdFlash
[ESP_AT] SaveEEPROM,sz=1024,Datasz=520,CSum=4809
[ESP_AT] h:Rst
```

#### 2.2 Got valid Credential from Config Portal, then connected to WiFi

```
Start SAMD_ESP8266Shield on ITSYBITSY_M4
ESP_AT_WM_Lite v1.5.2
Debug Level = 3
[ESP_AT] Use ES8266-AT Command
Flag read = 0xd0d04321
No doubleResetDetected
SetFlag write = 0xd0d01234
[ESP_AT] CCSum=0x12c9,RCSum=0x12c9
[ESP_AT] Valid Stored Dynamic Data
[ESP_AT] ======= Start Stored Config Data =======
[ESP_AT] ConMultiWifi
[ESP_AT] First connection, Using index=0
[ESP_AT] con2WF:SSID=HueNet1,PW=password
[ESP_AT] Remaining retry_time=3
[ESP_AT] WOK, lastConnectedIndex=0
[ESP_AT] con2WF:OK
Stop doubleResetDetecting
ClearFlag write = 0xd0d04321
H
Stored Dynamic Params:
MQTT Server = mqtt-server
Port = 1883
HHHHHHHHH HHHHHHHHHH HHHHHHHHHH HHHHHHHHHH 
```

#### 2.3 Lost a WiFi and autoconnect to another WiFi AP

```
Start SAMD_ESP8266Shield on ITSYBITSY_M4
ESP_AT_WM_Lite v1.5.2
Debug Level = 3
[ESP_AT] Use ES8266-AT Command
Flag read = 0xd0d04321
No doubleResetDetected
SetFlag write = 0xd0d01234
[ESP_AT] CCSum=0x12c9,RCSum=0x12c9
[ESP_AT] Valid Stored Dynamic Data
[ESP_AT] ======= Start Stored Config Data =======
[ESP_AT] ConMultiWifi
[ESP_AT] First connection, Using index=0
[ESP_AT] con2WF:SSID=HueNet1,PW=password
[ESP_AT] Remaining retry_time=3
[ESP_AT] WOK, lastConnectedIndex=0
[ESP_AT] con2WF:OK
Stop doubleResetDetecting
ClearFlag write = 0xd0d04321
H
Stored Dynamic Params:
MQTT Server = mqtt-server
Port = 1883
HH[ESP_AT] ConMultiWifi                 <== Lost Primary WiFi AP
[ESP_AT] Using index=1, lastConnectedIndex=0
[ESP_AT] con2WF:SSID=HueNet2,PW=password
[ESP_AT] Remaining retry_time=2
[ESP_AT] WOK, lastConnectedIndex=1     <== Reconnect to Secondary WiFi AP
[ESP_AT] con2WF:OK
HHHHH
```

---

#### 3. RPi_Pico_ESP8266Shield on RASPBERRY_PI_PICO

This is the terminal output when running [RPi_Pico_ESP8266Shield](examples/RPi_Pico_ESP8266Shield) example on **RASPBERRY_PI_PICO** :

#### 3.1 Open Config Portal

```
Start RPi_Pico_ESP8266Shield on RASPBERRY_PI_PICO
ESP_AT_WM_Lite v1.5.2
Debug Level = 3
[ESP_AT] Use ES8266-AT Command
LittleFS Flag read = 0xd0d04321
Flag read = 0xd0d04321
No doubleResetDetected
Saving DOUBLERESETDETECTOR_FLAG to DRD file : 0xd0d01234
Saving DRD file OK
SetFlag write = 0xd0d01234
[ESP_AT] LoadCfgFile 
[ESP_AT] OK
[ESP_AT] Invalid Stored WiFi Config Data.
[ESP_AT] SSID is blank or NULL
[ESP_AT] b:StayInCfgPortal:NoCfgDat
[ESP_AT] Scanning Network
[ESP_AT] scanWifiNetworks: Done, Scanned Networks n = 9
[ESP_AT] WiFi networks found:
[ESP_AT] 1: HueNetTek, -35dB
[ESP_AT] 2: HueNet1, -85dB
[ESP_AT] 3: , -83dB
[ESP_AT] 4: Waterhome, -82dB
[ESP_AT] 5: dlink-4F96, -77dB
[ESP_AT] 8: BELL627, -47dB
[ESP_AT] 9: Primus-f6b1, -82dB
[ESP_AT] SSID=CfgPrtl-SSID,PW=CfgPrtl-PW
[ESP_AT] IP=192.168.4.1,CH=1
Stop doubleResetDetecting
Saving to DRD file : 0xd0d04321
Saving DRD file OK
LittleFS Flag read = 0xd0d04321
ClearFlag write = 0xd0d04321
C
Stored Dynamic Params:
MQTT Server = mqtt-server
Port = 1883
CCCCCCCCC CCCC
```

#### 3.2 Got valid Credential from Config Portal, then connected to WiFi

```
Start RPi_Pico_ESP8266Shield on RASPBERRY_PI_PICO
ESP_AT_WM_Lite v1.5.2
Debug Level = 4
[ESP_AT] Use ES8266-AT Command
LittleFS Flag read = 0xd0d04321
Flag read = 0xd0d04321
No doubleResetDetected
Saving DOUBLERESETDETECTOR_FLAG to DRD file : 0xd0d01234
Saving DRD file OK
SetFlag write = 0xd0d01234
[ESP_AT] ======= Start Default Config Data =======
[ESP_AT] Hdr=SHD_ESP8266,SSID=SSID1,PW=password1
[ESP_AT] SSID1=SSID2,PW1=password2
[ESP_AT] BName=nRF52-ESP_AT
[ESP_AT] i=0,id=mqtt,data=mqtt-server
[ESP_AT] i=1,id=mqpt,data=1883
[ESP_AT] LoadCfgFile 
[ESP_AT] OK
[ESP_AT] LoadCredFile 
[ESP_AT] ChkCrR: Buffer allocated, Sz=35
[ESP_AT] ChkCrR:pdata=mqtt-server,len=34
[ESP_AT] ChkCrR:pdata=1883,len=6
[ESP_AT] OK
[ESP_AT] CrCCsum=0x55e,CrRCsum=0x55e
[ESP_AT] Buffer freed
[ESP_AT] CCSum=0x1186,RCSum=0x1186
[ESP_AT] LoadCredFile 
[ESP_AT] CrR:pdata=mqtt-server,len=34
[ESP_AT] CrR:pdata=1883,len=6
[ESP_AT] OK
[ESP_AT] CrCCsum=0x55e,CrRCsum=0x55e
[ESP_AT] Valid Stored Dynamic Data
[ESP_AT] ======= Start Stored Config Data =======
[ESP_AT] Hdr=SHD_ESP8266,SSID=HueNet1,PW=password
[ESP_AT] SSID1=HueNet2,PW1=password
[ESP_AT] BName=RPi-Pico
[ESP_AT] i=0,id=mqtt,data=mqtt-server
[ESP_AT] i=1,id=mqpt,data=1883
[ESP_AT] ConMultiWifi
[ESP_AT] First connection, Using index=0
[ESP_AT] con2WF:SSID=HueNet1,PW=password
[ESP_AT] Remaining retry_time=3
[ESP_AT] WOK, lastConnectedIndex=0
[ESP_AT] con2WF:OK
[ESP_AT] IP=192.168.2.76
[ESP_AT] b:WOK
Stop doubleResetDetecting
Saving to DRD file : 0xd0d04321
Saving DRD file OK
LittleFS Flag read = 0xd0d04321
ClearFlag write = 0xd0d04321
H
Stored Dynamic Params:
MQTT Server = mqtt-server
Port = 1883
HHHHHHHHH HHHHHHHHHH HHHHHHHHHH HHHHHHHHHH 
```

---

#### 4. RPi_Pico_ESP8266Shield on MBED RASPBERRY_PI_PICO

This is the terminal output when running [RPi_Pico_ESP8266Shield](examples/RPi_Pico_ESP8266Shield) example on **MBED RASPBERRY_PI_PICO** :

#### 4.1 Open Config Portal

```
Start RPi_Pico_ESP8266Shield on MBED RASPBERRY_PI_PICO
ESP_AT_WM_Lite v1.5.2
Debug Level = 3
[ESP_AT] Use ES8266-AT Command
LittleFS size (KB) = 64
LittleFS Mount OK
LittleFS Flag read = 0xd0d04321
Flag read = 0xd0d04321
No doubleResetDetected
Saving DOUBLERESETDETECTOR_FLAG to DRD file : 0xd0d01234
Saving DRD file OK
SetFlag write = 0xd0d01234
[ESP_AT] LoadCfgFile 
[ESP_AT] OK
[ESP_AT] Invalid Stored WiFi Config Data.
[ESP_AT] SSID is blank or NULL
[ESP_AT] b:StayInCfgPortal:NoCfgDat
[ESP_AT] Scanning Network
[ESP_AT] scanWifiNetworks: Done, Scanned Networks n = 9
[ESP_AT] WiFi networks found:
[ESP_AT] 1: HueNetTek, -35dB
[ESP_AT] 2: HueNet1, -85dB
[ESP_AT] 3: , -83dB
[ESP_AT] 4: Waterhome, -82dB
[ESP_AT] 5: dlink-4F96, -77dB
[ESP_AT] 8: BELL627, -47dB
[ESP_AT] 9: Primus-f6b1, -82dB
[ESP_AT] SSID=CfgPrtl-SSID,PW=CfgPrtl-PW
[ESP_AT] IP=192.168.4.1,CH=1
Stop doubleResetDetecting
Saving to DRD file : 0xd0d04321
Saving DRD file OK
LittleFS Flag read = 0xd0d04321
ClearFlag write = 0xd0d04321
C
Stored Dynamic Params:
MQTT Server = mqtt-server
Port = 1883
CCCCCCCCC CCCC
```

#### 4.2 Got valid Credential from Config Portal, then connected to WiFi

```
Start RPi_Pico_ESP8266Shield on MBED RASPBERRY_PI_PICO
ESP_AT_WM_Lite v1.5.2
Debug Level = 4
[ESP_AT] Use ES8266-AT Command
LittleFS size (KB) = 64
LittleFS Mount OK
LittleFS Flag read = 0xd0d04321
Flag read = 0xd0d04321
No doubleResetDetected
Saving DOUBLERESETDETECTOR_FLAG to DRD file : 0xd0d01234
Saving DRD file OK
SetFlag write = 0xd0d01234
[ESP_AT] ======= Start Default Config Data =======
[ESP_AT] Hdr=SHD_ESP8266,SSID=SSID1,PW=password1
[ESP_AT] SSID1=SSID2,PW1=password2
[ESP_AT] BName=nRF52-ESP_AT
[ESP_AT] LoadCfgFile 
[ESP_AT] OK
[ESP_AT] CCSum=0x1170,RCSum=0x1170
[ESP_AT] ======= Start Stored Config Data =======
[ESP_AT] Hdr=SHD_ESP8266,SSID=HueNet1,PW=password
[ESP_AT] SSID1=HueNet2,PW1=password
[ESP_AT] BName=MBED-RP2040
[ESP_AT] ConMultiWifi
[ESP_AT] First connection, Using index=0
[ESP_AT] con2WF:SSID=HueNet1,PW=password
[ESP_AT] Remaining retry_time=3
[ESP_AT] WOK, lastConnectedIndex=0
[ESP_AT] con2WF:OK
[ESP_AT] IP=192.168.2.76
[ESP_AT] b:WOK
Stop doubleResetDetecting
Saving to DRD file : 0xd0d04321
Saving DRD file OK
LittleFS Flag read = 0xd0d04321
ClearFlag write = 0xd0d04321
HHHHHHHHHH HHHHHHHHHH HHHHHHHHHH HH
```

---
---

#### Debug

Debug is enabled by default on Serial. To disable, add at the beginning of sketch

```cpp
/* Comment this out to disable prints and save space */
#define DRD_GENERIC_DEBUG             true

#define USE_NEW_WEBSERVER_VERSION     true  //false
#define _ESP_AT_LOGLEVEL_             1

/* Comment this out to disable prints and save space */
#define ESP_AT_DEBUG_OUTPUT           Serial

#define ESP_AT_DEBUG                  true
#define _ESP_AT_WM_LOGLEVEL_          3
```

---

## Troubleshooting

If you get compilation errors, more often than not, you may need to install a newer version of the board's core, `ESP8266 AT shield` AT-command or this library version.

Sometimes, the library will only work if you update the `ESP8266 AT shield` core to the newer or older version because some function compatibility.

---
---


### Issues ###

Submit issues to: [ESP_AT_WM_Lite issues](https://github.com/khoih-prog/ESP_AT_WM_Lite/issues)

---
---

### TO DO

1. Too many things to list, EEPROM, SPIFFS/FS/FAT FS (if available). Done.
2. Find better and easier way to add more parameters. Done.
3. Add more examples. Done.
4. Add more boards. Done.
5. Add support to RP2040-based boards such as RASPBERRY_PI_PICO, using [**Arduino-mbed RP2040** core](https://github.com/arduino/ArduinoCore-mbed)

### DONE

 1. Adding dynamic custom parameters
 2. Add MultiWiFi and Auto(Re)Connect feature
 3. Add support to AVR, SAM DUE, SAMD21, SAMD51, nRF52, STM32F/L/H/G/WB/MP1, etc.
 4. Many more to list ( WPA2 password length, special chars, etc.)
 5. Add DRD
 6. Add default Credentials
 7. Add Dynamic parameters
 8. Add Configurable Config Portal Title
 9. Split each example into several manageable files.
10. Use more efficient [FlashStorage_SAMD](https://github.com/khoih-prog/FlashStorage_SAMD) and [FlashStorage_STM32](https://github.com/khoih-prog/FlashStorage_STM32)
11. Add Table-of-Contents
12. Permit optionally inputting one set of WiFi SSID/PWD by using `REQUIRE_ONE_SET_SSID_PW == true`
13. Enforce WiFi PWD minimum length of 8 chars
14. Enable **scan of WiFi networks** for selection in Configuration Portal
15. Add support to new STM32 core v2.0.0 and new STM32L5 boards.
16. Add support to RP2040-based boards, such as **RASPBERRY_PI_PICO, ADAFRUIT_FEATHER_RP2040 and GENERIC_RP2040**, using [**Earle Philhower's arduino-pico** core](https://github.com/earlephilhower/arduino-pico).
17. Add support to RP2040-based boards, such as **RASPBERRY_PI_PICO, ADAFRUIT_FEATHER_RP2040 and GENERIC_RP2040**, using [**Arduino-mbed RP2040** core](https://github.com/arduino/ArduinoCore-mbed)
18. Add support to **Arduino Nano RP2040 Connect** using [**Arduino mbed OS for Nano boards**](https://github.com/arduino/ArduinoCore-mbed).
19. Fix the blocking issue in loop() with configurable `WIFI_RECON_INTERVAL`
20. Update to be compatible with new `FlashStorage_SAMD`
21. Use better `FlashStorage_STM32` or `FlashStorage_STM32F1` library for STM32
22. Add support to generic SAMD21 boards : `__SAMD21E1xA__`, `__SAMD21G1xA__` and `__SAMD21J1xA__`
23. Optimize code by passing by `reference` instead of `value`
24. Optional `Board_Name` in Config Portal
25. Add function `isConfigMode()` to signal system is in Config Portal mode

---
---

### Contributions and thanks

Please help contribute to this project and add your name here.

1. Thanks to [Michael H. "bizprof"](https://github.com/bizprof) with the impressive new feature : 
  - `Enable scan of WiFi networks for selection in Configuration Portal`. Check [PR for v1.3.0 - Enable scan of WiFi networks #10](https://github.com/khoih-prog/WiFiManager_NINA_Lite/pull/10) leading to v1.2.0

<table>
  <tr>
    <td align="center"><a href="https://github.com/bizprof"><img src="https://github.com/bizprof.png" width="100px;" alt="bizprof"/><br /><sub><b>⭐️ Michael H. "bizprof"</b></sub></a><br /></td>
  </tr> 
</table>

---

### Contributing

If you want to contribute to this project:
- Report bugs and errors
- Ask for enhancements
- Create issues and pull requests
- Tell other people about this library

---

### License

- The library is licensed under [MIT](https://github.com/khoih-prog/ESP_AT_WM_Lite/blob/master/LICENSE)

---

### Copyright

Copyright 2020- Khoi Hoang


