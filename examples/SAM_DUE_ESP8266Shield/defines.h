/****************************************************************************************************************************
  defines.h
  For SAM DUE boards using ESP8266 AT WiFi Shields, using much less code to support boards with smaller memory
  
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

#if ( defined(ARDUINO_SAM_DUE) || defined(__SAM3X8E__) )
  #if defined(ESP8266_AT_USE_SAM_DUE)
    #undef ESP8266_AT_USE_SAM_DUE
  #endif
  #define ESP8266_AT_USE_SAM_DUE      true
  #warning Use SAM_DUE architecture
#else
  #error This code is intended to run on the SAM DUE platform! Please check your Tools->Board setting.  
#endif

#if defined(ESP8266_AT_USE_SAM_DUE)
  // For SAM DUE
  #define EspSerial Serial1
  
  #if defined(ARDUINO_SAM_DUE)
    #define BOARD_TYPE      "SAM DUE"
  #elif defined(__SAM3X8E__)
    #define BOARD_TYPE      "SAM SAM3X8E"
  #else
    #define BOARD_TYPE      "SAM Unknown"
  #endif
#endif

#ifndef BOARD_NAME
  #define BOARD_NAME    BOARD_TYPE
#endif

// Start location in EEPROM to store config data. Default 0
#define EEPROM_START      0

/////////////////////////////////////////////

// Permit input only one set of WiFi SSID/PWD. The other can be "NULL or "blank"
// Default is false (if not defined) => must input 2 sets of SSID/PWD
#define REQUIRE_ONE_SET_SSID_PW             true    //false

// Max times to try WiFi per loop() iteration. To avoid blocking issue in loop()
// Default 1 if not defined, and minimum 1.
//#define MAX_NUM_WIFI_RECON_TRIES_PER_LOOP     2

// Default no interval between recon WiFi if lost
// Max permitted interval will be 10mins
// Uncomment to use. Be careful, WiFi reconnect will be delayed if using this method
// Only use whenever urgent tasks in loop() can't be delayed. But if so, it's better you have to rewrite your code, e.g. using higher priority tasks.
#define WIFI_RECON_INTERVAL                   30000

/////////////////////////////////////////////

#define USE_DYNAMIC_PARAMETERS                  false     //true

#warning Disable USE_DYNAMIC_PARAMETERS for ESP_AT_SHIELD

/////////////////////////////////////////////

#define SCAN_WIFI_NETWORKS                  true

// To be able to manually input SSID, not from a scanned SSID lists
#define MANUAL_SSID_INPUT_ALLOWED           true

// From 2-15
#define MAX_SSID_IN_LIST                    4

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

#include <Esp8266_AT_WM_Lite_DUE.h>

#define HOST_NAME   "SAM_DUE-ESP_AT"

// SSID and PW for Config Portal
String portal_ssid      = "CfgPrtl-SSID";
String portal_password  = "CfgPrtl-PW";

// Your SAM DUE <-> ESP8266 baud rate:
#define ESP8266_BAUD 115200

#endif      //defines_h
