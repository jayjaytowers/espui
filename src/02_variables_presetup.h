/*
***************************************************************************
  ebc_alarmclock - variables and presetup
***************************************************************************
  last update 20210327 by ericBcreator
***************************************************************************
*/

//
// main custom settings
//

char WN_ssid_ca[WIFI_CRED_LENGTH] = {};         // @EB-setup SSID for the local Wifi network. leave empty for web interface setup!
char WN_password_ca[WIFI_CRED_LENGTH] = {};     // @EB-setup password

const unsigned int wifiSetupTimeout = 300;      // @EB-custom timeout for wifi setup in seconds, the esp will restart when there is no input. set to zero to disable.
const int timeOffsetSec = 0;                    // @EB-custom custom time offset in seconds

//
// setup initial settings, can be set with the web interface and are stored in eeprom (overwriting these)
//

const int defaultTimeZone = 1;                  // @EB-setup default timezone
byte DSTmode = 2;                               // @EB-setup 0=off 1=on 2=auto (CET)
byte timeDisplayMode = 0;                       // @EB-setup 0 to 6, check UI_msg_timeDisplayModes[] for the description

//
// custom initial settings, can be set with the web interface and are stored in eeprom (overwriting these)
//

bool ampmMode = false;                          // @EB-custom if true display 12 hours, otherwise 24
bool displayInfo = true;                        // @EB-custom display full date and time every displayInfoInterval seconds
unsigned int displayInfoInterval = 120;         // @EB-custom display interval time in seconds
bool sleepMode = false;                         // @EB-custom in sleepmode the display is turned off, only the led alarm 1 indicator will display
unsigned int sleepDelay = 300;                  // @EB-custom delay time in seconds
byte ledBrightness = 3;                         // @EB-custom default brightness use a value between 0 and 15 for brightness
bool ledBrightnessManualSet = false;            // @EB-custom set to true to use ledBrightness instead of the read value when using a light sensor
                                                //            can be changed with the menu or user interface
unsigned int neoBacklight = 0;                  // @EB-custom neopixel backlight color

int timeZone = defaultTimeZone;
bool DST = false;
bool blinkingColon = true;
bool suspendMode = false;

#ifdef DAYLIGHTINFO
  bool displayDaylight = true;                  // @EB-custom display daylight info. note: DAYLIGHTINFO variables have to be set
#else
  bool displayDaylight = false;
#endif

#ifdef ENABLE_BME280
  bool BME280_display = true;                     // @EB-custom display BME280 sensor settings
  bool BME280_fahrenheid = false;                 // @EB-custom false (default) display temp in Celsius, true in Fahrenheid

                                                  // @EB-custom temperature, humidity and air pressure calibration: 
                                                  // the BME sensor readings are first multiplied with the factors and then the values are added 
                                                  // (or subtracted when negative). set the factor to 1 and the value to 0 for no calibration
  float BME280_temperatureCalFactor =  1.0;       // @EB-setup
  float BME280_temperatureCalValue  =  0.0;       // @EB-setup 
  float BME280_humidityCalFactor    =  1.0;       // @EB-setup 
  float BME280_humidityCalValue     =  0.0;       // @EB-setup 
  float BME280_pressureCalFactor    =  1.0;       // @EB-setup 
  float BME280_pressureCalValue     =  0.0;       // @EB-setup 

  bool BME280_tempTimeSwap = true;                // @EB-setup alternate time and temp display
  unsigned long BME280_dispTempInterval = 10000;  // @EB-setup duration of temp display (in milliseoonds)
  unsigned long BME280_dispTimeInterval = 30000;  // @EB-setup duration of time display (in milliseconds)

  bool BME280_tempTimeSwitch = false;
  float BME280_temperature, BME280_humidity, BME280_pressure, BME280_altitude;
#else
  bool BME280_display = false;
  bool BME280_fahrenheid = false;
  bool BME280_tempTimeSwap = false;
  bool BME280_tempTimeSwitch = false;
#endif

