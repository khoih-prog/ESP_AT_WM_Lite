/****************************************************************************************************************************
   Esp8266_AT_WM_Lite_Teensy.h
   For Teensy boards using ESP8266 AT WiFi Shields, using much less code to support boards with smaller memory

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
 
#ifndef Esp8266_AT_WM_Lite_Teensy_h
#define Esp8266_AT_WM_Lite_Teensy_h

#if ( defined(ESP8266) || defined(ESP32) || defined(ARDUINO_AVR_MEGA2560) || defined(ARDUINO_AVR_MEGA) || !defined(CORE_TEENSY) )
#error This code is intended to run on Teensy platform! Please check your Tools->Board setting.
#endif

#if defined(__AVR_AT90USB1286__)
#error Teensy 2.0++ not supported yet
#elif defined(__AVR_ATmega32U4__)
#error Teensy 2.0 not supported yet
#endif

#include <ESP8266_AT_WebServer.h>
#include <EEPROM.h>
#include <Esp8266_AT_WM_Lite_Debug.h>

// Configurable items besides fixed Header
#define NUM_CONFIGURABLE_ITEMS    2
typedef struct Configuration
{
  char header         [16];
  char wifi_ssid      [32];
  char wifi_pw        [32];
  int  checkSum;
} ESP8266_AT_Configuration;

// Currently CONFIG_DATA_SIZE  =   84
uint16_t CONFIG_DATA_SIZE = sizeof(ESP8266_AT_Configuration);

#define root_html_template "\
<!DOCTYPE html><html><head><title>Teensy_WM</title><style>.em{padding-bottom:0px;}div,input{padding:5px;font-size:1em;}input{width:95%;}\
body{text-align: center;}button{background-color:#16A1E7;color:#fff;line-height:2.4rem;font-size:1.2rem;width:100%;}fieldset{border-radius:0.3rem;margin:0px;}\
</style></head><div style=\"text-align:left;display:inline-block;min-width:260px;\"><fieldset>\
<div class=\"\"><label for=\"id\">SSID</label><input type=\"text\"value=\"[[id]]\"id=\"id\"><div class=\"em\"></div></div>\
<div class=\"\"><label for=\"pw\">PWD</label><input value=\"[[pw]]\"id=\"pw\"><div class=\"em\"></div></div></fieldset>\
<button onclick=\"sv()\">Save</button></div>\
<script id=\"jsbin-javascript\">\
function udVal(key,val){var request=new XMLHttpRequest();var url='/?key='+key+'&value='+val;request.open('GET',url,false);request.send(null);}\
function sv(){udVal('id',document.getElementById('id').value);udVal('pw',document.getElementById('pw').value);alert('Updated');}\
</script></html>"


String IPAddressToString(IPAddress _address)
{
  String str = String(_address[0]);
  str += ".";
  str += String(_address[1]);
  str += ".";
  str += String(_address[2]);
  str += ".";
  str += String(_address[3]);
  return str;
}

class ESP_AT_WiFiManager_Lite
{
    public:
    
    ESP_AT_WiFiManager_Lite(Stream* espSerial, uint32_t speed = 115200)
    {
      WiFi.init(espSerial); 
      
      // check for the presence of the shield
      if (WiFi.status() == WL_NO_SHIELD) 
      {
        DEBUG_WM1(F("WiFi shield not present"));
      }
      
      WiFi.reset();
    }

    ~ESP_AT_WiFiManager_Lite()
    {
    }
        
    bool connectWiFi(const char* ssid, const char* pass)
    {
      DEBUG_WM2(F("Con2 "), ssid);

      if ( WiFi.begin(ssid, pass) )  
      {
        displayWiFiData();
      }
      else
      {
        DEBUG_WM1(F("No WiFi"));
        return false;
      }

      DEBUG_WM1(F("WiFi OK"));

      wifi_connected = true;

      return true;
    }
   
    void begin(const char* ssid,
               const char* pass )
    {
      DEBUG_WM1(F("conWiFi"));
      connectWiFi(ssid, pass);
    }

    void begin(void)
    {
      #define TIMEOUT_CONNECT_WIFI			30000
      
      if (getConfigData())
      {
        hadConfigData = true;

        if (connectToWifi(TIMEOUT_CONNECT_WIFI))
        {
          DEBUG_WM1(F("WiFi OK"));
        }
        else
        {
          DEBUG_WM1(F("No WiFi"));
          // failed to connect to WiFi, will start configuration mode
          startConfigurationMode();
        }
      }
      else
      {
        INFO_WM1(F("Open Portal"));
        // failed to connect to WiFi, will start configuration mode
        hadConfigData = false;
        startConfigurationMode();
      }
    }

#ifndef TIMEOUT_RECONNECT_WIFI
#define TIMEOUT_RECONNECT_WIFI   10000L
#else
    // Force range of user-defined TIMEOUT_RECONNECT_WIFI between 10-60s
#if (TIMEOUT_RECONNECT_WIFI < 10000L)
#warning TIMEOUT_RECONNECT_WIFI too low. Reseting to 10000
#undef TIMEOUT_RECONNECT_WIFI
#define TIMEOUT_RECONNECT_WIFI   10000L
#elif (TIMEOUT_RECONNECT_WIFI > 60000L)
#warning TIMEOUT_RECONNECT_WIFI too high. Reseting to 60000
#undef TIMEOUT_RECONNECT_WIFI
#define TIMEOUT_RECONNECT_WIFI   60000L
#endif
#endif

#ifndef RESET_IF_CONFIG_TIMEOUT
#define RESET_IF_CONFIG_TIMEOUT   true
#endif

#ifndef CONFIG_TIMEOUT_RETRYTIMES_BEFORE_RESET
#define CONFIG_TIMEOUT_RETRYTIMES_BEFORE_RESET          10
#else
    // Force range of user-defined TIMES_BEFORE_RESET between 2-100
#if (CONFIG_TIMEOUT_RETRYTIMES_BEFORE_RESET < 2)
#warning CONFIG_TIMEOUT_RETRYTIMES_BEFORE_RESET too low. Reseting to 2
#undef CONFIG_TIMEOUT_RETRYTIMES_BEFORE_RESET
#define CONFIG_TIMEOUT_RETRYTIMES_BEFORE_RESET   2
#elif (CONFIG_TIMEOUT_RETRYTIMES_BEFORE_RESET > 100)
#warning CONFIG_TIMEOUT_RETRYTIMES_BEFORE_RESET too high. Resetting to 100
#undef CONFIG_TIMEOUT_RETRYTIMES_BEFORE_RESET
#define CONFIG_TIMEOUT_RETRYTIMES_BEFORE_RESET   100
#endif
#endif

    void run()
    {
      static int retryTimes = 0;

      // Lost connection in running. Give chance to reconfig.
      if ( !wifi_connected )
      {
        // If configTimeout but user hasn't connected to configWeb => try to reconnect WiFi
        // But if user has connected to configWeb, stay there until done, then reset hardware
        if ( configuration_mode && ( configTimeout == 0 ||  millis() < configTimeout ) )
        {
          retryTimes = 0;

          if (server)
          {
            server->handleClient();
          }
           
          return;
        }
        else
        {
#if RESET_IF_CONFIG_TIMEOUT
          // If we're here but still in configuration_mode, permit running TIMES_BEFORE_RESET times before reset hardware
          // to permit user another chance to config.
          if ( configuration_mode && (configTimeout != 0) )
          {
            if (++retryTimes <= CONFIG_TIMEOUT_RETRYTIMES_BEFORE_RESET)
            {
              DEBUG_WM2(F("run: W lost & config T-Out. Connect W. Retry# : "), retryTimes);
            }
            else
            {
              resetFunc();  //call reset
            }
          }
#endif

          // Not in config mode, try reconnecting before forcing to config mode
          if ( !wifi_connected )
          {
            DEBUG_WM1(F("run: W lost. Reconnect"));
            if (connectToWifi(TIMEOUT_RECONNECT_WIFI))
            {
              DEBUG_WM1(F("run: W OK"));
            }
          }
        }
      }
      else if (configuration_mode)
      {
        configuration_mode = false;
        DEBUG_WM1(F("run: got W back"));
      }
    }

    void setConfigPortalIP(IPAddress portalIP = IPAddress(192, 168, 4, 1))
    {
      portal_apIP = portalIP;
    }

    void setConfigPortalChannel(int channel = 10)
    {
      AP_channel = channel;
    }
    
    void setConfigPortal(String ssid = "", String pass = "")
    {
      portal_ssid = ssid;
      portal_pass = pass;
    }

    void setSTAStaticIPConfig(IPAddress ip)
    {
      static_IP = ip;
    }

    ESP8266_AT_Configuration* getFullConfigData(ESP8266_AT_Configuration *configData)
    {
      if (!hadConfigData)
        getConfigData();

      // Check if NULL pointer
      if (configData)
        memcpy(configData, &ESP8266_AT_config, sizeof(ESP8266_AT_Configuration));

      return (configData);
    }

    String localIP(void)
    {
      uint16_t indexNextLine;

      ipAddress = IPAddressToString(WiFi.localIP());
      ipAddress.replace("+CIFSR:STAIP,\"", "");
      ipAddress.replace("\"", "");

      indexNextLine = ipAddress.indexOf("\n");
      ipAddress = ipAddress.substring(0, indexNextLine);

      return ipAddress;
    }

    void clearConfigData()
    {
      memset(&ESP8266_AT_config, 0, sizeof(ESP8266_AT_config));
      EEPROM.put(EEPROM_START, ESP8266_AT_config);
    }

    void resetFunc()
    {
      #if defined(__IMXRT1062__)
        // Teensy 4.0
        SCB_AIRCR = 0x05FA0004; //write value for restart for Teensy
      #else 
        void(*resetFunc)(void) = 0;
        resetFunc();
      #endif
    }

  private:
    String ipAddress = "0.0.0.0";

    ESP8266_AT_WebServer* server = NULL;
    bool configuration_mode = false;

    unsigned long configTimeout;
    bool hadConfigData = false;

    ESP8266_AT_Configuration ESP8266_AT_config;

    String macAddress = "";
    boolean wifi_connected = false;

    IPAddress portal_apIP = IPAddress(192, 168, 4, 1);
    int AP_channel = 10;

    String portal_ssid = "";
    String portal_pass = "";

    IPAddress static_IP   = IPAddress(0, 0, 0, 0);

    void displayConfigData(void)
    {
      DEBUG_WM6(F("Hdr="), ESP8266_AT_config.header, F(", SSID="), ESP8266_AT_config.wifi_ssid,
                 F(", PW="),   ESP8266_AT_config.wifi_pw);
    }

    void displayWiFiData(void)
    {
      DEBUG_WM2(F("IP="), localIP());
    }

#define ESP_AT_BOARD_TYPE   "SHD_ESP8266"
#define NO_CONFIG           "nothing"

    //#define EEPROM_SIZE       E2END
    //#define EEPROM_SIZE       512

    //KH
    // Teensy 4.0 :  EEPROM_SIZE = 3824 = (255 * 15) - 1, why 1080 ???
    // Teensy++2.0, 3.5 and 3.6 : EEPROM_SIZE = 4096
    // Teensy++1.0, 3.0, 3.1 and 3.2 : EEPROM_SIZE = 2048
    // Teensy2.0 : EEPROM_SIZE = 1024
    // Teensy1.0 : EEPROM_SIZE = 512
    // Teensy LC : EEPROM_SIZE = 128

    /*
      Teensy 4.0 => EEPROM_SIZE = 3824 = (255 * 15) - 1
      #define FLASH_SECTORS  15
      #if E2END > (255*FLASH_SECTORS-1)
      #error "E2END is set larger than the maximum possible EEPROM size"
      #endif
      ======================================================
      Teensy3.x
      #if defined(__MK20DX128__)      //Teensy 3.0
      #define EEPROM_MAX  2048
      #elif defined(__MK20DX256__)    //Teensy 3.1 and 3.2
      #define EEPROM_MAX  2048
      #elif defined(__MK64FX512__)    //Teensy 3.5
      #define EEPROM_MAX  4096
      #elif defined(__MK66FX1M0__)    //Teensy 3.6
      #define EEPROM_MAX  4096
      #elif defined(__MKL26Z64__)     //Teensy LC
      #define EEPROM_MAX  255
      #endif
      ======================================================
      Teensy 2.x
      Teensy 2.0
      #if defined(__AVR_ATmega32U4__)     //Teensy 2.0
      #elif defined(__AVR_AT90USB162__)   //Teensy 1.0
      #elif defined(__AVR_AT90USB646__)   //Teensy++ 1.0
      #elif defined(__AVR_AT90USB1286__)  //Teensy++ 2.0
    */

