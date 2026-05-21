/*
***************************************************************************
  ebc_alarmclock - definitions and libraries
***************************************************************************
  last update 20210327 by ericBcreator
***************************************************************************
*/

//
// setup definitions
//

/*
  Explanation:
  
  // wifi  
  #define NO_WIFI
  @EB-setup uncomment when there is no Wifi connection, the web interface will be disabled and the date and time have to be set at startup 
  with the rotary encoder.  note: if the encoder is also disabled, the date and time will be set by code and settings can't be changed manually

  #define WIFI_CRED_LENGTH 21
  @EB-custom the max length for the SSID and password (including closing 0 char) note: changing it will erase the EEPROM settings

  // languages
  uncomment one of the definitions:
  #define LANGUAGE_ENG                            // @EB-setup set the language: English (default)
  #define LANGUAGE_DUT                            // @EB-setup set the language: Dutch
  #define LANGUAGE_DEU                            // @EB-setup set the language: Deutsch
  #define LANGUAGE_ITA                            // @EB-setup set the language: Italiano

  // ota
  #define OTA_UPDATE
  @EB-setup uncomment to enable OTA (over the air updates)

  // display settings
  #define DISPLAY_SYSTEM_INFO
  #EB-custom display additional system info on the web interface

  #define AMPM_2ND_MATRICES
  @EB-custom when using 8 (or more) matrices and setting AM/PM mode active, the AM/PM symbol will appear 
  on matrices 5-8 instead of only lighting the 2 middle leds on the far left

  // ESPUI
  #define UI_USE_FILESYSTEM
  @EB-setup enables the ESPUI filesystem, check the Github ESPUI webpage for more info.
  (the filesystem is prepared only at the 1st boot or after a firmware update)
  
  #define UI_INIT_FILESYSTEM
  @EB-custom force reinitializing the filesystem (each time at startup). needed after a firmware update of ESPUI and when changing language definitions

  ** ESPUI issue: ESP32 tested with over 170 controls: no problems but ESP8266: over ca 60-65 controls the web interface (randomly or completely) 
  stops appearing (only the "Control connected" message is visible, the rest of the page is blank)
  the problem is issued at the ESPUI Github page (https://github.com/s00500/ESPUI/issues/92), but there is no solution yet (as of 22 Oct 2020)
  defining UI_CONTROLS_MINIMAL will minimize the number of controls and might work (or not)

  #define UI_CONTROLS_MINIMAL
  @EB-custom use a minimal number of controls to avoid the disappearing web interface problem controls not available: 
  display info interval, sleep delay, suspendmode, alarm: month, day, recurring type, recurring interval, reset all alarms

  #define UI_CONTROLS_EXTENDED_SLIDERS
  @EB-custom use sliders instead of number controls. takes up 2 additional controls per item
  
  #define UI_CONTROLS_EXTENDED
  @EB-custom display additional controls (restart and reset wifi). for (ESP32) boards with no web interface problem

  #define UI_INITIAL_SIZE 15000
  @EB-custom initial (buffer?) size for the ESPUI library, default is 11000. defining this will overwite the default value
  increase when using many controls, i.e. 15000 for the ESP32 with maximum controls

  // sleep hours
  @EB-custom if defined, the display will turn off between the on and off times
  if SLEEP_HOURS_DISABLE_WIFI is defined, wifi will be turned off during sleep hours and will reconnect after
  
  #define SLEEP_HOURS           
  #define SLEEP_HOUR_ON   23
  #define SLEEP_MIN_ON    00
  #define SLEEP_HOUR_OFF  07
  #define SLEEP_MIN_OFF   00
  #define SLEEP_HOURS_DISABLE_WIFI

*/
//#define NO_WIFI                               // @EB-setup

#define LANGUAGE_ENG                            // @EB-setup set the language: English (default). note: uncomment only 1 of the language definitions
//#define LANGUAGE_DUT                            // @EB-setup set the language: Dutch
//#define LANGUAGE_DEU                            // @EB-setup set the language: Deutsch
//#define LANGUAGE_ITA                            // @EB-setup set the language: Italiano

