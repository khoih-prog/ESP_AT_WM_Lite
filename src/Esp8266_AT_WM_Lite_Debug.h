/***************************************************************************************************************************************
  Esp8266_AT_WM_Lite_Debug.h
  For AVR or Generic boards using ESP8266 AT WiFi Shields, using much less code to support boards with smaller memory

  ESP_AT_WM_Lite is a library for the Mega, Teensy, SAM DUE, SAMD and STM32, nRF52 boards (https://github.com/khoih-prog/ESP_AT_WM_Lite)
  to enable store Credentials in EEPROM to easy configuration/reconfiguration and autoconnect/autoreconnect of WiFi and other services
  without Hardcoding.

  Built by Khoi Hoang https://github.com/khoih-prog/ESP_AT_WM_Lite
  Licensed under MIT license
  Version: 1.4.1

  Version Modified By   Date        Comments
  ------- -----------  ----------   -----------
  1.0.0   K Hoang      09/03/2020  Initial coding
  1.0.1   K Hoang      20/03/2020  Add feature to enable adding dynamically more Credentials parameters in sketch
  1.0.2   K Hoang      17/04/2020  Fix bug. Add support to SAMD51 and SAMD DUE. WPA2 SSID PW to 63 chars.
                                  Permit to input special chars such as !,@,#,$,%,^,&,* into data fields.
  1.0.3   K Hoang      11/06/2020  Add support to nRF52 boards, such as AdaFruit Feather nRF52832, NINA_B30_ublox, etc.
                                  Add DRD support. Add MultiWiFi support 
  1.0.4   K Hoang      03/07/2020  Add support to ESP32-AT shields. Modify LOAD_DEFAULT_CONFIG_DATA logic.
                                  Enhance MultiWiFi connection logic. Fix WiFi Status bug.
  1.1.0   K Hoang      13/04/2021  Fix invalid "blank" Config Data treated as Valid. Optional one set of WiFi Credentials
  1.2.0   Michael H    28/04/2021  Enable scan of WiFi networks for selection in Configuration Portal
  1.3.0   K Hoang      12/05/2021  Add support to RASPBERRY_PI_PICO using Arduino-pico core
  1.4.0   K Hoang      01/06/2021  Add support to Nano_RP2040_Connect, RASPBERRY_PI_PICO using RP2040 Arduino mbed core  
  1.4.1   K Hoang      10/10/2021  Update `platform.ini` and `library.json`              
 ***************************************************************************************************************************************/

#ifndef Esp8266_AT_WM_Lite_Debug_h
#define Esp8266_AT_WM_Lite_Debug_h

#ifdef ESP_AT_DEBUG_OUTPUT
  #define DBG_PORT_ESP_AT       ESP_AT_DEBUG_OUTPUT
#else
  #define DBG_PORT_ESP_AT       Serial
#endif

// Change _ESP_AT_WM_LOGLEVEL_ to set tracing and logging verbosity
// 0: DISABLED: no logging
// 1: ERROR: errors
// 2: WARN: errors and warnings
// 3: INFO: errors, warnings and informational (default)
// 4: DEBUG: errors, warnings, informational and debug

#ifndef _ESP_AT_WM_LOGLEVEL_
  #define _ESP_AT_WM_LOGLEVEL_       0
#endif

const char ESP_AT_MARK[] = "[ESP_AT] ";

#define ESP_AT_PRINT_MARK   DBG_PORT_ESP_AT.print(ESP_AT_MARK)

#define ESP_AT_PRINT        DBG_PORT_ESP_AT.print
#define ESP_AT_PRINTLN      DBG_PORT_ESP_AT.println

//////////////////////////////////////////////

#define ESP_AT_LOGERROR0(x)       if(_ESP_AT_WM_LOGLEVEL_>0) { ESP_AT_PRINT(x); }
#define ESP_AT_LOGERROR(x)        if(_ESP_AT_WM_LOGLEVEL_>0) { ESP_AT_PRINT_MARK; ESP_AT_PRINTLN(x); }
#define ESP_AT_LOGERROR1(x,y)     if(_ESP_AT_WM_LOGLEVEL_>0) { ESP_AT_PRINT_MARK; ESP_AT_PRINT(x); ESP_AT_PRINTLN(y); }
#define ESP_AT_LOGERROR2(x,y,z)   if(_ESP_AT_WM_LOGLEVEL_>0) { ESP_AT_PRINT_MARK; ESP_AT_PRINT(x); ESP_AT_PRINT(y); ESP_AT_PRINTLN(z); }
#define ESP_AT_LOGERROR3(x,y,z,w) if(_ESP_AT_WM_LOGLEVEL_>0) { ESP_AT_PRINT_MARK; ESP_AT_PRINT(x); ESP_AT_PRINT(y); ESP_AT_PRINT(z); ESP_AT_PRINTLN(w); }
#define ESP_AT_LOGERROR5(x,y,z,w,xx,yy) if(_ESP_AT_WM_LOGLEVEL_>0) { ESP_AT_PRINT_MARK; ESP_AT_PRINT(x); ESP_AT_PRINT(y); ESP_AT_PRINT(z); ESP_AT_PRINT(w); ESP_AT_PRINT(xx); ESP_AT_PRINTLN(yy); }

