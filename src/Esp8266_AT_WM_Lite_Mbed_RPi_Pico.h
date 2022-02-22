/***************************************************************************************************************************************
  Esp8266_AT_WM_Lite_Mbed_RPi_Pico.h
  For RP2040-based boards, such as RASPBERRY_PI_PICO, using ESP8266 WiFi Shields

  ESP_AT_WM_Lite is a library for the Mega, Teensy, SAM DUE, SAMD and STM32, nRF52, RPi_Pico boards (https://github.com/khoih-prog/ESP_AT_WM_Lite)
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

#ifndef Esp8266_AT_WM_Lite_Mbed_RPi_Pico_h
#define Esp8266_AT_WM_Lite_Mbed_RPi_Pico_h

#if ( defined(ARDUINO_ARCH_RP2040) )
  #if defined(ESP8266_AT_USE_RPI_PICO)
    #undef ESP8266_AT_USE_RPI_PICO
  #endif
  #define ESP8266_AT_USE_RPI_PICO      true
  #warning Use RPI_PICO architecture from ESP8266_AT_WM_Lite
#else
  #error This code is intended to run on the RP2040 platform! Please check your Tools->Board setting.
#endif

#ifndef ESP_AT_WM_LITE_VERSION
  #define ESP_AT_WM_LITE_VERSION            "ESP_AT_WM_Lite v1.5.2"

  #define ESP_AT_WM_LITE_VERSION_MAJOR      1
  #define ESP_AT_WM_LITE_VERSION_MINOR      5
  #define ESP_AT_WM_LITE_VERSION_PATCH      2

  #define ESP_AT_WM_LITE_VERSION_INT        1005002

#endif

#define DEFAULT_BOARD_NAME            "MBED-RP2040"

//////////////////////////////////////////////

#include <ESP8266_AT_WebServer.h>

//////////////////////////////////////////

#warning Using LittleFS in Esp8266_AT_WM_Lite_Mbed_RPi_Pico.h

//////////////////////////////////////////

#include <Esp8266_AT_WM_Lite_Debug.h>

//////////////////////////////////////////////

#ifndef USING_CUSTOMS_STYLE
  #define USING_CUSTOMS_STYLE     false
#endif

#ifndef USING_CUSTOMS_HEAD_ELEMENT
  #define USING_CUSTOMS_HEAD_ELEMENT     false
#endif

#ifndef USING_CORS_FEATURE
  #define USING_CORS_FEATURE     false
#endif

//////////////////////////////////////////////

// New from v1.2.0
// KH, Some minor simplification
#if !defined(SCAN_WIFI_NETWORKS)
  #define SCAN_WIFI_NETWORKS     true     //false
#endif
	
#if SCAN_WIFI_NETWORKS
  #if !defined(MANUAL_SSID_INPUT_ALLOWED)
    #define MANUAL_SSID_INPUT_ALLOWED     true
  #endif

  #if !defined(MAX_SSID_IN_LIST)
    #define MAX_SSID_IN_LIST     6
  #elif (MAX_SSID_IN_LIST < 2)
    #warning Parameter MAX_SSID_IN_LIST defined must be >= 2 - Reset to 6
    #undef MAX_SSID_IN_LIST
    #define MAX_SSID_IN_LIST      6
  #elif (MAX_SSID_IN_LIST > 15)
    #warning Parameter MAX_SSID_IN_LIST defined must be <= 6 - Reset to 6
    #undef MAX_SSID_IN_LIST
    #define MAX_SSID_IN_LIST      6
  #endif
#else
  #warning SCAN_WIFI_NETWORKS disabled	
#endif

///////// NEW for DRD /////////////
// These defines must be put before #include <DoubleResetDetector_Generic.h>
// to select where to store DoubleResetDetector_Generic's variable.
// Otherwise, library will use default EEPROM storage
#define  DRD_FLAG_DATA_SIZE     4

#ifndef DOUBLERESETDETECTOR_DEBUG
  #define DOUBLERESETDETECTOR_DEBUG     false
#endif

#include <DoubleResetDetector_Generic.h>      //https://github.com/khoih-prog/DoubleResetDetector_Generic

// Number of seconds after reset during which a
// subsequent reset will be considered a double reset.
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

//////////////////////////////////////////////
// New in v1.0.3

#define SSID_MAX_LEN      32
//From v1.1.0, WPA2 passwords can be up to 63 characters long.
#define PASS_MAX_LEN      64

typedef struct
{
  char wifi_ssid[SSID_MAX_LEN];
  char wifi_pw  [PASS_MAX_LEN];
}  WiFi_Credentials;

#define NUM_WIFI_CREDENTIALS      2

#if USING_BOARD_NAME
  // Configurable items besides fixed Header, just add board_name 
  #define NUM_CONFIGURABLE_ITEMS    ( ( 2 * NUM_WIFI_CREDENTIALS ) + 1 )
#else
  // Configurable items besides fixed Header, just add board_name 
  #define NUM_CONFIGURABLE_ITEMS    ( ( 2 * NUM_WIFI_CREDENTIALS ))
#endif

////////////////

#define HEADER_MAX_LEN            16
#define BOARD_NAME_MAX_LEN        24

typedef struct Configuration
{
  char header         [HEADER_MAX_LEN];
  WiFi_Credentials  WiFi_Creds  [NUM_WIFI_CREDENTIALS];
  char board_name     [BOARD_NAME_MAX_LEN];
  int  checkSum;
} ESP8266_AT_Configuration;

// Currently CONFIG_DATA_SIZE  =   236  = (16 + 96 * 2 + 4 + 24)
uint16_t CONFIG_DATA_SIZE = sizeof(ESP8266_AT_Configuration);

//////////////////////////////////////////////

///New from v1.0.3
extern bool LOAD_DEFAULT_CONFIG_DATA;
extern ESP8266_AT_Configuration defaultConfig;

// -- HTML page fragments
const char ESP_AT_HTML_HEAD_START[] /*PROGMEM*/ = "<!DOCTYPE html><html><head><title>RPi_Pico_AT_WM_Lite</title>";

const char ESP_AT_HTML_HEAD_STYLE[] /*PROGMEM*/ = "<style>div,input{padding:5px;font-size:1em;}input{width:95%;}body{text-align: center;}button{background-color:#16A1E7;color:#fff;line-height:2.4rem;font-size:1.2rem;width:100%;}fieldset{border-radius:0.3rem;margin:0px;}</style>";

#if USING_BOARD_NAME
const char ESP_AT_HTML_HEAD_END[]   /*PROGMEM*/ = "</head><div style='text-align:left;display:inline-block;min-width:260px;'>\
<fieldset><div><label>*WiFi SSID</label><div>[[input_id]]</div></div>\
<div><label>*PWD (8+ chars)</label><input value='[[pw]]' id='pw'><div></div></div>\
<div><label>*WiFi SSID1</label><div>[[input_id1]]</div></div>\
<div><label>*PWD1 (8+ chars)</label><input value='[[pw1]]' id='pw1'><div></div></div></fieldset>\
<fieldset><div><label>Board Name</label><input value='[[nm]]' id='nm'><div></div></div></fieldset>";	// DO NOT CHANGE THIS STRING EVER!!!!
#else
const char ESP_AT_HTML_HEAD_END[]   /*PROGMEM*/ = "</head><div style='text-align:left;display:inline-block;min-width:260px;'>\
<fieldset><div><label>*WiFi SSID</label><div>[[input_id]]</div></div>\
<div><label>*PWD (8+ chars)</label><input value='[[pw]]' id='pw'><div></div></div>\
<div><label>*WiFi SSID1</label><div>[[input_id1]]</div></div>\
<div><label>*PWD1 (8+ chars)</label><input value='[[pw1]]' id='pw1'><div></div></div></fieldset>";	// DO NOT CHANGE THIS STRING EVER!!!!
#endif