void initCustomSettings() {
  ampmMode = false;
  displayInfo = true;
  displayInfoInterval = 120;
  sleepMode = false;
  sleepDelay = 300;
  ledBrightness = 3;
  ledBrightnessManualSet = false;
  blinkingColon = true;
  timeDisplayMode = 0;
  suspendMode = false;
  neoBacklight = 0;
  timeZone = defaultTimeZone;
  DSTmode = 2;
  #ifdef DAYLIGHTINFO
    displayDaylight = true;
  #else
    displayDaylight = false;
  #endif
  #ifdef ENABLE_BME280
    BME280_display = true;
    BME280_fahrenheid = false;
    BME280_tempTimeSwap = true;
  #else
    BME280_display = false;
    BME280_fahrenheid = false;
    BME280_tempTimeSwap = false;
  #endif
}

//
// custom settings
//

byte ledBrightnessAlarm = 10;                   // @EB-custom brightness when an alarm is triggered and bright is set

const byte scrollDelayTime = 25;                // @EB-custom scroll delay time in milliseconds
const unsigned long defaultAlarmDuration = 10;  // @EB-custom default duration of alarm in seconds

bool UI_enabled = true;                         // @EB-custom enable or disable the UI
const bool UI_secure = false;                   // @EB-custom when true, you need to enter the SSID and password to access the web UI
String UI_name = "  ";                          // @EB-custom UI name. note: the UI SSID and version are added
const int UI_channel = 8;                       // @EB-todo ignored, the ESP resets it to the routers WiFi channel (?)

#ifdef LIGHTSENSOR_MIN_VALUE
  int ls_minValue = LIGHTSENSOR_MIN_VALUE;      // if defined by code
#else
  int ls_minValue = 0;                          // @EB-custom min brightness (0-15) will be overwritten if LIGHTSENSOR_MIN_VALUE is defined
#endif

#ifdef LIGHTSENSOR_MAX_VALUE
  int ls_maxValue = LIGHTSENSOR_MAX_VALUE;      // if defined by code
#else
  int ls_maxValue = 15;                         // @EB-custom max brightness (0-15) will be overwritten if LIGHTSENSOR_MAX_VALUE is defined
#endif

#ifdef ENABLE_LIGHTSENSOR  
  const unsigned int ls_delay = 1000;           // @EB-custom delay time for checking the light sensor (in milliseconds)
  const int ls_triggerStep = 1;                 // @EB-custom if value 1 causes flickering try 2 or 3
  const unsigned int ls_minSensor = 200;        // @EB-custom min input value of the light sensor (0 to 1024 for ESP8266, 0 to 4096 for ESP32)
  #ifdef ESP32
    const unsigned int ls_maxSensor = 4096;     // @EB-custom max input value of the light sensor (ESP32)
  #else
    const unsigned int ls_maxSensor = 1024;     // @EB-custom max input value of the light sensor (ESP8266)
  #endif

  int ls_minMaxRange;
#endif

const char * timeserver1 = "pool.ntp.org";      // @EB-custom select the NTP time servers (check: https://gist.github.com/mutin-sa/eea1c396b1e610a2da1e5550d94b0453)
const char * timeserver2 = "time.google.com";
const char * timeserver3 = "time.nist.gov";

//
// custom settings no change needed
//

const unsigned int startupAnimDelay = 1000;     // @EB-custom min time in milliseconds for displaying the Wifi and time logo
const unsigned int BME280_updateDelay = 5000;   // @EB-custom delay in milliseconds for rereading the BME280 sensor settings (if used)

const unsigned int mainLoopDelay = 5;           // @EB-custom delay in milliseconds for the time loop (display time, blink and check alarms) in the main loop
const unsigned int delayAndCheckEncDelay = 2;   // @EB-custom delay in milliseconds for the delay and check loop (delay while checking the enoder, PIR and lightsensor)
const unsigned int handleEventsDelay = 500;     // @EB-custom delay in milliseconds for checking events in animation, etc loops
const unsigned int OTA_interval = 500;          // @EB-custom delay interval in milliseconds for handling OTA calls
const unsigned int timeRefreshInterval = 250;   // @EB-custom delay in milliseconds before updating the time display

const unsigned int UI_updateDelay = 1000;       // @EB-custom delay in milliseconds for updating the UI 
const unsigned int UI_updateSize = 2500;        // @EB-custom  2500. ESPUI default is 2000, this value is not affected by the amount of widgets

#ifdef UI_INITIAL_SIZE
  const unsigned int UI_InitialSize = UI_INITIAL_SIZE; // if defined by code
