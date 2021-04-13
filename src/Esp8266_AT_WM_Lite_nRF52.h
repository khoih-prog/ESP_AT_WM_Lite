/***************************************************************************************************************************************
   Esp8266_AT_WM_Lite_nRF52.h
   For nRF52 boards using ESP8266 WiFi Shields

   ESP_AT_WM_Lite is a library for the Mega, Teensy, SAM DUE, SAMD and STM32, nRF52 boards (https://github.com/khoih-prog/ESP_AT_WM_Lite)
   to enable store Credentials in EEPROM to easy configuration/reconfiguration and autoconnect/autoreconnect of WiFi and other services
   without Hardcoding.

   Built by Khoi Hoang https://github.com/khoih-prog/ESP_AT_WM_Lite
   Licensed under MIT license
   Version: 1.1.0

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
 ***************************************************************************************************************************************/

#ifndef Esp8266_AT_WM_Lite_nRF52_h
#define Esp8266_AT_WM_Lite_nRF52_h

#if ( defined(NRF52840_FEATHER) || defined(NRF52832_FEATHER) || defined(NRF52_SERIES) || defined(ARDUINO_NRF52_ADAFRUIT) || \
      defined(NRF52840_FEATHER_SENSE) || defined(NRF52840_ITSYBITSY) || defined(NRF52840_CIRCUITPLAY) || defined(NRF52840_CLUE) || \
      defined(NRF52840_METRO) || defined(NRF52840_PCA10056) || defined(PARTICLE_XENON) || defined(NINA_B302_ublox) || defined(NINA_B112_ublox) )
  #if defined(ESP8266_AT_USE_NRF528XX)
    #undef ESP8266_AT_USE_NRF528XX
  #endif
  #define ESP8266_AT_USE_NRF528XX      true
  #warning Use nFR52 architecture from ESP8266_AT_WM_Lite
#endif

#if ( defined(ESP8266) || defined(ESP32) || defined(ARDUINO_AVR_MEGA2560) || defined(ARDUINO_AVR_MEGA) || \
      defined(CORE_TEENSY) || !(ESP8266_AT_USE_NRF528XX) )
  #error This code is intended to run on the nRF52 platform! Please check your Tools->Board setting.
#endif

#define ESP_AT_WM_LITE_VERSION        "ESP_AT_WM_Lite v1.1.0"

//////////////////////////////////////////////

#include <ESP8266_AT_WebServer.h>

//Use LittleFS for nRF52
#include <Adafruit_LittleFS.h>
#include <InternalFileSystem.h>

using namespace Adafruit_LittleFS_Namespace;
File file(InternalFS);

#include <Esp8266_AT_WM_Lite_Debug.h>

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

///NEW
extern uint16_t NUM_MENU_ITEMS;
extern MenuItem myMenuItems [];

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

// Configurable items besides fixed Header, just add board_name 
#define NUM_CONFIGURABLE_ITEMS    ( ( 2 * NUM_WIFI_CREDENTIALS ) + 1 )

//////////////////////////////////////////////

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
const char ESP_AT_HTML_HEAD[]     /*PROGMEM*/ = "<!DOCTYPE html><html><head><title>nRF52_AT_WM_Lite</title><style>div,input{padding:5px;font-size:1em;}input{width:95%;}body{text-align: center;}button{background-color:#16A1E7;color:#fff;line-height:2.4rem;font-size:1.2rem;width:100%;}fieldset{border-radius:0.3rem;margin:0px;}</style></head><div style=\"text-align:left;display:inline-block;min-width:260px;\">\
<fieldset><div><label>*WiFi SSID</label><input value=\"[[id]]\"id=\"id\"><div></div></div>\
<div><label>*PWD (8+ chars)</label><input value=\"[[pw]]\"id=\"pw\"><div></div></div>\
<div><label>*WiFi SSID1</label><input value=\"[[id1]]\"id=\"id1\"><div></div></div>\
<div><label>*PWD1 (8+ chars)</label><input value=\"[[pw1]]\"id=\"pw1\"><div></div></div></fieldset>\
<fieldset><div><label>Board Name</label><input value=\"[[nm]]\"id=\"nm\"><div></div></div></fieldset>";