const char ESP_AT_HTML_INPUT_ID[]   /*PROGMEM*/ = "<input value='[[id]]' id='id'>";
const char ESP_AT_HTML_INPUT_ID1[]  /*PROGMEM*/ = "<input value='[[id1]]' id='id1'>";

const char ESP_AT_FLDSET_START[]  /*PROGMEM*/ = "<fieldset>";
const char ESP_AT_FLDSET_END[]    /*PROGMEM*/ = "</fieldset>";
const char ESP_AT_HTML_PARAM[]    /*PROGMEM*/ = "<div><label>{b}</label><input value='[[{v}]]'id='{i}'><div></div></div>";
const char ESP_AT_HTML_BUTTON[]   /*PROGMEM*/ = "<button onclick='sv()'>Save</button></div>";

#if USING_BOARD_NAME
const char ESP_AT_HTML_SCRIPT[]   /*PROGMEM*/ = "<script id=\"jsbin-javascript\">\
function udVal(key,val){var request=new XMLHttpRequest();var url='/?key='+key+'&value='+encodeURIComponent(val);\
request.open('GET',url,false);request.send(null);}\
function sv(){udVal('id',document.getElementById('id').value);udVal('pw',document.getElementById('pw').value);\
udVal('id1',document.getElementById('id1').value);udVal('pw1',document.getElementById('pw1').value);\
udVal('nm',document.getElementById('nm').value);";
#else
const char ESP_AT_HTML_SCRIPT[]   /*PROGMEM*/ = "<script id=\"jsbin-javascript\">\
function udVal(key,val){var request=new XMLHttpRequest();var url='/?key='+key+'&value='+encodeURIComponent(val);\
request.open('GET',url,false);request.send(null);}\
function sv(){udVal('id',document.getElementById('id').value);udVal('pw',document.getElementById('pw').value);\
udVal('id1',document.getElementById('id1').value);udVal('pw1',document.getElementById('pw1').value);";
#endif

const char ESP_AT_HTML_SCRIPT_ITEM[]  /*PROGMEM*/ = "udVal('{d}',document.getElementById('{d}').value);";
const char ESP_AT_HTML_SCRIPT_END[]   /*PROGMEM*/ = "alert('Updated');}</script>";
const char ESP_AT_HTML_END[]          /*PROGMEM*/ = "</html>";

#if SCAN_WIFI_NETWORKS
const char WM_SELECT_START[]      /*PROGMEM*/ = "<select id=";
const char WM_SELECT_END[]        /*PROGMEM*/ = "</select>";
const char WM_DATALIST_START[]    /*PROGMEM*/ = "<datalist id=";
const char WM_DATALIST_END[]      /*PROGMEM*/ = "</datalist>";
const char WM_OPTION_START[]      /*PROGMEM*/ = "<option>";
const char WM_OPTION_END[]        /*PROGMEM*/ = "";			// "</option>"; is not required
const char WM_NO_NETWORKS_FOUND[] /*PROGMEM*/ = "No suitable WiFi networks available!";
#endif

//////////////////////////////////////////

//KH Add repeatedly used const
//KH, from v1.1.0
const char WM_HTTP_HEAD_CL[]         PROGMEM = "Content-Length";
const char WM_HTTP_HEAD_TEXT_HTML[]  PROGMEM = "text/html";
const char WM_HTTP_HEAD_TEXT_PLAIN[] PROGMEM = "text/plain";

const char WM_HTTP_CACHE_CONTROL[]   PROGMEM = "Cache-Control";
const char WM_HTTP_NO_STORE[]        PROGMEM = "no-cache, no-store, must-revalidate";
const char WM_HTTP_PRAGMA[]          PROGMEM = "Pragma";
const char WM_HTTP_NO_CACHE[]        PROGMEM = "no-cache";
const char WM_HTTP_EXPIRES[]         PROGMEM = "Expires";

const char WM_HTTP_CORS[]            PROGMEM = "Access-Control-Allow-Origin";
const char WM_HTTP_CORS_ALLOW_ALL[]  PROGMEM = "*";

//////////////////////////////////////////////

// New from v1.1.0
#if !defined(REQUIRE_ONE_SET_SSID_PW)
  #define REQUIRE_ONE_SET_SSID_PW     false
#endif

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
      if (server)
      {
        delete server;

#if SCAN_WIFI_NETWORKS
        if (indices)
        {
          free(indices); //indices array no longer required so free memory
        }
#endif
      }
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
      ESP_AT_LOGDEBUG(F("conWiFi"));
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
        if (connectMultiWiFi(RETRY_TIMES_CONNECT_WIFI))
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
               
              if (connectMultiWiFi(RETRY_TIMES_RECONNECT_WIFI))
              {
                ESP_AT_LOGERROR(F("r:WOK"));
              }
            }
#else
            ESP_AT_LOGERROR(F("r:WLost.ReconW"));
            
            if (connectMultiWiFi(RETRY_TIMES_RECONNECT_WIFI))
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
    #define MAX_WIFI_CHANNEL      11    // Channel 12,13 is flaky, because of bad number 13 ;-)

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
    
    String getWiFiSSID(const uint8_t& index)
    { 
      if (index >= NUM_WIFI_CREDENTIALS)
        return String("");
        
      if (!hadConfigData)
        getConfigData();

      return (String(ESP8266_AT_config.WiFi_Creds[index].wifi_ssid));
    }
    
    //////////////////////////////////////////////

    String getWiFiPW(const uint8_t& index)
    {
      if (index >= NUM_WIFI_CREDENTIALS)
        return String("");
        
      if (!hadConfigData)
        getConfigData();

      return (String(ESP8266_AT_config.WiFi_Creds[index].wifi_pw));
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
      ipAddress.replace("+CIFSR:STAIP,'", "");
      ipAddress.replace("'", "");

      indexNextLine = ipAddress.indexOf("\n");
      ipAddress = ipAddress.substring(0, indexNextLine);

      return ipAddress;
    }

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