#else
  #ifdef ESP32
    const unsigned int UI_InitialSize = 15000;  // @EB-custom 15000 (for ESP8266). ESPUI default is 8000, adjust when you have too many widgets or options
  #else
    const unsigned int UI_InitialSize = 11000;  // @EB-custom 11000 (for ESP8266). ESPUI default is 8000, adjust when you have too many widgets or options
  #endif
#endif

const byte maxAlarms = 12;                      // @EB-custom the max number of alarms. check if it fits in eeprom
                                                //           (the max number fitting into EEPROM is displayed in the serial monitor with DEBUG defined)
const byte PIRdelay = 5;                        // @EB-custom delay in seconds before checking the PIR to prevent retriggering
const byte eepromSaveDelay = 30;                // @EB-custom delay in seconds before saving to EEPROM after using the encode
const byte wifiTimeout = 30;                    // @EB-custom timeout in seconds
const byte timeServerTimeout = 30;              // @EB-custom timeout in seconds
const byte alarmMsgLength = 33;                 // @EB-custom alarm message length
const byte menuTimeout = 10;                    // @EB-custom timeout in seconds for the menu to continue when there is no input

RotaryEncoder RE_encoder(RE_PINA, RE_PINB);     // initialize the rotary encoder

//
// led settings
//

int charSpacer = 1;
int charDefaultWidth = 5 + charSpacer;                // The normal font width is 5 pixels
byte charPos[] = {2, 8, 18, 24};                      // Column positions for the time, UU : MM normal font, blinking colon
byte charPosBigFont[] = {2, 9, 17, 24};               // Column positions for the time, UU : MM big font, blinking colon
byte charPosSec[] = {1, 7, 13, 19, 25, 29};           // Column positions for the time, UU MM ss normal font, seconds 3 pixels
byte charPosSmall[] = {0, 5, 11, 16, 22, 27};         // Column positions for the time, uu mm ss 4 pixels
byte charPosSmallPM[] = {2, 7, 12, 17, 22, 27};       // Column positions for the time, uu mm ss 4 pixels, pm mode
byte charPosSmallCol[] = {0, 5, 11, 16, 23, 28};      // Column positions for the time, uu mm : ss 4 pixels, blinking colon
byte charPosSmallColPM[] = {1, 6, 11, 16, 23, 28};    // Column positions for the time, uu mm : ss 4 pixels, blinking colon, pm mode
byte charPosBME[] = {1, 6, 11, 18, 24, 28};           // Column positions for the temp and humidity

byte charPosLong[] = {1, 7, 13, 19, 25};              // Column positions for the menu, 5 pos
byte charPosOnOff[] = {1, 7, 15, 21, 26};             // Column positions for the menu, on/off characters
byte yOffset = 0;

int xOffset2ndMatrixSet = abs(LED_NUM_OF_HOR_DISPLAYS >= 8) * 32; // offset when using 8 matrices and displaying btoh time and temp

//
// initialize variables
//

const char *WN_ssid = WN_ssid_ca;
const char *WN_password = WN_password_ca;

const char *UI_ssid = UI_ssid_ca;
char * UI_title;
int UI_activeTab = -1;

#ifdef UI_CONTROLS_MINIMAL
  bool UI_controls_minimal = true;
  bool UI_controls_extended = false;
#else
  bool UI_controls_minimal = false;
  #ifdef UI_CONTROLS_EXTENDED
    bool UI_controls_extended = true;
  #else
    bool UI_controls_extended = false;
  #endif
#endif

#ifdef UI_CONTROLS_EXTENDED_SLIDERS
  bool UI_controls_extended_sliders = true;
#else
  bool UI_controls_extended_sliders = false;
#endif

char fileSystemInitialized;
unsigned int versionChecksum = 0;

const char * charPtr;

bool refreshTimeDisplay = true;
unsigned long displayInfoStartTime;
unsigned long startUIupdateTime;

#ifndef NO_WIFI
  IPAddress lanIP;
#endif

#ifdef REVERSE_LR_ENCODER
  int encLeftResult = 1;
  int encRightResult = -1;
#else
  int encLeftResult = -1;
  int encRightResult = 1;
#endif

unsigned long OTA_startTime;
int colon = HIGH;
bool timeManualSet = false;
bool wifiConnected = false;
bool timeServerRead = false;
bool pm = false;
bool announceMode = false;
int announceItem = 0;
int reSwitch, reEnc;
byte returnCode = 0;
int itemToDisplay = 0;
String daylightInfo;