#define WIFI_CRED_LENGTH 21                     // @EB-custom
#define OTA_UPDATE                              // @EB-custom
#define DISPLAY_SYSTEM_INFO                     // @EB-custom
#define AMPM_2ND_MATRICES                       // @EB-custom

// preset clock definitions
//#define CLOCK_ESP8266_TEST                    // ESP8266 test
//#define CLOCK_ESP32_TEST                      // ESP32 test
//#define CLOCK_ESP32_MINI_TEST                 // ESP32 mini test. note: GPIO 12 single led. board: WEMOS D1 MINI ESP32

//
// debug settings
//

//#define DEBUG                                 // print debug messages
//#define DEBUG_FAST_START                      // disable startup messages
//#define DEBUG_NO_TIMESERVER                   // disable the timeserver. search for @EB-DEBUG_NO_TIMESERVER to set the time manually (or with the rotary encoder)
//#define DEBUG_UI_MESSAGES                     // print ESPUI debug messages
//#define DEBUG_RESET_WIFI                      // reset ssid and password stored in EEPROM
//#define DEBUG_NO_WIFI                         // disable Wifi and syncing to the timeserver
//#define DEBUG_SET_TEST_ALARMS                 // setup some test alarms
//#define DEBUG_NO_WRITE_ALARMS_TO_EEPROM       // don't write to EEPROM
//#define DEBUG_RESET_EEPROM                    // clears the EEPROM settings
//#define DEBUG_LIGHTSENSOR                     // display light sensor readings
//#define DEBUG_PIR                             // display PIR motion detections

#define DEBUG_ESP32_DISABLE_BROWNOUT            // @EB-todo: ESP32 disable brownout detection which causes (wifi) startup problems

//
// libraries
//

#include <Adafruit_GFX.h>                       // Needed for the matrix library      https://github.com/adafruit/Adafruit-GFX-Library
#include <Dusk2Dawn.h>                          // Dusk2Dawn sunrise sunset library   https://github.com/dmkishi/Dusk2Dawn
                                                // @EB-setup comment the line above when Dusk2dawn is not installed/used and DAYLIGHTINFO is undefined
#include <EEPROM.h>                             // Eeprom library
#include <Max72xxPanel.h>                       // Matrix library                     https://github.com/markruys/arduino-Max72xxPanel
#include <RotaryEncoder.h>                      // Rotary encoder library             https://github.com/mathertel/RotaryEncoder
#include <TimeLib.h>                            // Time library                       https://github.com/PaulStoffregen/Time

//#ifndef NO_WIFI (@EB: disabled because excluding the libraries causes multiple undefined errors)
  #ifdef ESP32
    #include <WiFi.h>
    #ifdef DEBUG_ESP32_DISABLE_BROWNOUT         // @EB-todo disable brownout detection to prevent Wifi startup problems
      #include "soc/soc.h"
      #include "soc/rtc_cntl_reg.h"
    #endif
  #else
    #include <ESP8266WiFi.h>
  #endif

  #include <ArduinoOTA.h>                       // Arduino OTA (over the air) library
  #include <DNSServer.h>
  #include <ESPUI.h>                            // ESP user interface library         https://github.com/s00500/ESPUI
//#endif

//
// define features and pins
//

#ifdef CLOCK_ESP32_TEST
  char UI_ssid_ca[WIFI_CRED_LENGTH] = "ericBclock-E32";
  const char *UI_password = "ericBclock";
  IPAddress UI_apIP(192, 168, 4, 2);
//#define UI_USE_FILESYSTEM
//#define UI_INIT_FILESYSTEM
  #define UI_CONTROLS_EXTENDED_SLIDERS
  #define UI_CONTROLS_EXTENDED
  #define UI_INITIAL_SIZE 15000
  #define NO_BUZZER
  #define NO_ENCODER
  #define ANNOUNCE_MODE
  #define DAYLIGHTINFO
  #define DEBUG
  #define DEBUG_FAST_START

#elif defined(CLOCK_ESP32_MINI_TEST)
  // rotary encoder: blue GND, grey SWITCH, white A, black B (VCC not used)