#if 0    
    typedef struct
    {
      uint32_t CPUID;                  /*!< Offset: 0x000 (R/ )  CPUID Base Register */
      uint32_t ICSR;                   /*!< Offset: 0x004 (R/W)  Interrupt Control and State Register */
      uint32_t RESERVED0;
      uint32_t AIRCR;                  /*!< Offset: 0x00C (R/W)  Application Interrupt and Reset Control Register */
      uint32_t SCR;                    /*!< Offset: 0x010 (R/W)  System Control Register */
      uint32_t CCR;                    /*!< Offset: 0x014 (R/W)  Configuration Control Register */
      uint32_t RESERVED1;
      uint32_t SHP[2U];                /*!< Offset: 0x01C (R/W)  System Handlers Priority Registers. [0] is RESERVED */
      uint32_t SHCSR;                  /*!< Offset: 0x024 (R/W)  System Handler Control and State Register */
    } SCB_Type;

    void NVIC_SystemReset()
    {                  
    /* SCB Application Interrupt and Reset Control Register Definitions */
    #define SCB_AIRCR_VECTKEY_Pos              16U                                      /*!< SCB AIRCR: VECTKEY Position */
    #define SCB_AIRCR_VECTKEY_Msk              (0xFFFFUL << SCB_AIRCR_VECTKEY_Pos)      /*!< SCB AIRCR: VECTKEY Mask */
        
    #define SCB_AIRCR_SYSRESETREQ_Pos           2U                                      /*!< SCB AIRCR: SYSRESETREQ Position */
    #define SCB_AIRCR_SYSRESETREQ_Msk          (1UL << SCB_AIRCR_SYSRESETREQ_Pos)       /*!< SCB AIRCR: SYSRESETREQ Mask */    

    #define SCS_BASE            (0xE000E000UL)                            /*!< System Control Space Base Address */
    #define SysTick_BASE        (SCS_BASE +  0x0010UL)                    /*!< SysTick Base Address */
    #define NVIC_BASE           (SCS_BASE +  0x0100UL)                    /*!< NVIC Base Address */
    #define SCB_BASE            (SCS_BASE +  0x0D00UL)                    /*!< System Control Block Base Address */

    #define SCB                 ((SCB_Type       *)     SCB_BASE      )   /*!< SCB configuration struct */
    #define SysTick             ((SysTick_Type   *)     SysTick_BASE  )   /*!< SysTick configuration struct */
    #define NVIC                ((NVIC_Type      *)     NVIC_BASE     )   /*!< NVIC configuration struct */

                                  
      SCB->AIRCR  = ((0x5FAUL << SCB_AIRCR_VECTKEY_Pos) | SCB_AIRCR_SYSRESETREQ_Msk);

      while(true);
    }