byte activeTimeDisplayMode = 0;
byte alarmNum = 0;
bool alarmTriggered = false;
bool alarmMessage = false;
bool alarmRepeat = false;
bool alarmSuspended = true;                       // don't trigger at start up
bool alarmIsInterrupted = false;
bool prevPIRstate = LOW;
bool sleeping = false;
bool forceStartSleepMode = false;
bool forceEndSleepMode = false;
bool forceFirstAlarm = false;
bool animationStarted = false;
int refreshTempSwapTime = 0;
int matrixWidth;
unsigned long sleepStartedTime;

bool UI_alarmToInterrupt = false;                 // set with the UI
bool UI_timeToResync = false;                     // set with the UI
bool UI_displayTimeRead = true;                   // display only at startup or manual resyncing of the time
int UI_numOfControls;

unsigned long sleepStartTime = 0;
unsigned long PIRstartTime = 0;
unsigned long BME280_startTime = 0;

// tm structure
// struct tm {
//   int tm_sec;         /* seconds,  range 0 to 59                       */
//   int tm_min;         /* minutes, range 0 to 59                        */
//   int tm_hour;        /* hours, range 0 to 23                          */
//   int tm_mday;        /* day of the month, range 1 to 31               */
//   int tm_mon;         /* month, range 0 to 11                          */
//   int tm_year;        /* The number of years since 1900                */
//   int tm_wday;        /* day of the week since Sunday, range 0 to 6    */
//   int tm_yday;        /* day in the year, range 0 to 365               */
//   int tm_isdst;       /* daylight saving time                          */
// };
struct tm timeData;

/*
  Notes on alarms:
  - an alarm is triggered when it matches the hour, min and sec. if a specific day, day of the week or month or
    weekDays or weekendDays is set, it has to match that as well.
    it also means that if an alarm is set at 6:30:00 with no other settings, it will trigger every day at that time
  - checking for the other settings is done from top (month) to bottom (weekendDays) where the last one
    has the highest priority. So if the weekday is set to Thursday and weekenddays is set to true,
    the alarm won't trigger as the last check for Saturday or Sunday is not true
  - alarms are checked from the 1st (alarm 1) to the last. at the 1st match it will trigger
    and not any other alarms (but it will update recurring ones)
  - recurring alarms start after the 1st time they match the set hour, min and sec
*/

struct alarmType {
  bool active = false;                          // if unactive the alarm is not checked
  byte hour = 0;                                // hour, min and sec: time to trigger the alarm
  byte min = 0;
  byte sec = 0;
  byte month = 0;                               // month to trigger the alarm (1-12, additional to hour, min and sec, day can be set optionally)
  byte day = 0;                                 // day of the month to trigger the alarm (1-31, additional to hour, min and sec, month can be set optionally)
  byte weekDay = 0;                             // triggers day of the week (Sunday to Saturday, 1 to 7)
  bool weekDays = false;                        // triggers daily Monday to Friday
  bool weekendDays = false;                     // triggers Saturday and Sunday
  byte recurringType = 0;                       // the alarm triggers every recurringInterval: 1 sec, 2 min, 3 hour, 4 day, 5 month
  unsigned int recurringInterval = 0;           // the interval for recurring alarms
  char message[alarmMsgLength] = "";            // message to display (or the time when left empty)
  unsigned long duration = 0;                   // duration of the alarm (message) in seconds
  byte buzzer = 0;                              // 0: off, > 0 and < 10: num of beeps. use 4 for the alarm clock (= 1 second)
                                                // 11: big ben, 12: Avicii
  bool bright = false;                          // true: alarm triggers bright leds
  byte fx = 0;                                  // 0: off, 1: blink (doesn't work with a message)
  byte actionType = 0;                          // 0: off
                                                //    set 
                                                // 1: set GPIO pin defined by actionNum high, set duration to 0 to keep it high. 
                                                // 2: set GPIO pin defined by actionNum low, set duration to 0 to keep it low. GPIO will be set high on startup
                                                // 3 to 6 neopixels mode: NUM_OF_NEOPIXELS and PIN_NEOPIXELS have to be defined
                                                // 3: neopixels wakeup light
                                                // 4: neopixels wakeup light, chaining the next alarm when ended (not interrupted)
                                                // 5: neopixels on, using the color actionNum of the neoPalette array
                                                // 6: neopixels off (or set to backlight color, if set)
                                                