//  #define LED_NUM_OF_HOR_DISPLAYS 8  
  char UI_ssid_ca[WIFI_CRED_LENGTH] = "ericBclock-ME32";
  const char *UI_password = "ericBclock";
  IPAddress UI_apIP(192, 168, 4, 2);
//#define UI_USE_FILESYSTEM
//#define UI_INIT_FILESYSTEM
  #define UI_CONTROLS_EXTENDED_SLIDERS
  #define UI_CONTROLS_EXTENDED
  #define UI_INITIAL_SIZE 20000
//  #define ENABLE_LIGHTSENSOR
//  #define LIGHTSENSOR_AVG
//  #define ENABLE_PIR
//  #define PIR_DEFAULT LOW
//  #define ANNOUNCE_MODE
  #define DAYLIGHTINFO
//  #define ENABLE_NEOPIXELS
//  #define NUM_OF_NEOPIXELS 26
//  #define ENABLE_BME280
//  #define BME280_SEA_LEVEL_PRESSURE (1013.00)   // @EB-custom (1013.25 is generic)
//  #define BME280_I2C_ADDRESS 0x76               // define device I2C address: 0x76 or 0x77 (0x77 is library default address)
  #define DEBUG

#elif defined(CLOCK_ESP8266_TEST)
  char UI_ssid_ca[WIFI_CRED_LENGTH] = "ericBclock-E66";
  const char *UI_password = "ericBclock";
  IPAddress UI_apIP(192, 168, 4, 2);
  #define UI_USE_FILESYSTEM
  #define UI_CONTROLS_EXTENDED_SLIDERS
  #define NO_BUZZER
  #define NO_ENCODER
  #define ANNOUNCE_MODE
  #define DAYLIGHTINFO  
  #define DEBUG
  #define DEBUG_FAST_START

#else  // default setup

//#define LED_NUM_OF_HOR_DISPLAYS 4             // @EB-setup defaults to 4, can be more, set to 8 for 2 4x8x8 matrices (support time and temp displaying at the same time)
  #define UI_USE_FILESYSTEM                     // @EB-setup
//#define UI_INIT_FILESYSTEM                    // @EB-custom
//#define UI_CONTROLS_MINIMAL                   // @EB-custom
//#define UI_CONTROLS_EXTENDED_SLIDERS          // @EB-custom
//#define UI_CONTROLS_EXTENDED                  // @EB-custom
//#define UI_INITIAL_SIZE 15000                 // @EB-custom. will be set to 11000 for ESP8266 and 15000 for ESP32 if not defined

//  #define SLEEP_HOURS                         // @EB-setup. the clock will enter sleepmode (display off) between the on and off times. disabled if SLEEP_HOURS is undefined
//  #define SLEEP_HOUR_ON   23
//  #define SLEEP_MIN_ON     0
//  #define SLEEP_HOUR_OFF   5
//  #define SLEEP_MIN_OFF   30
//  #define SLEEP_HOURS_DISABLE_WIFI            //            if defined, it disables wifi when sleep time starts and reconnects when sleep time is over

  char UI_ssid_ca[WIFI_CRED_LENGTH] = "ericBclock-01";        
                                                // @EB-custom UI SSID. note: a dash and the lower 2 bytes of the chipid will be added
  const char *UI_password = "ericBclock";       // @EB-custom UI password (min. length 8 char or it won't work!!)
  IPAddress UI_apIP(192, 168, 4, 1);            // @EB-setup web user interface IP
  
//#define NO_BUZZER                             // @EB-setup uncomment when there is no buzzer
//#define NO_ENCODER                            // @EB-setup uncomment when not using a rotary encoder (only using the web interface)
  #define REVERSE_LR_ENCODER                    // @EB-setup uncomment to reverse left and right function of the encoder
  
  #define ENABLE_LIGHTSENSOR                    // @EB-setup uncomment when using a lightsensor
  #define LIGHTSENSOR_AVG                       // @EB-setup uncomment to average the sensor values