#ifndef EEPROM_SIZE
#define EEPROM_SIZE     512
#else
#if (EEPROM_SIZE > 4096)
#warning EEPROM_SIZE must be <= 4096. Reset to 4096
#undef EEPROM_SIZE
#define EEPROM_SIZE     4096
#endif
#if (EEPROM_SIZE < CONFIG_DATA_SIZE)
#warning EEPROM_SIZE must be > CONFIG_DATA_SIZE. Reset to 512
#undef EEPROM_SIZE
#define EEPROM_SIZE     512
#endif
#endif

#ifndef EEPROM_START
#define EEPROM_START     0
#else
#if (EEPROM_START + CONFIG_DATA_SIZE > EEPROM_SIZE)
#error EPROM_START + CONFIG_DATA_SIZE > EEPROM_SIZE. Please adjust.
#endif
#endif

    int calcChecksum()
    {
      int checkSum = 0;
      for (uint16_t index = 0; index < (sizeof(ESP8266_AT_config) - sizeof(ESP8266_AT_config.checkSum)); index++)
      {
        checkSum += * ( ( (byte*) &ESP8266_AT_config ) + index);
      }

      return checkSum;
    }

    bool getConfigData()
    {
      EEPROM.get(EEPROM_START, ESP8266_AT_config);

      int calChecksum = calcChecksum();

      DEBUG_WM4(F("CCsum="), calChecksum, F(",RCsum="), ESP8266_AT_config.checkSum);

      if ( (strncmp(ESP8266_AT_config.header, ESP_AT_BOARD_TYPE, strlen(ESP_AT_BOARD_TYPE)) != 0) ||
           (calChecksum != ESP8266_AT_config.checkSum) )
      {
        memset(&ESP8266_AT_config, 0, sizeof(ESP8266_AT_config));

        DEBUG_WM2(F("Init EEPROM sz="), EEPROM.length());
        // doesn't have any configuration
        strcpy(ESP8266_AT_config.header,           ESP_AT_BOARD_TYPE);
        strcpy(ESP8266_AT_config.wifi_ssid,        NO_CONFIG);
        strcpy(ESP8266_AT_config.wifi_pw,          NO_CONFIG);

        // Don't need
        ESP8266_AT_config.checkSum = 0;

        EEPROM.put(EEPROM_START, ESP8266_AT_config);

        return false;
      }
      else if ( !strncmp(ESP8266_AT_config.wifi_ssid,       NO_CONFIG, strlen(NO_CONFIG) ) ||
                !strncmp(ESP8266_AT_config.wifi_pw,         NO_CONFIG, strlen(NO_CONFIG) ) ||
                !strlen(ESP8266_AT_config.wifi_ssid) ||
                !strlen(ESP8266_AT_config.wifi_pw)  )
      {
        // If SSID, PW ="nothing", stay in config mode forever until having config Data.
        return false;
      }
      else
      {
        displayConfigData();
      }

      return true;
    }

    void saveConfigData()
    {
      int calChecksum = calcChecksum();
      ESP8266_AT_config.checkSum = calChecksum;
      DEBUG_WM4(F("Save EEPROM,sz="), EEPROM.length(), F(",chkSum="), calChecksum);

      EEPROM.put(EEPROM_START, ESP8266_AT_config);
    }

    boolean connectToWifi(int timeout)
    {
      int sleep_time = 250;
      unsigned long currMillis = millis();

      DEBUG_WM1(F("con2WF:start"));

      if (static_IP != IPAddress(0, 0, 0, 0))
      {
        DEBUG_WM1(F("Use SIP"));
        WiFi.config(static_IP);
      }

      while ( !wifi_connected && ( 0 < timeout ) && ( (millis() - currMillis) < (unsigned long) timeout )  )
      {
        DEBUG_WM2(F("con2WF:spent millis="), millis() - currMillis);

        if (connectWiFi(ESP8266_AT_config.wifi_ssid, ESP8266_AT_config.wifi_pw))
        {
          wifi_connected = true;
        }
        else
        {
          delay(sleep_time);
          timeout -= sleep_time;
        }
      }

      if (wifi_connected)
      {
        DEBUG_WM1(F("con2WF:OK"));
        displayWiFiData();
      }
      else
      {
        DEBUG_WM1(F("con2WF:failed"));
      }

      return wifi_connected;
    }

    void handleRequest()
    {
      if (server)
      {
        String key    = server->arg("key");
        String value  = server->arg("value");

        static int number_items_Updated = 0;

        if (key == "" && value == "")
        {
          String result = root_html_template;

          // Reset configTimeout to stay here until finished.
          configTimeout = 0;

          result.replace("[[id]]",     ESP8266_AT_config.wifi_ssid);
          result.replace("[[pw]]",     ESP8266_AT_config.wifi_pw);

          server->send(200, "text/html", result);

          return;
        }

        if (number_items_Updated == 0)
        {
          memset(&ESP8266_AT_config, 0, sizeof(ESP8266_AT_config));
          strcpy(ESP8266_AT_config.header, ESP_AT_BOARD_TYPE);
        }

        if (key == "id")
        {
          DEBUG_WM2(F("\nhR: id="), value.c_str() );
          number_items_Updated++;
          if (strlen(value.c_str()) < sizeof(ESP8266_AT_config.wifi_ssid) - 1)
            strcpy(ESP8266_AT_config.wifi_ssid, value.c_str());
          else
            strncpy(ESP8266_AT_config.wifi_ssid, value.c_str(), sizeof(ESP8266_AT_config.wifi_ssid) - 1);
        }
        else if (key == "pw")
        {
          DEBUG_WM2(F("\nhR: pw="), value.c_str() )
          number_items_Updated++;          
          if (strlen(value.c_str()) < sizeof(ESP8266_AT_config.wifi_pw) - 1)
            strcpy(ESP8266_AT_config.wifi_pw, value.c_str());
          else
            strncpy(ESP8266_AT_config.wifi_pw, value.c_str(), sizeof(ESP8266_AT_config.wifi_pw) - 1);
        }

        server->send(200, "text/html", "OK");

        if (number_items_Updated == NUM_CONFIGURABLE_ITEMS)
        {
          DEBUG_WM1(F("hR: Upd EEPROM"));

          saveConfigData();

          DEBUG_WM1(F("hR: Rst"));

          // TO DO : what command to reset
          // Delay then reset the board after save data
          delay(1000);
          resetFunc();  //call reset
        }
      }   // if (server)
    }

    void startConfigurationMode()
    {
#define CONFIG_TIMEOUT			60000L
      WiFi.reset();

      WiFi.configAP(portal_apIP);

      if ( (portal_ssid == "") || portal_pass == "" )
      {
        String randomNum = String(random(0xFFFFFF), HEX);
        randomNum.toUpperCase();

        portal_ssid = "ESP_AT_" + randomNum;
        portal_pass = "MyESP_AT_" + randomNum;
      }

      INFO_WM4(F("SSID="), portal_ssid, F(", PW="), portal_pass);
      INFO_WM4(F("IP="), portal_apIP, F(", CH="), AP_channel);

      // start access point, AP only, channel 10
      
      WiFi.beginAP(portal_ssid.c_str(), AP_channel, portal_pass.c_str(), ENC_TYPE_WPA2_PSK, true);

      if (!server)
      {
        server = new ESP8266_AT_WebServer;
      }

      // Don't know if it's working or not. Can we use ESP8266_Lib wifi TCP server for this ??

      //See https://stackoverflow.com/questions/39803135/c-unresolved-overloaded-function-type?rq=1

      if (server)
      {
        server->on("/", [this](){ handleRequest(); });
        server->begin();
      }

      // If there is no saved config Data, stay in config mode forever until having config Data.
      // or SSID, PW, Server,Token ="nothing"
      if (hadConfigData)
        configTimeout = millis() + CONFIG_TIMEOUT;
      else
        configTimeout = 0;

      configuration_mode = true;
    }
};


#endif    //Esp8266_AT_WM_Lite_Teensy_h