//////////////////////////////////////////////

#define ESP_AT_LOGWARN0(x)       if(_ESP_AT_WM_LOGLEVEL_>1) { ESP_AT_PRINT(x); }
#define ESP_AT_LOGWARN(x)        if(_ESP_AT_WM_LOGLEVEL_>1) { ESP_AT_PRINT_MARK; ESP_AT_PRINTLN(x); }
#define ESP_AT_LOGWARN1(x,y)     if(_ESP_AT_WM_LOGLEVEL_>1) { ESP_AT_PRINT_MARK; ESP_AT_PRINT(x); ESP_AT_PRINTLN(y); }
#define ESP_AT_LOGWARN2(x,y,z)   if(_ESP_AT_WM_LOGLEVEL_>1) { ESP_AT_PRINT_MARK; ESP_AT_PRINT(x); ESP_AT_PRINT(y); ESP_AT_PRINTLN(z); }
#define ESP_AT_LOGWARN3(x,y,z,w) if(_ESP_AT_WM_LOGLEVEL_>1) { ESP_AT_PRINT_MARK; ESP_AT_PRINT(x); ESP_AT_PRINT(y); ESP_AT_PRINT(z); ESP_AT_PRINTLN(w); }
#define ESP_AT_LOGWARN5(x,y,z,w,xx,yy) if(_ESP_AT_WM_LOGLEVEL_>1) { ESP_AT_PRINT_MARK; ESP_AT_PRINT(x); ESP_AT_PRINT(y); ESP_AT_PRINT(z); ESP_AT_PRINT(w); ESP_AT_PRINT(xx); ESP_AT_PRINTLN(yy); }

//////////////////////////////////////////////

#define ESP_AT_LOGINFO0(x)       if(_ESP_AT_WM_LOGLEVEL_>2) { ESP_AT_PRINT(x); }
#define ESP_AT_LOGINFO(x)        if(_ESP_AT_WM_LOGLEVEL_>2) { ESP_AT_PRINT_MARK; ESP_AT_PRINTLN(x); }
#define ESP_AT_LOGINFO1(x,y)     if(_ESP_AT_WM_LOGLEVEL_>2) { ESP_AT_PRINT_MARK; ESP_AT_PRINT(x); ESP_AT_PRINTLN(y); }
#define ESP_AT_LOGINFO2(x,y,z)   if(_ESP_AT_WM_LOGLEVEL_>3) { ESP_AT_PRINT_MARK; ESP_AT_PRINT(x); ESP_AT_PRINT(y); ESP_AT_PRINTLN(z); }
#define ESP_AT_LOGINFO3(x,y,z,w) if(_ESP_AT_WM_LOGLEVEL_>3) { ESP_AT_PRINT_MARK; ESP_AT_PRINT(x); ESP_AT_PRINT(y); ESP_AT_PRINT(z); ESP_AT_PRINTLN(w); }
#define ESP_AT_LOGINFO5(x,y,z,w,xx,yy) if(_ESP_AT_WM_LOGLEVEL_>2) { ESP_AT_PRINT_MARK; ESP_AT_PRINT(x); ESP_AT_PRINT(y); ESP_AT_PRINT(z); ESP_AT_PRINT(w); ESP_AT_PRINT(xx); ESP_AT_PRINTLN(yy); }

//////////////////////////////////////////////

#define ESP_AT_LOGDEBUG0(x)       if(_ESP_AT_WM_LOGLEVEL_>3) { ESP_AT_PRINT(x); }
#define ESP_AT_LOGDEBUG(x)        if(_ESP_AT_WM_LOGLEVEL_>3) { ESP_AT_PRINT_MARK; ESP_AT_PRINTLN(x); }
#define ESP_AT_LOGDEBUG1(x,y)     if(_ESP_AT_WM_LOGLEVEL_>3) { ESP_AT_PRINT_MARK; ESP_AT_PRINT(x); ESP_AT_PRINTLN(y); }
#define ESP_AT_LOGDEBUG2(x,y,z)   if(_ESP_AT_WM_LOGLEVEL_>3) { ESP_AT_PRINT_MARK; ESP_AT_PRINT(x); ESP_AT_PRINT(y); ESP_AT_PRINTLN(z); }
#define ESP_AT_LOGDEBUG3(x,y,z,w) if(_ESP_AT_WM_LOGLEVEL_>3) { ESP_AT_PRINT_MARK; ESP_AT_PRINT(x); ESP_AT_PRINT(y); ESP_AT_PRINT(z); ESP_AT_PRINTLN(w); }
#define ESP_AT_LOGDEBUG5(x,y,z,w,xx,yy) if(_ESP_AT_WM_LOGLEVEL_>3) { ESP_AT_PRINT_MARK; ESP_AT_PRINT(x); ESP_AT_PRINT(y); ESP_AT_PRINT(z); ESP_AT_PRINT(w); ESP_AT_PRINT(xx); ESP_AT_PRINTLN(yy); }

//////////////////////////////////////////////


#endif    //Esp8266_AT_WM_Lite_Debug_h
