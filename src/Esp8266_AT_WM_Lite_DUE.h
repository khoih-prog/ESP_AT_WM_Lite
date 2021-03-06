/****************************************************************************************************************************
   Esp8266_AT_WM_Lite_DUE.h
   For SAM DUE boards using ESP8266 AT WiFi Shields, using much less code to support boards with smaller memory

   ESP_AT_WM_Lite is a library for the Mega, Teensy, SAM DUE, SAMD and STM32, nRF52 boards (https://github.com/khoih-prog/ESP_AT_WM_Lite)
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

#ifndef Esp8266_AT_WM_Lite_DUE_h
#define Esp8266_AT_WM_Lite_DUE_h

#if ( defined(ARDUINO_SAM_DUE) || defined(__SAM3X8E__) )
  #if defined(ESP8266_AT_USE_SAM_DUE)
  #undef ESP8266_AT_USE_SAM_DUE
  #endif
  #define ESP8266_AT_USE_SAM_DUE      true
  #warning Use SAM_DUE architecture from ESP8266_AT_WiFiManager
#endif

#if ( defined(ESP8266) || defined(ESP32) || defined(ARDUINO_AVR_MEGA2560) || defined(ARDUINO_AVR_MEGA) || \
      defined(CORE_TEENSY) || defined(CORE_TEENSY) || !(ESP8266_AT_USE_SAM_DUE) )
#error This code is intended to run on the SAM DUE platform! Please check your Tools->Board setting.
#endif

#include <ESP8266_AT_WebServer.h>
#include <Esp8266_AT_WM_Lite_Debug.h>

//Use DueFlashStorage to simulate EEPROM
#include <DueFlashStorage.h>                 //https://github.com/sebnil/DueFlashStorage

DueFlashStorage dueFlashStorageData;

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

// New in v1.0.3

#define SSID_MAX_LEN      32
//From v1.0.3, WPA2 passwords can be up to 63 characters long.
#define PASS_MAX_LEN      64

typedef struct
{
  char wifi_ssid[SSID_MAX_LEN];
  char wifi_pw  [PASS_MAX_LEN];
}  WiFi_Credentials;

#define NUM_WIFI_CREDENTIALS      2

// Configurable items besides fixed Header, just add board_name 
#define NUM_CONFIGURABLE_ITEMS    ( ( 2 * NUM_WIFI_CREDENTIALS ) + 1 )
////////////////

typedef struct Configuration
{
  char header         [16];
  WiFi_Credentials  WiFi_Creds  [NUM_WIFI_CREDENTIALS];
  char board_name     [24];
  int  checkSum;
} ESP8266_AT_Configuration;

// Currently CONFIG_DATA_SIZE  =   236  = (16 + 96 * 2 + 4 + 24)
uint16_t CONFIG_DATA_SIZE = sizeof(ESP8266_AT_Configuration);

///New from v1.0.3
extern bool LOAD_DEFAULT_CONFIG_DATA;
extern ESP8266_AT_Configuration defaultConfig;

// -- HTML page fragments
const char ESP_AT_HTML_HEAD[]     /*PROGMEM*/ = "<!DOCTYPE html><html><head><title>SAM_DUE_AT_WM_Lite</title><style>div,input{padding:5px;font-size:1em;}input{width:95%;}body{text-align: center;}button{background-color:#16A1E7;color:#fff;line-height:2.4rem;font-size:1.2rem;width:100%;}fieldset{border-radius:0.3rem;margin:0px;}</style></head><div style=\"text-align:left;display:inline-block;min-width:260px;\">\
<fieldset><div><label>WiFi SSID</label><input value=\"[[id]]\"id=\"id\"><div></div></div>\
<div><label>PWD</label><input value=\"[[pw]]\"id=\"pw\"><div></div></div>\
<div><label>WiFi SSID1</label><input value=\"[[id1]]\"id=\"id1\"><div></div></div>\
<div><label>PWD1</label><input value=\"[[pw1]]\"id=\"pw1\"><div></div></div></fieldset>\
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
///

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
        DEBUG_WM1(F("NoESP"));
      }
      
      //WiFi.reset();
    }

    ~ESP_AT_WiFiManager_Lite()
    {
    }
        
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
   
    void begin(const char* ssid,
               const char* pass )
    {
      DEBUG_WM1(F("conW"));
      connectWiFi(ssid, pass);
    }

    void begin(void)
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
        //hadConfigData = true;

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

    void setConfigPortalIP(IPAddress portalIP = IPAddress(192, 168, 4, 1))
    {
      portal_apIP = portalIP;
    }

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
    
    void setConfigPortal(String ssid = "", String pass = "")
    {
      portal_ssid = ssid;
      portal_pass = pass;
    }

    void setSTAStaticIPConfig(IPAddress ip)
    {
      static_IP = ip;
    }
    
    String getWiFiSSID(uint8_t index)
    { 
      if (index >= NUM_WIFI_CREDENTIALS)
        return String("");
        
      if (!hadConfigData)
        getConfigData();

      return (String(ESP8266_AT_config.WiFi_Creds[index].wifi_ssid));
    }

    String getWiFiPW(uint8_t index)
    {
      if (index >= NUM_WIFI_CREDENTIALS)
        return String("");
        
      if (!hadConfigData)
        getConfigData();

      return (String(ESP8266_AT_config.WiFi_Creds[index].wifi_pw));
    }
    
    bool getWiFiStatus(void)
    {
      return wifi_connected;
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
      
      for (int i = 0; i < NUM_MENU_ITEMS; i++)
      {
        // Actual size of pdata is [maxlen + 1]
        memset(myMenuItems[i].pdata, 0, myMenuItems[i].maxlen + 1);
      }

      saveConfigData();
      
    }
        
    bool isConfigDataValid(void)
    {
      return hadConfigData;
    }

    void resetFunc()
    {
      rstc_start_software_reset(RSTC);    
    }

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

    void displayConfigData(ESP8266_AT_Configuration configData)
    {
      DEBUG_WM6(F("Hdr="),   configData.header, F(",SSID="), configData.WiFi_Creds[0].wifi_ssid,
                F(",PW="),   configData.WiFi_Creds[0].wifi_pw);
      DEBUG_WM4(F("SSID1="), configData.WiFi_Creds[1].wifi_ssid, F(",PW1="),  configData.WiFi_Creds[1].wifi_pw);     
      DEBUG_WM2(F("BName="), configData.board_name);     
              
      for (int i = 0; i < NUM_MENU_ITEMS; i++)
      {
        DEBUG_WM6("i=", i, ",id=", myMenuItems[i].id, ",data=", myMenuItems[i].pdata);
      }         
    }

    void displayWiFiData(void)
    {
      DEBUG_WM2(F("IP="), localIP() );
    }

