/***************************************************************************************************************************************
  Esp8266_AT_WM_Lite.h
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

#ifndef Esp8266_AT_WM_Lite_h
#define Esp8266_AT_WM_Lite_h

#if ( defined(ESP8266) || defined(ESP32) || defined(CORE_TEENSY) )
  #error This code is not intended to run on the ESP8266, ESP32 nor Teensy platform! Please check your Tools->Board setting.
#endif

#if !( defined(ARDUINO_AVR_MEGA2560) || defined(ARDUINO_AVR_MEGA) )
  #error This code is intended to run on the Mega2560 platform! Please check your Tools->Board setting.
#endif

#ifndef ESP_AT_WM_LITE_VERSION
  #define ESP_AT_WM_LITE_VERSION            "ESP_AT_WM_Lite v1.5.2"

  #define ESP_AT_WM_LITE_VERSION_MAJOR      1
  #define ESP_AT_WM_LITE_VERSION_MINOR      5
  #define ESP_AT_WM_LITE_VERSION_PATCH      2

  #define ESP_AT_WM_LITE_VERSION_INT        1005002

#endif

#define DEFAULT_BOARD_NAME            "AVR-MEGA"

//////////////////////////////////////////////

#include <ESP8266_AT_WebServer.h>
#include <EEPROM.h>
#include <Esp8266_AT_WM_Lite_Debug.h>

///////// NEW for DRD /////////////
// These defines must be put before #include <DoubleResetDetector_Generic.h>
// to select where to store DoubleResetDetector_Generic's variable.
// Otherwise, library will use default EEPROM storage
#define  DRD_FLAG_DATA_SIZE     4

#ifndef DRD_GENERIC_DEBUG
  #define DRD_GENERIC_DEBUG     false
#endif

#include <DoubleResetDetector_Generic.h>      //https://github.com/khoih-prog/DoubleResetDetector_Generic

// Number of seconds after reset during which a
// subseqent reset will be considered a double reset.
#define DRD_TIMEOUT 10

// RTC Memory Address for the DoubleResetDetector_Generic to use
#define DRD_ADDRESS 0

DoubleResetDetector_Generic* drd;

///////// NEW for DRD /////////////

//NEW
#define MAX_ID_LEN                5
#define MAX_DISPLAY_NAME_LEN      16

typedef struct
{
  char id             [MAX_ID_LEN + 1];
  char displayName    [MAX_DISPLAY_NAME_LEN + 1];
  char *pdata;
  uint8_t maxlen;
} MenuItem;
//

#if USE_DYNAMIC_PARAMETERS
  #warning Using Dynamic Parameters
  ///NEW
  extern uint16_t NUM_MENU_ITEMS;
  extern MenuItem myMenuItems [];
  bool *menuItemUpdated = NULL;
#else
  #warning Not using Dynamic Parameters
#endif

// Configurable items besides fixed Header
#define NUM_CONFIGURABLE_ITEMS    2

//////////////////////////////////////////////

#define SSID_MAX_LEN      32
//From v1.0.3, WPA2 passwords can be up to 63 characters long.
#define PASS_MAX_LEN      64

#define HEADER_MAX_LEN            16

typedef struct Configuration
{
  char header         [HEADER_MAX_LEN];
  char wifi_ssid      [SSID_MAX_LEN];
  char wifi_pw        [PASS_MAX_LEN];
  int  checkSum;
} ESP8266_AT_Configuration;

// Currently CONFIG_DATA_SIZE  =   116
uint16_t CONFIG_DATA_SIZE = sizeof(ESP8266_AT_Configuration);

//////////////////////////////////////////////
// New in v1.0.3

extern bool LOAD_DEFAULT_CONFIG_DATA;
extern ESP8266_AT_Configuration defaultConfig;

//////////////////////////////////////////////

// New from v1.1.0
#define PASSWORD_MIN_LEN        8

//////////////////////////////////////////////

String IPAddressToString(const IPAddress& _address)
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

//////////////////////////////////////////////

class ESP_AT_WiFiManager_Lite
{
    public:
    
    ESP_AT_WiFiManager_Lite(Stream* espSerial, uint32_t speed = 115200)
    {
      (void) speed;
      
      WiFi.init(espSerial); 
      
      // check for the presence of the shield
      if (WiFi.status() == WL_NO_SHIELD) 
      {
        ESP_AT_LOGDEBUG(F("NoESP"));
      }
      
      //WiFi.reset();
    }
    
    //////////////////////////////////////////////

    ~ESP_AT_WiFiManager_Lite()
    {
    }
    
    //////////////////////////////////////////////
        
    bool connectWiFi(const char* ssid, const char* pass)
    {
      ESP_AT_LOGDEBUG1(F("Con2:"), ssid);

      if ( WiFi.begin(ssid, pass) )  
      {
        displayWiFiData();
      }
      else
      {
        ESP_AT_LOGDEBUG(F("NoW"));
        return false;
      }

      ESP_AT_LOGDEBUG(F("WOK"));

      wifi_connected = true;

      return true;
    }
    
    //////////////////////////////////////////////
   
    void begin(const char* ssid,
               const char* pass )
    {
      ESP_AT_LOGDEBUG(F("conW"));
      connectWiFi(ssid, pass);
    }
    
    //////////////////////////////////////////////

    void begin()
    {
#define RETRY_TIMES_CONNECT_WIFI			3

      // Due to notorious 2K buffer limitation of ESP8266-AT shield, the NUM_MENU_ITEMS is limited to max 3
      // to avoid WebServer not working due to HTML data larger than 2K can't be sent successfully
      // The items with index larger than 3 will be ignored
      // Limit NUM_MENU_ITEMS to max 3     
#if USE_DYNAMIC_PARAMETERS        
      if (NUM_MENU_ITEMS > 3)
        NUM_MENU_ITEMS = 3;
#endif

#if USING_CONFIG_MODE_LED  
      //Turn OFF
      pinMode(LED_BUILTIN, OUTPUT);
      digitalWrite(LED_BUILTIN, LED_OFF);
#endif
                 
      //// New DRD ////
      drd = new DoubleResetDetector_Generic(DRD_TIMEOUT, DRD_ADDRESS);  
      bool useConfigPortal = false;
   
      if (drd->detectDoubleReset())
      {
        ESP_AT_LOGDEBUG(F("Double Reset Detected"));
     
        useConfigPortal = true;
      }
      //// New DRD ////
      ESP_AT_LOGDEBUG(F("======= Start Default Config Data ======="));
      displayConfigData(defaultConfig);
      
      hadConfigData = getConfigData();
        
      //// New DRD ////
      //  useConfigPortal when getConfigData() not OK or DRD'ed
      if (hadConfigData && !useConfigPortal) 
      //// New DRD //// 
      {
        if (connectToWifi(RETRY_TIMES_CONNECT_WIFI))
        {
          ESP_AT_LOGDEBUG(F("b:WOK"));
        }
        else
        {
          ESP_AT_LOGDEBUG(F("b:NoW"));
          // failed to connect to WiFi, will start configuration mode
          startConfigurationMode();
        }
      }
      else
      {
        ESP_AT_LOGERROR1(F("b:StayInCfgPortal:"), useConfigPortal ? F("DRD") : F("NoCfgDat"));
        
        // failed to connect to WiFi, will start configuration mode
        hadConfigData = false;
        startConfigurationMode();
      }
    }
    
    //////////////////////////////////////////////

#ifndef RETRY_TIMES_RECONNECT_WIFI
  #define RETRY_TIMES_RECONNECT_WIFI   2
#else
  // Force range of user-defined RETRY_TIMES_RECONNECT_WIFI between 2-5 times
  #if (RETRY_TIMES_RECONNECT_WIFI < 2)
    #warning RETRY_TIMES_RECONNECT_WIFI too low. Reseting to 2
    #undef RETRY_TIMES_RECONNECT_WIFI
    #define RETRY_TIMES_RECONNECT_WIFI   2
  #elif (RETRY_TIMES_RECONNECT_WIFI > 5)
    #warning RETRY_TIMES_RECONNECT_WIFI too high. Reseting to 5
    #undef RETRY_TIMES_RECONNECT_WIFI
    #define RETRY_TIMES_RECONNECT_WIFI   5
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

    //////////////////////////////////////////////

#if !defined(WIFI_RECON_INTERVAL)      
  #define WIFI_RECON_INTERVAL       0         // default 0s between reconnecting WiFi
#else
  #if (WIFI_RECON_INTERVAL < 0)
    #define WIFI_RECON_INTERVAL     0
  #elif  (WIFI_RECON_INTERVAL > 600000)
    #define WIFI_RECON_INTERVAL     600000    // Max 10min
  #endif
#endif

    void run()
    {
      static int retryTimes = 0;
      static bool wifiDisconnectedOnce = false;
      
      // Lost connection in running. Give chance to reconfig.
      // Check WiFi status every 5s and update status
      // Check twice to be sure wifi disconnected is real
      static unsigned long checkstatus_timeout = 0;
      #define WIFI_STATUS_CHECK_INTERVAL    5000L
      
      static uint32_t curMillis;
      
      curMillis = millis();
      
      //// New DRD ////
      // Call the double reset detector loop method every so often,
      // so that it can recognise when the timeout expires.
      // You can also call drd.stop() when you wish to no longer
      // consider the next reset as a double reset.
      drd->loop();
      //// New DRD ////
         
      if ( !configuration_mode && (curMillis > checkstatus_timeout) )
      {       
        if (WiFi.status() == WL_CONNECTED)
        {
          wifi_connected = true;
          
#if USING_CONFIG_MODE_LED
          // turn the LED_BUILTIN OFF to tell us we exit configuration mode.
          digitalWrite(CONFIG_MODE_LED, LED_OFF);
#endif
        }
        else
        {
          if (wifiDisconnectedOnce)
          {
            wifiDisconnectedOnce = false;
            wifi_connected = false;
            ESP_AT_LOGERROR(F("r:Check&WLost"));
          }
          else
          {
            wifiDisconnectedOnce = true;
          }
        }
        
        checkstatus_timeout = curMillis + WIFI_STATUS_CHECK_INTERVAL;
      }    

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
            //ESP_AT_LOGDEBUG(F("r:handleClient"));
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
              ESP_AT_LOGERROR1(F("r:WLost&TOut.ConW.Retry#"), retryTimes);
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
            
 
#if (WIFI_RECON_INTERVAL > 0)

            static uint32_t lastMillis = 0;
            
            if ( (lastMillis == 0) || (curMillis - lastMillis) > WIFI_RECON_INTERVAL )
            {
              lastMillis = curMillis;
              
              ESP_AT_LOGERROR(F("r:WLost.ReconW"));
               
              if (connectToWifi(RETRY_TIMES_RECONNECT_WIFI))
              {
                ESP_AT_LOGERROR(F("r:WOK"));
              }
            }
#else
            ESP_AT_LOGERROR(F("r:WLost.ReconW"));
            
            if (connectToWifi(RETRY_TIMES_RECONNECT_WIFI))
            {
              ESP_AT_LOGERROR(F("r:WOK"));
            }
#endif            
          }
        }
      }
      else if (configuration_mode)
      {
        configuration_mode = false;
        ESP_AT_LOGERROR(F("r:gotWBack"));
        
#if USING_CONFIG_MODE_LED
          // turn the LED_BUILTIN OFF to tell us we exit configuration mode.
          digitalWrite(CONFIG_MODE_LED, LED_OFF);
#endif        
      }
    }
    
    //////////////////////////////////////////////

    void setConfigPortalIP(const IPAddress& portalIP = IPAddress(192, 168, 4, 1))
    {
      portal_apIP = portalIP;
    }
    
    //////////////////////////////////////////////

    #define MIN_WIFI_CHANNEL      1
    #define MAX_WIFI_CHANNEL      12    // Channel 13 is flaky, because of bad number 13 ;-)

    int setConfigPortalChannel(const int& channel = 1)
    {
      // If channel < MIN_WIFI_CHANNEL - 1 or channel > MAX_WIFI_CHANNEL => channel = 1
      // If channel == 0 => will use random channel from MIN_WIFI_CHANNEL to MAX_WIFI_CHANNEL
      // If (MIN_WIFI_CHANNEL <= channel <= MAX_WIFI_CHANNEL) => use it
      if ( (channel < MIN_WIFI_CHANNEL - 1) || (channel > MAX_WIFI_CHANNEL) )
        AP_channel = 1;
      else if ( (channel >= MIN_WIFI_CHANNEL - 1) && (channel <= MAX_WIFI_CHANNEL) )
        AP_channel = channel;

      return AP_channel;
    }
    
    //////////////////////////////////////////////
    
    void setConfigPortal(const String& ssid = "", const String& pass = "")
    {
      portal_ssid = ssid;
      portal_pass = pass;
    }
    
    //////////////////////////////////////////////

    void setSTAStaticIPConfig(const IPAddress& ip)
    {
      static_IP = ip;
    }
    
    //////////////////////////////////////////////
    
    String getWiFiSSID()
    {        
      if (!hadConfigData)
        getConfigData();

      return (String(ESP8266_AT_config.wifi_ssid));
    }
    
    //////////////////////////////////////////////

    String getWiFiPW()
    {
      if (!hadConfigData)
        getConfigData();

      return (String(ESP8266_AT_config.wifi_pw));
    }
    
    //////////////////////////////////////////////
    
    bool getWiFiStatus()
    {
      return wifi_connected;
    }
    
    //////////////////////////////////////////////

    ESP8266_AT_Configuration* getFullConfigData(ESP8266_AT_Configuration *configData)
    {
      if (!hadConfigData)
        getConfigData();

      // Check if NULL pointer
      if (configData)
        memcpy(configData, &ESP8266_AT_config, sizeof(ESP8266_AT_Configuration));

      return (configData);
    }
    
    //////////////////////////////////////////////

    String localIP()
    {
      uint16_t indexNextLine;

      ipAddress = IPAddressToString(WiFi.localIP());
      ipAddress.replace("+CIFSR:STAIP,\"", "");
      ipAddress.replace("\"", "");

      indexNextLine = ipAddress.indexOf("\n");
      ipAddress = ipAddress.substring(0, indexNextLine);

      return ipAddress;
    }
    
    //////////////////////////////////////////////

    void clearConfigData()
    {
      memset(&ESP8266_AT_config, 0, sizeof(ESP8266_AT_config));
      
#if USE_DYNAMIC_PARAMETERS      
      for (uint16_t i = 0; i < NUM_MENU_ITEMS; i++)
      {
        // Actual size of pdata is [maxlen + 1]
        memset(myMenuItems[i].pdata, 0, myMenuItems[i].maxlen + 1);
      }
#endif

      saveConfigData();
    }
    
    //////////////////////////////////////////////
    
    bool isConfigDataValid()
    {
      return hadConfigData;
    }
    
    //////////////////////////////////////////////
    
    bool isConfigMode()
    {
      return configuration_mode;
    }
    
    //////////////////////////////////////////////

    void resetFunc()
    {
      asm volatile ("  jmp 0");
    }
    
    //////////////////////////////////////////////

  private:
    String ipAddress = "0.0.0.0";

    ESP8266_AT_WebServer* server = NULL;
    bool configuration_mode = false;

    unsigned long configTimeout;
    bool hadConfigData = false;

    ESP8266_AT_Configuration ESP8266_AT_config;
    
    uint16_t totalDataSize = 0;

    String macAddress = "";
    bool wifi_connected = false;

    IPAddress portal_apIP = IPAddress(192, 168, 4, 1);
    int AP_channel = 10;

    String portal_ssid = "";
    String portal_pass = "";

    IPAddress static_IP   = IPAddress(0, 0, 0, 0);
    
    //////////////////////////////////////////////

    void displayConfigData(const ESP8266_AT_Configuration& configData)
    {
      ESP_AT_LOGDEBUG5(F("Hdr="),   configData.header, F(",SSID="), configData.wifi_ssid,
                F(",PW="),   configData.wifi_pw);  
      
#if USE_DYNAMIC_PARAMETERS     
      for (uint16_t i = 0; i < NUM_MENU_ITEMS; i++)
      {
        ESP_AT_LOGERROR5("i=", i, ",id=", myMenuItems[i].id, ",data=", myMenuItems[i].pdata);
      }
#endif    
    }
    
    //////////////////////////////////////////////

    void displayWiFiData()
    {
      ESP_AT_LOGDEBUG1(F("IP="), localIP() );
    }
    
    //////////////////////////////////////////////

#if USE_ESP32_AT
  #define ESP_AT_BOARD_TYPE   "SHD_ESP32"
#else
  #define ESP_AT_BOARD_TYPE   "SHD_ESP8266"
#endif

#define WM_NO_CONFIG        "blank"

//////////////////////////////////////////////

#ifndef EEPROM_SIZE
  #define EEPROM_SIZE     4096
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
  #define EEPROM_START     0      //define 256 in DRD
  #warning EEPROM_START not defined. Set to 0
#else
  #if (EEPROM_START + DRD_FLAG_DATA_SIZE + CONFIG_DATA_SIZE > EEPROM_SIZE)
    #error EPROM_START + DRD_FLAG_DATA_SIZE + CONFIG_DATA_SIZE > EEPROM_SIZE. Please adjust.
  #endif
#endif

// Stating positon to store Blynk8266_WM_config
#define CONFIG_EEPROM_START    (EEPROM_START + DRD_FLAG_DATA_SIZE)

    //////////////////////////////////////////////
    
    int calcChecksum()
    {
      int checkSum = 0;
      for (uint16_t index = 0; index < (sizeof(ESP8266_AT_config) - sizeof(ESP8266_AT_config.checkSum)); index++)
      {
        checkSum += * ( ( (byte*) &ESP8266_AT_config ) + index);
      }

      return checkSum;
    }
    
    //////////////////////////////////////////////

#if USE_DYNAMIC_PARAMETERS
   
    bool checkDynamicData()
    {
      int checkSum = 0;
      int readCheckSum;
      
      #define BUFFER_LEN      128
      char readBuffer[BUFFER_LEN + 1];
      
      uint16_t offset = CONFIG_EEPROM_START + sizeof(ESP8266_AT_config);
                
      // Find the longest pdata, then dynamically allocate buffer. Remember to free when done
      // This is used to store tempo data to calculate checksum to see of data is valid
      // We dont like to destroy myMenuItems[i].pdata with invalid data
      
      for (uint8_t i = 0; i < NUM_MENU_ITEMS; i++)
      {       
        if (myMenuItems[i].maxlen > BUFFER_LEN)
        {
          // Size too large, abort and flag false
          ESP_AT_LOGDEBUG(F("ChkCrR: Error Small Buffer."));
          return false;
        }
      }
         
      for (uint8_t i = 0; i < NUM_MENU_ITEMS; i++)
      {       
        char* _pointer = readBuffer;
        
        // Prepare buffer, more than enough
        memset(readBuffer, 0, sizeof(readBuffer));
        
        // Read more than necessary, but OK and easier to code
        EEPROM.get(offset, readBuffer);
        // NULL terminated
        readBuffer[myMenuItems[i].maxlen] = 0;
   
        ESP_AT_LOGDEBUG3(F("ChkCrR:pdata="), readBuffer, F(",len="), myMenuItems[i].maxlen);         
               
        for (uint16_t j = 0; j < myMenuItems[i].maxlen; j++,_pointer++)
        {         
          checkSum += *_pointer;  
        }   
        
        offset += myMenuItems[i].maxlen;    
      }

      EEPROM.get(offset, readCheckSum);
           
      ESP_AT_LOGDEBUG3(F("ChkCrR:CrCCsum=0x"), String(checkSum, HEX), F(",CrRCsum=0x"), String(readCheckSum, HEX));
           
      if ( checkSum != readCheckSum)
      {
        return false;
      }
      
      return true;    
    }
    
    //////////////////////////////////////////////
    
    bool EEPROM_getDynamicData()
    {
      int readCheckSum;
      int checkSum = 0;
      uint16_t offset = CONFIG_EEPROM_START + sizeof(ESP8266_AT_config);
           
      totalDataSize = sizeof(ESP8266_AT_config) + sizeof(readCheckSum);
      
      for (uint8_t i = 0; i < NUM_MENU_ITEMS; i++)
      {       
        char* _pointer = myMenuItems[i].pdata;
        totalDataSize += myMenuItems[i].maxlen;
        
        // Actual size of pdata is [maxlen + 1]
        memset(myMenuItems[i].pdata, 0, myMenuItems[i].maxlen + 1);
               
        for (uint16_t j = 0; j < myMenuItems[i].maxlen; j++,_pointer++,offset++)
        {
          *_pointer = EEPROM.read(offset);
          
          checkSum += *_pointer;  
         }       
      }
      
      EEPROM.get(offset, readCheckSum);
      
      ESP_AT_LOGDEBUG3(F("CrCCsum=0x"), String(checkSum, HEX), F(",CrRCsum=0x"), String(readCheckSum, HEX));
      
      if ( checkSum != readCheckSum)
      {
        return false;
      }
      
      return true;
    }
    
    //////////////////////////////////////////////

    void EEPROM_putDynamicData()
    {
      int checkSum = 0;
      uint16_t offset = CONFIG_EEPROM_START + sizeof(ESP8266_AT_config);
                
      for (uint8_t i = 0; i < NUM_MENU_ITEMS; i++)
      {       
        char* _pointer = myMenuItems[i].pdata;
        
        //ESP_AT_LOGDEBUG3(F("pdata="), myMenuItems[i].pdata, F(",len="), myMenuItems[i].maxlen);
                            
        for (uint16_t j = 0; j < myMenuItems[i].maxlen; j++,_pointer++,offset++)
        {
          EEPROM.write(offset, *_pointer);
          
          checkSum += *_pointer;     
         }
      }
      
      EEPROM.put(offset, checkSum);
      
      ESP_AT_LOGDEBUG1(F("CrCCSum=0x"), String(checkSum, HEX));
    }

#endif
    
    //////////////////////////////////////////////  
    
    bool isWiFiConfigValid()
    {
      // If SSID ="blank" or NULL, or PWD length < 8 (as required by standard) => invalid set
      // Need both sets of valid SSID/PWD
      if ( !strncmp(ESP8266_AT_config.wifi_ssid,  WM_NO_CONFIG, strlen(WM_NO_CONFIG) )  ||
           !strncmp(ESP8266_AT_config.wifi_pw,    WM_NO_CONFIG, strlen(WM_NO_CONFIG) )  ||
           ( strlen(ESP8266_AT_config.wifi_ssid) == 0 ) || 
           ( strlen(ESP8266_AT_config.wifi_pw)   < PASSWORD_MIN_LEN ) )
      {
        // If SSID, PW ="blank" or NULL, set the flag
        ESP_AT_LOGERROR(F("Invalid Stored WiFi Config Data"));
        
        // If SSID, PW ="blank" or NULL, or PPWD len < 8, set the flag      
        if ( ( strlen(ESP8266_AT_config.wifi_pw) < PASSWORD_MIN_LEN ) )
        {    
          ESP_AT_LOGERROR(F("WiFi PWD len < 8"));
        }
        else
        {
          ESP_AT_LOGERROR(F("SSID is blank or NULL"));
        }
        
        hadConfigData = false;
        
        return false;
      }
      
      return true;
    }
    
    //////////////////////////////////////////////
 
    void NULLTerminateConfig()
    {
      //#define HEADER_MAX_LEN      16
      
      // NULL Terminating to be sure
      ESP8266_AT_config.header    [HEADER_MAX_LEN - 1]      = 0;
      
      // For WiFi SSID/PWD here
      ESP8266_AT_config.wifi_ssid[SSID_MAX_LEN - 1] = 0;
      ESP8266_AT_config.wifi_pw[PASS_MAX_LEN - 1]   = 0;
    }
            
    //////////////////////////////////////////////
    
    bool EEPROM_get()
    {      
      EEPROM.get(CONFIG_EEPROM_START, ESP8266_AT_config);
      
      NULLTerminateConfig();
      
      // If WiFi SSID/PWD "blank" or NULL, or PWD len < 8, set false flag and exit
      return isWiFiConfigValid();
    }
    
    //////////////////////////////////////////////
    
    void EEPROM_put()
    {
      EEPROM.put(CONFIG_EEPROM_START, ESP8266_AT_config);  
    }
    
    //////////////////////////////////////////////

    void saveConfigData()
    {
      int calChecksum = calcChecksum();
      ESP8266_AT_config.checkSum = calChecksum;
      
      ESP_AT_LOGDEBUG5(F("SaveEEPROM,sz="), EEPROM.length(), F(",Datasz="), totalDataSize, F(",CSum="), calChecksum);

      EEPROM_put();

#if USE_DYNAMIC_PARAMETERS      
      EEPROM_putDynamicData();
#endif      
    }
    
    //////////////////////////////////////////////
    
    // New from v1.0.4
    void loadAndSaveDefaultConfigData()
    {
      // Load Default Config Data from Sketch
      memcpy(&ESP8266_AT_config, &defaultConfig, sizeof(ESP8266_AT_config));
      strcpy(ESP8266_AT_config.header, ESP_AT_BOARD_TYPE);
      
      // Including config and dynamic data, and assume valid
      saveConfigData();
          
      ESP_AT_LOGDEBUG(F("======= Start Loaded Config Data ======="));
      displayConfigData(ESP8266_AT_config);    
    }
    
    //////////////////////////////////////////////
    
    bool getConfigData()
    {            
      bool dynamicDataValid = true;
      int calChecksum;   
      
      hadConfigData = false; 
      
      EEPROM.begin();
            
      // Use new LOAD_DEFAULT_CONFIG_DATA logic
      if (LOAD_DEFAULT_CONFIG_DATA)
      {     
        // Load Config Data from Sketch
        loadAndSaveDefaultConfigData();
        
        // Don't need Config Portal anymore
        return true;
      }
      else
      {   
        // Load stored config data from EEPROM
        ESP_AT_LOGDEBUG1(F("EEPROMsz:"), EEPROM_SIZE);
        
        // If WiFi SSID/PWD "blank" or NULL, or PWD len < 8, set false flag and exit
        if (!EEPROM_get())
        {
          return false;
        }
       
        calChecksum = calcChecksum();

        ESP_AT_LOGDEBUG3(F("CCSum=0x"), String(calChecksum, HEX), F(",RCSum=0x"), String(ESP8266_AT_config.checkSum, HEX));

#if USE_DYNAMIC_PARAMETERS   
        // Load stored dynamic data from EEPROM
        // Verify ChkSum
        dynamicDataValid = checkDynamicData();
#endif

        // If checksum = 0 => simulated EEPROM has been cleared (by uploading new FW, etc) => force to CP
        // If bad checksum = 0 => force to CP
        if ( (calChecksum != 0) && (calChecksum == ESP8266_AT_config.checkSum) )
        {   
          if (dynamicDataValid)
          {
  #if USE_DYNAMIC_PARAMETERS        
            EEPROM_getDynamicData();
               
            ESP_AT_LOGDEBUG(F("Valid Stored Dynamic Data"));   
  #endif
                 
            ESP_AT_LOGDEBUG(F("======= Start Stored Config Data ======="));
            displayConfigData(ESP8266_AT_config);
            
            // Don't need Config Portal anymore
            return true;
          }
          else
          {
            // Invalid Stored config data => Config Portal
            ESP_AT_LOGDEBUG(F("Invalid Stored Dynamic Data. Load default from Sketch"));
            
            // Load Default Config Data from Sketch, better than just "blank"
            loadAndSaveDefaultConfigData();
                             
            // Need Config Portal here as data can be just dummy
            // Even if you don't open CP, you're OK on next boot if your default config data is valid 
            return false;
          }      
        }
      }
      
      if ( (strncmp(ESP8266_AT_config.header, ESP_AT_BOARD_TYPE, strlen(ESP_AT_BOARD_TYPE)) != 0) ||
           (calChecksum != ESP8266_AT_config.checkSum) || !dynamicDataValid ||
           ( (calChecksum == 0) && (ESP8266_AT_config.checkSum == 0) ) )
      {
        // Including Credentials CSum
        ESP_AT_LOGDEBUG1(F("InitCfgDat,Sz="), sizeof(ESP8266_AT_config));

        // doesn't have any configuration        
        if (LOAD_DEFAULT_CONFIG_DATA)
        {
          memcpy(&ESP8266_AT_config, &defaultConfig, sizeof(ESP8266_AT_config));
        }
        else
        {
          memset(&ESP8266_AT_config, 0, sizeof(ESP8266_AT_config));

#if USE_DYNAMIC_PARAMETERS
          for (uint8_t i = 0; i < NUM_MENU_ITEMS; i++)
          {
            // Actual size of pdata is [maxlen + 1]
            memset(myMenuItems[i].pdata, 0, myMenuItems[i].maxlen + 1);
          }
#endif
              
          strcpy(ESP8266_AT_config.wifi_ssid,   WM_NO_CONFIG);
          strcpy(ESP8266_AT_config.wifi_pw,     WM_NO_CONFIG);

#if USE_DYNAMIC_PARAMETERS          
          for (uint8_t i = 0; i < NUM_MENU_ITEMS; i++)
          {
            strncpy(myMenuItems[i].pdata, WM_NO_CONFIG, myMenuItems[i].maxlen);
          }
#endif          
        }
    
        strcpy(ESP8266_AT_config.header, ESP_AT_BOARD_TYPE);

#if USE_DYNAMIC_PARAMETERS        
        for (uint8_t i = 0; i < NUM_MENU_ITEMS; i++)
        {
          ESP_AT_LOGDEBUG3(F("g:myMenuItems["), i, F("]="), myMenuItems[i].pdata );
        }
#endif
        
        // Don't need
        ESP8266_AT_config.checkSum = 0;

        saveConfigData();
        
        return false;  
      }
      else if ( !isWiFiConfigValid() )
      {
        // If SSID, PW ="blank" or NULL, or PWD len < 8
        // stay in config mode forever until having config Data.
        return false;
      }
      else
      {
        displayConfigData(ESP8266_AT_config);
      }

      return true;
    }
    
    //////////////////////////////////////////////
    
// Max times to try WiFi per loop() iteration. To avoid blocking issue in loop()
// Default 1 and minimum 1.
#if !defined(MAX_NUM_WIFI_RECON_TRIES_PER_LOOP)      
  #define MAX_NUM_WIFI_RECON_TRIES_PER_LOOP     1
#else
  #if (MAX_NUM_WIFI_RECON_TRIES_PER_LOOP < 1)  
    #define MAX_NUM_WIFI_RECON_TRIES_PER_LOOP     1
  #endif
#endif    

    // New connection logic for ESP32-AT from v1.0.6
    bool connectToWifi(int retry_time)
    {
      int sleep_time  = 250;
      uint8_t status  = WL_IDLE_STATUS;
                       
      ESP_AT_LOGDEBUG(F("Con2Wifi"));

      if (static_IP != IPAddress(0, 0, 0, 0))
      {
        ESP_AT_LOGDEBUG(F("UseStatIP"));
        WiFi.config(static_IP);
      }
         
      ESP_AT_LOGDEBUG3(F("con2WF:SSID="), ESP8266_AT_config.wifi_ssid, F(",PW="), ESP8266_AT_config.wifi_pw);
      
      uint8_t numWiFiReconTries = 0;
             
      while ( !wifi_connected && ( 0 < retry_time ) && (numWiFiReconTries++ < MAX_NUM_WIFI_RECON_TRIES_PER_LOOP) )
      {      
        ESP_AT_LOGDEBUG1(F("Remaining retry_time="), retry_time);
        
        status = WiFi.begin(ESP8266_AT_config.wifi_ssid, ESP8266_AT_config.wifi_pw); 
            
        // Need restart WiFi at beginning of each cycle 
        if (status == WL_CONNECTED)
        {
          wifi_connected = true;                   
          break;
        }
        else
        {
          delay(sleep_time);
          retry_time--;
        }         
      }
          
      if (retry_time <= 0)
      {      
        ESP_AT_LOGDEBUG1(F("Con2WF:Failed, retry_time="), retry_time);             
      }  

      if (wifi_connected)
      {
        ESP_AT_LOGDEBUG(F("con2WF:OK"));
        displayWiFiData();
      }
      else
      {
        ESP_AT_LOGDEBUG(F("con2WF:failed"));  
        // Can't connect, so try another index next time. Faking this index is OK and lost
      }

      return wifi_connected;  
    }
    
    //////////////////////////////////////////////
      
    void createHTML(String& root_html_template)
    {   
      // -- HTML page fragments
      root_html_template= "<!DOCTYPE html><html><head><title>AVR_WM_Lite</title><style>div,input{padding:5px;font-size:1em;}input{width:95%;}body{text-align: center;}button{background-color:#16A1E7;color:#fff;line-height:2.4rem;font-size:1.2rem;width:100%;}fieldset{border-radius:0.3rem;margin:0px;}</style></head><div style=\"text-align:left;display:inline-block;min-width:260px;\"><fieldset><div><label>*SSID</label><input value=\"[[id]]\"id=\"id\"><div></div></div><div><label>*PWD (8+ chars)</label><input value=\"[[pw]]\"id=\"pw\"><div></div></div></fieldset>";

      String ESP_AT_FLDSET_START = "<fieldset>";
      String ESP_AT_FLDSET_END     = "</fieldset>";
      String ESP_AT_HTML_PARAM    = "<div><label>{b}</label><input value='[[{v}]]'id='{i}'><div></div></div>";
      String ESP_AT_HTML_BUTTON    = "<button onclick=\"sv()\">Save</button></div>";
      String ESP_AT_HTML_SCRIPT    = "<script id=\"jsbin-javascript\">function udVal(key,val){var request=new XMLHttpRequest();var url='/?key='+key+'&value='+encodeURIComponent(val);request.open('GET',url,false);request.send(null);}\
      function sv(){udVal('id',document.getElementById('id').value);udVal('pw',document.getElementById('pw').value);";

      String ESP_AT_HTML_SCRIPT_ITEM   = "udVal('{d}',document.getElementById('{d}').value);";

      String ESP_AT_HTML_SCRIPT_END   = "alert('Updated');}</script>";

      String ESP_AT_HTML_END   = "</html>";
      ///

      String pitem;
      
#if USE_DYNAMIC_PARAMETERS     
      if (NUM_MENU_ITEMS > 0)
      { 
        root_html_template += ESP_AT_FLDSET_START;
        
        for (uint8_t i = 0; i < NUM_MENU_ITEMS; i++)
        {
          pitem = ESP_AT_HTML_PARAM;
          pitem.replace("{b}", myMenuItems[i].displayName);
          pitem.replace("{v}", myMenuItems[i].id);
          pitem.replace("{i}", myMenuItems[i].id);
          
          root_html_template += pitem;
        }
        
        root_html_template += ESP_AT_FLDSET_END;
      }
#endif
       
      root_html_template += ESP_AT_HTML_BUTTON;
      root_html_template += ESP_AT_HTML_SCRIPT;

#if USE_DYNAMIC_PARAMETERS   
      if (NUM_MENU_ITEMS > 0)
      {   
        for (uint8_t i = 0; i < NUM_MENU_ITEMS; i++)
        {
          pitem = ESP_AT_HTML_SCRIPT_ITEM;
          
          pitem.replace("{d}", myMenuItems[i].id);
          
          root_html_template += pitem;
        }
      }
#endif
      
      root_html_template += ESP_AT_HTML_SCRIPT_END;
      root_html_template += ESP_AT_HTML_END;
           
      return;     
    }
    
    //////////////////////////////////////////////

    void handleRequest()
    {
      if (server)
      {
        String key    = server->arg("key");
        String value  = server->arg("value");

        static uint8_t number_items_Updated = 0;

        if (key == "" && value == "")
        {
          String result;
          createHTML(result);
          
          ESP_AT_LOGDEBUG(F("h:repl"));

          // Reset configTimeout to stay here until finished.
          configTimeout = 0;
                   
          if (hadConfigData)
          {         
            result.replace("[[id]]", ESP8266_AT_config.wifi_ssid);
            result.replace("[[pw]]", ESP8266_AT_config.wifi_pw);
          }
          else
          {
            result.replace("[[id]]", "");
            result.replace("[[pw]]", "");
          }
          
#if USE_DYNAMIC_PARAMETERS          
          for (uint8_t i = 0; i < NUM_MENU_ITEMS; i++)
          {
            String toChange = String("[[") + myMenuItems[i].id + "]]";
            result.replace(toChange, myMenuItems[i].pdata);
          }
#endif
          
          // Check if HTML size is larger than 2K, warn that WebServer won't work
          // because of notorious 2K buffer limitation of ESP8266-AT. 
          // Use conservative value 2000 instead of 2048
          uint16_t HTML_page_size = result.length();
          
          ESP_AT_LOGDEBUG1(F("h:HTML page size:"), HTML_page_size);
          
          if (HTML_page_size > 2000)
          {
            ESP_AT_LOGERROR(F("h:HTML page larger than 2K. Config Portal won't work. Reduce dynamic params"));
          }   
          
          server->send(200, "text/html", result);

          return;
        }

        if (number_items_Updated == 0)
        {
          memset(&ESP8266_AT_config, 0, sizeof(ESP8266_AT_config));
          strcpy(ESP8266_AT_config.header, ESP_AT_BOARD_TYPE);
        }

#if USE_DYNAMIC_PARAMETERS
        if (!menuItemUpdated)
        {
          // Don't need to free
          menuItemUpdated = new bool[NUM_MENU_ITEMS];
          
          if (menuItemUpdated)
          {
            for (uint16_t i = 0; i < NUM_MENU_ITEMS; i++)
            {           
              // To flag item is not yet updated
              menuItemUpdated[i] = false;       
            }
            
            ESP_AT_LOGDEBUG1(F("h: Init menuItemUpdated :" ), NUM_MENU_ITEMS);                    
          }
          else
          {
            ESP_AT_LOGERROR(F("h: Error can't alloc memory for menuItemUpdated" ));
          }
        }  
#endif

        static bool id_Updated  = false;
        static bool pw_Updated  = false;
        
        if (!id_Updated && (key == String("id")))
        {
          ESP_AT_LOGDEBUG(F("h:repl id"));
          id_Updated = true;
          
          number_items_Updated++;
          
          if (strlen(value.c_str()) < sizeof(ESP8266_AT_config.wifi_ssid) - 1)
            strcpy(ESP8266_AT_config.wifi_ssid, value.c_str());
          else
            strncpy(ESP8266_AT_config.wifi_ssid, value.c_str(), sizeof(ESP8266_AT_config.wifi_ssid) - 1);
        }
        else if (!pw_Updated && (key == String("pw")))
        {
          ESP_AT_LOGDEBUG(F("h:repl pw"));
          pw_Updated = true;
          
          number_items_Updated++;
          
          if (strlen(value.c_str()) < sizeof(ESP8266_AT_config.wifi_pw) - 1)
            strcpy(ESP8266_AT_config.wifi_pw, value.c_str());
          else
            strncpy(ESP8266_AT_config.wifi_pw, value.c_str(), sizeof(ESP8266_AT_config.wifi_pw) - 1);
        }
        else
        {
        
#if USE_DYNAMIC_PARAMETERS        
          for (uint16_t i = 0; i < NUM_MENU_ITEMS; i++)
          {           
            if ( !menuItemUpdated[i] && (key == myMenuItems[i].id) )
            {
              ESP_AT_LOGDEBUG3(F("h:"), myMenuItems[i].id, F("="), value.c_str() );
              
              menuItemUpdated[i] = true;
              
              number_items_Updated++;

              // Actual size of pdata is [maxlen + 1]
              memset(myMenuItems[i].pdata, 0, myMenuItems[i].maxlen + 1);

              if ((int) strlen(value.c_str()) < myMenuItems[i].maxlen)
                strcpy(myMenuItems[i].pdata, value.c_str());
              else
                strncpy(myMenuItems[i].pdata, value.c_str(), myMenuItems[i].maxlen);
                
              break;  
            }
          }
#endif
        }
        
        ESP_AT_LOGDEBUG1(F("h:items updated ="), number_items_Updated);
        ESP_AT_LOGDEBUG3(F("h:key ="), key, ", value =", value);
        
        server->send(200, "text/html", "OK");

#if USE_DYNAMIC_PARAMETERS        
        if (number_items_Updated == NUM_CONFIGURABLE_ITEMS + NUM_MENU_ITEMS)
#else
        if (number_items_Updated == NUM_CONFIGURABLE_ITEMS)
#endif
        {
          ESP_AT_LOGDEBUG(F("h:UpdEEPROM"));

          saveConfigData();

          ESP_AT_LOGDEBUG(F("h:Rst"));

          // TO DO : what command to reset
          // Delay then reset the board after save data
          delay(1000);
          resetFunc();  //call reset
        }
      }   // if (server)
    }
    
    //////////////////////////////////////////////

#ifndef CONFIG_TIMEOUT
  #warning Default CONFIG_TIMEOUT = 60s
  #define CONFIG_TIMEOUT			60000L
#endif

    void startConfigurationMode()
    {
#if USING_CONFIG_MODE_LED
      // turn the LED_BUILTIN ON to tell us we enter configuration mode.
      digitalWrite(CONFIG_MODE_LED, LED_ON);
#endif
    
      WiFi.configAP(portal_apIP);

      if ( (portal_ssid == "") || portal_pass == "" )
      {
        String randomNum = String(random(0xFFFFFF), HEX);
        randomNum.toUpperCase();

        portal_ssid = "ESP_AT_" + randomNum;
        portal_pass = "MyESP_AT_" + randomNum;
      }

      // start access point, AP only, channel 10
           
      uint16_t channel;
     
      // Use random channel if  AP_channel == 0     
      if (AP_channel == 0)
        channel = (millis() % MAX_WIFI_CHANNEL) + 1;     //random(MAX_WIFI_CHANNEL) + 1;
      else
        channel = AP_channel;

      ESP_AT_LOGERROR3(F("SSID="), portal_ssid, F(",PW="), portal_pass);
      ESP_AT_LOGERROR3(F("IP="), portal_apIP, F(",CH="), channel);
     
      WiFi.beginAP(portal_ssid.c_str(), channel, portal_pass.c_str(), ENC_TYPE_WPA2_PSK, true);

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

#endif    //Esp8266_AT_WM_Lite_h
