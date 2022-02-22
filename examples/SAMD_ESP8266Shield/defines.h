/****************************************************************************************************************************
  defines.h
  For SAMD boards using ESP8266 AT WiFi Shields, using much less code to support boards with smaller memory
  
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

#if  ( defined(ARDUINO_SAMD_ZERO) || defined(ARDUINO_SAMD_MKR1000) || defined(ARDUINO_SAMD_MKRWIFI1010) \
    || defined(ARDUINO_SAMD_NANO_33_IOT) || defined(ARDUINO_SAMD_MKRFox1200) || defined(ARDUINO_SAMD_MKRWAN1300) || defined(ARDUINO_SAMD_MKRWAN1310) \
    || defined(ARDUINO_SAMD_MKRGSM1400) || defined(ARDUINO_SAMD_MKRNB1500) || defined(ARDUINO_SAMD_MKRVIDOR4000) \
    || defined(ARDUINO_SAMD_CIRCUITPLAYGROUND_EXPRESS) || defined(__SAMD51__) || defined(__SAMD51J20A__) \
    || defined(__SAMD51J19A__) || defined(__SAMD51G19A__) || defined(__SAMD51P19A__)  \
    || defined(__SAMD21E15A__) || defined(__SAMD21E16A__) || defined(__SAMD21E17A__) || defined(__SAMD21E18A__) \
    || defined(__SAMD21G15A__) || defined(__SAMD21G16A__) || defined(__SAMD21G17A__) || defined(__SAMD21G18A__) \
    || defined(__SAMD21J15A__) || defined(__SAMD21J16A__) || defined(__SAMD21J17A__) || defined(__SAMD21J18A__) )
  #if defined(ESP8266_AT_USE_SAMD)
    #undef ESP8266_AT_USE_SAMD
  #endif
  #define ESP8266_AT_USE_SAMD      true
#else
  #error This code is intended to run only on the SAMD boards ! Please check your Tools->Board setting.
#endif

#if defined(ESP8266_AT_USE_SAMD)
  // For SAMD
  #define EspSerial Serial1
  
  #if defined(ARDUINO_SAMD_ZERO)
    #define BOARD_TYPE      "SAMD Zero"
  #elif defined(ARDUINO_SAMD_MKR1000)
    #define BOARD_TYPE      "SAMD MKR1000"
  #elif defined(ARDUINO_SAMD_MKRWIFI1010)
    #define BOARD_TYPE      "SAMD MKRWIFI1010"
  #elif defined(ARDUINO_SAMD_NANO_33_IOT)
    #define BOARD_TYPE      "SAMD NANO_33_IOT"
  #elif defined(ARDUINO_SAMD_MKRFox1200)
    #define BOARD_TYPE      "SAMD MKRFox1200"
  #elif ( defined(ARDUINO_SAMD_MKRWAN1300) || defined(ARDUINO_SAMD_MKRWAN1310) )
    #define BOARD_TYPE      "SAMD MKRWAN13X0"
  #elif defined(ARDUINO_SAMD_MKRGSM1400)
    #define BOARD_TYPE      "SAMD MKRGSM1400"
  #elif defined(ARDUINO_SAMD_MKRNB1500)
    #define BOARD_TYPE      "SAMD MKRNB1500"
  #elif defined(ARDUINO_SAMD_MKRVIDOR4000)
    #define BOARD_TYPE      "SAMD MKRVIDOR4000"
  #elif defined(ARDUINO_SAMD_CIRCUITPLAYGROUND_EXPRESS)
    #define BOARD_TYPE      "SAMD ARDUINO_SAMD_CIRCUITPLAYGROUND_EXPRESS"
  #elif defined(ADAFRUIT_ITSYBITSY_M4_EXPRESS)
    #define BOARD_TYPE      "SAMD ADAFRUIT_ITSYBITSY_M4_EXPRESS"
  #elif defined(__SAMD21E18A__)
    #define BOARD_TYPE      "SAMD21E18A"
  #elif defined(__SAMD21G18A__)
    #define BOARD_TYPE      "SAMD21G18A"
  #elif defined(__SAMD51G19A__)
    #define BOARD_TYPE      "SAMD51G19A"
  #elif defined(__SAMD51J19A__)
    #define BOARD_TYPE      "SAMD51J19A"
  #elif defined(__SAMD51J20A__)
    #define BOARD_TYPE      "SAMD51J20A"
  #elif defined(__SAM3X8E__)
    #define BOARD_TYPE      "SAM3X8E"
  #elif defined(__CPU_ARC__)
    #define BOARD_TYPE      "CPU_ARC"
  #elif defined(__SAMD51__)
    #define BOARD_TYPE      "SAMD51"
  #else
    #define BOARD_TYPE      "SAMD Unknown"
  #endif

#endif

#ifndef BOARD_NAME
  #define BOARD_NAME    BOARD_TYPE
#endif

// Start location in EEPROM to store config data. Default 0
#define EEPROM_START      0
#define EEPROM_SIZE       (2 * 1024)

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

#include <Esp8266_AT_WM_Lite_SAMD.h>

#define HOST_NAME   "SAMD_ESP_AT"

// SSID and PW for Config Portal
String portal_ssid      = "SAMD-CfgPrtl-SSID";
String portal_password  = "SAMD-CfgPrtl-PW";

// Your SAMD <-> ESP8266 baud rate:
#define ESP8266_BAUD 115200

#endif      //defines_h
