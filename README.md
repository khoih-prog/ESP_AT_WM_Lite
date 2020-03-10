## Blynk_Esp8266AT_WM

[![arduino-library-badge](https://www.ardu-badge.com/badge/Blynk_Esp8266AT_WM.svg?)](https://www.ardu-badge.com/Blynk_Esp8266AT_WM)

### New Version v1.0.3

1. Add support to STM32 (STM32F1, F2, F3, F4, F7, etc) boards
2. Add clearConfigData() function.

### New Version v1.0.2

1. Add support to SAMD (DUE, ZERO, MKR, NANO_33_IOT, M0, M0 Pro, AdaFruit CIRCUITPLAYGROUND_EXPRESS, etc.) boards

To help you to eliminate `hardcoding` your Wifi and Blynk credentials for Mega/Teensy boards running ESP8266 AT shields, and updating/reflashing every time when you need to change them. Configuration data are saved in configurable locatioon in EEPROM.

With version `v1.0.0` or later, you now can configure:

1. `Config Portal Static IP address, Name and Password.`
2. `Static IP address, Gateway, Subnet Mask and 2 DNS Servers IP addresses.`

## Prerequisite
1. [`Arduino IDE 1.8.12 or later` for Arduino](https://www.arduino.cc/en/Main/Software)
2. [`Blynk library 0.6.1 or later`](https://www.arduino.cc/en/guide/libraries#toc3)
3. [`ESP8266_AT_WebServer library`](https://github.com/khoih-prog/ESP8266_AT_WebServer)
4. `Arduino AVR core 1.8.2 or later` for AVR boards (Use Arduino Board Manager)
5. [`Teensy core 1.51 or later`](https://www.pjrc.com/teensy/td_download.html) for Teensy boards
6. [`Arduino Core for STM32 v1.8.0 or later`](https://github.com/stm32duino/Arduino_Core_STM32) for STM32 boards
7. [`FlashStorage library`](https://github.com/khoih-prog/FlashStorage) for SAMD boards

## Installation

### Use Arduino Library Manager
The best and easiest way is to use `Arduino Library Manager`. Search for `Blynk_Esp8266AT_WM`, then select / install the latest version.
You can also use this link [![arduino-library-badge](https://www.ardu-badge.com/badge/Blynk_Esp8266AT_WM.svg?)](https://www.ardu-badge.com/Blynk_Esp8266AT_WM) for more detailed instructions.

### Manual Install

Another way to install is to:

1. Navigate to [Blynk_Esp8266AT_WM](https://github.com/khoih-prog/Blynk_Esp8266AT_WM) page.
2. Download the latest release `Blynk_Esp8266AT_WM-master.zip`.
3. Extract the zip file to `Blynk_Esp8266AT_WM-master` directory 
4. Copy whole `Blynk_Esp8266AT_WM-master/src` folder to Arduino libraries' directory such as `~/Arduino/libraries/`.

### How to use

In your code, to use WiFiManager Blynk features, replace
1. `BlynkSimpleShieldEsp8266.h` with `BlynkSimpleShieldEsp8266_WM.h` for Mega boards.
2. `BlynkSimpleShieldEsp8266.h` with `BlynkSimpleShieldEsp8266_Teensy_WM.h`  for Teensy boards.
3. `BlynkSimpleShieldEsp8266.h` with `BlynkSimpleShieldEsp8266_SAMD_WM.h`  for SAMD boards.
4. `BlynkSimpleShieldEsp8266.h` with `BlynkSimpleShieldEsp8266_STM32_WM.h`  for STM32 boards.

to use Blynk only, with hardcoded Credentials, replace
1. `BlynkSimpleShieldEsp8266.h` with `BlynkSimpleShieldEsp8266_Teensy.h`  for Teensy boards.
2. `BlynkSimpleShieldEsp8266.h` with `BlynkSimpleShieldEsp8266_SAMD.h`  for SAMD boards.
3. `BlynkSimpleShieldEsp8266.h` with `BlynkSimpleShieldEsp8266_STM32.h`  for STM32 boards.
4. Keep the same`BlynkSimpleShieldEsp8266.h` for Mega boards

to use EEPROM ( 156 bytes for Mega, 180 bytes for Teensy from address EEPROM_START ) to save your configuration data.
EEPROM_SIZE can be specified from 256 to 4096 bytes.
For SAMD boards, data is stored in Flash using 256-byte block.

See examples 
1. [Mega_ESP8266Shield](examples/Mega_ESP8266Shield)
2. [Teensy40_ESP8266Shield](examples/Teensy40_ESP8266Shield)
3. [SAMD_ESP8266Shield](examples/SAMD_ESP8266Shield)
3. [STM32_ESP8266Shield](examples/STM32_ESP8266Shield)


```
// Force some params in Blynk, only valid for library version 1.0.0 and later
#define TIMEOUT_RECONNECT_WIFI                    10000L
#define RESET_IF_CONFIG_TIMEOUT                   true
#define CONFIG_TIMEOUT_RETRYTIMES_BEFORE_RESET    5

```

Then replace `Blynk.begin(...)` with :

1. `Blynk.begin()`

in your code. Keep `Blynk.run()` intact.

That's it.


## So, how it works?
If it cannot connect to the Blynk server in 30 seconds, it will switch to `Configuration Mode`. You will see your built-in LED turned ON. In `Configuration Mode`, it starts a configurable access point, default called `Teensy4_XXXXXX`, `SAMD_XXXXXX`, `Mega_XXXXXX` or `STM32_XXXXXX`. Connect to it using password `MyTeensy4_XXXXXX`, `MySAMD_XXXXXX`, `MyMega_XXXXXX` or `MySTM32_XXXXXX`.

<p align="center">
    <img src="https://github.com/khoih-prog/Blynk_Esp8266AT_WM/blob/master/pics/AccessPoint.jpg">
</p>

After you connected, please, go to http://192.168.4.1 or the AP IP you specified.

<p align="center">
    <img src="https://github.com/khoih-prog/Blynk_Esp8266AT_WM/blob/master/pics/Config_Portal.png">
</p>

Enter your credentials, then click `Save`. After you restarted, you will see your built-in LED turned OFF. That means, it connected to your Blynk server successfully.

The following is the sample terminal output when running example [Teensy40_ESP8266Shield](examples/Teensy40_ESP8266Shield)

1. No Config Data => Config Portal

```
Start Blynk WiFiManager using ESP8266_AT_Shield on TEENSY 4.0
Start Blynk_WM
[7866] AT version:0.40.0.0(Aug  8 2015 14:45:58)
SDK version:1.3.0
Ai-Thinker Technology Co.,Ltd.
Build:1.3.0.2 Sep 11 2015 11:48:04
OK
[8392] Init new EEPROM, size = 1080
[8393] bg: No configdat. Stay forever in config portal
[12836] startConfig: SSID = Teensy4_CCE61, PW = MyTeensy4_CCE61, IP = 192.168.100.1

```

2. Config Data Ready => Run


```
Start Blynk WiFiManager using ESP8266_AT_Shield on TEENSY 4.0
Start Blynk_WM
[7866] AT version:0.40.0.0(Aug  8 2015 14:45:58)
SDK version:1.3.0
Ai-Thinker Technology Co.,Ltd.
Build:1.3.0.2 Sep 11 2015 11:48:04
OK
[8390] Header = SHD_ESP8266, SSID = ****, PW = ****
[8390] Server = ****.duckdns.org, Port = 8080, Token = ****
[8390] Board Name = Teensy4-WM
[8390] 
    ___  __          __
   / _ )/ /_ _____  / /__
  / _  / / // / _ \/  '_/
 /____/_/\_, /_//_/_/\_\
        /___/ v0.6.1 on Teensy 4.0

[8390] con2WF: start
[8390] connectToWifi: Try connectWiFi
[8390] Connecting to ****
[14956] AT version:0.40.0.0(Aug  8 2015 14:45:58)
SDK version:1.3.0
Ai-Thinker Technology Co.,Ltd.
Build:1.3.0.2 Sep 11 2015 11:48:04
OK
[15493] Get macAddress = 5c:cf:7f:66:05:d2
[22526] IP = 192.168.2.107

[22543] Connected to WiFi
[22543] con2WF: con OK
[22543] IP = 192.168.2.107

[22560] bg: WiFi connected. Try Blynk
[32717] Ready (ping: 18ms).
[32932] bg: WiFi+Blynk connected
BBBBBBBBBB BBBBBBBBBB BBBBBBBBBB BBBBBBBBBB 
F[634808] run: Blynk lost. Connect Blynk
F[649868] run: Blynk lost. Connect Blynk
F[664928] run: Blynk lost. Connect Blynk
[675040] Ready (ping: 13ms).
[675265] run: Blynk reconnected
BBBBBBBBBB BBBBBBBBBB BBBBBBBBBB BBBBBBBBBB 

```

This `Blynk.begin()` is not a blocking call, so you can use it for critical functions requiring in loop(). 
Anyway, this is better for projects using Blynk just for GUI (graphical user interface).

In operation, if WiFi or Blynk connection is lost, `Blynk.run()` will try reconnecting automatically. Therefore, `Blynk.run()` must be called in the `loop()` function. Don't use:

```cpp
void loop()
{
  if (Blynk.connected())
     Blynk.run();
     
  ...
}
```
just

```cpp
void loop()
{
  Blynk.run();
  ...
}
```

## TO DO

1. Same features for other boards ESP8266 AT-command WiFi shields.
2. To fix the ***EEPROM not working*** in some STM32 boards

## DONE

 1. Permit EEPROM size and location configurable to avoid conflict with others.
 2. More flexible to configure reconnection timeout.
 3. For fresh config data, don't need to wait for connecting timeout before entering config portal.
 4. If the config data not entered completely (SSID, password, Server and Blynk token), entering config portal
 5. Add configurable Config Portal IP, SSID and Password
 6. Add configurable Static IP, GW, Subnet Mask and 2 DNS Servers' IP Addresses.

## Example
Please take a look at examples, as well.

```
#define BLYNK_PRINT Serial

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
// For Mega
#define EspSerial Serial3
#define BOARD_TYPE      "AVR Mega"
#endif

#include <ESP8266_Lib.h>

// Start location in EEPROM to store config data. Default 0
// Config data Size currently is 128 bytes)
#define EEPROM_START     0

#define USE_BLYNK_WM      true
//#define USE_BLYNK_WM      false

#if USE_BLYNK_WM
  #ifdef CORE_TEENSY
    #include <BlynkSimpleShieldEsp8266_Teensy_WM.h>
  #else
    #include <BlynkSimpleShieldEsp8266_WM.h>
  #endif
#else
  #include <BlynkSimpleShieldEsp8266_Teensy.h>

  #define USE_LOCAL_SERVER      true

  #if USE_LOCAL_SERVER
    char auth[] = "****";
    String BlynkServer = "account.duckdns.org";
    //String BlynkServer = "192.168.2.112";
  #else
    char auth[] = "****";
    String BlynkServer = "blynk-cloud.com";
  #endif

  #define BLYNK_SERVER_HARDWARE_PORT    8080

  // Your WiFi credentials.
  char ssid[] = "****";
  char pass[] = "****";
  
#endif

// Your Teensy <-> ESP8266 baud rate:
#define ESP8266_BAUD 115200

ESP8266 wifi(&EspSerial);

void heartBeatPrint(void)
{
  static int num = 1;

  if (Blynk.connected())
  {
    Serial.print("B");
  }
  else
  {
    Serial.print("F");
  }
  
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

#define STATUS_CHECK_INTERVAL     15000L

  // Send status report every STATUS_REPORT_INTERVAL (60) seconds: we don't need to send updates frequently if there is no status change.
  if ((millis() > checkstatus_timeout) || (checkstatus_timeout == 0))
  {
    // report status to Blynk
    heartBeatPrint();

    checkstatus_timeout = millis() + STATUS_CHECK_INTERVAL;
  }
}

void setup() 
{
  // Debug console
  Serial.begin(115200);
  delay(1000);
 
  // initialize serial for ESP module
  EspSerial.begin(ESP8266_BAUD);
  Serial.println("\nStart Blynk WiFiManager using ESP8266_AT_Shield on " + String(BOARD_TYPE));

  #if USE_BLYNK_WM
    Serial.println("Start Blynk_WM");
    Blynk.setConfigPortalIP(IPAddress(192, 168, 100, 1));
    //Blynk.setConfigPortal("Teensy4", "MyTeensy4");
    Blynk.begin(wifi);
  #else
    Serial.println("Start Blynk");
    Blynk.begin(auth, wifi, ssid, pass, BlynkServer.c_str(), BLYNK_SERVER_HARDWARE_PORT);
  #endif
}

void loop()
{
  Blynk.run();
  check_status();
}
```
### New Version v1.0.3

1. Add support to STM32 (STM32F1, F2, F3, F4, F7, etc) boards
2. Add clearConfigData() function.

### New Release v1.0.2

1. Add support to SAMD (DUE, ZERO, MKR, NANO_33_IOT, M0, M0 Pro, AdaFruit CIRCUITPLAYGROUND_EXPRESS, etc.) boards

### Release v1.0.1

1. Add ESP8266_AT_WebServer dependency for autoinstall via Library Manager
2. Add prerequisite to README.md
3. Add checksum, fix bug

### Release v1.0.0

***Features***

1. This release of very-easy-to-use will help you to eliminate hardcoding your Wifi and Blynk credentials for Mega/Teensy boards running ESP8266 AT shields, and updating/reflashing every time when you need to change them.
2. Configuration data are stored in configurable location in EEPROM.
3. When WiFi and/or Blynk connection is lost, the WM will try auto-reconnect.
4. `Config Portal Static IP address, Name and Password.`
5. `Static IP address, Gateway, Subnet Mask.`

## Contributing

If you want to contribute to this project:
- Report bugs and errors
- Ask for enhancements
- Create issues and pull requests
- Tell other people about this library

## Copyright

Copyright 2020- Khoi Hoang
