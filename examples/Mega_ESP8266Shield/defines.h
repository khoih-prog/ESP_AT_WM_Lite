/****************************************************************************************************************************
  defines.h for Mega_ESP8266Shield.ino
  For AVR or Generic boards using ESP8266 AT WiFi Shields, using much less code to support boards with smaller memory
  
  ESP_AT_WM_Lite is a library for the Mega, Teensy, SAM DUE, SAMD and STM32 boards (https://github.com/khoih-prog/ESP_AT_WM_Lite)
  to enable store Credentials in EEPROM to easy configuration/reconfiguration and autoconnect/autoreconnect of WiFi and other services
  without Hardcoding.
  
  Built by Khoi Hoang https://github.com/khoih-prog/ESP_AT_WM_Lite
  Licensed under MIT license
 *****************************************************************************************************************************/

#ifndef defines_h
#define defines_h

/* Comment this out to disable prints and save space */
#define DRD_GENERIC_DEBUG             true

#define USE_NEW_WEBSERVER_VERSION     true  //false
#define _ESP_AT_LOGLEVEL_             1

/* Comment this out to disable prints and save space */
#define ESP_AT_DEBUG_OUTPUT           Serial

#define ESP_AT_DEBUG                  true
#define _ESP_AT_WM_LOGLEVEL_          3

// Uncomment to use ESP32-AT commands
//#define USE_ESP32_AT                  true


#if !( defined(ARDUINO_AVR_MEGA) || defined(ARDUINO_AVR_MEGA2560) )
  #error This code is intended to run only on the Arduino Mega 1280/2560 boards ! Please check your Tools->Board setting.
#endif

// For Mega, use Serial1 or Serial3
#define EspSerial Serial3

#if defined(ARDUINO_AVR_MEGA2560)
  #define BOARD_TYPE      "AVR Mega2560"
#else
  #define BOARD_TYPE      "AVR Mega"
#endif

#ifndef BOARD_NAME
  #define BOARD_NAME    BOARD_TYPE
#endif

#define ESP_AT_USE_AVR      true

// Start location in EEPROM to store config data. Default 0
// Config data Size currently is 116 bytes)
#define EEPROM_START      0

/////////////////////////////////////////////

// Max times to try WiFi per loop() iteration. To avoid blocking issue in loop()
// Default 1 if not defined, and minimum 1.
//#define MAX_NUM_WIFI_RECON_TRIES_PER_LOOP     2

// Default no interval between recon WiFi if lost
// Max permitted interval will be 10mins
// Uncomment to use. Be careful, WiFi reconnect will be delayed if using this method
// Only use whenever urgent tasks in loop() can't be delayed. But if so, it's better you have to rewrite your code, e.g. using higher priority tasks.
#define WIFI_RECON_INTERVAL                   30000

/////////////////////////////////////////////

#define USE_DYNAMIC_PARAMETERS                false     //true

#warning Disable USE_DYNAMIC_PARAMETERS for ESP_AT_SHIELD

/////////////////////////////////////////////

// Optional, to use Board Name in Menu
#define USING_BOARD_NAME                    false     //true

/////////////////////////////////////////////

// Optional, to use Board Name in Menu
#define USING_CONFIG_MODE_LED               true

#if USING_CONFIG_MODE_LED
  #if defined(LED_BUILTIN)
    #define CONFIG_MODE_LED     LED_BUILTIN
  #else
    // Using default pin 13 for CONFIG_MODE_LED. To be changed as necessary
    #define CONFIG_MODE_LED     13
  #endif

  #define LED_ON      HIGH
  #define LED_OFF     LOW
#endif

/////////////////////////////////////////////

#include <Esp8266_AT_WM_Lite.h>

#define HOST_NAME   "Mega-ESP_AT"

// SSID and PW for Config Portal
String portal_ssid      = "CfgPrtl-SSID";
String portal_password  = "CfgPrtl-PW";

// Your Mega <-> ESP8266 baud rate:
#define ESP8266_BAUD 115200


#endif      //defines_h
