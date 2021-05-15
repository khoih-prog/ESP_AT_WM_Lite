/****************************************************************************************************************************
   defines.h for nRF52_ESP8266Shield.ino
   For nRF52 boards using ESP8266 AT WiFi Shields, using much less code to support boards with smaller memory

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
#define _ESP_AT_LOGLEVEL_             0

/* Comment this out to disable prints and save space */
#define ESP_AT_DEBUG_OUTPUT           Serial

#define ESP_AT_DEBUG                  true
#define _ESP_AT_WM_LOGLEVEL_          4

// Uncomment to use ESP32-AT commands
//#define USE_ESP32_AT                  true

#if ( defined(ARDUINO_ARCH_RP2040) || defined(ARDUINO_RASPBERRY_PI_PICO) || defined(ARDUINO_ADAFRUIT_FEATHER_RP2040) || defined(ARDUINO_GENERIC_RP2040) )
  #if defined(ESP8266_AT_USE_RP2040)
    #undef ESP8266_AT_USE_RP2040
  #endif
  #define ESP8266_AT_USE_RP2040      true
#else
  #error This code is intended to run only on the RP2040-based boards ! Please check your Tools->Board setting.
#endif

#if (ESP8266_AT_USE_RP2040)

  #if defined(ARDUINO_RASPBERRY_PI_PICO)
    #define BOARD_TYPE      "ARDUINO_RASPBERRY_PI_PICO"
  #elif defined(ARDUINO_ADAFRUIT_FEATHER_RP2040)
    #define BOARD_TYPE      "ARDUINO_ADAFRUIT_FEATHER_RP2040"
  #elif defined(ARDUINO_GENERIC_RP2040)
    #define BOARD_TYPE      "ARDUINO_GENERIC_RP2040"
  #else
    #define BOARD_TYPE      "RP2040 Unknown"
  #endif
  
  #define EspSerial Serial1

#endif    //ESP8266_AT_USE_RP2040

#ifndef BOARD_NAME
  #define BOARD_NAME    BOARD_TYPE
#endif

/////////////////////////////////////////////

// Permit input only one set of WiFi SSID/PWD. The other can be "NULL or "blank"
// Default is false (if not defined) => must input 2 sets of SSID/PWD
#define REQUIRE_ONE_SET_SSID_PW       false

#define USE_DYNAMIC_PARAMETERS        true

/////////////////////////////////////////////

#define SCAN_WIFI_NETWORKS                  true

// To be able to manually input SSID, not from a scanned SSID lists
#define MANUAL_SSID_INPUT_ALLOWED           true

// From 2-15
#define MAX_SSID_IN_LIST                    6

/////////////////////////////////////////////

#include <Esp8266_AT_WM_Lite_RPi_Pico.h>

#define HOST_NAME   "RP2040-ESP_AT"

// SSID and PW for Config Portal
String portal_ssid      = "CfgPrtl-SSID";
String portal_password  = "CfgPrtl-PW";

// Your RP2040 <-> ESP8266 baud rate:
#define ESP8266_BAUD 115200

#endif      //defines_h
