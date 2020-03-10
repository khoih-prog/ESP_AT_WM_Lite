/****************************************************************************************************************************
 * SAM_DUE_ESP8266Shield.ino
 * For SAM DUE boards using ESP8266 AT WiFi Shields, using much less code to support boards with smaller memory

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

/* Comment this out to disable prints and save space */
#define ESP_AT_DEBUG_OUTPUT Serial

#define ESP_AT_DEBUG    true

#if ( defined(ARDUINO_SAM_DUE) || defined(__SAM3X8E__) )
  #if defined(ESP8266_AT_USE_SAM_DUE)
  #undef ESP8266_AT_USE_SAM_DUE
  #endif
  #define ESP8266_AT_USE_SAM_DUE      true
  #warning Use SAM_DUE architecture
#endif

#if ( defined(ESP8266) || defined(ESP32) || defined(ARDUINO_AVR_MEGA2560) || defined(ARDUINO_AVR_MEGA) || \
      defined(CORE_TEENSY) || defined(CORE_TEENSY) || !(ESP8266_AT_USE_SAM_DUE) )
#error This code is intended to run on the SAM DUE platform! Please check your Tools->Board setting.
#endif

#if defined(ESP8266_AT_USE_SAM_DUE) 
   // For SAM DUE
  #define EspSerial Serial1
  
  #if defined(ARDUINO_SAM_DUE)
    #define BOARD_TYPE      "SAM SUE"
  #elif defined(__SAM3X8E__)
    #define BOARD_TYPE      "SAM SAM3X8E"    
  #else
    #define BOARD_TYPE      "SAM Unknown"
  #endif
#endif

// Start location in EEPROM to store config data. Default 0
// Config data Size currently is 128 bytes)
#define EEPROM_START      (64)

#include <Esp8266_AT_WM_Lite_DUE.h>

// Your Mega <-> ESP8266 baud rate:
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

ESP_AT_WiFiManager_Lite* ESP_AT_WiFiManager;

void setup()
{
  // Debug console
  Serial.begin(115200);
  delay(1000);

  Serial.println("\nStart SAM_DUE_ESP8266Shield on " + String(BOARD_TYPE));

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