  byte actionNum = 0;                           // for actionType 1-2: the GPIO pin number. note: use (GPIO) numbers, so 12 instead of D6 for the Wemos D1
                                                // for actionType 5: the color actionNum of the neoPalette array
};

alarmType alarmList[maxAlarms];

time_t currentTime = 0;
String timeString;
String setFeatures;

bool alarmRecTriggered[maxAlarms];
time_t alarmRecNext[maxAlarms];

const int eepromBufSize = 1024;                 // @EB-todo
unsigned long eepromSavedTime = 0;
bool eepromValChanged = false;

struct {                                        // EEPROM init code
  char idStart = ' ';
  bool displayInfo = displayInfo;
  unsigned int displayInfoInterval = displayInfoInterval;
  bool suspendMode = suspendMode;
  bool sleepMode = sleepMode;
  unsigned int sleepDelay = sleepDelay;
  bool displayDaylight = displayDaylight;
  byte ledBrightness = ledBrightness;
  bool ledBrightnessManualSet = ledBrightnessManualSet;
  bool ampmMode = ampmMode;
  byte timeDisplayMode = timeDisplayMode;
  char fileSystemInitialized = ' ';
  unsigned int versionChecksum = versionChecksum;
  unsigned int neoBacklight = neoBacklight;
  int timeZone = timeZone;
  byte DSTmode = DSTmode;
  byte BME280_display = BME280_display;
  byte BME280_fahrenheid = BME280_fahrenheid;
  bool BME280_tempTimeSwap = BME280_tempTimeSwap;
  char spacer[9] = {};
  char idEnd = ' ';
} eepromInit;

const byte UI_DNS_PORT = 53;
DNSServer UI_dnsServer;
byte UI_alarmNum = 0;

//
// user interface wifi setup init
//

const byte UI_sw_length = WIFI_CRED_LENGTH;     // note: including closing 0 char
struct UI_sw_settings_ST {
  char idStart = ' ';
  char ssid[UI_sw_length];
  char password[UI_sw_length];
  char idEnd = ' ';
};
struct UI_sw_settings_ST UI_sw_EEPROM;
bool UI_sw_eepromset;

//
// font number definitions for 3 and 4 colums (only ascii characters 48 to 57 = 0 to 9)
// online font editor: https://github.com/pjrp/MD_MAX72XX-MD_Parola-Font-Editor
//

unsigned int numberFont3[] PROGMEM {  
  3, 124, 68, 124,  // 48 0
  3, 72, 124, 64,   // 49 1
  3, 72, 100, 88,   // 50 2
  3, 68, 84, 124,   // 51 3
  3, 60, 32, 112,   // 52 4
  3, 92, 84, 116,   // 53 5
  3, 124, 84, 116,  // 54 6
  3, 4, 4, 124,     // 55 7
  3, 124, 84, 124,  // 56 8
  3, 92, 84, 124,   // 57 9
  3, 120, 20, 120,  // 58 a
  3, 124, 20, 28,   // 59 p
  3, 124, 8, 124,   // 60 m
};

unsigned int numberFont4[] PROGMEM {
  4, 126, 66, 66, 126,  // 48 0
  4, 0, 68, 126, 64,    // 49 1
  4, 68, 98, 82, 76,    // 50 2
  4, 66, 74, 74, 126,   // 51 3
  4, 30, 16, 16, 124,   // 52 4
  4, 78, 74, 74, 122,   // 53 5
  4, 126, 74, 74, 122,  // 54 6
  4, 66, 34, 18, 14,    // 55 7
  4, 126, 74, 74, 126,  // 56 8
  4, 94, 82, 82, 126,   // 57 9
  4, 60, 66, 66, 66,    // 58 C
  4, 126, 18, 18, 2,    // 59 F
  4, 8, 8, 8, 8,        // 60 - (ascii 45)
};

// flexibel width font characters: the first variable is the max width per char and defines where the next char starts.
// the 1st variable in the char lines defines the width per char

