/****************************************************************************************************************************
   Esp8266_AT_WM_Lite_Debug.h
   For AVR or Generic boards using ESP8266 AT WiFi Shields, using much less code to support boards with smaller memory

   Esp8266_AT_WM_Lite is a library for the Mega, Teensy, SAM DUE, SAMD and STM32 boards (https://github.com/khoih-prog/ESP_AT_WM_Lite)
   to enable store Credentials in EEPROM to easy configuration/reconfiguration and autoconnect/autoreconnect of WiFi and other services
   without Hardcoding.

   Built by Khoi Hoang https://github.com/khoih-prog/ESP_AT_WM_Lite
   Licensed under MIT license
   Version: 1.0.0

   Version Modified By   Date        Comments
   ------- -----------  ----------   -----------
    1.0.0   K Hoang      09/03/2020  Initial coding
 *****************************************************************************************************************************/

#ifndef Esp8266_AT_WM_Lite_Debug_h
#define Esp8266_AT_WM_Lite_Debug_h

#if !defined(ESP_AT_DEBUG)
  #define ESP_AT_DEBUG    false
#endif

  // Use these to always display
  #define INFO_WM1(p1)                 { ESP_AT_DEBUG_OUTPUT.print(F("*AT: ")); ESP_AT_DEBUG_OUTPUT.println(p1); }
  #define INFO_WM2(p1,p2)              { ESP_AT_DEBUG_OUTPUT.print(F("*AT: ")); ESP_AT_DEBUG_OUTPUT.print(p1); \
                                          ESP_AT_DEBUG_OUTPUT.println(p2); }
  #define INFO_WM3(p1,p2,p3)           { ESP_AT_DEBUG_OUTPUT.print(F("*AT: ")); ESP_AT_DEBUG_OUTPUT.print(p1); \
                                          ESP_AT_DEBUG_OUTPUT.print(p2); ESP_AT_DEBUG_OUTPUT.println(p3); }
  #define INFO_WM4(p1,p2,p3,p4)        { ESP_AT_DEBUG_OUTPUT.print(F("*AT: ")); ESP_AT_DEBUG_OUTPUT.print(p1); \
                                          ESP_AT_DEBUG_OUTPUT.print(p2); ESP_AT_DEBUG_OUTPUT.print(p3); ESP_AT_DEBUG_OUTPUT.println(p4); }
  #define INFO_WM6(p1,p2,p3,p4,p5,p6)  { ESP_AT_DEBUG_OUTPUT.print(F("*AT: ")); ESP_AT_DEBUG_OUTPUT.print(p1); \
                                          ESP_AT_DEBUG_OUTPUT.print(p2); ESP_AT_DEBUG_OUTPUT.print(p3); ESP_AT_DEBUG_OUTPUT.print(p4); \
                                          ESP_AT_DEBUG_OUTPUT.print(p5); ESP_AT_DEBUG_OUTPUT.println(p6); }
                                          
#if ESP_AT_DEBUG
  #define DEBUG_WM1(p1)                 { ESP_AT_DEBUG_OUTPUT.print(F("*AT: ")); ESP_AT_DEBUG_OUTPUT.println(p1); }
  #define DEBUG_WM2(p1,p2)              { ESP_AT_DEBUG_OUTPUT.print(F("*AT: ")); ESP_AT_DEBUG_OUTPUT.print(p1); \
                                          ESP_AT_DEBUG_OUTPUT.println(p2); }
  #define DEBUG_WM3(p1,p2,p3)           { ESP_AT_DEBUG_OUTPUT.print(F("*AT: ")); ESP_AT_DEBUG_OUTPUT.print(p1); \
                                          ESP_AT_DEBUG_OUTPUT.print(p2); ESP_AT_DEBUG_OUTPUT.println(p3); }
  #define DEBUG_WM4(p1,p2,p3,p4)        { ESP_AT_DEBUG_OUTPUT.print(F("*AT: ")); ESP_AT_DEBUG_OUTPUT.print(p1); \
                                          ESP_AT_DEBUG_OUTPUT.print(p2); ESP_AT_DEBUG_OUTPUT.print(p3); ESP_AT_DEBUG_OUTPUT.println(p4); }
  #define DEBUG_WM6(p1,p2,p3,p4,p5,p6)  { ESP_AT_DEBUG_OUTPUT.print(F("*AT: ")); ESP_AT_DEBUG_OUTPUT.print(p1); \
                                          ESP_AT_DEBUG_OUTPUT.print(p2); ESP_AT_DEBUG_OUTPUT.print(p3); ESP_AT_DEBUG_OUTPUT.print(p4); \
                                          ESP_AT_DEBUG_OUTPUT.print(p5); ESP_AT_DEBUG_OUTPUT.println(p6); }
#else
  #define DEBUG_WM1(p1)
  #define DEBUG_WM2(p1,p2)
  #define DEBUG_WM3(p1,p2,p3)
  #define DEBUG_WM4(p1,p2,p3,p4)
  #define DEBUG_WM6(p1,p2,p3,p4,p5,p6)
#endif

#endif    //Esp8266_AT_WM_Lite_Debug_h
