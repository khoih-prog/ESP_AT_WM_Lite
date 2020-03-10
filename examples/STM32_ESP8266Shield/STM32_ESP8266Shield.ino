/****************************************************************************************************************************
   STM32_ESP8266Shield.ino
   For STM32 boards using ESP8266 AT WiFi Shields, using much less code to support boards with smaller memory

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

#if ( defined(STM32F0) || defined(STM32F1) || defined(STM32F2) || defined(STM32F3)  ||defined(STM32F4) || defined(STM32F7) )
#if defined(ESP8266_AT_USE_STM32)
#undef ESP8266_AT_USE_STM32
#endif
#define ESP8266_AT_USE_STM32      true
#endif

#if ( defined(ESP8266) || defined(ESP32) || defined(ARDUINO_AVR_MEGA) || defined(ARDUINO_AVR_MEGA2560) || defined(CORE_TEENSY) || !(ESP8266_AT_USE_STM32) )
//#error This code is intended to run on STM32 platform! Please check your Tools->Board setting.
#endif

#if ESP8266_AT_USE_STM32
// For STM32, you have to declare and enable coreresponding Serial Port somewhere else before using it
#define EspSerial Serial1

#if defined(STM32F0)
#define BOARD_TYPE  "STM32F0"
#error Board STM32F0 not supported
#elif defined(STM32F1)
#define BOARD_TYPE  "STM32F1"
#elif defined(STM32F2)
#define BOARD_TYPE  "STM32F2"
#elif defined(STM32F3)
#define BOARD_TYPE  "STM32F3"
#elif defined(STM32F4)
#define BOARD_TYPE  "STM32F4"
#elif defined(STM32F7)
#define BOARD_TYPE  "STM32F7"
#else
#warning STM32 unknown board selected
#define BOARD_TYPE  "STM32 Unknown"
#endif
#else
// For Mega
#define EspSerial Serial3
#define BOARD_TYPE      "AVR Mega"
#endif

// Start location in EEPROM to store config data. Default 0
// Config data Size currently is 128 bytes)
#define EEPROM_START      (64)

#include <Esp8266_AT_WM_Lite_STM32.h>

// Your STM32 <-> ESP8266 baud rate:
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

  Serial.println("\nStart STM32_ESP8266Shield on " + String(BOARD_TYPE));

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
