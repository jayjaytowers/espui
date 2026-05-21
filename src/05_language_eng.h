/*
***************************************************************************
  ebc_alarmclock - language settings - English
***************************************************************************
  last update 20210319 by ericBcreator
***************************************************************************
*/

//
// language settings
//

#ifndef _LANGUAGE
  #define _LANGUAGE "ENG"
  
  const String months[]                   = { "January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December" };
  const String days[]                     = { "Sunday", "Monday", "Tuesday", "Wednessday", "Thursday", "Friday", "Saturday" };
  
  const char * msgAlarm0Active            = "Alarm 1 active, set at";
  const char * msgAlarm0Inactive          = "Alarm 1 inactive";
  const char * msgAlarm0Set               = "Alarm 1 set at";
  const char * msgAlarmOff                = "Alarm off";
  const char * msgConnTSfailed            = "* Connection to the timeserver failed...";
  const char * msgConnWifiFailed          = "* Wifi connection failed...";
  const char * msgDateSet                 = "Date set at";
  const char * msgDaylightTime            = "hours daylight";
  const char * msgDSTon                   = "DST on";
  const char * msgDSToff                  = "DST off";
  const char * msgLedBrightnessSet        = "Brightness set to";
  const char * msgMenu                    = "Menu";
  const char * msgNewVersion              = "New firmware version ";
  const char * msgNoWifi                  = "No Wifi connection";
  const char * msgNoWifiNoRE              = "Warning: Wifi and encoder disabled";           // no Wifi and no encoder so unable to change any settings
  const char * msgOn                      = "On "; // 3 characters
  const char * msgOff                     = "Off"; // 3 characters
  const char * msgOTAupdateStart          = ">> Starting firmware update over the air";
  const char * msgOTAupdateEnd            = "<< Firmware updated, restarting...";
  const char * msgOTAupdateError          = ">* Error firmware update: ";
  const char * msgRecurring               = "Recurring: ";
  const char * msgSunrise                 = "Sunrise";
  const char * msgSunset                  = "Sunset";
  const char * msgbmeTemperature          = "Temperature";
  const char * msgbmeHumidity             = "humidity";
  const char * msgbmePressure             = "pressure";
  const char * msgTimeSet                 = "Time set at";
  const char * msgTimeSynced              = "Time read ,";
  const char * msgUIfilesystemInit0       = "Preparing the filesystem...";
  const char * msgUIfilesystemInit1       = "Filesystem prepared";
  const char * msgUIstart0                = "Web interface started. Connect to SSID ";
  const char * msgUIstart1                = " and browse to ";
  const char * msgUIstart2                = " or lan ";
  const char * msgUIwifiSetup0            = "Starting WiFi web setup. Connect to SSID ";
  const char * msgUIwifiSetup1            = " and browse to ";
  const char * msgUIwifiReset             = "Wifi reset, restarting...";
  const char * msgWakeupLight             = " - wake up light";
  const char * msgWakeupLightChain        = " - wake up light, chain next";
  
  // menu items
  const String menu[] {
    "Set alarm 1",
    "Led brightness",
    "Neopixel backlight",
    "Sync time",
    "Sleep mode",
    "Suspend mode",
    "Date display",
    "Interval info display",
    "Set time",
    "Set date",
    "Display mode",
  //  "UI minimal controls",  // @EB-todo disabled
    "System info",
    "Save settings",
    "Restart",
  #ifdef ANNOUNCE_MODE
    "Announcements",
  #endif
    "Exit menu"
  };
  
  const String timeDisplayModes[] {
    "HH:MM",
    "HH:MM big font",
    "HH MM ss",
    "HH MM am/pm",
    "hh mm ss",
    "hh mm:ss",
    #ifdef ENABLE_BME280
      "tmp hu"
    #endif
  };
  
  const char * msgMenuAlarm0              = "Set the time for alarm 1";
  const char * msgMenuAlarm0wul           = "Wake up light";
  const char * msgMenuLedBrightness       = "Set the led brightness, -1 is auto (sensor)";
  const char * msgMenuNeoBacklight        = "Set the neopixels backlight color";
  const char * msgMenuSyncTime            = "Sync with the timeserver";
  const char * msgMenuSleepMode           = "Set sleep mode";
  const char * msgMenuSuspendMode         = "Set suspend mode";
  const char * msgMenudisplayInfo         = "Display the date";
  const char * msgMenuDisplayInfoInterval = "Set the display info interval (in seconds)";
  const char * msgMenuRestart             = "Restart";
  const char * msgMenuSave                = "Save settings";
  const char * msgMenuSaved               = "Settings saved";
  const char * msgMenuSetTime0            = "Set the time: hours and minutes";
  const char * msgMenuSetTime1            = "seconds";
  const char * msgMenuSetDate0            = "Set the date: year";
  const char * msgMenuSetDate1            = "month and day";
  const char * msgMenuTimeDisplayMode     = "Set the time display mode";
  const char * msgMenuUIminimal           = "Web interface minimal controls";
  const char * msgMenuAnnouncement0       = "Announcement mode, select the message";
  const char * msgMenuAnnouncement1       = "Starting announcement mode";
  const char * msgMenuNotAvailable        = "Not available"; 
  
  // web user interface - wifi setup
  const char * UI_msg_sw_info =           "Setup the WiFi connection";
  const char * UI_msg_sw_ssid =           "Select the SSID";
  const char * UI_msg_sw_password =       "Enter the password";
  const char * UI_msg_sw_saveButton =     "Save";
  const char * UI_msg_sw_restartButton =  "Restart";
   
  #define UI_sw_msgInfo0                  "Select the SSID and enter the password of the network to connect to (default: max 20 entries). <br><br> Note: the max. length of the SSID and password is "
  #define UI_sw_msgInfo1                  " characters, longer entries will be truncated. <br><br> Click the Save button to store the new settings and restart.<br>Click the Restart button to restart and reread the networks."
  #define UI_sw_msgRestarting             "Settings saved, restarting..."
  
  // web user interface
  const char * UI_msg_saveToEEPROM        = "Save to memory";
  const char * UI_msg_info0               = "Status";
  const char * UI_msg_info1               = "System info";
  const char * UI_msg_resync              = "Sync with a timeserver";
  const char * UI_msg_interruptAlarm      = "Alarm off / stop";
  const char * UI_msg_timeZone            = "Timezone";
  
  const char * UI_msg_DSTmode             = "DST mode";
  const String UI_msg_DSTmodes[] {
    "Off",
    "On",
    "Auto"
  };
  
  const char * UI_msg_timeDisplayMode     = "Time display mode";
  const String UI_msg_timeDisplayModes[] {
    "Hours and minutes (colon)",
    "Hours and minutes big font",
    "Hours, minutes and small seconds",
    "Hours, minutes and AM/PM",
    "Small hours, minutes and seconds",
    "Small hours, minutes and seconds (colon)",
    #ifdef ENABLE_BME280
      "Temperature and humidity" 
    #endif
  };
  
  const char * UI_msg_ampmMode            = "AM/PM mode";
  const char * UI_msg_displayInfo         = "Display info";
  const char * UI_msg_displayInfoInterval = "Display info interval (in seconds)";
  const char * UI_msg_suspendMode         = "Suspend mode";
  const char * UI_msg_sleepMode           = "Sleep mode";
  const char * UI_msg_sleepDelay          = "Sleep delay (in seconds)";
  const char * UI_msg_sleepStart          = "Start sleep mode";
  const char * UI_msg_displayDaylight     = "Display daylight info";
  const char * UI_msg_displayBME280       = "Display temperature info";
  const char * UI_msg_BME280_fahrenheid   = "Temperature in Fahrenheid";
  const char * UI_msg_BME280_tempTimeSwap = "Alternate time / temperature";
  const char * UI_msg_brightnessAuto      = "Brightness auto";
  const char * UI_msg_brightness          = "Brightness";
  const char * UI_msg_neoBacklight        = "Neopixel backlight color (0 is off)";
  const char * UI_msg_resetWifi           = "Reset wifi settings";
  const char * UI_msg_restart             = "Restart";
  
  const char * UI_msg_pad                 = "Select the alarm";
  const char * UI_msg_alarmNumber         = "Alarm number";
  const char * UI_msg_message             = "Message";
  const char * UI_msg_alarmActive         = "Alarm active";
  const char * UI_msg_hour                = "Hour";
  const char * UI_msg_min                 = "Minutes";
  const char * UI_msg_sec                 = "Seconds";
  const char * UI_msg_month               = "Month";
  const char * UI_msg_day                 = "Day";
  const char * UI_msg_weekDay             = "Day of the week";
  const char * UI_msg_duration            = "Duration (in seconds)" ;
  const char * UI_msg_weekDays            = "Week days";
  const char * UI_msg_weekendDays         = "Weekend days";
  
  const char * UI_msg_recurringType       = "Recurring type";
  const String UI_msg_recurringTypes[] {
    "(off)",
    "Second(s)",
    "Minute(s)",
    "Hour(s)",
    "Day(s)",
    "Month(s)"
  };
  
  const char * UI_msg_recurringInterval   = "Recurring interval";
  const char * UI_msg_buzzer              = "Buzzer";
  const char * UI_msg_bright              = "Bright";
  const char * UI_msg_fx                  = "Effect";
  const char * UI_msg_fx0                 = "(off)";
  const char * UI_msg_fx1                 = "Blinking text / flashing neopixels";
  
  const char * UI_msg_actionType          = "Action type";
  const String UI_msg_actionTypes[] {
    "(off)",                                                              // 0
    "Set pin action number high (duration 0 will keep it high)",          // 1
    "Set pin action number low (duration 0 will keep it low)",            // 2
    "Neopixel wakeup light",                                              // 3
    "Neopixel wakeup light, chain next alarm",                            // 4
    "Neopixels on, action number is the palette color",                   // 5
    "Neopixels off (backlight color)"                                     // 6
  };
  
  #define actionGPIOhigh     1
  #define actionGPIOlow      2
  #define actionNeoWUL       3
  #define actionNeoWULchain  4
  #define actionNeoOn        5
  #define actionNeoOff       6
  
  const char * UI_msg_actionNum           = "Action number";
  const char * UI_msg_testAlarm           = "Test the alarm";
  const char * UI_msg_alarmsReset         = "Reset all alarms";
  
  const char * UI_msg_announcements       = "Announcement list";
  const char * UI_msg_customAnnouncement  = "Custom announcement";
  const char * UI_msg_announceMode        = "Announcement mode active";
  
  const char * UI_msg_tab_general         = "Main settings";
  const char * UI_msg_tab_alarms          = "Alarms";
  const char * UI_msg_tab_announcements   = "Announcements";
  
  const char * UI_msg_click               = "OK";
  const char * UI_msg_txt_saved           = "SAVED";
  const char * UI_msg_txt_invalid         = "INVALID";
  const char * UI_msg_txt_reset           = "ALARMS RESET";

  const char * UI_msg_info_manualSet      = "manual set";
  const char * UI_msg_info_sleeping       = "sleeping";
  const char * UI_msg_info_timeZone       = "timezone";
  const char * UI_msg_info_DST            = "DST";
#endif