//#define LIGHTSENSOR_CURVE                     // @EB-setup uncomment to use a curve (slow rising) for setting the brightness
  #define LIGHTSENSOR_OFFSET 0                  // @EB-setup -/+ offset for the brightness when using a lightsensor
//#define LIGHTSENSOR_MIN_VALUE 0               // @EB-setup brightness min value (0-15). will be set to 0 if not defined
//#define LIGHTSENSOR_MAX_VALUE 15              // @EB-setup brightness max value (0-15). will be set to 15 if not defined
  
  #define ENABLE_PIR                            // @EB-setup uncomment when using a motion sensor
  #define PIR_DEFAULT LOW                       // @EB-setup set HIGH if the motion sensor defaults to HIGH or else set LOW
  
  #define ANNOUNCE_MODE                         // @EB-setup uncomment to activate announcement mode
  #define DAYLIGHTINFO                          // @EB-setup enable daylight info. if enabled, you need to set the next 4 variables

//#define ENABLE_NEOPIXELS                      // @EB-setup uncomment to enable neopixel support
//#define NUM_OF_NEOPIXELS 26                   // @EB-setup the number of neopixels

//#define ENABLE_BME280                         // @EB-setup uncomment when a BME280 sensor is used
//#define BME280_SEA_LEVEL_PRESSURE (1013.25)   // @EB-setup uncomment to display the altitude. use the correct setting, 1013.25 is generic
//#define BME280_I2C_ADDRESS 0x76               // @EB-setup define the I2C address: 0x76 or 0x77 (0x77 is library default address)
  
#endif

//#define NOPACMAN                              // #EB-setup: now why would you ;-)

#ifdef DAYLIGHTINFO
  char d2d_City[] = "Amsterdam";                // @EB-setup City name
  float d2d_lattitude = 52.37936;               // @EB-setup
  float d2d_longitude = 4.900293;               // @EB-setup
  int d2d_timezone = 1;                         // @EB-setup UTC timezone offset
#endif

#ifdef ENABLE_BME280
  #include <Wire.h>                             // include Arduino wire library (required for I2C devices)
  #include <Adafruit_BME280.h>                  // include Adafruit BME280 sensor library
  Adafruit_BME280 BME280;
#endif

//
// define pins
//

#ifdef ESP32                                    // ESP32 (mini) GPIO pin settings. note: pins can also be defined per clock
  #ifndef PIN_CS                                // @EB-setup CS pin LED Matrix (SPI)      // VCC -> 5V  GND -> GND  DIN -> 23 (MOSI)  CS -> 16  CLK -> 18 (SCK)
    #define PIN_CS     16 
  #endif
  #ifndef PIN_BUZZER                            // @EB-setup buzzer/speaker pin (<16)     // black -> GND
    #define PIN_BUZZER  5 
  #endif   
  #ifndef RE_PINA                               // @EB-setup rotary encoder pinA (clock)  // VCC -> 5V  GND -> GND
    #define RE_PINA    27 
  #endif   
  #ifndef RE_PINB                               // @EB-setup rotary encoder pinB (data) 
    #define RE_PINB    32 
  #endif
  #ifndef RE_SWITCH                             // @EB-setup rotary encoder switch
    #define RE_SWITCH  17 
  #endif
  #ifndef PIN_LS                                // @EB-setup light sensor pin - analog!   // VCC -> 5V  GND -> GND
    #define PIN_LS     36 
  #endif
  #ifndef PIN_SCL                               // @EB-setup I2C SCL pin. I2C can be used for a BME280 sensor
    #define PIN_SCL    22 
  #endif
  #ifndef PIN_SDA                               // @EB-setup I2C SDA pin
    #define PIN_SDA    21 
  #endif

  #ifdef ENABLE_PIR
    #ifndef PIN_PIR                             // @EB-setup motion sensor pin            // VCC -> 5V  GND -> GND
      #define PIN_PIR  26 
    #endif
  #endif

  #ifdef ENABLE_NEOPIXELS
    #ifndef PIN_NEOPIXELS                       // @EB-setup pin for the neopixel output
      #define PIN_NEOPIXELS 19 					// = GPIO 12
    #endif
  #endif
    
