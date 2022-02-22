/***************************************************************************************************************************************
  Esp8266_AT_WM_Lite_Debug.h
  For AVR or Generic boards using ESP8266 AT WiFi Shields, using much less code to support boards with smaller memory

  ESP_AT_WM_Lite is a library for the Mega, Teensy, SAM DUE, SAMD and STM32, nRF52 boards (https://github.com/khoih-prog/ESP_AT_WM_Lite)
  to enable store Credentials in EEPROM to easy configuration/reconfiguration and autoconnect/autoreconnect of WiFi and other services
  without Hardcoding.

  Built by Khoi Hoang https://github.com/khoih-prog/ESP_AT_WM_Lite
  Licensed under MIT license
  Version: 1.5.2

  Version Modified By   Date        Comments
  ------- -----------  ----------   -----------
  1.0.0   K Hoang      09/03/2020  Initial coding
  ...
  1.5.0   K Hoang      08/01/2022  Fix the blocking issue in loop() with configurable WIFI_RECON_INTERVAL
  1.5.1   K Hoang      26/01/2022  Update to be compatible with new FlashStorage libraries. Add support to more SAMD/STM32 boards
  1.5.2   K Hoang      22/02/2022  Optional Board_Name in Menu. Optimize code by using passing by reference
                                   Add optional CONFIG_MODE_LED. Add function isConfigMode()     
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