const char ESP_AT_FLDSET_START[]  /*PROGMEM*/ = "<fieldset>";
const char ESP_AT_FLDSET_END[]    /*PROGMEM*/ = "</fieldset>";
const char ESP_AT_HTML_PARAM[]    /*PROGMEM*/ = "<div><label>{b}</label><input value='[[{v}]]'id='{i}'><div></div></div>";
const char ESP_AT_HTML_BUTTON[]   /*PROGMEM*/ = "<button onclick=\"sv()\">Save</button></div>";
const char ESP_AT_HTML_SCRIPT[]   /*PROGMEM*/ = "<script id=\"jsbin-javascript\">\
function udVal(key,val){var request=new XMLHttpRequest();var url='/?key='+key+'&value='+encodeURIComponent(val);\
request.open('GET',url,false);request.send(null);}\
function sv(){udVal('id',document.getElementById('id').value);udVal('pw',document.getElementById('pw').value);\
udVal('id1',document.getElementById('id1').value);udVal('pw1',document.getElementById('pw1').value);\
udVal('nm',document.getElementById('nm').value);";

const char ESP_AT_HTML_SCRIPT_ITEM[]  /*PROGMEM*/ = "udVal('{d}',document.getElementById('{d}').value);";
const char ESP_AT_HTML_SCRIPT_END[]   /*PROGMEM*/ = "alert('Updated');}</script>";
const char ESP_AT_HTML_END[]          /*PROGMEM*/ = "</html>";

//////////////////////////////////////////////

// New from v1.1.0
#if !defined(REQUIRE_ONE_SET_SSID_PW)
  #define REQUIRE_ONE_SET_SSID_PW     false
#endif

#define PASSWORD_MIN_LEN        8

