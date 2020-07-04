/****************************************************************************************************************************
   dynamicParams.h for SAMD_ESP8266Shield.ino
   For SAMD boards using ESP8266 AT WiFi Shields, using much less code to support boards with smaller memory

   ESP_AT_WM_Lite is a library for the Mega, Teensy, SAM DUE, SAMD and STM32 boards (https://github.com/khoih-prog/ESP_AT_WM_Lite)
   to enable store Credentials in EEPROM to easy configuration/reconfiguration and autoconnect/autoreconnect of WiFi and other services
   without Hardcoding.

   Built by Khoi Hoang https://github.com/khoih-prog/ESP_AT_WM_Lite
   Licensed under MIT license
   Version: 1.0.4

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
 *****************************************************************************************************************************/

#ifndef dynamicParams_h
#define dynamicParams_h

#include "defines.h"

#define USE_DYNAMIC_PARAMETERS      true

/////////////// Start dynamic Credentials ///////////////

//Defined in <Esp8266_AT_WM_Lite_SAMD.h>
/**************************************
  #define MAX_ID_LEN                5
  #define MAX_DISPLAY_NAME_LEN      16

  typedef struct
  {
  char id             [MAX_ID_LEN + 1];
  char displayName    [MAX_DISPLAY_NAME_LEN + 1];
  char *pdata;
  uint8_t maxlen;
  } MenuItem;
**************************************/

#if USE_DYNAMIC_PARAMETERS

#define MAX_MQTT_SERVER_LEN      34
char MQTT_Server  [MAX_MQTT_SERVER_LEN + 1]   = "mqtt-server";

#define MAX_MQTT_PORT_LEN        6
char MQTT_Port   [MAX_MQTT_PORT_LEN + 1]  = "1883";


MenuItem myMenuItems [] =
{
  { "mqtt", "MQTT Server",      MQTT_Server,      MAX_MQTT_SERVER_LEN },
  { "mqpt", "Port",             MQTT_Port,        MAX_MQTT_PORT_LEN   },
};

// Due to notorious 2K buffer limitation of ESO8266-AT shield, the NUM_MENU_ITEMS is limited to max 3
// to avoid WebServer not working due to HTML data larger than 2K can't be sent successfully
// The items with index larger than 3 will be ignored

uint16_t NUM_MENU_ITEMS = min( 3, sizeof(myMenuItems) / sizeof(MenuItem));  //MenuItemSize;

#else

MenuItem myMenuItems [] = {};

uint16_t NUM_MENU_ITEMS = 0;
#endif

/////// // End dynamic Credentials ///////////


#endif      //dynamicParams_h