#else                                           // ESP8266 pin settings
  #ifndef PIN_CS                                // @EB-setup CS pin LED Matrix            // VCC -> 5V  GND -> GND  DIN -> D7 (MOSI)  CS -> D4  CLK -> D5 (SCK)
    #define PIN_CS     D4 
  #endif
  #ifndef PIN_BUZZER                            // @EB-setup buzzer/speaker pin (<16)     // black -> GND
    #define PIN_BUZZER D8 
  #endif
  #ifndef RE_SWITCH                             // @EB-setup rotary encoder switch
    #define RE_SWITCH  D3 
  #endif
  #ifndef PIN_LS                                // @EB-setup light sensor pin - analog!   // VCC -> 5V  GND -> GND
    #define PIN_LS     A0 
  #endif
  
  #ifdef ENABLE_PIR
    #ifndef PIN_PIR                             // @EB-setup motion sensor pin            // VCC -> 5V  GND -> GND
      #define PIN_PIR  D0 
    #endif
  #endif

  // @EB-setup note when using an ESP8266: 
  // The Wemos D1 mini supports I2C on D1 and D2 but they are used for the rotary encoder because there are not enough pins available.
  // So when using a BME280, which has to be connected to D1 and D2 for I2C, you will need 2 extra pins to connect the rotary encoder. 
  // Defaults settings are GPIO pin 9 and 10 (or SD2 and SD3)

  #ifdef ENABLE_BME280
    #ifndef PIN_SCL                             // @EB-setup I2C SCL pin. I2C can be used for a BME280 sensor
      #define PIN_SCL  D1 
    #endif
    #ifndef PIN_SDA                             // @EB-setup I2C SDA pin.
      #define PIN_SDA  D2 
    #endif
    #ifndef RE_PINA                             // SD2 @EB-setup rotary encoder pinA (clock)  // VCC -> 5V  GND -> GND
      #define RE_PINA   9 
    #endif
    #ifndef RE_PINB                             // SD3 @EB-setup rotary encoder pinB (data) 
      #define RE_PINB  10 
    #endif
  #else
    #ifndef RE_PINA                             // @EB-setup rotary encoder pinA (clock)  // VCC -> 5V  GND -> GND
      #define RE_PINA  D1 
    #endif
    #ifndef RE_PINB                             // @EB-setup rotary encoder pinB (data) 
      #define RE_PINB  D2 
    #endif
  #endif

  #ifdef ENABLE_NEOPIXELS
    #ifndef PIN_NEOPIXELS                           // @EB-setup pin for the neopixel output
      #define PIN_NEOPIXELS D6 
    #endif
  #endif
  
#endif

#ifdef ENABLE_NEOPIXELS
  #include <FastLED.h>                          // Fastled library                    https://github.com/FastLED/FastLED  
  CRGB neopixels[NUM_OF_NEOPIXELS];
#endif

//
// set led matrix setup
//

#ifndef LED_NUM_OF_HOR_DISPLAYS                 // if not defined in a preset, setup the led matrix, 4 as default
  #define LED_NUM_OF_HOR_DISPLAYS 4             // @EB-setup led matrix number of columns
#endif

#ifndef LED_NUM_OF_VERT_DISPLAYS
  #define LED_NUM_OF_VERT_DISPLAYS 1            // @EB-setup led matrix number of rows
#endif

#define LED_MATRIX_ROTATION_0 1                 // @EB-setup matrix 0 rotation (0-3)
#define LED_MATRIX_ROTATION_1 1
#define LED_MATRIX_ROTATION_2 1
#define LED_MATRIX_ROTATION_3 1
#define LED_MATRIX_ROTATION_4 1
#define LED_MATRIX_ROTATION_5 1
#define LED_MATRIX_ROTATION_6 1
#define LED_MATRIX_ROTATION_7 1

// debug printing messages in the serial monitor, do not change these lines
#ifdef DEBUG
  #define DEBUGPRINT(x)   Serial.print(x)
  #define DEBUGPRINTLN(x) Serial.println(x)
#else
  #define DEBUGPRINT(x)
  #define DEBUGPRINTLN(x)
#endif
//