//////////////////////////////////////////////

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
        DEBUG_WM1(F("NoESP"));
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
      DEBUG_WM2(F("Con2:"), ssid);

      if ( WiFi.begin(ssid, pass) )  
      {
        displayWiFiData();
      }
      else
      {
        DEBUG_WM1(F("NoW"));
        return false;
      }

      DEBUG_WM1(F("WOK"));

      wifi_connected = true;

      return true;
    }
    
    //////////////////////////////////////////////
   
    void begin(const char* ssid,
               const char* pass )
    {
      DEBUG_WM1(F("conWiFi"));
      connectWiFi(ssid, pass);
    }
    
    //////////////////////////////////////////////

    void begin()
    {
#define RETRY_TIMES_CONNECT_WIFI			3
           
      //// New DRD ////
      drd = new DoubleResetDetector_Generic(DRD_TIMEOUT, DRD_ADDRESS);  
      bool useConfigPortal = false;
   
      if (drd->detectDoubleReset())
      {
        DEBUG_WM1(F("Double Reset Detected"));
     
        useConfigPortal = true;
      }
      //// New DRD ////
      DEBUG_WM1(F("======= Start Default Config Data ======="));
      displayConfigData(defaultConfig);
      
      hadConfigData = getConfigData();
        
      //// New DRD ////
      //  useConfigPortal when getConfigData() not OK or DRD'ed
      if (hadConfigData && !useConfigPortal) 
      //// New DRD //// 
      {
        if (connectMultiWiFi(RETRY_TIMES_CONNECT_WIFI))
        {
          DEBUG_WM1(F("b:WOK"));
        }
        else
        {
          DEBUG_WM1(F("b:NoW"));
          // failed to connect to WiFi, will start configuration mode
          startConfigurationMode();
        }
      }
      else
      {
        INFO_WM2(F("b:StayInCfgPortal:"), useConfigPortal ? F("DRD") : F("NoCfgDat"));
        
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

    void run()
    {
      static int retryTimes = 0;
      static bool wifiDisconnectedOnce = false;
      
      // Lost connection in running. Give chance to reconfig.
      // Check WiFi status every 5s and update status
      // Check twice to be sure wifi disconnected is real
      static unsigned long checkstatus_timeout = 0;
      #define WIFI_STATUS_CHECK_INTERVAL    5000L
      
      //// New DRD ////
      // Call the double reset detector loop method every so often,
      // so that it can recognise when the timeout expires.
      // You can also call drd.stop() when you wish to no longer
      // consider the next reset as a double reset.
      drd->loop();
      //// New DRD ////
         
      if ( !configuration_mode && (millis() > checkstatus_timeout) )
      {       
        if (WiFi.status() == WL_CONNECTED)
        {
          wifi_connected = true;
        }
        else
        {
          if (wifiDisconnectedOnce)
          {
            wifiDisconnectedOnce = false;
            wifi_connected = false;
            DEBUG_WM1(F("r:Check&WLost"));
          }
          else
          {
            wifiDisconnectedOnce = true;
          }
        }
        
        checkstatus_timeout = millis() + WIFI_STATUS_CHECK_INTERVAL;
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
            //DEBUG_WM1(F("r:handleClient"));
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
              DEBUG_WM2(F("r:WLost&TOut.ConW.Retry#"), retryTimes);
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
            DEBUG_WM1(F("r:WLost.ReconW"));
            
            if (connectMultiWiFi(RETRY_TIMES_RECONNECT_WIFI))
            {
              DEBUG_WM1(F("r:WOK"));
            }
          }
        }
      }
      else if (configuration_mode)
      {
        configuration_mode = false;
        DEBUG_WM1(F("r:gotWBack"));
      }
    }
    
    //////////////////////////////////////////////

    void setConfigPortalIP(IPAddress portalIP = IPAddress(192, 168, 4, 1))
    {
      portal_apIP = portalIP;
    }
    
    //////////////////////////////////////////////

    #define MIN_WIFI_CHANNEL      1
    #define MAX_WIFI_CHANNEL      12    // Channel 13 is flaky, because of bad number 13 ;-)

    int setConfigPortalChannel(int channel = 1)
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
    
    void setConfigPortal(String ssid = "", String pass = "")
    {
      portal_ssid = ssid;
      portal_pass = pass;
    }
    
    //////////////////////////////////////////////

    void setSTAStaticIPConfig(IPAddress ip)
    {
      static_IP = ip;
    }
    
    //////////////////////////////////////////////
    
    String getWiFiSSID(uint8_t index)
    { 
      if (index >= NUM_WIFI_CREDENTIALS)
        return String("");
        
      if (!hadConfigData)
        getConfigData();

      return (String(ESP8266_AT_config.WiFi_Creds[index].wifi_ssid));
    }
    
    //////////////////////////////////////////////

    String getWiFiPW(uint8_t index)
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
      ipAddress.replace("+CIFSR:STAIP,\"", "");
      ipAddress.replace("\"", "");

      indexNextLine = ipAddress.indexOf("\n");
      ipAddress = ipAddress.substring(0, indexNextLine);

      return ipAddress;
    }

    void clearConfigData()
    {
      memset(&ESP8266_AT_config, 0, sizeof(ESP8266_AT_config));
      
      for (uint8_t i = 0; i < NUM_MENU_ITEMS; i++)
      {
        // Actual size of pdata is [maxlen + 1]
        memset(myMenuItems[i].pdata, 0, myMenuItems[i].maxlen + 1);
      }

      saveConfigData();
    }
    
    //////////////////////////////////////////////
    
    bool isConfigDataValid()
    {
      return hadConfigData;
    }
    
    //////////////////////////////////////////////

    void resetFunc()
    {
      delay(1000);
      // Restart for nRF52
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
    
    void displayConfigData(ESP8266_AT_Configuration configData)
    {
      DEBUG_WM6(F("Hdr="),   configData.header, F(",SSID="), configData.WiFi_Creds[0].wifi_ssid,
                F(",PW="),   configData.WiFi_Creds[0].wifi_pw);
      DEBUG_WM4(F("SSID1="), configData.WiFi_Creds[1].wifi_ssid, F(",PW1="),  configData.WiFi_Creds[1].wifi_pw);     
      DEBUG_WM2(F("BName="), configData.board_name);     
      
      #if 0          
      for (uint8_t i = 0; i < NUM_MENU_ITEMS; i++)
      {
        DEBUG_WM6("i=", i, ",id=", myMenuItems[i].id, ",data=", myMenuItems[i].pdata);
      }      
      #endif     
    }
    
    //////////////////////////////////////////////

    void displayWiFiData()
    {
      DEBUG_WM2(F("IP="), localIP() );
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
    
// Use LittleFS/InternalFS for nRF52
#define  CONFIG_FILENAME              ("/wm_config.dat")
#define  CONFIG_FILENAME_BACKUP       ("/wm_config.bak")

#define  CREDENTIALS_FILENAME         ("/wm_cred.dat")
#define  CREDENTIALS_FILENAME_BACKUP  ("/wm_cred.bak")
   
   //////////////////////////////////////////////
   
    bool checkDynamicData()
    {
      int checkSum = 0;
      int readCheckSum;
      char* readBuffer = NULL;
           
      file.open(CREDENTIALS_FILENAME, FILE_O_READ);
      DEBUG_WM1(F("LoadCredFile "));

      if (!file)
      {
        DEBUG_WM1(F("failed"));

        // Trying open redundant config file
       //file(CREDENTIALS_FILENAME_BACKUP, FILE_O_READ);
        file.open(CREDENTIALS_FILENAME_BACKUP, FILE_O_READ);
        DEBUG_WM1(F("LoadBkUpCredFile "));

        if (!file)
        {
          DEBUG_WM1(F("failed"));
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
        if (readBuffer == NULL)
        {
          DEBUG_WM1(F("ChkCrR: Error can't allocate buffer."));
          return false;
        }     
        else
        {
          DEBUG_WM2(F("ChkCrR: Buffer allocated, Sz="), maxBufferLength + 1);
        }    
      }
     
      uint16_t offset = 0;
      
      for (uint8_t i = 0; i < NUM_MENU_ITEMS; i++)
      {       
        char* _pointer = readBuffer;

        // Actual size of pdata is [maxlen + 1]
        memset(readBuffer, 0, myMenuItems[i].maxlen + 1);
        
        // Redundant, but to be sure correct position
        file.seek(offset);
        file.read(_pointer, myMenuItems[i].maxlen);
        
        offset += myMenuItems[i].maxlen;
     
        DEBUG_WM4(F("ChkCrR:pdata="), readBuffer, F(",len="), myMenuItems[i].maxlen);         
               
        for (uint16_t j = 0; j < myMenuItems[i].maxlen; j++,_pointer++)
        {         
          checkSum += *_pointer;  
        }       
      }

      file.read((char *) &readCheckSum, sizeof(readCheckSum));
      
      DEBUG_WM1(F("OK"));
      file.close();
      
      DEBUG_WM4(F("CrCCsum=0x"), String(checkSum, HEX), F(",CrRCsum=0x"), String(readCheckSum, HEX));
      
      // Free buffer
      if (readBuffer != NULL)
      {
        free(readBuffer);
        DEBUG_WM1(F("Buffer freed"));
      }
      
      if ( checkSum != readCheckSum)
      {
        return false;
      }
      
      return true;    
    }
    
    //////////////////////////////////////////////

    bool loadDynamicData()
    {
      int checkSum = 0;
      int readCheckSum;
      totalDataSize = sizeof(ESP8266_AT_config) + sizeof(readCheckSum);
      
      file.open(CREDENTIALS_FILENAME, FILE_O_READ);
      DEBUG_WM1(F("LoadCredFile "));

      if (!file)
      {
        DEBUG_WM1(F("failed"));

        // Trying open redundant config file
        file.open(CREDENTIALS_FILENAME_BACKUP, FILE_O_READ);
        DEBUG_WM1(F("LoadBkUpCredFile "));

        if (!file)
        {
          DEBUG_WM1(F("failed"));
          return false;
        }
      }
     
      uint16_t offset = 0;
      
      for (uint8_t i = 0; i < NUM_MENU_ITEMS; i++)
      {       
        char* _pointer = myMenuItems[i].pdata;
        totalDataSize += myMenuItems[i].maxlen;

        // Actual size of pdata is [maxlen + 1]
        memset(myMenuItems[i].pdata, 0, myMenuItems[i].maxlen + 1);
        
        // Redundant, but to be sure correct position
        file.seek(offset);
        file.read(_pointer, myMenuItems[i].maxlen);
        
        offset += myMenuItems[i].maxlen;        
    
        DEBUG_WM4(F("CrR:pdata="), myMenuItems[i].pdata, F(",len="), myMenuItems[i].maxlen);         
               
        for (uint16_t j = 0; j < myMenuItems[i].maxlen; j++,_pointer++)
        {         
          checkSum += *_pointer;  
        }       
      }

      file.read((char *) &readCheckSum, sizeof(readCheckSum));
      
      DEBUG_WM1(F("OK"));
      file.close();
      
      DEBUG_WM4(F("CrCCsum=0x"), String(checkSum, HEX), F(",CrRCsum=0x"), String(readCheckSum, HEX));
      
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
    
      file.open(CREDENTIALS_FILENAME, FILE_O_WRITE);
      DEBUG_WM1(F("SaveCredFile "));

      uint16_t offset = 0;
      
      for (uint8_t i = 0; i < NUM_MENU_ITEMS; i++)
      {       
        char* _pointer = myMenuItems[i].pdata;
       
        DEBUG_WM4(F("CW1:pdata="), myMenuItems[i].pdata, F(",len="), myMenuItems[i].maxlen);
        
        if (file)
        {
          // Redundant, but to be sure correct position
          file.seek(offset);                   
          file.write((uint8_t*) _pointer, myMenuItems[i].maxlen); 
          
          offset += myMenuItems[i].maxlen;      
        }
        else
        {
          DEBUG_WM1(F("failed"));
        }        
                     
        for (uint16_t j = 0; j < myMenuItems[i].maxlen; j++,_pointer++)
        {         
          checkSum += *_pointer;     
         }
      }
      
      if (file)
      {
        file.write((uint8_t*) &checkSum, sizeof(checkSum));     
        file.close();
        DEBUG_WM1(F("OK"));    
      }
      else
      {
        DEBUG_WM1(F("failed"));
      }   
           
      DEBUG_WM2(F("CrWCSum=0x"), String(checkSum, HEX));
      
      // Trying open redundant Auth file
      file.open(CREDENTIALS_FILENAME_BACKUP, FILE_O_WRITE);
      DEBUG_WM1(F("SaveBkUpCredFile "));

      offset = 0;
      
      for (uint8_t i = 0; i < NUM_MENU_ITEMS; i++)
      {       
        char* _pointer = myMenuItems[i].pdata;
     
        DEBUG_WM4(F("CW2:pdata="), myMenuItems[i].pdata, F(",len="), myMenuItems[i].maxlen);
        
        if (file)
        {
          file.seek(offset);                   
          file.write((uint8_t*) _pointer, myMenuItems[i].maxlen); 
          
          // Redundant, but to be sure correct position
          offset += myMenuItems[i].maxlen; 
        }
        else
        {
          DEBUG_WM1(F("failed"));
        }        
                     
        for (uint16_t j = 0; j < myMenuItems[i].maxlen; j++,_pointer++)
        {         
          checkSum += *_pointer;     
         }
      }
      
      if (file)
      {
        file.write((uint8_t*) &checkSum, sizeof(checkSum));     
        file.close();
        DEBUG_WM1(F("OK"));    
      }
      else
      {
        DEBUG_WM1(F("failed"));
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
        // If SSID, PW ="blank" or NULL, set the flag
        INFO_WM1(F("Invalid Stored WiFi Config Data"));
        
        hadConfigData = false;
        
        return false;
      }
      
      return true;
    }
    
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
        ESP8266_AT_config.WiFi_Creds[i].wifi_pw[PASS_MAX_LEN - 1]   = 0;
      }
    }
            
    //////////////////////////////////////////////

    bool loadConfigData()
    {
      DEBUG_WM1(F("LoadCfgFile "));
      
      // file existed
      file.open(CONFIG_FILENAME, FILE_O_READ);      
      if (!file)
      {
        DEBUG_WM1(F("failed"));

        // Trying open redundant config file
        file.open(CONFIG_FILENAME_BACKUP, FILE_O_READ);
        DEBUG_WM1(F("LoadBkUpCfgFile "));

        if (!file)
        {
          DEBUG_WM1(F("failed"));
          return false;
        }
      }
     
      file.seek(0);
      file.read((char *) &ESP8266_AT_config, sizeof(ESP8266_AT_config));

      DEBUG_WM1(F("OK"));
      file.close();
      
      NULLTerminateConfig();
      
      return isWiFiConfigValid();
    }
    
    //////////////////////////////////////////////

    void saveConfigData()
    {
      DEBUG_WM1(F("SaveCfgFile "));

      int calChecksum = calcChecksum();
      ESP8266_AT_config.checkSum = calChecksum;
      DEBUG_WM2(F("WCSum=0x"), String(calChecksum, HEX));
      
      file.open(CONFIG_FILENAME, FILE_O_WRITE);

      if (file)
      {
        file.seek(0);
        file.write((uint8_t*) &ESP8266_AT_config, sizeof(ESP8266_AT_config));
        
        file.close();
        DEBUG_WM1(F("OK"));
      }
      else
      {
        DEBUG_WM1(F("failed"));
      }
      
      DEBUG_WM1(F("SaveBkUpCfgFile "));
      
      // Trying open redundant Auth file
      file.open(CONFIG_FILENAME_BACKUP, FILE_O_WRITE);

      if (file)
      {
        file.seek(0);
        file.write((uint8_t *) &ESP8266_AT_config, sizeof(ESP8266_AT_config));
        
        file.close();
        DEBUG_WM1(F("OK"));
      }
      else
      {
        DEBUG_WM1(F("failed"));
      }
      
      saveDynamicData();
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
          
      DEBUG_WM1(F("======= Start Loaded Config Data ======="));
      displayConfigData(ESP8266_AT_config);    
    }
    
    //////////////////////////////////////////////
    
    // Return false if init new EEPROM or SPIFFS. No more need trying to connect. Go directly to config mode
    bool getConfigData()
    {
      bool dynamicDataValid;  
      int calChecksum; 
      
      hadConfigData = false;
      
      // Initialize Internal File System
      if (!InternalFS.begin())
      {
        DEBUG_WM1(F("InternalFS failed"));
        return false;
      }

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
        
        // Load stored dynamic data from LittleFS
        // Verify ChkSum
        dynamicDataValid = checkDynamicData();
        
        calChecksum = calcChecksum();

        DEBUG_WM4(F("CCSum=0x"), String(calChecksum, HEX), F(",RCSum=0x"), String(ESP8266_AT_config.checkSum, HEX));
                   
        if (dynamicDataValid)
        {
          loadDynamicData();
             
          DEBUG_WM1(F("Valid Stored Dynamic Data"));        
          DEBUG_WM1(F("======= Start Stored Config Data ======="));
          displayConfigData(ESP8266_AT_config);
          
          // Don't need Config Portal anymore
          return true;
        }
        else
        {
          // Invalid Stored config data => Config Portal
          DEBUG_WM1(F("Invalid Stored Dynamic Data. Load default from Sketch"));
          
          // Load Default Config Data from Sketch, better than just "blank"
          loadAndSaveDefaultConfigData();
                           
          // Need Config Portal here as data can be just dummy
          // Even if you don't open CP, you're OK on next boot if your default config data is valid 
          return false;
        }      
      }
      
      if ( (strncmp(ESP8266_AT_config.header, ESP_AT_BOARD_TYPE, strlen(ESP_AT_BOARD_TYPE)) != 0) ||
           (calChecksum != ESP8266_AT_config.checkSum) || !dynamicDataValid )
      {
        // Including Credentials CSum
        DEBUG_WM2(F("InitCfgDat,sz="), sizeof(ESP8266_AT_config));

        // doesn't have any configuration        
        if (LOAD_DEFAULT_CONFIG_DATA)
        {
          memcpy(&ESP8266_AT_config, &defaultConfig, sizeof(ESP8266_AT_config));
        }
        else
        {
          memset(&ESP8266_AT_config, 0, sizeof(ESP8266_AT_config));

          for (uint8_t i = 0; i < NUM_MENU_ITEMS; i++)
          {
            // Actual size of pdata is [maxlen + 1]
            memset(myMenuItems[i].pdata, 0, myMenuItems[i].maxlen + 1);
          }
              
          strcpy(ESP8266_AT_config.WiFi_Creds[0].wifi_ssid,   WM_NO_CONFIG);
          strcpy(ESP8266_AT_config.WiFi_Creds[0].wifi_pw,     WM_NO_CONFIG);
          strcpy(ESP8266_AT_config.WiFi_Creds[1].wifi_ssid,   WM_NO_CONFIG);
          strcpy(ESP8266_AT_config.WiFi_Creds[1].wifi_pw,     WM_NO_CONFIG);
          strcpy(ESP8266_AT_config.board_name, WM_NO_CONFIG);
          
          for (uint8_t i = 0; i < NUM_MENU_ITEMS; i++)
          {
            strncpy(myMenuItems[i].pdata, WM_NO_CONFIG, myMenuItems[i].maxlen);
          }
        }
    
        strcpy(ESP8266_AT_config.header, ESP_AT_BOARD_TYPE);
        
        for (uint8_t i = 0; i < NUM_MENU_ITEMS; i++)
        {
          DEBUG_WM4(F("g:myMenuItems["), i, F("]="), myMenuItems[i].pdata );
        }
        
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

    // New connection logic for ESP32-AT from v1.0.6
    bool connectMultiWiFi(int retry_time)
    {
      int sleep_time  = 250;
      int index       = 0;
      uint8_t status  = WL_IDLE_STATUS;
                       
      static int lastConnectedIndex = 255;

      DEBUG_WM1(F("ConMultiWifi"));

      if (static_IP != IPAddress(0, 0, 0, 0))
      {
        DEBUG_WM1(F("UseStatIP"));
        WiFi.config(static_IP);
      }
    
      if (lastConnectedIndex != 255)
      {
        index = (lastConnectedIndex + 1) % NUM_WIFI_CREDENTIALS;
        
        if ( strlen(ESP8266_AT_config.WiFi_Creds[index].wifi_pw) >= PASSWORD_MIN_LEN )
        {    
          DEBUG_WM4(F("Using index="), index, F(", lastConnectedIndex="), lastConnectedIndex);
        }
        else
        {
          DEBUG_WM4(F("Ignore invalid WiFi PW : index="), index, F(", PW="), ESP8266_AT_config.WiFi_Creds[index].wifi_pw);
          
          // Using the other valid index
          index = (lastConnectedIndex + 1) % NUM_WIFI_CREDENTIALS;
        }
      }
      
      DEBUG_WM4(F("con2WF:SSID="), ESP8266_AT_config.WiFi_Creds[index].wifi_ssid,
                F(",PW="), ESP8266_AT_config.WiFi_Creds[index].wifi_pw);
             
      while ( !wifi_connected && ( 0 < retry_time ) )
      {      
        DEBUG_WM2(F("Remaining retry_time="), retry_time);
        
        status = WiFi.begin(ESP8266_AT_config.WiFi_Creds[index].wifi_ssid, ESP8266_AT_config.WiFi_Creds[index].wifi_pw); 
            
        // Need restart WiFi at beginning of each cycle 
        if (status == WL_CONNECTED)
        {
          wifi_connected = true;          
          lastConnectedIndex = index;                                     
          DEBUG_WM2(F("WOK, lastConnectedIndex="), lastConnectedIndex);
          
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
        DEBUG_WM4(F("Failed using index="), index, F(", retry_time="), retry_time);             
      }  

      if (wifi_connected)
      {
        DEBUG_WM1(F("con2WF:OK"));
        displayWiFiData();
      }
      else
      {
        DEBUG_WM1(F("con2WF:failed"));  
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
      
      root_html_template = String(ESP_AT_HTML_HEAD)  + ESP_AT_FLDSET_START;
      
      for (uint8_t i = 0; i < NUM_MENU_ITEMS; i++)
      {
        pitem = String(ESP_AT_HTML_PARAM);

        pitem.replace("{b}", myMenuItems[i].displayName);
        pitem.replace("{v}", myMenuItems[i].id);
        pitem.replace("{i}", myMenuItems[i].id);
        
        root_html_template += pitem;
      }
      
      root_html_template += String(ESP_AT_FLDSET_END) + ESP_AT_HTML_BUTTON + ESP_AT_HTML_SCRIPT;     
      
      for (uint8_t i = 0; i < NUM_MENU_ITEMS; i++)
      {
        pitem = String(ESP_AT_HTML_SCRIPT_ITEM);
        
        pitem.replace("{d}", myMenuItems[i].id);
        
        root_html_template += pitem;
      }
      
      root_html_template += String(ESP_AT_HTML_SCRIPT_END) + ESP_AT_HTML_END;
      
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
          
          DEBUG_WM1(F("h:repl"));

          // Reset configTimeout to stay here until finished.
          configTimeout = 0;
          
          if ( ESP8266_AT_config.board_name[0] != 0 )
          {
            // Or replace only if board_name is valid.  Otherwise, keep intact
            result.replace("nRF52_AT_WM_Lite", ESP8266_AT_config.board_name);
          }
         
          result.replace("[[id]]",     ESP8266_AT_config.WiFi_Creds[0].wifi_ssid);
          result.replace("[[pw]]",     ESP8266_AT_config.WiFi_Creds[0].wifi_pw);
          result.replace("[[id1]]",    ESP8266_AT_config.WiFi_Creds[1].wifi_ssid);
          result.replace("[[pw1]]",    ESP8266_AT_config.WiFi_Creds[1].wifi_pw);
          result.replace("[[nm]]",     ESP8266_AT_config.board_name);
          
          for (uint8_t i = 0; i < NUM_MENU_ITEMS; i++)
          {
            String toChange = String("[[") + myMenuItems[i].id + "]]";
            result.replace(toChange, myMenuItems[i].pdata);
          }
          
          // Check if HTML size is larger than 2K, warn that WebServer won't work
          // because of notorious 2K buffer limitation of ESP8266-AT. 
          // Use conservative value 2000 instead of 2048
          uint16_t HTML_page_size = result.length();
          
          DEBUG_WM2(F("h:HTML page size:"), HTML_page_size);
          
          if (HTML_page_size > 2000)
          {
            DEBUG_WM1(F("h:HTML page larger than 2K. Config Portal won't work. Reduce dynamic params"));
          }   
          
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
          number_items_Updated++;
          if (strlen(value.c_str()) < sizeof(ESP8266_AT_config.WiFi_Creds[0].wifi_ssid) - 1)
            strcpy(ESP8266_AT_config.WiFi_Creds[0].wifi_ssid, value.c_str());
          else
            strncpy(ESP8266_AT_config.WiFi_Creds[0].wifi_ssid, value.c_str(), sizeof(ESP8266_AT_config.WiFi_Creds[0].wifi_ssid) - 1);
        }
        else if (key == "pw")
        {
          number_items_Updated++;
          if (strlen(value.c_str()) < sizeof(ESP8266_AT_config.WiFi_Creds[0].wifi_pw) - 1)
            strcpy(ESP8266_AT_config.WiFi_Creds[0].wifi_pw, value.c_str());
          else
            strncpy(ESP8266_AT_config.WiFi_Creds[0].wifi_pw, value.c_str(), sizeof(ESP8266_AT_config.WiFi_Creds[0].wifi_pw) - 1);
        }
        else if (key == "id1")
        {
          number_items_Updated++;
          if (strlen(value.c_str()) < sizeof(ESP8266_AT_config.WiFi_Creds[1].wifi_ssid) - 1)
            strcpy(ESP8266_AT_config.WiFi_Creds[1].wifi_ssid, value.c_str());
          else
            strncpy(ESP8266_AT_config.WiFi_Creds[1].wifi_ssid, value.c_str(), sizeof(ESP8266_AT_config.WiFi_Creds[1].wifi_ssid) - 1);
        }
        else if (key == "pw1")
        {
          number_items_Updated++;
          if (strlen(value.c_str()) < sizeof(ESP8266_AT_config.WiFi_Creds[1].wifi_pw) - 1)
            strcpy(ESP8266_AT_config.WiFi_Creds[1].wifi_pw, value.c_str());
          else
            strncpy(ESP8266_AT_config.WiFi_Creds[1].wifi_pw, value.c_str(), sizeof(ESP8266_AT_config.WiFi_Creds[1].wifi_pw) - 1);
        }
        else if (key == "nm")
        {
          number_items_Updated++;
          if (strlen(value.c_str()) < sizeof(ESP8266_AT_config.board_name) - 1)
            strcpy(ESP8266_AT_config.board_name, value.c_str());
          else
            strncpy(ESP8266_AT_config.board_name, value.c_str(), sizeof(ESP8266_AT_config.board_name) - 1);
        }
        
        for (uint8_t i = 0; i < NUM_MENU_ITEMS; i++)
        {
          if (key == myMenuItems[i].id)
          {
            DEBUG_WM4(F("h:"), myMenuItems[i].id, F("="), value.c_str() );
            number_items_Updated++;

            // Actual size of pdata is [maxlen + 1]
            memset(myMenuItems[i].pdata, 0, myMenuItems[i].maxlen + 1);

            if ((int) strlen(value.c_str()) < myMenuItems[i].maxlen)
              strcpy(myMenuItems[i].pdata, value.c_str());
            else
              strncpy(myMenuItems[i].pdata, value.c_str(), myMenuItems[i].maxlen);
          }
        }

        server->send(200, "text/html", "OK");

        // NEW
        if (number_items_Updated == NUM_CONFIGURABLE_ITEMS + NUM_MENU_ITEMS)
        {
          DEBUG_WM1(F("h:UpdLittleFS"));

          saveConfigData();

          DEBUG_WM1(F("h:Rst"));

          // TO DO : what command to reset
          // Delay then reset the board after save data
          delay(1000);
          resetFunc();  //call reset
        }
      }   // if (server)
    }
    
    //////////////////////////////////////////////

    void startConfigurationMode()
    {
#define CONFIG_TIMEOUT			60000L

      // ReInitialize ESP module
      //WiFi.reInit();    

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

      INFO_WM4(F("SSID="), portal_ssid, F(",PW="), portal_pass);
      INFO_WM4(F("IP="), portal_apIP, F(",CH="), channel);
     
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


#endif    //Esp8266_AT_WM_Lite_nRF52_h