#if USE_ESP32_AT
  #define ESP_AT_BOARD_TYPE   "SHD_ESP32"
#else
  #define ESP_AT_BOARD_TYPE   "SHD_ESP8266"
#endif

#define WM_NO_CONFIG        "blank"

#ifndef EEPROM_START
#define EEPROM_START     0
#warning EEPROM_START not defined. Set to 0
#endif

// Stating positon to store ESP8266_AT_config
#define CONFIG_EEPROM_START    (EEPROM_START + DRD_FLAG_DATA_SIZE)

    int calcChecksum()
    {
      int checkSum = 0;
      for (uint16_t index = 0; index < (sizeof(ESP8266_AT_config) - sizeof(ESP8266_AT_config.checkSum)); index++)
      {
        checkSum += * ( ( (byte*) &ESP8266_AT_config ) + index);
      }

      return checkSum;
    }
   
    bool checkDynamicData(void)
    {
      int checkSum = 0;
      int readCheckSum;
      
      uint16_t  byteCount = 0;
           
      #define BIG_BUFFER_LEN        768     
      byte bigBuffer[BIG_BUFFER_LEN + 1];      
      
      uint16_t offset = CONFIG_EEPROM_START + sizeof(ESP8266_AT_config);      
           
      // Make address 4-byte aligned
      if ( (offset % 4) != 0 )
      {
        offset += 4 - (offset % 4);
      }
      
      // Find the longest pdata, then dynamically allocate buffer. Remember to free when done
      // This is used to store tempo data to calculate checksum to see of data is valid
      // We dont like to destroy myMenuItems[i].pdata with invalid data
      int totalLength = 0;
            
      for (int i = 0; i < NUM_MENU_ITEMS; i++)
      {       
        totalLength += myMenuItems[i].maxlen;
        
        if ( (totalLength > BIG_BUFFER_LEN) || (myMenuItems[i].maxlen > BIG_BUFFER_LEN) )
        {
          // Size too large, abort and flag false
          DEBUG_WM1(F("ChkCrR: Error Small Buffer."));
          return false;
        }
      }
                         
      // Both Buffers big enough, read all dynamicData to BigBuffer once 
      // as address need to be 4-byte aligned
      byte* dataPointer = (byte* ) dueFlashStorageData.readAddress(offset);
      
      // Prepare buffer, more than enough
      memset(bigBuffer, 0, sizeof(bigBuffer));
      memcpy(bigBuffer, dataPointer, sizeof(bigBuffer));               
         
      // Don't need readBuffer
      // Now to split into individual piece to add to CSum
      for (int i = 0; i < NUM_MENU_ITEMS; i++)
      {       
        char* _pointer = (char*) bigBuffer;
        
        for (uint16_t j = 0; j < myMenuItems[i].maxlen; j++, _pointer++, byteCount++)
        {
          *_pointer = bigBuffer[byteCount];
                  
          checkSum += *_pointer;  
        }    
      }
      
      memcpy(&readCheckSum, &bigBuffer[byteCount], sizeof(readCheckSum));
          
      DEBUG_WM4(F("ChkCrR:CrCCsum=0x"), String(checkSum, HEX), F(",CrRCsum=0x"), String(readCheckSum, HEX));
           
      if ( checkSum != readCheckSum )
      {
        return false;
      }
      
      return true;    
    }
    
    bool dueFlashStorage_get(void)
    {
      uint16_t offset = CONFIG_EEPROM_START;
            
      byte* dataPointer = (byte* ) dueFlashStorageData.readAddress(offset);
      
      memcpy(&ESP8266_AT_config, dataPointer, sizeof(ESP8266_AT_config));
      
      offset += sizeof(ESP8266_AT_config);
           
      // Make address 4-byte aligned
      if ( (offset % 4) != 0 )
      {
        offset += 4 - (offset % 4);
      }
            
      int checkSum = 0;
      int readCheckSum;
      
      uint16_t  byteCount = 0;
           
      byte buffer[768];
      
      dataPointer = (byte* ) dueFlashStorageData.readAddress(offset);
      
      memcpy(buffer, dataPointer, sizeof(buffer));
      
      totalDataSize = sizeof(ESP8266_AT_config) + sizeof(readCheckSum);
   
      for (int i = 0; i < NUM_MENU_ITEMS; i++)
      {       
        char* _pointer = myMenuItems[i].pdata;
        totalDataSize += myMenuItems[i].maxlen;
        
        // Actual size of pdata is [maxlen + 1]
        memset(myMenuItems[i].pdata, 0, myMenuItems[i].maxlen + 1);
                      
        for (uint16_t j = 0; j < myMenuItems[i].maxlen; j++,_pointer++, byteCount++)
        {
          *_pointer = buffer[byteCount];
          
          checkSum += *_pointer;  
         }     
      }
      
      memcpy(&readCheckSum, &buffer[byteCount], sizeof(readCheckSum));
      
      byteCount += sizeof(readCheckSum);      
      
      DEBUG_WM6(F("CrCCsum=0x"), String(checkSum, HEX), F(",CrRCsum=0x"), String(readCheckSum, HEX), 
                F(",TotalDataSz="), totalDataSize);
      
      if ( checkSum != readCheckSum)
      {
        return false;
      }
      
      return true;
    }    
    
    void dueFlashStorage_put(void)
    {
      uint16_t offset = CONFIG_EEPROM_START;
      
      dueFlashStorageData.write(offset, (byte *) &ESP8266_AT_config, sizeof(ESP8266_AT_config));
      
      offset += sizeof(ESP8266_AT_config);
      
      // Make address 4-byte aligned
      if ( (offset % 4) != 0 )
      {
        offset += 4 - (offset % 4);
      }
      
      int       checkSum = 0;
      uint16_t  byteCount = 0;
           
      // Use 2K buffer, if need more memory, can reduce this
      byte buffer[2048];
         
      for (int i = 0; i < NUM_MENU_ITEMS; i++)
      {       
        char* _pointer = myMenuItems[i].pdata;
        
        DEBUG_WM4(F("pdata="), myMenuItems[i].pdata, F(",len="), myMenuItems[i].maxlen);
                     
        for (uint16_t j = 0; j < myMenuItems[i].maxlen; j++, _pointer++, /*offset++,*/ byteCount++)
        {
          if (byteCount >= sizeof(buffer))
          {
            DEBUG_WM2(F("Danger:dynamic data too long >"), sizeof(buffer));
          }
          
          buffer[byteCount] = *_pointer;          
          checkSum += *_pointer;     
         }
      }
           
      memcpy(&buffer[byteCount], &checkSum, sizeof(checkSum));
      
      byteCount += sizeof(checkSum);
      
      dueFlashStorageData.write(offset, buffer, byteCount);
      
      DEBUG_WM4(F("CrCCSum=0x"), String(checkSum, HEX), F(",byteCount="), byteCount);
    }    
    
    // New from v1.0.4
    void loadAndSaveDefaultConfigData(void)
    {
      // Load Default Config Data from Sketch
      memcpy(&ESP8266_AT_config, &defaultConfig, sizeof(ESP8266_AT_config));
      strcpy(ESP8266_AT_config.header, ESP_AT_BOARD_TYPE);
      
      // Including config and dynamic data, and assume valid
      saveConfigData();
          
      DEBUG_WM1(F("======= Start Loaded Config Data ======="));
      displayConfigData(ESP8266_AT_config);    
    }
 
    bool getConfigData()
    {
      bool dynamicDataValid;
      int calChecksum;
      
      hadConfigData = false;    
          
      // For DUE, DATA_LENGTH = ((IFLASH1_PAGE_SIZE/sizeof(byte))*4) = 1KBytes
      DEBUG_WM2(F("Simulate EEPROM, Sz:"), DATA_LENGTH);

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
        // Load stored config / dynamic data from dueFlashStorage
        // Verify ChkSum
        dynamicDataValid = checkDynamicData();
        
        calChecksum = calcChecksum();

        DEBUG_WM4(F("CCSum=0x"), String(calChecksum, HEX), F(",RCSum=0x"), String(ESP8266_AT_config.checkSum, HEX));
                   
        if (dynamicDataValid)
        {
          dueFlashStorage_get();
             
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
        DEBUG_WM2(F("InitCfgDat,Sz="), sizeof(ESP8266_AT_config));

        // doesn't have any configuration        
        if (LOAD_DEFAULT_CONFIG_DATA)
        {
          memcpy(&ESP8266_AT_config, &defaultConfig, sizeof(ESP8266_AT_config));
        }
        else
        {
          memset(&ESP8266_AT_config, 0, sizeof(ESP8266_AT_config));

          for (int i = 0; i < NUM_MENU_ITEMS; i++)
          {
            // Actual size of pdata is [maxlen + 1]
            memset(myMenuItems[i].pdata, 0, myMenuItems[i].maxlen + 1);
          }
                   
          // doesn't have any configuration    
          strcpy(ESP8266_AT_config.WiFi_Creds[0].wifi_ssid,   WM_NO_CONFIG);
          strcpy(ESP8266_AT_config.WiFi_Creds[0].wifi_pw,     WM_NO_CONFIG);
          strcpy(ESP8266_AT_config.WiFi_Creds[1].wifi_ssid,   WM_NO_CONFIG);
          strcpy(ESP8266_AT_config.WiFi_Creds[1].wifi_pw,     WM_NO_CONFIG);
          strcpy(ESP8266_AT_config.board_name, WM_NO_CONFIG);
          
          for (int i = 0; i < NUM_MENU_ITEMS; i++)
          {
            strncpy(myMenuItems[i].pdata, WM_NO_CONFIG, myMenuItems[i].maxlen);
          }
        }
    
        strcpy(ESP8266_AT_config.header, ESP_AT_BOARD_TYPE);
        
        for (int i = 0; i < NUM_MENU_ITEMS; i++)
        {
          DEBUG_WM4(F("g:myMenuItems["), i, F("]="), myMenuItems[i].pdata );
        }
        
        // Don't need
        ESP8266_AT_config.checkSum = 0;

        saveConfigData();
        
        return false;
      }
      else if ( !strncmp(ESP8266_AT_config.WiFi_Creds[0].wifi_ssid,       WM_NO_CONFIG, strlen(WM_NO_CONFIG) )  ||
                !strncmp(ESP8266_AT_config.WiFi_Creds[0].wifi_pw,         WM_NO_CONFIG, strlen(WM_NO_CONFIG) )  ||
                !strncmp(ESP8266_AT_config.WiFi_Creds[1].wifi_ssid,       WM_NO_CONFIG, strlen(WM_NO_CONFIG) )  ||
                !strncmp(ESP8266_AT_config.WiFi_Creds[1].wifi_pw,         WM_NO_CONFIG, strlen(WM_NO_CONFIG) )  ||
                !strlen(ESP8266_AT_config.WiFi_Creds[0].wifi_ssid) || 
                !strlen(ESP8266_AT_config.WiFi_Creds[1].wifi_ssid) ||
                !strlen(ESP8266_AT_config.WiFi_Creds[0].wifi_pw)   ||
                !strlen(ESP8266_AT_config.WiFi_Creds[1].wifi_pw)  )
      {
        // If SSID, PW ="nothing", stay in config mode forever until having config Data.
        return false;
      }
      else
      {
        displayConfigData(ESP8266_AT_config);
      }

      return true;
    }

    void saveConfigData()
    {
      int calChecksum = calcChecksum();
      ESP8266_AT_config.checkSum = calChecksum;
      
      DEBUG_WM2(F("SaveData,ChkSum=0x"), String(calChecksum, HEX));

      dueFlashStorage_put();
    }

    // New connection logic for ESP32-AT from v1.0.6
    bool connectMultiWiFi(int retry_time)
    {
      int sleep_time  = 250;
      int index       = 0;
      uint8_t status;
                       
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
        DEBUG_WM4(F("Using index="), index, F(", lastConnectedIndex="), lastConnectedIndex);
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

    // NEW
    void createHTML(String& root_html_template)
    {
      String pitem;
      
      root_html_template = String(ESP_AT_HTML_HEAD)  + ESP_AT_FLDSET_START;
      
      for (int i = 0; i < NUM_MENU_ITEMS; i++)
      {
        pitem = String(ESP_AT_HTML_PARAM);

        pitem.replace("{b}", myMenuItems[i].displayName);
        pitem.replace("{v}", myMenuItems[i].id);
        pitem.replace("{i}", myMenuItems[i].id);
        
        root_html_template += pitem;
      }
      
      root_html_template += String(ESP_AT_FLDSET_END) + ESP_AT_HTML_BUTTON + ESP_AT_HTML_SCRIPT;     
      
      for (int i = 0; i < NUM_MENU_ITEMS; i++)
      {
        pitem = String(ESP_AT_HTML_SCRIPT_ITEM);
        
        pitem.replace("{d}", myMenuItems[i].id);
        
        root_html_template += pitem;
      }
      
      root_html_template += String(ESP_AT_HTML_SCRIPT_END) + ESP_AT_HTML_END;
      
      return;     
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
          String result;
          createHTML(result);
          
          DEBUG_WM1(F("h:repl"));

          // Reset configTimeout to stay here until finished.
          configTimeout = 0;
          
          if ( ESP8266_AT_config.board_name[0] != 0 )
          {
            // Or replace only if board_name is valid.  Otherwise, keep intact
            result.replace("SAM_DUE_AT_WM_Lite", ESP8266_AT_config.board_name);
          }
         
          result.replace("[[id]]",     ESP8266_AT_config.WiFi_Creds[0].wifi_ssid);
          result.replace("[[pw]]",     ESP8266_AT_config.WiFi_Creds[0].wifi_pw);
          result.replace("[[id1]]",    ESP8266_AT_config.WiFi_Creds[1].wifi_ssid);
          result.replace("[[pw1]]",    ESP8266_AT_config.WiFi_Creds[1].wifi_pw);
          result.replace("[[nm]]",     ESP8266_AT_config.board_name);
          
          for (int i = 0; i < NUM_MENU_ITEMS; i++)
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
          DEBUG_WM1(F("h:repl id"));
          
          number_items_Updated++;
          if (strlen(value.c_str()) < sizeof(ESP8266_AT_config.WiFi_Creds[0].wifi_ssid) - 1)
            strcpy(ESP8266_AT_config.WiFi_Creds[0].wifi_ssid, value.c_str());
          else
            strncpy(ESP8266_AT_config.WiFi_Creds[0].wifi_ssid, value.c_str(), sizeof(ESP8266_AT_config.WiFi_Creds[0].wifi_ssid) - 1);
        }
        else if (key == "pw")
        {
          DEBUG_WM1(F("h:repl pw"));
          
          number_items_Updated++;
          if (strlen(value.c_str()) < sizeof(ESP8266_AT_config.WiFi_Creds[0].wifi_pw) - 1)
            strcpy(ESP8266_AT_config.WiFi_Creds[0].wifi_pw, value.c_str());
          else
            strncpy(ESP8266_AT_config.WiFi_Creds[0].wifi_pw, value.c_str(), sizeof(ESP8266_AT_config.WiFi_Creds[0].wifi_pw) - 1);
        }
        else if (key == "id1")
        {
          DEBUG_WM1(F("h:repl id1"));
          
          number_items_Updated++;
          if (strlen(value.c_str()) < sizeof(ESP8266_AT_config.WiFi_Creds[1].wifi_ssid) - 1)
            strcpy(ESP8266_AT_config.WiFi_Creds[1].wifi_ssid, value.c_str());
          else
            strncpy(ESP8266_AT_config.WiFi_Creds[1].wifi_ssid, value.c_str(), sizeof(ESP8266_AT_config.WiFi_Creds[1].wifi_ssid) - 1);
        }
        else if (key == "pw1")
        {
          DEBUG_WM1(F("h:repl pw1"));
          
          number_items_Updated++;
          if (strlen(value.c_str()) < sizeof(ESP8266_AT_config.WiFi_Creds[1].wifi_pw) - 1)
            strcpy(ESP8266_AT_config.WiFi_Creds[1].wifi_pw, value.c_str());
          else
            strncpy(ESP8266_AT_config.WiFi_Creds[1].wifi_pw, value.c_str(), sizeof(ESP8266_AT_config.WiFi_Creds[1].wifi_pw) - 1);
        }
        else if (key == "nm")
        {
          DEBUG_WM1(F("h:repl nm"));
          
          number_items_Updated++;
          if (strlen(value.c_str()) < sizeof(ESP8266_AT_config.board_name) - 1)
            strcpy(ESP8266_AT_config.board_name, value.c_str());
          else
            strncpy(ESP8266_AT_config.board_name, value.c_str(), sizeof(ESP8266_AT_config.board_name) - 1);
        }
        
        for (int i = 0; i < NUM_MENU_ITEMS; i++)
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
          DEBUG_WM1(F("h:UpdFlash"));

          saveConfigData();

          DEBUG_WM1(F("h:Rst"));

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
      //srand(MAX_WIFI_CHANNEL);   
      srand((uint16_t) millis());
      
      if (AP_channel == 0)
        channel = (rand() % MAX_WIFI_CHANNEL) + 1;     //random(MAX_WIFI_CHANNEL) + 1;
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


#endif    //Esp8266_AT_WM_Lite_DUE_h
