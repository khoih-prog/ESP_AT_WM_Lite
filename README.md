# ESP_AT_WM_Lite (Light Weight Credentials / WiFi Manager for ESP8266 AT shields)

[![arduino-library-badge](https://www.ardu-badge.com/badge/ESP_AT_WM_Lite.svg?)](https://www.ardu-badge.com/ESP_AT_WM_Lite)

This library is a Light Weight Credentials / WiFi Manager for ESP8266 AT shields, specially designed to support ***Teensy, SAM DUE, SAMD, STM32, etc. boards running ESP8266 AT-command shields.*** with smaller memory (64+K bytes)

The AVR-family boards (UNO, Nano, etc.) are ***not supported*** as they don't have enough memory to run Config Portal WebServer.

This is a Credentials / WiFi Connection Manager with fallback web configuration portal.

The web configuration portal, served from the `ESP8266 AT-command shields` is operating as an access point (AP) with configurable static IP address or use default IP Address of 192.168.4.1

The configuration portal is captive, so it will present the configuration dialogue regardless of the web address selected, excluding https requests.

## Prerequisite
1. [`Arduino IDE 1.8.12 or later` for Arduino](https://www.arduino.cc/en/Main/Software)
2. [`Arduino Core for STM32 v1.8.0 or later`](https://github.com/khoih-prog/Arduino_Core_STM32) for STM32 boards
3. [`Teensy core 1.51 or later`](https://www.pjrc.com/teensy/td_download.html) for Teensy (4.0, 3.6, 3.5, 3,2, 3.1, 3.0) boards
4. [`ESP8266_AT_WebServer library`](https://github.com/khoih-prog/ESP8266_AT_WebServer)
5. [`FlashStorage library`](https://github.com/khoih-prog/FlashStorage) for SAMD boards (ZERO, MKR, NANO_33_IOT, M0, M0 Pro, AdaFruit CIRCUITPLAYGROUND_EXPRESS, etc.)
6. [`DueFlashStorage library`](https://github.com/sebnil/DueFlashStorage) for SAM DUE


## How It Works

- The [Mega_ESP8266Shield](examples/Mega_ESP8266Shield) example shows how it works and should be used as the basis for a sketch that uses this library.
- The concept of [Mega_ESP8266Shield](examples/Mega_ESP8266Shield) is that a new `ESP8266 AT shield` will start a WiFi configuration portal when powered up and has no stored Credentials. It will then save the configuration data in host's non-volatile memory. If there is valid stored Credentials, it'll go directly to connect to WiFi without starting / using the Config Portal.
- Using any WiFi enabled device with a browser (computer, phone, tablet) connect to the newly created AP and type in the AP IP address (default 192.168.4.1).
- Input SSID and Password of the APs to be connected, thn click `Save`.
- `ESP8266 AT shield` will try to connect. If successful, the dynamic DHCP or configured static IP address will be displayed in the configuration portal. 
- The `ESP8266 AT shield` WiFi Config Portal network and Web Server will shutdown to return control to the sketch code.

## Quick Start

The best and easiest way is to use `Arduino Library Manager`. Search for `ESP_AT_WM_Lite`, then select / install the latest version.
You can also use this link [![arduino-library-badge](https://www.ardu-badge.com/badge/ESP_AT_WM_Lite.svg?)](https://www.ardu-badge.com/ESP_AT_WM_Lite) for more detailed instructions.

### Manual Install

Another way to install is to:

1. Navigate to [ESP_AT_WM_Lite](https://github.com/khoih-prog/ESP_AT_WM_Lite) page.
2. Download the latest release `ESP_AT_WM_Lite-master.zip`.
3. Extract the zip file to `ESP_AT_WM_Lite-master` directory 
4. Copy whole `ESP_AT_WM_Lite-master/src` folder to Arduino libraries' directory such as `~/Arduino/libraries/`.

### Using
- Include in your sketch
```cpp

// Select depending on board
#define EspSerial Serial1

// Must be before #include <Esp8266_AT_WM_Lite.h>
// Start location in EEPROM to store config data. Default 0
// Config data Size currently is 84 bytes)
#define EEPROM_START      (64)

#include <Esp8266_AT_WM_Lite.h>

ESP_AT_WiFiManager_Lite* ESP_AT_WiFiManager; 

// Your Mega <-> ESP8266 baud rate:
#define ESP8266_BAUD 115200
```

- When you want to open a config portal, just add

```cpp
ESP_AT_WiFiManager = new ESP_AT_WiFiManager_Lite(&EspSerial, ESP8266_BAUD);
ESP_AT_WiFiManager->begin();
```

- To not use default AP WiFi Channel 10 to avoid conflict with other WiFi APs, call 
```cpp
ESP_AT_WiFiManager->setConfigPortalChannel(newChannel);
```

- To use different static AP IP (not use default `192.168.4.1`), call 
```cpp
ESP_AT_WiFiManager->setConfigPortalIP(IPAddress(xxx,xxx,xxx,xxx));
```

While in AP mode, connect to it using its `SSID` (ESP_AT_XXXXXX) / `Password` ("MyESP_AT_XXXXXX"), then open a browser to the Portal AP IP, default `192.168.4.1`, configure wifi then save. The Credentials / WiFi connection information will be saved in non-volatile memory. It will then autoconnect.


OnceCredentials / WiFi network information is saved in the host non-volatile memory, it will try to autoconnect to WiFi every time it is started, without requiring any function calls in the sketch.


Also see examples: 
1. [Mega_ESP8266Shield](examples/Mega_ESP8266Shield)
2. [Teensy40_ESP8266Shield](examples/Teensy40_ESP8266Shield)
3. [SAMD_ESP8266Shield](examples/SAMD_ESP8266Shield)
4. [SAM_DUE_ESP8266Shield](examples/SAM_DUE_ESP8266Shield)
5. [STM32_ESP8266Shield](examples/STM32_ESP8266Shield)

## So, how it works?
In `Configuration Portal Mode`, it starts an AP called `ESP_AT_XXXXXX`. Connect to it using the `configurable password` you can define in the code. For example, `MyESP_AT_XXXXXX` (see examples):

```cpp
// SSID and PW for Config Portal
String ssid = "ESP_AT_" + String(0x1ABCDEF, HEX);
const char* password = "ESP_AT_PW";
```
After you connected, please, go to http://192.168.4.1 or newly configured AP IP, you'll see this `Main` page:

<p align="center">
    <img src="https://github.com/khoih-prog/ESP_AT_WM_Lite/blob/master/Images/Main.png">
</p>

Enter your credentials, 

<p align="center">
    <img src="https://github.com/khoih-prog/ESP_AT_WM_Lite/blob/master/Images/Input.png">
</p>

then click `Save`. 

<p align="center">
    <img src="https://github.com/khoih-prog/ESP_AT_WM_Lite/blob/master/Images/Save.png">
</p>

The WiFi Credentials will be saved and the board connect to the selected WiFi AP.

If you're already connected to a listed WiFi AP and don't want to change anything, just select `Exit` from the `Main` page to reboot the board and connect to the previously-stored AP. The WiFi Credentials are still intact.


#### On Demand Configuration Portal

Sample Code

```cpp
/* Comment this out to disable prints and save space */
#define ESP_AT_DEBUG_OUTPUT Serial

#define ESP_AT_DEBUG    true

#if ( defined(ESP8266) || defined(ESP32) || defined(ARDUINO_AVR_MEGA) || defined(ARDUINO_AVR_MEGA2560) || !defined(CORE_TEENSY) )
#error This code is intended to run on Teensy platform! Please check your Tools->Board setting.
#endif

#ifdef CORE_TEENSY
// For Teensy 4.0
#define EspSerial Serial2   //Serial2, Pin RX2 : 7, TX2 : 8
#if defined(__IMXRT1062__)
#define BOARD_TYPE      "TEENSY 4.0"
#else
#define BOARD_TYPE      BLYNK_INFO_DEVICE
#endif

#else
// For Other Boards
#define EspSerial Serial3
#define BOARD_TYPE      "Unknown Board"
#endif

// Start location in EEPROM to store config data. Default 0
// Config data Size currently is 128 bytes)
#define EEPROM_START     0

#include <Esp8266_AT_WM_Lite_Teensy.h>

// Your Teensy <-> ESP8266 baud rate:
#define ESP8266_BAUD 115200

void heartBeatPrint(void)
{
  static int num = 1;

  if (WiFi.status() == WL_CONNECTED)
    Serial.print("H");        // H means connected to WiFi
  else
    Serial.print("F");        // F means not connected to WiFi

  if (num == 80)
  {
    Serial.println();
    num = 1;
  }
  else if (num++ % 10 == 0)
  {
    Serial.print(" ");
  }
}

void check_status()
{
  static unsigned long checkstatus_timeout = 0;

  //KH
#define HEARTBEAT_INTERVAL    10000L
  // Print hearbeat every HEARTBEAT_INTERVAL (10) seconds.
  if ((millis() > checkstatus_timeout) || (checkstatus_timeout == 0))
  {
    heartBeatPrint();
    checkstatus_timeout = millis() + HEARTBEAT_INTERVAL;
  }
}

ESP_AT_WiFiManager_Lite* ESP_AT_WiFiManager;    //(&EspSerial, ESP8266_BAUD);

void setup()
{
  // Debug console
  Serial.begin(115200);
  delay(1000);

  Serial.println("\nStart Teensy_ESP8266Shield on " + String(BOARD_TYPE));

  // initialize serial for ESP module
  EspSerial.begin(115200);

  ESP_AT_WiFiManager = new ESP_AT_WiFiManager_Lite(&EspSerial, ESP8266_BAUD);

  // Optional to change default AP IP(192.168.4.1) and channel(10)
  //ESP_AT_WiFiManager->setConfigPortalIP(IPAddress(192, 168, 120, 1));
  //ESP_AT_WiFiManager->setConfigPortalChannel(1);

  ESP_AT_WiFiManager->begin();
}

void loop()
{
  ESP_AT_WiFiManager->run();
  check_status();
}
```

This is the terminal output when running [Teensy_ESP8266Shield](examples/Teensy_ESP8266Shield) example on ***Teensy 4.0***:

1. Open Config Portal

```
Start Teensy_ESP8266Shield on TEENSY 4.0
*AT: CCsum=5543,RCsum=1127042391
*AT: Init EEPROM sz=1080
*AT: Open Portal
*AT: SSID=ESP_AT_CCE61, PW=MyESP_AT_CCE61
*AT: IP=192.168.4.1, CH=10
FFF
```

2. Got valid Credential from onfig Portal, then connected to WiFi

```
Start Teensy_ESP8266Shield on TEENSY 4.0
*AT: CCsum=2271,RCsum=2271
*AT: Hdr=SHD_ESP8266, SSID=HueNet1, PW=****
*AT: con2WF:start
*AT: con2WF:spent millis=0
*AT: Con2 HueNet1
*AT: IP=192.168.2.82
*AT: WiFi OK
*AT: con2WF:OK
*AT: IP=192.168.2.82
*AT: WiFi OK
HHHHHHHHHH HHHHHHHHHH HHHHHHHHHH HHHHHHHHHH HHHHHHHHHH HHHHHHHHHH HHHHHHHHHH HHHHHHHHHH
HHHHHHHHHH HHHHHHHHHH HHHHHHHHHH HHHHHHHHHH HHHHHHHHHH HHHHHHHHHH HHHHHHHHHH HHHHHHHHHH

```

#### Debug
Debug is enabled by default on Serial. To disable, add at the beginning of sketch

```cpp
/* Comment this out to disable prints and save space */
#define ESP_AT_DEBUG_OUTPUT Serial

#define ESP_AT_DEBUG    false
```

## Troubleshooting
If you get compilation errors, more often than not, you may need to install a newer version of the board's core, `ESP8266 AT shield` AT-command or this library version.

Sometimes, the library will only work if you update the `ESP8266 AT shield` core to the newer or older version because some function compatibility.

### TO DO

1. Too many things to list, EEPROM, SPIFFS/FS/FAT FS (if available)
2. Find better and easier way to add more parameters.
3. Add more examples 

#### New in v1.0.0

- This is a Light-Weight Credentials / WiFi Connection Manager with fallback web configuration portal. Completely new to support ***Teensy, SAM DUE, SAMD, STM32, etc. boards running ESP8266 AT-command shields.*** with small memory (64+K bytes)


### Contributing

If you want to contribute to this project:
- Report bugs and errors
- Ask for enhancements
- Create issues and pull requests
- Tell other people about this library

### Copyright

Copyright 2020- Khoi Hoang