#endif

    void resetFunc()
    {
      delay(1000);
      // Restart for RPi_Pico
      NVIC_SystemReset();
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
    
    // default to channel 1
    int AP_channel = 1;

    String portal_ssid = "";
    String portal_pass = "";

    IPAddress static_IP   = IPAddress(0, 0, 0, 0);
    
    /////////////////////////////////////
    
    // Add customs headers from v1.2.0
    
#if USING_CUSTOMS_STYLE
    const char* ESP_AT_HTML_HEAD_CUSTOMS_STYLE = NULL;
#endif
    
#if USING_CUSTOMS_HEAD_ELEMENT
    const char* _CustomsHeadElement = NULL;
#endif
    
#if USING_CORS_FEATURE    
    const char* _CORS_Header        = WM_HTTP_CORS_ALLOW_ALL;   //"*";
#endif
       
    /////////////////////////////////////
    // Add WiFi Scan from v1.2.0
    
#if SCAN_WIFI_NETWORKS
  int WiFiNetworksFound = 0;		// Number of SSIDs found by WiFi scan, including low quality and duplicates
  int *indices;					        // WiFi network data, filled by scan (SSID, BSSID)
  String ListOfSSIDs = "";		  // List of SSIDs found by scan, in HTML <option> format
#endif

    //////////////////////////////////////
    
    void displayConfigData(const ESP8266_AT_Configuration& configData)
    {
      ESP_AT_LOGDEBUG5(F("Hdr="),   configData.header, F(",SSID="), configData.WiFi_Creds[0].wifi_ssid,
                F(",PW="),   configData.WiFi_Creds[0].wifi_pw);
      ESP_AT_LOGDEBUG3(F("SSID1="), configData.WiFi_Creds[1].wifi_ssid, F(",PW1="),  configData.WiFi_Creds[1].wifi_pw);     
      ESP_AT_LOGDEBUG1(F("BName="), configData.board_name);     
      
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
    
// Use LittleFS/InternalFS for RPi_Pico
#define  CONFIG_FILENAME              ("/fs/wm_config.dat")
#define  CONFIG_FILENAME_BACKUP       ("/fs/wm_config.bak")

#define  CREDENTIALS_FILENAME         ("/fs/wm_cred.dat")
#define  CREDENTIALS_FILENAME_BACKUP  ("/fs/wm_cred.bak")
   
   //////////////////////////////////////////////

#if USE_DYNAMIC_PARAMETERS
   
    bool checkDynamicData()
    {
      int checkSum = 0;
      int readCheckSum;
      char * readBuffer = nullptr;
                
      // file existed
      FILE *file = fopen(CREDENTIALS_FILENAME, "r");
  
      ESP_AT_LOGDEBUG(F("LoadCredFile "));

      if (!file)
      {
        ESP_AT_LOGDEBUG(F("failed"));

        // Trying open redundant config file
        file = fopen(CREDENTIALS_FILENAME_BACKUP, "r"); 
        
        ESP_AT_LOGDEBUG(F("LoadBkUpCredFile "));

        if (!file)
        {
          ESP_AT_LOGDEBUG(F("failed"));
          return false;
        }
      }
      
      // Find the longest pdata, then dynamically allocate buffer. Remember to free when done
      // This is used to store tempo data to calculate checksum to see of data is valid
      // We dont like to destroy myMenuItems[i].pdata with invalid data
      
      uint16_t maxBufferLength = 0;
      
      for (uint8_t i = 0; i < NUM_MENU_ITEMS; i++)
      {       
        if (myMenuItems[i].maxlen > maxBufferLength)
          maxBufferLength = myMenuItems[i].maxlen;
      }
      
      if (maxBufferLength > 0)
      {
        readBuffer = new char[ maxBufferLength + 1 ];
        
        // check to see NULL => stop and return false
        if (readBuffer == nullptr)
        {
          ESP_AT_LOGDEBUG(F("ChkCrR: Error can't allocate buffer."));
          return false;
        }     
        else
        {
          ESP_AT_LOGDEBUG1(F("ChkCrR: Buffer allocated, Sz="), maxBufferLength + 1);
        }    
     
        uint16_t offset = 0;
             
        for (uint8_t i = 0; i < NUM_MENU_ITEMS; i++)
        {       
          uint8_t * _pointer = (uint8_t *) readBuffer;

          // Actual size of pdata is [maxlen + 1]
          memset(readBuffer, 0, myMenuItems[i].maxlen + 1);
          
          // Redundant, but to be sure correct position
          fseek(file, offset, SEEK_SET);
          fread(_pointer, 1, myMenuItems[i].maxlen, file);
          
          offset += myMenuItems[i].maxlen;
       
          ESP_AT_LOGDEBUG3(F("ChkCrR:pdata="), readBuffer, F(",len="), myMenuItems[i].maxlen);         
                 
          for (uint16_t j = 0; j < myMenuItems[i].maxlen; j++,_pointer++)
          {         
            checkSum += *_pointer;  
          }       
        }

        fread((uint8_t *) &readCheckSum, 1, sizeof(readCheckSum), file);
        fclose(file);
        
        ESP_AT_LOGDEBUG(F("OK"));
        
        totalDataSize = sizeof(ESP8266_AT_config) + sizeof(readCheckSum) + offset;
        
        ESP_AT_LOGDEBUG3(F("CrCCsum=0x"), String(checkSum, HEX), F(",CrRCsum=0x"), String(readCheckSum, HEX));
        
        // Free buffer
        if (readBuffer != nullptr)
        {
          // Free buffer
          delete [] readBuffer;
          ESP_AT_LOGDEBUG(F("Buffer freed"));
        }
        
        if ( checkSum == readCheckSum)
        {
          return true;
        }
      }   
      
      return false;    
    }
    
    //////////////////////////////////////////////

    bool loadDynamicData()
    {
      int checkSum = 0;
      int readCheckSum;
      
      // file existed
      FILE *file = fopen(CREDENTIALS_FILENAME, "r");
      
      ESP_AT_LOGDEBUG(F("LoadCredFile "));

      if (!file)
      {
        ESP_AT_LOGDEBUG(F("failed"));

        // Trying open redundant config file
        file = fopen(CREDENTIALS_FILENAME_BACKUP, "r");
        
        ESP_AT_LOGDEBUG(F("LoadBkUpCredFile "));

        if (!file)
        {
          ESP_AT_LOGDEBUG(F("failed"));
          return false;
        }
      }
     
      uint16_t offset = 0;
      
      for (uint8_t i = 0; i < NUM_MENU_ITEMS; i++)
      {       
        uint8_t * _pointer = (uint8_t *) myMenuItems[i].pdata;

        // Actual size of pdata is [maxlen + 1]
        memset(myMenuItems[i].pdata, 0, myMenuItems[i].maxlen + 1);
        
        // Redundant, but to be sure correct position
        fseek(file, offset, SEEK_SET);
        fread(_pointer, 1, myMenuItems[i].maxlen, file);
        
        offset += myMenuItems[i].maxlen;        
    
        ESP_AT_LOGDEBUG3(F("CrR:pdata="), myMenuItems[i].pdata, F(",len="), myMenuItems[i].maxlen);         
               
        for (uint16_t j = 0; j < myMenuItems[i].maxlen; j++,_pointer++)
        {         
          checkSum += *_pointer;  
        }       
      }

      fread((uint8_t *) &readCheckSum, 1, sizeof(readCheckSum), file);
      fclose(file);
      
      totalDataSize = sizeof(ESP8266_AT_config) + sizeof(readCheckSum) + offset;
      
      ESP_AT_LOGDEBUG(F("OK"));
      
      ESP_AT_LOGDEBUG3(F("CrCCsum=0x"), String(checkSum, HEX), F(",CrRCsum=0x"), String(readCheckSum, HEX));
      
      if ( checkSum != readCheckSum)
      {
        return false;
      }
      
      return true;    
    }
    
    //////////////////////////////////////////////

    void saveDynamicData()
    {
      int checkSum = 0;
    
      // file existed
      FILE *file = fopen(CREDENTIALS_FILENAME, "w");
      
      ESP_AT_LOGDEBUG(F("SaveCredFile "));

      uint16_t offset = 0;
      
      for (uint8_t i = 0; i < NUM_MENU_ITEMS; i++)
      {       
        uint8_t* _pointer = (uint8_t *) myMenuItems[i].pdata;
       
        ESP_AT_LOGDEBUG3(F("CW1:pdata="), myMenuItems[i].pdata, F(",len="), myMenuItems[i].maxlen);
        
        if (file)
        {
          // Redundant, but to be sure correct position
          fseek(file, offset, SEEK_SET);
          fwrite(_pointer, 1, myMenuItems[i].maxlen, file); 
          
          offset += myMenuItems[i].maxlen;      
        }
        else
        {
          ESP_AT_LOGDEBUG(F("failed"));
        }        
                     
        for (uint16_t j = 0; j < myMenuItems[i].maxlen; j++,_pointer++)
        {         
          checkSum += *_pointer;     
         }
      }
      
      if (file)
      {
        fwrite((uint8_t *) &checkSum, 1, sizeof(checkSum), file);        
        fclose(file);
        
        ESP_AT_LOGDEBUG(F("OK"));    
      }
      else
      {
        ESP_AT_LOGDEBUG(F("failed"));
      }   
           
      ESP_AT_LOGDEBUG1(F("CrWCSum=0x"), String(checkSum, HEX));
      
      // Trying open redundant Auth file
      file = fopen(CREDENTIALS_FILENAME_BACKUP, "w");
      
      ESP_AT_LOGDEBUG(F("SaveBkUpCredFile "));

      offset = 0;
      
      for (uint8_t i = 0; i < NUM_MENU_ITEMS; i++)
      {       
        uint8_t* _pointer = (uint8_t *) myMenuItems[i].pdata;
     
        ESP_AT_LOGDEBUG3(F("CW2:pdata="), myMenuItems[i].pdata, F(",len="), myMenuItems[i].maxlen);
        
        if (file)
        {
          fseek(file, offset, SEEK_SET);
          fwrite(_pointer, 1, myMenuItems[i].maxlen, file);
          
          // Redundant, but to be sure correct position
          offset += myMenuItems[i].maxlen; 
        }
        else
        {
          ESP_AT_LOGDEBUG(F("failed"));
        }        
                     
        for (uint16_t j = 0; j < myMenuItems[i].maxlen; j++,_pointer++)
        {         
          checkSum += *_pointer;     
         }
      }
      
      if (file)
      {
        fwrite((uint8_t *) &checkSum, 1, sizeof(checkSum), file);        
        fclose(file);
        
        ESP_AT_LOGDEBUG(F("OK"));    
      }
      else
      {
        ESP_AT_LOGDEBUG(F("failed"));
      }   
    }

#endif

    //////////////////////////////////////////////
 
    void NULLTerminateConfig()
    {
      //#define HEADER_MAX_LEN      16
      //#define BOARD_NAME_MAX_LEN  24
      
      // NULL Terminating to be sure
      ESP8266_AT_config.header    [HEADER_MAX_LEN - 1]      = 0;
      ESP8266_AT_config.board_name[BOARD_NAME_MAX_LEN - 1]  = 0;
      
      // For WiFi SSID/PWD here
      for (uint8_t i = 0; i < NUM_WIFI_CREDENTIALS; i++)
      {
        ESP8266_AT_config.WiFi_Creds[i].wifi_ssid[SSID_MAX_LEN - 1] = 0;
        ESP8266_AT_config.WiFi_Creds[i].wifi_pw  [PASS_MAX_LEN - 1] = 0;
      }
    }
    
    //////////////////////////////////////////////  
    
    bool isWiFiConfigValid()
    {
      #if REQUIRE_ONE_SET_SSID_PW
      // If SSID ="blank" or NULL, or PWD length < 8 (as required by standard) => return false
      // Only need 1 set of valid SSID/PWD
      if (!( ( ( strncmp(ESP8266_AT_config.WiFi_Creds[0].wifi_ssid, WM_NO_CONFIG, strlen(WM_NO_CONFIG)) && 
                 strlen(ESP8266_AT_config.WiFi_Creds[0].wifi_ssid) >  0 )  &&
             (   strlen(ESP8266_AT_config.WiFi_Creds[0].wifi_pw) >= PASSWORD_MIN_LEN ) ) ||
             ( ( strncmp(ESP8266_AT_config.WiFi_Creds[1].wifi_ssid, WM_NO_CONFIG, strlen(WM_NO_CONFIG)) && 
                 strlen(ESP8266_AT_config.WiFi_Creds[1].wifi_ssid) >  0 )  &&
               ( strlen(ESP8266_AT_config.WiFi_Creds[1].wifi_pw) >= PASSWORD_MIN_LEN ) ) ))
      #else
      // If SSID ="blank" or NULL, or PWD length < 8 (as required by standard) => invalid set
      // Need both sets of valid SSID/PWD
      if ( !strncmp(ESP8266_AT_config.WiFi_Creds[0].wifi_ssid,   WM_NO_CONFIG, strlen(WM_NO_CONFIG) )  ||
           !strncmp(ESP8266_AT_config.WiFi_Creds[0].wifi_pw,     WM_NO_CONFIG, strlen(WM_NO_CONFIG) )  ||
           !strncmp(ESP8266_AT_config.WiFi_Creds[1].wifi_ssid,   WM_NO_CONFIG, strlen(WM_NO_CONFIG) )  ||
           !strncmp(ESP8266_AT_config.WiFi_Creds[1].wifi_pw,     WM_NO_CONFIG, strlen(WM_NO_CONFIG) )  ||
           ( strlen(ESP8266_AT_config.WiFi_Creds[0].wifi_ssid) == 0 ) || 
           ( strlen(ESP8266_AT_config.WiFi_Creds[1].wifi_ssid) == 0 ) ||
           ( strlen(ESP8266_AT_config.WiFi_Creds[0].wifi_pw)   < PASSWORD_MIN_LEN ) ||
           ( strlen(ESP8266_AT_config.WiFi_Creds[1].wifi_pw)   < PASSWORD_MIN_LEN ) )
      #endif     
      {
        ESP_AT_LOGERROR(F("Invalid Stored WiFi Config Data."));
        
        // Nullify the invalid data to avoid displaying garbage
        memset(&ESP8266_AT_config, 0, sizeof(ESP8266_AT_config));
        
        // If SSID, PW ="blank" or NULL, or PPWD len < 8, set the flag      
        if ( ( strlen(ESP8266_AT_config.WiFi_Creds[0].wifi_pw) < PASSWORD_MIN_LEN ) ||
             ( strlen(ESP8266_AT_config.WiFi_Creds[1].wifi_pw) < PASSWORD_MIN_LEN ) )
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

    bool loadConfigData()
    {
      ESP_AT_LOGWARN(F("LoadCfgFile "));
      
      // file existed
      FILE *file = fopen(CONFIG_FILENAME, "r");
         
      if (!file)
      {
        ESP_AT_LOGWARN(F("failed"));

        // Trying open redundant config file
        file = fopen(CONFIG_FILENAME_BACKUP, "r");
        
        ESP_AT_LOGWARN(F("LoadBkUpCfgFile "));

        if (!file)
        {
          ESP_AT_LOGWARN(F("failed"));
          return false;
        }
      }
     
      fseek(file, 0, SEEK_SET);
      fread((uint8_t *) &ESP8266_AT_config, 1, sizeof(ESP8266_AT_config), file);
      fclose(file);

      ESP_AT_LOGWARN(F("OK"));
      
      NULLTerminateConfig();
      
      return isWiFiConfigValid();
    }
    
    //////////////////////////////////////////////

    void saveConfigData()
    {
      ESP_AT_LOGWARN(F("SaveCfgFile "));

      int calChecksum = calcChecksum();
      ESP8266_AT_config.checkSum = calChecksum;
      ESP_AT_LOGWARN1(F("WCSum=0x"), String(calChecksum, HEX));
      
      // file existed
      FILE *file = fopen(CONFIG_FILENAME, "w");

      if (file)
      {
        fseek(file, 0, SEEK_SET);
        fwrite((uint8_t *) &ESP8266_AT_config, 1, sizeof(ESP8266_AT_config), file);
        fclose(file);
        
        ESP_AT_LOGWARN(F("OK"));
      }
      else
      {
        ESP_AT_LOGWARN(F("failed"));
      }
      
      ESP_AT_LOGWARN(F("SaveBkUpCfgFile "));
      
      // Trying open redundant Auth file
      file = fopen(CONFIG_FILENAME_BACKUP, "w");

      if (file)
      {
        fseek(file, 0, SEEK_SET);
        fwrite((uint8_t *) &ESP8266_AT_config, 1, sizeof(ESP8266_AT_config), file);
        fclose(file);
        
        ESP_AT_LOGWARN(F("OK"));
      }
      else
      {
        ESP_AT_LOGWARN(F("failed"));
      }
      
#if USE_DYNAMIC_PARAMETERS      
      saveDynamicData();
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
          
      ESP_AT_LOGWARN(F("======= Start Loaded Config Data ======="));
      displayConfigData(ESP8266_AT_config);    
    }
    
    //////////////////////////////////////////////
    
    // Return false if init new EEPROM or SPIFFS. No more need trying to connect. Go directly to config mode
    bool getConfigData()
    {
      bool dynamicDataValid = true;  
      int calChecksum; 
      
      hadConfigData = false;
      
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
        // Load stored config data from LittleFS
        // If WiFi SSID/PWD "blank" or NULL, or PWD len < 8, set false flag and exit
        if (!loadConfigData())
        {
          return false;
        }
        
        calChecksum = calcChecksum();

        ESP_AT_LOGWARN3(F("CCSum=0x"), String(calChecksum, HEX), F(",RCSum=0x"), String(ESP8266_AT_config.checkSum, HEX));
        
#if USE_DYNAMIC_PARAMETERS        
        // Load stored dynamic data from LittleFS
        dynamicDataValid = checkDynamicData();
#endif
        
        // If checksum = 0 => LittleFS has been cleared (by uploading new FW, etc) => force to CP
        // If bad checksum = 0 => force to CP
        if ( (calChecksum != 0) && (calChecksum == ESP8266_AT_config.checkSum) )
        {            
          if (dynamicDataValid)
          {
    #if USE_DYNAMIC_PARAMETERS        
            loadDynamicData();
               
            ESP_AT_LOGWARN(F("Valid Stored Dynamic Data"));
    #endif
            
            ESP_AT_LOGWARN(F("======= Start Stored Config Data ======="));
            displayConfigData(ESP8266_AT_config);
            
            // Don't need Config Portal anymore
            return true;
          }
          else
          {
            // Invalid Stored config data => Config Portal
            ESP_AT_LOGWARN(F("Invalid Stored Dynamic Data. Load default from Sketch"));
            
            // Load Default Config Data from Sketch, better than just "blank"
            loadAndSaveDefaultConfigData();
                             
            // Need Config Portal here as data can be just dummy
            // Even if you don't open CP, you're OK on next boot if your default config data is valid 
            return false;
          }
        }  
      }
      
      if ( (strncmp(ESP8266_AT_config.header, ESP_AT_BOARD_TYPE, strlen(ESP_AT_BOARD_TYPE)) != 0) ||
           (calChecksum != ESP8266_AT_config.checkSum) || !dynamicDataValid )
      {
        // Including Credentials CSum
        ESP_AT_LOGWARN1(F("InitCfgDat,sz="), sizeof(ESP8266_AT_config));

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
              
          strcpy(ESP8266_AT_config.WiFi_Creds[0].wifi_ssid,   WM_NO_CONFIG);
          strcpy(ESP8266_AT_config.WiFi_Creds[0].wifi_pw,     WM_NO_CONFIG);
          strcpy(ESP8266_AT_config.WiFi_Creds[1].wifi_ssid,   WM_NO_CONFIG);
          strcpy(ESP8266_AT_config.WiFi_Creds[1].wifi_pw,     WM_NO_CONFIG);
          strcpy(ESP8266_AT_config.board_name, DEFAULT_BOARD_NAME);

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
        // If SSID, PW ="blank" or NULL, stay in config mode forever until having config Data.
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

    // New connection logic from v1.2.0
    bool connectMultiWiFi(int retry_time)
    {
      int sleep_time  = 250;
      int index       = 0;
      int new_index   = 0;
      uint8_t status  = WL_IDLE_STATUS;
                       
      static int lastConnectedIndex = 255;

      ESP_AT_LOGDEBUG(F("ConMultiWifi"));
      
      if (static_IP != IPAddress(0, 0, 0, 0))
      {
        ESP_AT_LOGDEBUG(F("UseStatIP"));
        WiFi.config(static_IP);
      }
      
      if (lastConnectedIndex != 255)
      {
        //  Successive connection, index = ??
        // Checking if new_index is OK
        new_index = (lastConnectedIndex + 1) % NUM_WIFI_CREDENTIALS;
        
        if ( strlen(ESP8266_AT_config.WiFi_Creds[new_index].wifi_pw) >= PASSWORD_MIN_LEN )
        {    
          index = new_index;
          ESP_AT_LOGDEBUG3(F("Using index="), index, F(", lastConnectedIndex="), lastConnectedIndex);
        }
        else
        {
          ESP_AT_LOGERROR3(F("Ignore invalid WiFi PW : index="), new_index, F(", PW="), ESP8266_AT_config.WiFi_Creds[new_index].wifi_pw);
          
          // Using the previous valid index
          index = lastConnectedIndex;
        }
      }
      else
      {
        //  First connection ever, index = 0
        if ( strlen(ESP8266_AT_config.WiFi_Creds[0].wifi_pw) >= PASSWORD_MIN_LEN )
        {    
          ESP_AT_LOGDEBUG(F("First connection, Using index=0"));
        }
        else
        {
          ESP_AT_LOGERROR3(F("Ignore invalid WiFi PW : index=0, SSID="), ESP8266_AT_config.WiFi_Creds[0].wifi_ssid,
                       F(", PWD="), ESP8266_AT_config.WiFi_Creds[0].wifi_pw);
          
          // Using the next valid index
          index = 1;
        }
      } 
         
      ESP_AT_LOGERROR3(F("con2WF:SSID="), ESP8266_AT_config.WiFi_Creds[index].wifi_ssid,
                   F(",PW="), ESP8266_AT_config.WiFi_Creds[index].wifi_pw);
      
      uint8_t numIndexTried = 0;
      
      uint8_t numWiFiReconTries = 0;
     
      while ( !wifi_connected && (numIndexTried++ < NUM_WIFI_CREDENTIALS) && (numWiFiReconTries++ < MAX_NUM_WIFI_RECON_TRIES_PER_LOOP) )
      {         
        while ( 0 < retry_time )
        {      
          ESP_AT_LOGDEBUG1(F("Remaining retry_time="), retry_time);
          
          status = WiFi.begin(ESP8266_AT_config.WiFi_Creds[index].wifi_ssid, ESP8266_AT_config.WiFi_Creds[index].wifi_pw); 
              
          // Need restart WiFi at beginning of each cycle 
          if (status == WL_CONNECTED)
          {
            wifi_connected = true;          
            lastConnectedIndex = index;                                     
            ESP_AT_LOGDEBUG1(F("WOK, lastConnectedIndex="), lastConnectedIndex);
            
            break;
          }
          else
          {
            delay(sleep_time);
            retry_time--;
          }         
        }
        
        if (status == WL_CONNECTED)
        {         
          break;
        }
        else
        {        
          if (retry_time <= 0)
          {      
            ESP_AT_LOGERROR3(F("Failed using index="), index, F(", retry_time="), retry_time);
            retry_time = RETRY_TIMES_CONNECT_WIFI;  
          }
          
          new_index = (index + 1) % NUM_WIFI_CREDENTIALS;
          
          // only use new index if valid (len >= PASSWORD_MIN_LEN = 8)
          if ( strlen(ESP8266_AT_config.WiFi_Creds[new_index].wifi_pw) >= PASSWORD_MIN_LEN )
          {
            index = new_index;
          }
          
          //WiFi.end();
        }
      }

      if (wifi_connected)
      {
        ESP_AT_LOGERROR(F("con2WF:OK"));
        
        ESP_AT_LOGERROR1(F("IP="), WiFi.localIP() );
        
        displayWiFiData();
      }
      else
      {
        ESP_AT_LOGERROR(F("con2WF:failed"));  
        // Can't connect, so try another index next time. Faking this index is OK and lost
        lastConnectedIndex = index;  
      }

      return wifi_connected;  
    }
       
    //////////////////////////////////////////////
    
    // NEW
    void createHTML(String& root_html_template)
    {
      String pitem;
      
      root_html_template  = ESP_AT_HTML_HEAD_START;
      
  #if USING_CUSTOMS_STYLE
      // Using Customs style when not NULL
      if (ESP_AT_HTML_HEAD_CUSTOMS_STYLE)
        root_html_template  += ESP_AT_HTML_HEAD_CUSTOMS_STYLE;
      else
        root_html_template  += ESP_AT_HTML_HEAD_STYLE;
  #else     
      root_html_template  += ESP_AT_HTML_HEAD_STYLE;
  #endif
      
  #if USING_CUSTOMS_HEAD_ELEMENT
      if (_CustomsHeadElement)
        root_html_template += _CustomsHeadElement;
  #endif          
      
#if SCAN_WIFI_NETWORKS
      ESP_AT_LOGDEBUG1(WiFiNetworksFound, F(" SSIDs found, generating HTML now"));
      // Replace HTML <input...> with <select...>, based on WiFi network scan in startConfigurationMode()

      ListOfSSIDs = "";

      for (int i = 0, list_items = 0; (i < WiFiNetworksFound) && (list_items < MAX_SSID_IN_LIST); i++)
      {
        if (indices[i] == -1) 
          continue; 		// skip duplicates and those that are below the required quality
          
        ListOfSSIDs += WM_OPTION_START + String(WiFi.SSID(indices[i])) + WM_OPTION_END;	
        list_items++;		// Count number of suitable, distinct SSIDs to be included in list
      }

      ESP_AT_LOGDEBUG(ListOfSSIDs);

      if (ListOfSSIDs == "")		// No SSID found or none was good enough
        ListOfSSIDs = WM_OPTION_START + String(WM_NO_NETWORKS_FOUND) + WM_OPTION_END;

      pitem = String(ESP_AT_HTML_HEAD_END);

#if MANUAL_SSID_INPUT_ALLOWED
      pitem.replace("[[input_id]]",  "<input id='id' list='SSIDs'>"  + String(WM_DATALIST_START) + "'SSIDs'>" + ListOfSSIDs + WM_DATALIST_END);
      ESP_AT_LOGDEBUG1(F("pitem:"), pitem);
      pitem.replace("[[input_id1]]", "<input id='id1' list='SSIDs'>" + String(WM_DATALIST_START) + "'SSIDs'>" + ListOfSSIDs + WM_DATALIST_END);
      
      ESP_AT_LOGDEBUG1(F("pitem:"), pitem);

#else
      pitem.replace("[[input_id]]",  "<select id='id'>"  + ListOfSSIDs + WM_SELECT_END);
      pitem.replace("[[input_id1]]", "<select id='id1'>" + ListOfSSIDs + WM_SELECT_END);
#endif

      root_html_template += pitem + ESP_AT_FLDSET_START;

#else

      pitem = String(ESP_AT_HTML_HEAD_END);
      pitem.replace("[[input_id]]",  ESP_AT_HTML_INPUT_ID);
      pitem.replace("[[input_id1]]", ESP_AT_HTML_INPUT_ID1);
      root_html_template += pitem + ESP_AT_FLDSET_START;

#endif    // SCAN_WIFI_NETWORKS

#if USE_DYNAMIC_PARAMETERS      
      for (uint16_t i = 0; i < NUM_MENU_ITEMS; i++)
      {
        pitem = String(ESP_AT_HTML_PARAM);

        pitem.replace("{b}", myMenuItems[i].displayName);
        pitem.replace("{v}", myMenuItems[i].id);
        pitem.replace("{i}", myMenuItems[i].id);
        
        root_html_template += pitem;
      }
#endif
      
      root_html_template += String(ESP_AT_FLDSET_END) + ESP_AT_HTML_BUTTON + ESP_AT_HTML_SCRIPT;     

#if USE_DYNAMIC_PARAMETERS      
      for (uint16_t i = 0; i < NUM_MENU_ITEMS; i++)
      {
        pitem = String(ESP_AT_HTML_SCRIPT_ITEM);
        
        pitem.replace("{d}", myMenuItems[i].id);
        
        root_html_template += pitem;
      }
#endif
      
      root_html_template += String(ESP_AT_HTML_SCRIPT_END) + ESP_AT_HTML_END;
      
      return;     
    }
    
    //////////////////////////////////////////////

    void serverSendHeaders()
    {
      ESP_AT_LOGDEBUG3(F("serverSendHeaders:WM_HTTP_CACHE_CONTROL:"), WM_HTTP_CACHE_CONTROL, "=", WM_HTTP_NO_STORE);
      server->sendHeader(WM_HTTP_CACHE_CONTROL, WM_HTTP_NO_STORE);
      
#if USING_CORS_FEATURE
      // New from v1.1.0, for configure CORS Header, default to WM_HTTP_CORS_ALLOW_ALL = "*"
      ESP_AT_LOGDEBUG3(F("serverSendHeaders:WM_HTTP_CORS:"), WM_HTTP_CORS, " : ", _CORS_Header);
      server->sendHeader(WM_HTTP_CORS, _CORS_Header);
#endif
     
      ESP_AT_LOGDEBUG3(F("serverSendHeaders:WM_HTTP_PRAGMA:"), WM_HTTP_PRAGMA, " : ", WM_HTTP_NO_CACHE);
      server->sendHeader(WM_HTTP_PRAGMA, WM_HTTP_NO_CACHE);
      
      ESP_AT_LOGDEBUG3(F("serverSendHeaders:WM_HTTP_EXPIRES:"), WM_HTTP_EXPIRES, " : ", "-1");
      server->sendHeader(WM_HTTP_EXPIRES, "-1");
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
          // New from v1.2.0         
          serverSendHeaders();        
          //////
          
          String result;
          createHTML(result);
          
          ESP_AT_LOGDEBUG(F("h:repl"));

          // Reset configTimeout to stay here until finished.
          configTimeout = 0;
          
          if ( ESP8266_AT_config.board_name[0] != 0 )
          {
            // Or replace only if board_name is valid.  Otherwise, keep intact
            result.replace("RPi_Pico_AT_WM_Lite", ESP8266_AT_config.board_name);
          }
         
          if (hadConfigData)
          {
            result.replace("[[id]]",     ESP8266_AT_config.WiFi_Creds[0].wifi_ssid);
            result.replace("[[pw]]",     ESP8266_AT_config.WiFi_Creds[0].wifi_pw);
            result.replace("[[id1]]",    ESP8266_AT_config.WiFi_Creds[1].wifi_ssid);
            result.replace("[[pw1]]",    ESP8266_AT_config.WiFi_Creds[1].wifi_pw);
            
#if USING_BOARD_NAME            
            result.replace("[[nm]]",     ESP8266_AT_config.board_name);
#endif            
          }
          else
          {
            result.replace("[[id]]",  "");
            result.replace("[[pw]]",  "");
            result.replace("[[id1]]", "");
            result.replace("[[pw1]]", "");
            
#if USING_BOARD_NAME            
            result.replace("[[nm]]",  "");
#endif            
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
        static bool id1_Updated = false;
        static bool pw1_Updated = false;
        
#if USING_BOARD_NAME         
        static bool nm_Updated  = false;
#endif
        
        if (!id_Updated && (key == String("id")))
        {
          ESP_AT_LOGDEBUG(F("h:repl id"));
          id_Updated = true;
          
          number_items_Updated++;
          
          if (strlen(value.c_str()) < sizeof(ESP8266_AT_config.WiFi_Creds[0].wifi_ssid) - 1)
            strcpy(ESP8266_AT_config.WiFi_Creds[0].wifi_ssid, value.c_str());
          else
            strncpy(ESP8266_AT_config.WiFi_Creds[0].wifi_ssid, value.c_str(), sizeof(ESP8266_AT_config.WiFi_Creds[0].wifi_ssid) - 1);
        }
        else if (!pw_Updated && (key == String("pw")))
        {
          ESP_AT_LOGDEBUG(F("h:repl pw"));
          pw_Updated = true;
          
          number_items_Updated++;
          
          if (strlen(value.c_str()) < sizeof(ESP8266_AT_config.WiFi_Creds[0].wifi_pw) - 1)
            strcpy(ESP8266_AT_config.WiFi_Creds[0].wifi_pw, value.c_str());
          else
            strncpy(ESP8266_AT_config.WiFi_Creds[0].wifi_pw, value.c_str(), sizeof(ESP8266_AT_config.WiFi_Creds[0].wifi_pw) - 1);
        }
        else if (!id1_Updated && (key == String("id1")))
        {
          ESP_AT_LOGDEBUG(F("h:repl id1"));
          id1_Updated = true;
          
          number_items_Updated++;
          
          if (strlen(value.c_str()) < sizeof(ESP8266_AT_config.WiFi_Creds[1].wifi_ssid) - 1)
            strcpy(ESP8266_AT_config.WiFi_Creds[1].wifi_ssid, value.c_str());
          else
            strncpy(ESP8266_AT_config.WiFi_Creds[1].wifi_ssid, value.c_str(), sizeof(ESP8266_AT_config.WiFi_Creds[1].wifi_ssid) - 1);
        }
        else if (!pw1_Updated && (key == String("pw1")))
        {
          ESP_AT_LOGDEBUG(F("h:repl pw1"));
          pw1_Updated = true;
          
          number_items_Updated++;
          
          if (strlen(value.c_str()) < sizeof(ESP8266_AT_config.WiFi_Creds[1].wifi_pw) - 1)
            strcpy(ESP8266_AT_config.WiFi_Creds[1].wifi_pw, value.c_str());
          else
            strncpy(ESP8266_AT_config.WiFi_Creds[1].wifi_pw, value.c_str(), sizeof(ESP8266_AT_config.WiFi_Creds[1].wifi_pw) - 1);
        }
#if USING_BOARD_NAME        
        else if (!nm_Updated && (key == String("nm")))
        {
          ESP_AT_LOGDEBUG(F("h:repl nm"));
          nm_Updated = true;
          
          number_items_Updated++;
          if (strlen(value.c_str()) < sizeof(ESP8266_AT_config.board_name) - 1)
            strcpy(ESP8266_AT_config.board_name, value.c_str());
          else
            strncpy(ESP8266_AT_config.board_name, value.c_str(), sizeof(ESP8266_AT_config.board_name) - 1);
        }    
#endif

#if USE_DYNAMIC_PARAMETERS
        else
        {    
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
        }
#endif
        
        ESP_AT_LOGDEBUG1(F("h:items updated ="), number_items_Updated);
        ESP_AT_LOGDEBUG3(F("h:key ="), key, ", value =", value);
        
        server->send(200, "text/html", "OK");

#if USE_DYNAMIC_PARAMETERS        
        if (number_items_Updated == NUM_CONFIGURABLE_ITEMS + NUM_MENU_ITEMS)
#else
        if (number_items_Updated == NUM_CONFIGURABLE_ITEMS)
#endif
        {
          ESP_AT_LOGERROR(F("h:UpdLittleFS"));

          saveConfigData();

          ESP_AT_LOGERROR(F("h:Rst"));

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
      // ReInitialize ESP module
      //WiFi.reInit();
      
#if USING_CONFIG_MODE_LED
      // turn the LED_BUILTIN ON to tell us we enter configuration mode.
      digitalWrite(CONFIG_MODE_LED, LED_ON);
#endif
      
#if SCAN_WIFI_NETWORKS
	    configTimeout = 0;  // To allow user input in CP
	    
	    WiFiNetworksFound = scanWifiNetworks(&indices);	
#endif
    
      WiFi.config(portal_apIP);

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
     
      // start access point, AP only,default channel 10
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
    
#if SCAN_WIFI_NETWORKS

	  // Source code adapted from https://github.com/khoih-prog/ESP_WiFiManager/blob/master/src/ESP_WiFiManager-Impl.h

    int           _paramsCount            = 0;
    int           _minimumQuality         = -1;
    bool          _removeDuplicateAPs     = true;
	
	  //////////////////////////////////////////
    
    void swap(int *thisOne, int *thatOne)
    {
       int tempo;

       tempo    = *thatOne;
       *thatOne = *thisOne;
       *thisOne = tempo;
    }

    //////////////////////////////////////////
	
	  void setMinimumSignalQuality(const int& quality)
	  {
	    _minimumQuality = quality;
	  }

	  //////////////////////////////////////////

	  //if this is true, remove duplicate Access Points - default true
	  void setRemoveDuplicateAPs(const bool& removeDuplicates)
	  {
	    _removeDuplicateAPs = removeDuplicates;
	  }

	  //////////////////////////////////////////

	  //Scan for WiFiNetworks in range and sort by signal strength
	  //space for indices array allocated on the heap and should be freed when no longer required  
	  int scanWifiNetworks(int **indicesptr)
	  {
	    ESP_AT_LOGERROR(F("Scanning Network"));

	    int n = WiFi.scanNetworks();

	    ESP_AT_LOGERROR1(F("scanWifiNetworks: Done, Scanned Networks n = "), n); 

	    //KH, Terrible bug here. WiFi.scanNetworks() returns n < 0 => malloc( negative == very big ) => crash!!!
	    //In .../esp32/libraries/WiFi/src/WiFiType.h
	    //#define WIFI_SCAN_RUNNING   (-1)
	    //#define WIFI_SCAN_FAILED    (-2)
	    //if (n == 0)
	    if (n <= 0)
	    {
		    ESP_AT_LOGERROR(F("No network found"));
		    return (0);
	    }
	    else
	    {
		    // Allocate space off the heap for indices array.
		    // This space should be freed when no longer required.
		    int* indices = (int *)malloc(n * sizeof(int));

		    if (indices == NULL)
		    {
		      ESP_AT_LOGERROR(F("ERROR: Out of memory"));
		      *indicesptr = NULL;
		      return (0);
		    }

		    *indicesptr = indices;
	       
		    //sort networks
		    for (int i = 0; i < n; i++)
		    {
		      indices[i] = i;
		    }

		    ESP_AT_LOGDEBUG(F("Sorting"));

		    // RSSI SORT
		    // old sort
		    for (int i = 0; i < n; i++)
		    {
		      for (int j = i + 1; j < n; j++)
		      {
			      if (WiFi.RSSI(indices[j]) > WiFi.RSSI(indices[i]))
			      {
                    //std::swap(indices[i], indices[j]);
                    // Using locally defined swap()
                    swap(&indices[i], &indices[j]);
       			}
		      }
		    }

		    ESP_AT_LOGDEBUG(F("Removing Dup"));

		    // remove duplicates ( must be RSSI sorted )
		    if (_removeDuplicateAPs)
		    {
		      String cssid;
		      
		      for (int i = 0; i < n; i++)
		      {
			      if (indices[i] == -1)
			        continue;

			      cssid = WiFi.SSID(indices[i]);
			      
			      for (int j = i + 1; j < n; j++)
			      {
			        if (cssid == WiFi.SSID(indices[j]))
			        {
				        ESP_AT_LOGDEBUG1("DUP AP:", WiFi.SSID(indices[j]));
				        indices[j] = -1; // set dup aps to index -1
			        }
			      }
		      }
		    }

		    for (int i = 0; i < n; i++)
		    {
		      if (indices[i] == -1)
			      continue; // skip dups

		      int quality = getRSSIasQuality(WiFi.RSSI(indices[i]));

		      if (!(_minimumQuality == -1 || _minimumQuality < quality))
		      {
			      indices[i] = -1;
			      ESP_AT_LOGDEBUG(F("Skipping low quality"));
		      }
		    }

		    ESP_AT_LOGERROR(F("WiFi networks found:"));
		    
		    for (int i = 0; i < n; i++)
		    {
		      if (indices[i] == -1)
			      continue; // skip dups
		      else
			      ESP_AT_LOGERROR5(i+1,": ",WiFi.SSID(indices[i]), ", ", WiFi.RSSI(i), "dB");
		    }

		    return (n);
	    }
	  }

	  //////////////////////////////////////////

	  int getRSSIasQuality(const int& RSSI)
	  {
	    int quality = 0;

	    if (RSSI <= -100)
	    {
		    quality = 0;
	    }
	    else if (RSSI >= -50)
	    {
		    quality = 100;
	    }
	    else
	    {
		    quality = 2 * (RSSI + 100);
	    }

	    return quality;
	  }

  //////////////////////////////////////////

#endif    
};


#endif    //Esp8266_AT_WM_Lite_Mbed_RPi_Pico_h