unsigned int numberFontBig[] PROGMEM = { 0x06,
  0x06, 0x7E, 0xFF, 0x81, 0x81, 0xFF, 0x7E, // 48 0
  0x06, 0x00, 0x82, 0xFF, 0xFF, 0x80, 0x00, // 49 1
  0x06, 0xC2, 0xE3, 0xB1, 0x99, 0x8F, 0x86, // 50 2
  0x06, 0x42, 0xC3, 0x89, 0x89, 0xFF, 0x76, // 51 3
  0x06, 0x38, 0x3C, 0x26, 0x23, 0xFF, 0xFF, // 52 4
  0x06, 0x4F, 0xCF, 0x89, 0x89, 0xF9, 0x71, // 53 5
  0x06, 0x7E, 0xFF, 0x89, 0x89, 0xFB, 0x72, // 54 6
  0x06, 0x01, 0x01, 0xF1, 0xF9, 0x0F, 0x07, // 55 7
  0x06, 0x76, 0xFF, 0x89, 0x89, 0xFF, 0x76, // 56 8
  0x06, 0x4E, 0xDF, 0x91, 0x91, 0xFF, 0x7E, // 57 9
};


//
// custom characters
// note: the 1st number is the number of columns
//

unsigned int fontCharLogo[]       { 10, 0, 60, 66, 129, 157, 145, 145, 66, 60, 0 };
//const unsigned int fontCharWifi[]       { 8, 2, 9, 37, 117, 37, 9, 2, 0 };                // wifi version 0
unsigned int fontCharWifi[]       { 8, 2, 9, 37, 85, 37, 9, 2, 0 };                 // wifi version 1
unsigned int fontCharHourglass[]  { 8, 227, 149, 201, 229, 201, 149, 227, 0 };
//const unsigned int fontCharArrowRight[] { 8, 28, 28, 28, 62, 28, 8, 0, 0 };               // arrow right
unsigned int fontCharArrowRight[] { 8, 56, 0, 56, 56, 56, 124, 56, 16 };            // arrow right with blank bank left

unsigned int fontCharPacMan[][10] {     // animated PacMan ;-)
  { 9, 0, 28, 62, 127, 119, 101, 38, 4, 0 },
  { 9, 0, 28, 62, 127, 127, 125, 62, 28, 0 }
};

unsigned int fontCharClock[][9] {       // animated clock
  { 8, 28, 34, 65, 77, 73, 34, 28, 0 },
  { 8, 28, 34, 65, 93, 65, 34, 28, 0 },
  { 8, 28, 34, 73, 77, 65, 34, 28, 0 },
  { 8, 28, 34, 65, 77, 65, 34, 28, 0 }
};

unsigned int fontCharUpdate[][9] {      // animated arrow
  { 8, 56, 68, 128, 128, 128, 68, 56, 0 },
  { 8, 56, 68, 130, 130, 130, 68, 0, 0 },
  { 8, 56, 68, 2, 2, 2, 68, 56, 0 },
  { 8, 0, 68, 130, 130, 130, 68, 56, 0 }
};

//
// neopixel settings
//

#ifdef ENABLE_NEOPIXELS
  // byte array: R,G,B,brightness (all 0-255)
  
  byte neoColorsSunrise[][4] = {    // @EB-setup sunset colors
    {120,   0,   0,  30},
    {219,  58,  19,  50},
    {254, 173,  18,  80},           // Dark Tangerine 254,173,18
    {255, 255, 255, 255}            // white 255,255,255
  };  

  byte neoPalette[][4] = {          // #EB-custom neopixel pallete colors (R,G,B)
    {  0,   0,   0,   0},           // 0 off
    {255, 191,   0,  31},           // 1  12,5% amber
    {255,   0,   0, 128},           // 2  50% red
    {  0, 255,   0, 128},           // 3  50% green
    {  0,   0, 255, 128},           // 4  50% blue
    {255, 255, 255, 128},           // 5  50% white
    {255, 255, 255, 255},           // 6 100% white
    {255, 255,   0, 128},           // 7  50% yellow
    {  0, 255, 255, 128},           // 8  50% aqua
    {255,   0, 255, 128},           // 9  50% purple
  };

  const int neoFadeNumOfSteps = 50; // @EB-setup number of steps to fade in between colors

  int numOfNeoPalette;
  float neoCurColorR = 0;
  float neoCurColorG = 0;
  float neoCurColorB = 0;
  
  byte neoCurBrightness = 0;

#endif
