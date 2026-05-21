#define _VERSION  " 3.023"
#define _DATE     "20210327"
#define _AUTHOR   "@eBc"

/*
*******************************************************************************
  ESP8266/32 programmable WiFi alarm clock with web interface by ericBcreator
*******************************************************************************
  version 3.023 - SHARE - last update 20210327 by eBc - tweaks
  version 3.022 - last update 20210320 by eBc - added Italian translation by Giuseppe Macaluso
                                                added restart function to the rotary encoder menu
  version 3.021 - last update 20210314 by eBc - Arduino IDE 2.x compatibility fixes, tweaks & bugfixes
  version 3.020 - last update 20210302 by eBc - fixed recurring alarm 1 keeps ringing after setting it inactive and active again, added wifi disable during sleep hours
  version 3.019 - last update 20210301 by eBc - German translation updated
  version 3.018 - last update 20210210 by eBc - added German translation by Franz Wagner and custom sleep hours (for sleepmode)
  version 3.017 - last update 20210110 by eBc - added timeout for Wifi setup
  version 3.016 - SHARE - last update 20201216 by eBc - tweaks
  version 3.016 - last update 20201213 by eBc - tweaks
  version 3.015 - last update 20201212 by eBc - fixed pin error bug, bugfix am/pm display hh mm ss modes
  version 3.014 - last update 20201211 by eBc - added flexibel pin definitions
  version 3.013 - last update 20201210 by eBc - bugfix matrixWidth defined inside DEBUG def
  version 3.012 - last update 20201209 by eBc - tweaks
  version 3.011 - last update 20201206 by eBc - tweaks
  version 3.010 - last update 20201130 by eBc - tweaks
  version 3.009 - last update 20201129 by eBc - expanded multiple language support, added a start sleep mode button, added 1 to the (displayed) alarm numbers
  version 3.008 - last update 20201128 by eBc - tweaks
  version 3.007 - last update 20201127 by eBc - moved the restart button to the top
  version 3.006 - last update 20201127 by eBc - added save settings to the rotary menu
  version 3.005 - last update 20201127 by eBc - bugfix no displaydaylight not compiling and big font on ESP8266 crashing
                                                added restart button for wifi setup, var char width support alt font
  version 3.004 - last update 20201124 by eBc - bugfix menu max display time modes
  version 3.003 - last update 20201124 by eBc - added a new bigger number font
  version 3.002 - last update 20201122 by eBc - tweaks and bugfixes
  version 3.001 - last update 20201121 by eBc - split info/system control on web interface, tweaks
  version 3.000 - last update 20201121 by eBc - added support for more than 4 led 8x8 matrices (8 intended)

  version 2.910 - SHARE - last update 20201119 by eBc - bugfix ESPUI input/update and PM symbol not appearing problem. increased version numbers
  
  version 2.90 - SHARE - last update 20201115 by eBc - replaced the Adafruit Neopixel library with Fastled because the Adafruit is buggy with an ESP32
  
  version 2.83 - last update 20201114 by eBc - tweaks
  version 2.82 - last update 20201114 by eBc - tweaks
  version 2.81 - last update 20201113 by eBc - BME280 tweaks
  version 2.80 - last update 20201112 by eBc - added BME280 support. 
                 note: it reinitializes the eeprom
                 note: there are 2 pin numbers changes when using an ESP8266 and the BME280
  
  version 2.72 - last update 20201108 by eBc - ESP32 tweak, bug fixes
  version 2.71 - last update 20201106 by eBc - Wemos ESP32 mini definitions
  version 2.70 - last update 20201030 by eBc - tweaks
  version 2.69 - last update 20201030 by eBc - bugfix lightsensor limits, added 777
  version 2.68 - SHARE - last update 20201030 by eBc - bugfix melodies
  version 2.67 - last update 20201029 by eBc - bugfix and update brightness settings
  version 2.66 - last update 20201028 by eBc - tweaks
  version 2.65 - last update 20201027 by eBc - added reverse PacMan animation
  version 2.64 - last update 20201025 by eBc - added PacMan animation
  version 2.63 - last update 20201025 by eBc - added animations
  version 2.62 - last update 20201025 by eBc - tweaks
  version 2.61 - SHARE - last update 20201024 by eBc - added timezone and DST settings to the web interface, extra AM/PM setting, tweaks
  version 2.60 - last update 20201023 by eBc - bugfix ESP32 eeprom settings occasionally resetting
  version 2.59 - last update 20201022 by eBc - added a minimal ui controls option
  version 2.58 - last update 20201020 by eBc - tweaks
  version 2.57 - last update 20201018 by eBc - ESP32 testing littlefs (disabled), tweaks
  version 2.56 - last update 20201017 by eBc - updates to the actions
  version 2.55 - last update 20201014 by eBc - added alarm test button, tweaks
  version 2.54 - last update 20201013 by eBc - neopixel updates: added alarm chain for wake up light and neopixel palette colors
  version 2.53 - last update 20201010 by eBc - tweaks (neopixels, selection lists)
  version 2.52 - SHARE - last update 20201004 by eBc - added wake up light setting for alarm 1 to the encoder menu
  version 2.51 - last update 20201003 by eBc - tweaks to wake up light
  version 2.50 - last update 20201003 by eBc - added neopixel wake up light mode
  
  version 2.41 - last update 20201003 by eBc - recurring type list, sliders disabled
  version 2.40 - last update 20200930 by eBc - UI lists, increased UI_InitialSize
  version 2.39 - last update 20200929 by eBc - UI tab callback function
  version 2.38 - last update 20200929 by eBc - display local IP 
  version 2.37 - last update 20200926 by eBc - tweaks
  version 2.36 - last update 20200923 by eBc - tweaks
  version 2.35 - last update 20200922 by eBc - tweaks
  version 2.34 - last update 20200921 by eBc - tweaks
  version 2.33 - last update 20200920 by eBc - UI filesystem implemented
  version 2.32 - last update 20200920 by eBc - tweaks
  version 2.31 - last update 20200919 by eBc - added UI Wifi selection list
  version 2.30 - last update 20200918 by eBc - replaced UI numbers with sliders
  
  version 2.23 - last update 20200917 by eBc - bugfixes, optimized playTone()
  version 2.22 - last update 20200917 by eBc - bugfixes
  version 2.21 - last update 20200916 by eBc - tweaks & solved eeprom save bug
  version 2.20 - last update 20200915 by eBc - tweaks & bugfixes
  version 2.19 - last update 20200915 by eBc - tweaks & bugfixes
  version 2.18 - last update 20200911 by eBc - tweaks & bugfixes
  version 2.17 - last update 20200910 by eBc - announcement mode updates
  version 2.16 - last update 20200910 by eBc - added announcement mode
  version 2.15 - last update 20200910 by eBc - tweaks & bugfixes
  version 2.14 - last update 20200909 by eBc - tweaks & bugfixes  
  version 2.13 - last update 20200908 by eBc - startup logo, bugfix AM/PM mode and other tweaks
  
  version 2.12 - SHARE - last update 20200906 by eBc
  - added 3 time displays with seconds:
    - UU:MM ss normal size hours and minutes, small seconds
    - uu mm ss small hours, minutes and seconds (with and without colon)
  - added AM/PM mode. PM is indicated by 2 LEDS on the middle left
  
  version 2.11 - last update 20200903 by eBc
  - several tweaks and updates
  - separated settings in multiple source code files
  
  version 2.10 - last update 20200831 by eBc - added sunrise/sunset/daylight info
  version 2.09 - last update 20200825 by eBc - several tweaks and updates
  
  version 2.08 - last update 20200824 by eBc
  - added OTA (over the air) update support (from the Arduino IDE)
    read here how to set it up: https://github.com/esp8266/Arduino/blob/master/doc/ota_updates/readme.rst#arduino-ide
    note 1: connect the pc/mac to the Wifi network set by the ESP for the web user interface to make the port visible in the Arduino IDE
    note 2: the serial monitor doesn't work with OTA...Termite can be used but only with the ESP connected by USB
    
  version 2.07 - last update 20200822 by eBc
  - added NO_WIFI option
  - added setting time and date manually with the rotary encoder
  - several tweaks
  
  version 2.06 - SHARE - last update 20200802 by eBc
  - added a NO_ENCODER setting when not using a rotary encoder
    (settings have to be done with the web interface)
    
  version 2.05 - last update 20200329 by eBc - bugfix DST not resyncing
  version 2.04 - last update 20200110 by eBc - several tweaks and updates
  version 2.03 - last update 20200102 by eBc - scrollDelayTime tweak  
  version 2.02 - last update 20200101 by eBc - bugfix menu not updating displayInfo variable, tweaks  
  version 2.01 - last update 20191231 by eBc - tweaks  
  version 2.00 - last update 20191230 by eBc - update for ESPUI 2.0, tweaks
  
  version 1.21 - last update 20191226 by eBc - tweaks to the menu
  version 1.20 - SHARE - last update 20191225 by eBc
  - bugfix alarm not triggering at the same second
  - several tweaks to the menu
  version 1.19 - last update 20191224 by eBc - tweaks to the menu
  version 1.18 - last update 20191222 by eBc - added a rotary encoder menu for changing settings
  version 1.17 - last update 20191221 by eBc - added timeOffsetSec, tweaks  
  version 1.16 - last update 20191130 by eBc - several improvements
  version 1.15 - last update 20191110 by eBc - added Avicii tune (number 12)
  version 1.14 - SHARE - last update 20191027 by eBc - fixed DST error (year and month check and not checking while sleeping)
  version 1.13 - last update 20190602 by eBc
  version 1.11 - SHARE - last update 20190602 by eBc - fixed error message when ENABLE_LIGHTSENSOR was undefined
  version 1.10 - SHARE - last update 20190601 by eBc
  - added motion sensor support
  - added light sensor support
  version 1.00 - SHARE - last update 20190525 by eBc
  version 0.92 - SHARE - last update 20190521 by eBc  
*******************************************************************************
*/
