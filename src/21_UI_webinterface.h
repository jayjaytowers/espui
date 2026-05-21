/*
***************************************************************************
  ebc_alarmclock - web interface functions 
***************************************************************************
  last update 20210327 by ericBcreator
***************************************************************************
*/

uint16_t UI_ID_info0;
uint16_t UI_ID_info1;
uint16_t UI_ID_timeZone;
uint16_t UI_ID_DSTmode;
uint16_t UI_ID_ampmMode;
uint16_t UI_ID_timeDisplayMode;
uint16_t UI_ID_displayInfo;
uint16_t UI_ID_displayInfoInterval;
uint16_t UI_ID_suspendMode;
uint16_t UI_ID_sleepMode;
uint16_t UI_ID_sleepDelay;
uint16_t UI_ID_sleepStart;
uint16_t UI_ID_brightnessAuto;
uint16_t UI_ID_brightness;
uint16_t UI_ID_neoBacklight;
uint16_t UI_ID_displayDaylight;
uint16_t UI_ID_displayBME280;
uint16_t UI_ID_BME280_fahrenheid;
uint16_t UI_ID_BME280_tempTimeSwap;
uint16_t UI_ID_resetWifi;
uint16_t UI_ID_restart;
uint16_t UI_ID_alarmNumber;
uint16_t UI_ID_message;
uint16_t UI_ID_alarmActive;
uint16_t UI_ID_pad;
uint16_t UI_ID_hour;
uint16_t UI_ID_min;
uint16_t UI_ID_sec;
uint16_t UI_ID_duration;
uint16_t UI_ID_month;
uint16_t UI_ID_day;
uint16_t UI_ID_weekDay;
uint16_t UI_ID_weekDays;
uint16_t UI_ID_weekendDays;
uint16_t UI_ID_recurringType;
uint16_t UI_ID_recurringInterval;
uint16_t UI_ID_actionType;
uint16_t UI_ID_actionNum;
uint16_t UI_ID_buzzer;
uint16_t UI_ID_fx;
uint16_t UI_ID_bright;
uint16_t UI_ID_testAlarm;
uint16_t UI_ID_alarmsReset;
uint16_t UI_ID_saveToEEPROM;
uint16_t UI_ID_interruptAlarm;
uint16_t UI_ID_resync;
uint16_t UI_ID_announcements;
uint16_t UI_ID_customAnnouncement;
uint16_t UI_ID_announceMode;

uint16_t UI_tabG, UI_tabA, UI_tabAn;

String UI_tmpMsg1, UI_tmpMsg2;

void UI_filesystem() {                 // check / setup the filesystem
  #ifdef UI_USE_FILESYSTEM  
    #ifdef UI_INIT_FILESYSTEM
      fileSystemInitialized = 'N';
    #endif
    
    if (fileSystemInitialized != 'Y') {
      DEBUGPRINTLN("UI - preparing filesystem");
      #ifndef DEBUG_FAST_START
        displayMessage(msgUIfilesystemInit0);
      #endif      
      displayChar(13, fontCharHourglass, HIGH, false);
      matrix.write();
      
      ESPUI.prepareFileSystem();
      
      DEBUGPRINTLN("UI - finished preparing filesystem");
      playBuzzer(20);
      fileSystemInitialized = 'Y';
      #ifndef DEBUG_FAST_START
        displayMessage(msgUIfilesystemInit1);
      #endif
      eepromChanged();
    }
  #endif
}

void UI_setup() {
  //  WiFi.mode(WIFI_AP); // @EB-todo
  WiFi.softAPConfig(UI_apIP, UI_apIP, IPAddress(255, 255, 255, 0));
  #ifdef ESP32
    WiFi.setHostname(UI_ssid_ca);
  #else
    WiFi.hostname(UI_ssid_ca);
  #endif

  DEBUGPRINTLN("UI - Initial size: " + (String) UI_InitialSize);
  DEBUGPRINT("UI - Controls: ");
  if (UI_controls_extended)         DEBUGPRINT("'extended' ");
  if (UI_controls_minimal)          DEBUGPRINT("'minimal' ");
  if (UI_controls_extended_sliders) DEBUGPRINT("'sliders'");
  DEBUGPRINTLN();
    
  DEBUGPRINT("UI - Hostname set to ");
  DEBUGPRINTLN(UI_ssid_ca);
  
  WiFi.softAP(UI_ssid, UI_password, UI_channel);
  DEBUGPRINTLN("UI - Access point started");

  UI_dnsServer.start (UI_DNS_PORT, "*", UI_apIP);
  DEBUGPRINTLN("UI - DNS server started");

  UI_addControlsTop();
  UI_addControlsMain();
  UI_addControlsAlarms();
  UI_addControlsAnnounce();
  DEBUGPRINTLN("UI - Number of controls: " + (String) UI_numOfControls);  

  ESPUI.jsonUpdateDocumentSize = UI_updateSize;
  ESPUI.jsonInitialDocumentSize = UI_InitialSize;

  #ifdef DEBUG_UI_MESSAGES
    ESPUI.setVerbosity(Verbosity::VerboseJSON);
  #endif

  #ifdef UI_USE_FILESYSTEM
    DEBUGPRINTLN("UI - Begin (filesystem)");
    if (UI_secure)      
      ESPUI.beginSPIFFS(UI_title, UI_ssid, UI_password);
    else
      ESPUI.beginSPIFFS(UI_title);
  #else
    DEBUGPRINTLN("UI - Begin");
    if (UI_secure)
      ESPUI.begin(UI_title, UI_ssid, UI_password);
    else
      ESPUI.begin(UI_title);
  #endif

  String tmpMsg = msgUIstart0 + (String) UI_ssid + msgUIstart1 + WiFi.softAPIP().toString();
  tmpMsg += msgUIstart2 + lanIP.toString();
  DEBUGPRINTLN("UI - " + tmpMsg);

  #ifndef DEBUG_FAST_START
    displayMessage(tmpMsg);
  #endif
}

// add controls

uint16_t UI_addSlider(const char *label, int value, int minValue, int maxValue, ControlColor color, uint16_t parentControl, void (*callback)(Control *, int)) {
  uint16_t controlID;
  
  if (UI_controls_extended_sliders) {
    controlID = ESPUI.addControl(ControlType::Slider, label, (String) value, color, parentControl, callback);
    ESPUI.addControl(ControlType::Min, label, String(minValue), ControlColor::None, controlID);
    ESPUI.addControl(ControlType::Max, label, String(maxValue), ControlColor::None, controlID);
  } else {
    controlID = ESPUI.addControl(ControlType::Number, label, (String) value, color, parentControl, callback);
  }
  
  return controlID;
}

void UI_addControlsTop() {
  // tabs  
  UI_tabG = ESPUI.addControl(ControlType::Tab, UI_msg_tab_general, UI_msg_tab_general, ControlColor::Emerald, Control::noParent, &UI_callTab);
  UI_tabA = ESPUI.addControl(ControlType::Tab, UI_msg_tab_alarms, UI_msg_tab_alarms, ControlColor::Emerald, Control::noParent, &UI_callTab);

  #ifdef ANNOUNCE_MODE
    UI_tabAn = ESPUI.addControl(ControlType::Tab, UI_msg_tab_announcements, UI_msg_tab_announcements, ControlColor::Emerald, Control::noParent, &UI_callTab);
  #endif

  // controls above the tabs
  UI_ID_info0 = ESPUI.addControl(ControlType::Label, UI_msg_info0, UI_getInfo(0), ControlColor::None, Control::noParent);
  
  #ifdef DISPLAY_SYSTEM_INFO
    UI_ID_info1 = ESPUI.addControl(ControlType::Label, UI_msg_info1, UI_getInfo(1), ControlColor::None, Control::noParent);
  #endif
  
  UI_ID_saveToEEPROM = ESPUI.addControl(ControlType::Button, UI_msg_saveToEEPROM, UI_msg_click, ControlColor::Emerald, Control::noParent, &UI_callButton);
  UI_ID_resync = ESPUI.addControl(ControlType::Button, UI_msg_resync, UI_msg_click, ControlColor::Emerald, Control::noParent,&UI_callButton);
  UI_ID_interruptAlarm = ESPUI.addControl(ControlType::Button, UI_msg_interruptAlarm, UI_msg_click, ControlColor::Alizarin, Control::noParent, &UI_callButton);
  UI_ID_restart = ESPUI.addControl(ControlType::Button, UI_msg_restart, UI_msg_click, ControlColor::Alizarin, Control::noParent, &UI_callButton);  
}

void UI_addControlsMain() {  
  UI_ID_timeZone = UI_addSlider(UI_msg_timeZone, timeZone, -11, 14, ControlColor::Wetasphalt, UI_tabG, &UI_callNumber);
  UI_ID_DSTmode = ESPUI.addControl(ControlType::Select, UI_msg_DSTmode, (String) DSTmode, ControlColor::Wetasphalt, UI_tabG, &UI_callOption);
  for (int i = 0; i < (FN_ARRAY_SIZE(UI_msg_DSTmodes)); i++) {      
    charPtr = UI_msg_DSTmodes[i].c_str();
    ESPUI.addControl(ControlType::Option, charPtr, (String) i, ControlColor::Wetasphalt, UI_ID_DSTmode);
  }    

  UI_ID_timeDisplayMode = ESPUI.addControl(ControlType::Select, UI_msg_timeDisplayMode, (String) timeDisplayMode, ControlColor::Wetasphalt, UI_tabG, &UI_callOption);
  for (int i = 0; i < (FN_ARRAY_SIZE(UI_msg_timeDisplayModes)); i++) {      
    charPtr = UI_msg_timeDisplayModes[i].c_str();
    ESPUI.addControl(ControlType::Option, charPtr, (String) i, ControlColor::Wetasphalt, UI_ID_timeDisplayMode);
  }    

  UI_ID_ampmMode = ESPUI.addControl(ControlType::Switcher, UI_msg_ampmMode, (String) ampmMode, ControlColor::Wetasphalt, UI_tabG, &UI_callSwitch);
  
  if (UI_controls_minimal) {
    UI_tmpMsg2 = (String) UI_msg_sleepMode + " (" + (String) sleepDelay + " seconds)";
    charPtr = UI_tmpMsg2.c_str();
    UI_ID_sleepMode = ESPUI.addControl(ControlType::Switcher, charPtr, (String) sleepMode, ControlColor::Wetasphalt, UI_tabG, &UI_callSwitch);
    
    UI_tmpMsg1 = (String) UI_msg_displayInfo + " (" + (String) displayInfoInterval + " seconds)";
    charPtr = UI_tmpMsg1.c_str();
    UI_ID_displayInfo = ESPUI.addControl(ControlType::Switcher, charPtr, (String) displayInfo, ControlColor::Wetasphalt, UI_tabG, &UI_callSwitch);    
  } else {
    UI_ID_sleepMode = ESPUI.addControl(ControlType::Switcher, UI_msg_sleepMode, (String) sleepMode, ControlColor::Wetasphalt, UI_tabG, &UI_callSwitch);
    UI_ID_sleepDelay = ESPUI.addControl(ControlType::Number, UI_msg_sleepDelay, (String) sleepDelay, ControlColor::Wetasphalt, UI_tabG, &UI_callNumber);
    UI_ID_sleepStart = ESPUI.addControl(ControlType::Button, UI_msg_sleepStart, UI_msg_click, ControlColor::Emerald, UI_tabG, &UI_callButton);  
    UI_ID_suspendMode = ESPUI.addControl(ControlType::Switcher, UI_msg_suspendMode, (String) suspendMode, ControlColor::Wetasphalt, UI_tabG, &UI_callSwitch);
    UI_ID_displayInfo = ESPUI.addControl(ControlType::Switcher, UI_msg_displayInfo, (String) displayInfo, ControlColor::Wetasphalt, UI_tabG, &UI_callSwitch);
    UI_ID_displayInfoInterval = ESPUI.addControl(ControlType::Number, UI_msg_displayInfoInterval, (String) displayInfoInterval, ControlColor::Wetasphalt, UI_tabG, &UI_callNumber);
  }

  #ifdef DAYLIGHTINFO
    UI_ID_displayDaylight = ESPUI.addControl(ControlType::Switcher, UI_msg_displayDaylight, (String) displayDaylight, ControlColor::Wetasphalt, UI_tabG, &UI_callSwitch);
  #endif

  #ifdef ENABLE_BME280
    UI_ID_displayBME280 = ESPUI.addControl(ControlType::Switcher, UI_msg_displayBME280, (String) BME280_display, ControlColor::Wetasphalt, UI_tabG, &UI_callSwitch);
    UI_ID_BME280_fahrenheid = ESPUI.addControl(ControlType::Switcher, UI_msg_BME280_fahrenheid, (String) BME280_fahrenheid, ControlColor::Wetasphalt, UI_tabG, &UI_callSwitch);
    
    if (LED_NUM_OF_HOR_DISPLAYS < 8) {      // don't display when using 8 (or more) matrices, the temp will appear on the right 4 matrices
      UI_ID_BME280_tempTimeSwap = ESPUI.addControl(ControlType::Switcher, UI_msg_BME280_tempTimeSwap, (String) BME280_tempTimeSwap, ControlColor::Wetasphalt, UI_tabG, &UI_callSwitch);
    }
  #endif

  #ifdef ENABLE_LIGHTSENSOR
    UI_ID_brightnessAuto = ESPUI.addControl(ControlType::Switcher, UI_msg_brightnessAuto, (String) !ledBrightnessManualSet, ControlColor::Wetasphalt, UI_tabG, &UI_callSwitch);
  #endif 
  
  UI_ID_brightness = UI_addSlider(UI_msg_brightness, ledBrightness, ls_minValue, ls_maxValue, ControlColor::Wetasphalt, UI_tabG, &UI_callNumber);

  #ifdef ENABLE_NEOPIXELS
    UI_ID_neoBacklight = UI_addSlider(UI_msg_neoBacklight, neoBacklight, 0, numOfNeoPalette, ControlColor::Wetasphalt, UI_tabG, &UI_callNumber);
  #endif

  if (UI_controls_extended) {
    UI_ID_resetWifi = ESPUI.addControl(ControlType::Button, UI_msg_resetWifi, UI_msg_click, ControlColor::Alizarin, UI_tabG, &UI_callButton);  
  }
}

void UI_addControlsAlarms() {
  UI_ID_pad = ESPUI.addControl(ControlType::Pad, UI_msg_pad, "", ControlColor::Wetasphalt, UI_tabA, &UI_callPad);
  UI_ID_testAlarm = ESPUI.addControl(ControlType::Button, UI_msg_testAlarm, UI_msg_click, ControlColor::Alizarin, UI_tabA, &UI_callButton);
  
  UI_ID_alarmNumber = ESPUI.addControl(ControlType::Label, UI_msg_alarmNumber, (String) UI_alarmNum, ControlColor::Wetasphalt, UI_tabA);
  UI_ID_alarmActive = ESPUI.addControl(ControlType::Switcher, UI_msg_alarmActive, (String) alarmList[UI_alarmNum].active, ControlColor::Wetasphalt, UI_tabA, &UI_callSwitch);
  UI_ID_message = ESPUI.addControl(ControlType::Text, UI_msg_message, (String) alarmList[UI_alarmNum].message, ControlColor::Wetasphalt, UI_tabA, &UI_callText);
  
  UI_ID_hour = UI_addSlider(UI_msg_hour, alarmList[UI_alarmNum].hour, 0, 23, ControlColor::Peterriver, UI_tabA, &UI_callNumber);
  UI_ID_min = UI_addSlider(UI_msg_min, alarmList[UI_alarmNum].min, 0, 59, ControlColor::Peterriver, UI_tabA, &UI_callNumber);
  UI_ID_sec = UI_addSlider(UI_msg_sec, alarmList[UI_alarmNum].sec, 0, 59, ControlColor::Peterriver, UI_tabA, &UI_callNumber);
  
  if (!UI_controls_minimal) {
    UI_ID_month = UI_addSlider(UI_msg_month, alarmList[UI_alarmNum].month, 0, 12, ControlColor::Peterriver, UI_tabA, &UI_callNumber);
    UI_ID_day = UI_addSlider(UI_msg_day, alarmList[UI_alarmNum].day, 0, 31, ControlColor::Peterriver, UI_tabA, &UI_callNumber);
  }
  
  UI_ID_duration = ESPUI.addControl(ControlType::Number, UI_msg_duration, (String) alarmList[UI_alarmNum].duration, ControlColor::Peterriver, UI_tabA, &UI_callNumber);

  UI_ID_weekDays = ESPUI.addControl(ControlType::Switcher, UI_msg_weekDays, (String) alarmList[UI_alarmNum].weekDays, ControlColor::Turquoise, UI_tabA, &UI_callSwitch);
  UI_ID_weekendDays = ESPUI.addControl(ControlType::Switcher, UI_msg_weekendDays, (String) alarmList[UI_alarmNum].weekendDays, ControlColor::Turquoise, UI_tabA, &UI_callSwitch);
  UI_ID_weekDay = UI_addSlider(UI_msg_weekDay, alarmList[UI_alarmNum].weekDay, 0, 7, ControlColor::Turquoise, UI_tabA, &UI_callNumber);

  if (!UI_controls_minimal) {
    UI_ID_recurringType = ESPUI.addControl(ControlType::Select, UI_msg_recurringType, (String) alarmList[UI_alarmNum].recurringType, ControlColor::Turquoise, UI_tabA, &UI_callNumber);
    for (int i = 0; i < (FN_ARRAY_SIZE(UI_msg_recurringTypes)); i++) {      
      charPtr = UI_msg_recurringTypes[i].c_str();
      ESPUI.addControl(ControlType::Option, charPtr, (String) i, ControlColor::Turquoise, UI_ID_recurringType);    
    }
    UI_ID_recurringInterval = ESPUI.addControl(ControlType::Number, UI_msg_recurringInterval, (String) alarmList[UI_alarmNum].recurringInterval, ControlColor::Turquoise, UI_tabA, &UI_callNumber);
  }
  
  UI_ID_buzzer  = UI_addSlider(UI_msg_buzzer, alarmList[UI_alarmNum].buzzer, 0, 19, ControlColor::Carrot, UI_tabA, &UI_callNumber);  
  UI_ID_bright = ESPUI.addControl(ControlType::Switcher, UI_msg_bright, (String) alarmList[UI_alarmNum].bright, ControlColor::Carrot, UI_tabA, &UI_callSwitch);
  
  UI_ID_fx = ESPUI.addControl(ControlType::Select, UI_msg_fx, (String) alarmList[UI_alarmNum].fx, ControlColor::Carrot, UI_tabA, &UI_callNumber);
  ESPUI.addControl(ControlType::Option, UI_msg_fx0, "0", ControlColor::Carrot, UI_ID_fx);
  ESPUI.addControl(ControlType::Option, UI_msg_fx1, "1", ControlColor::Carrot, UI_ID_fx);
  
  UI_ID_actionType = ESPUI.addControl(ControlType::Select, UI_msg_actionType, (String) alarmList[UI_alarmNum].actionType, ControlColor::Carrot, UI_tabA, &UI_callNumber);
  
  #ifdef ENABLE_NEOPIXELS
    int j = (FN_ARRAY_SIZE(UI_msg_actionTypes));
  #else
    int j = (FN_ARRAY_SIZE(UI_msg_actionTypes)) - 4;
  #endif
  
  for (int i = 0; i < j; i++) {      
    charPtr = UI_msg_actionTypes[i].c_str();
    ESPUI.addControl(ControlType::Option, charPtr, (String) i, ControlColor::Carrot, UI_ID_actionType);    
  }    
  UI_ID_actionNum = UI_addSlider(UI_msg_actionNum, alarmList[UI_alarmNum].actionNum, 0, 40, ControlColor::Carrot, UI_tabA, &UI_callNumber);  
  
  if (UI_controls_minimal)
    UI_numOfControls = UI_ID_actionNum;
  else {
    UI_ID_alarmsReset = ESPUI.addControl(ControlType::Button, UI_msg_alarmsReset, UI_msg_click, ControlColor::Alizarin, UI_tabA, &UI_callButton);
    UI_numOfControls = UI_ID_alarmsReset;
  }
}

void UI_addControlsAnnounce() {
  #ifdef ANNOUNCE_MODE    
    UI_ID_announcements = ESPUI.addControl(ControlType::Select, UI_msg_announcements, "", ControlColor::Sunflower, UI_tabAn, &UI_callOption);

    for (int i = 0; i < (FN_ARRAY_SIZE(announcements) - 1); i++) {      
      charPtr = announcements[i].c_str();
      ESPUI.addControl(ControlType::Option, charPtr, (String) i, ControlColor::Wetasphalt, UI_ID_announcements);
    }    
  
    UI_ID_customAnnouncement = ESPUI.addControl(ControlType::Text, UI_msg_customAnnouncement, (String) customAnnouncement, ControlColor::Sunflower, UI_tabAn, &UI_callText);
    UI_ID_announceMode = ESPUI.addControl(ControlType::Switcher, UI_msg_announceMode, (String) announceMode, ControlColor::Sunflower, UI_tabAn, &UI_callSwitch);

    UI_numOfControls = UI_ID_announceMode;    
  #endif
}

// reread functions

void UI_rereadTop() {
  ESPUI.updateText(UI_ID_info0, UI_getInfo(0));
  ESPUI.updateText(UI_ID_info1, UI_getInfo(1));
}

void UI_rereadMain() {
  ESPUI.updateSlider(UI_ID_timeZone, timeZone);
  ESPUI.updateSelect(UI_ID_DSTmode, (String) DSTmode);
  ESPUI.updateSwitcher(UI_ID_ampmMode, ampmMode);
  ESPUI.updateSelect(UI_ID_timeDisplayMode, (String) timeDisplayMode);
  ESPUI.updateSwitcher(UI_ID_displayInfo, displayInfo);
  ESPUI.updateNumber(UI_ID_displayInfoInterval, displayInfoInterval);  
  ESPUI.updateSwitcher(UI_ID_suspendMode, suspendMode);
  ESPUI.updateSwitcher(UI_ID_sleepMode, sleepMode);
  ESPUI.updateNumber(UI_ID_sleepDelay, sleepDelay);  
  UI_rereadLS();
  #ifdef DAYLIGHTINFO
    ESPUI.updateSwitcher(UI_ID_displayDaylight, displayDaylight);
  #endif
  #ifdef ENABLE_BME280
    ESPUI.updateSwitcher(UI_ID_displayBME280, BME280_display);
    ESPUI.updateSwitcher(UI_ID_BME280_fahrenheid, BME280_fahrenheid);
    ESPUI.updateSwitcher(UI_ID_BME280_tempTimeSwap, BME280_tempTimeSwap);
  #endif  
  #ifdef ENABLE_NEOPIXELS
    ESPUI.updateSlider(UI_ID_neoBacklight, neoBacklight);
  #endif
}

void UI_rereadLS() {
  #ifdef ENABLE_LIGHTSENSOR
    ESPUI.updateSwitcher(UI_ID_brightnessAuto, !ledBrightnessManualSet);
  #endif
  ESPUI.updateSlider(UI_ID_brightness, ledBrightness);
}

void UI_rereadAlarms() {
  String tmpMsg = "";
  if (alarmTriggered && UI_alarmNum == alarmNum)
    tmpMsg = " TRIGGERED";
  else if (forceFirstAlarm)
    tmpMsg = " TESTING";

  if (alarmRepeat)
    tmpMsg += " RECURRING";
    
  ESPUI.print(UI_ID_alarmNumber, (String) (UI_alarmNum + 1) + tmpMsg);
  ESPUI.updateText(UI_ID_message, (String) alarmList[UI_alarmNum].message);
  ESPUI.updateSwitcher(UI_ID_alarmActive, alarmList[UI_alarmNum].active);
  ESPUI.updateSlider(UI_ID_hour, alarmList[UI_alarmNum].hour);
  ESPUI.updateSlider(UI_ID_min, alarmList[UI_alarmNum].min);
  ESPUI.updateSlider(UI_ID_sec, alarmList[UI_alarmNum].sec);
  ESPUI.updateSlider(UI_ID_month, alarmList[UI_alarmNum].month);
  ESPUI.updateSlider(UI_ID_day, alarmList[UI_alarmNum].day);
  ESPUI.updateSlider(UI_ID_weekDay, alarmList[UI_alarmNum].weekDay);
  ESPUI.updateNumber(UI_ID_duration, alarmList[UI_alarmNum].duration);
  ESPUI.updateSwitcher(UI_ID_weekDays, alarmList[UI_alarmNum].weekDays);
  ESPUI.updateSwitcher(UI_ID_weekendDays, alarmList[UI_alarmNum].weekendDays);
  ESPUI.updateSlider(UI_ID_recurringType, alarmList[UI_alarmNum].recurringType);
  ESPUI.updateNumber(UI_ID_recurringInterval, alarmList[UI_alarmNum].recurringInterval);
  ESPUI.updateSlider(UI_ID_buzzer, alarmList[UI_alarmNum].buzzer);
  ESPUI.updateSelect(UI_ID_fx, (String) alarmList[UI_alarmNum].fx);
  ESPUI.updateSwitcher(UI_ID_bright, alarmList[UI_alarmNum].bright);  
  ESPUI.updateSelect(UI_ID_actionType, (String) alarmList[UI_alarmNum].actionType);
  ESPUI.updateSlider(UI_ID_actionNum, alarmList[UI_alarmNum].actionNum);
}

void UI_rereadAnnounce() {
  #ifdef ANNOUNCE_MODE
    ESPUI.updateSelect(UI_ID_announcements, (String) announceItem);
    ESPUI.updateText(UI_ID_customAnnouncement, (String) customAnnouncement);
    ESPUI.updateSwitcher(UI_ID_announceMode, announceMode);
  #endif
}

void UI_rereadTab(int tabCode) { 
  UI_rereadTop();

  if (UI_activeTab == -1 || tabCode == -1) {     // the active tab is undefined so reread all controls or reread all is requested
    UI_rereadMain();
    UI_rereadAlarms();
    UI_rereadAnnounce();
  } else if (UI_activeTab == UI_tabG)
    UI_rereadMain();
  else if (UI_activeTab == UI_tabA)    
    UI_rereadAlarms();
  else if (UI_activeTab == UI_tabAn)
    UI_rereadAnnounce();
}

// callback functions

String UI_getInfo(int type) {
  String tmp;

  if (type == 0) {
    tmp = days[timeData.tm_wday] + " " + fillZero(timeData.tm_mday) + "-" + fillZero(timeData.tm_mon + 1) + "-" + (String) (timeData.tm_year + 1900);
    tmp += " " + fillZero(getPMhour()) + ":" + fillZero(timeData.tm_min)+ ":" + fillZero(timeData.tm_sec);
  
    if (ampmMode) {
      if (pm) tmp += " PM";
      else    tmp += " AM";
    }

    if (sleeping) 
      tmp += " - " + (String) UI_msg_info_sleeping;

    if (timeManualSet) {
      tmp += "<br>" + (String) UI_msg_info_manualSet;
    } else {
      tmp += "<br>" + (String) UI_msg_info_timeZone + " " + (String) timeZone + " ";
      tmp += (DST ? msgDSTon : msgDSToff);
      if (DSTmode == 2) 
        tmp += " (auto)";    
    }
    
    #ifdef DAYLIGHTINFO
      tmp += "<br>" + (String) d2d_City + " : " + daylightInfo;
    #endif  
  
    #ifdef ENABLE_BME280
      char charBuffer[10];  
      sprintf(charBuffer, "%3.1f", BME280_temperature);
      tmp += "<br>" + (String) msgbmeTemperature + ' ' + (String) charBuffer + (BME280_fahrenheid ? "F " : "C ");
      tmp += (String) msgbmeHumidity + ' ' +  (String) int((BME280_humidity + .5)) + "% ";
      tmp += (String) msgbmePressure + ' ' +  (String) int((BME280_pressure + .5)) + " hPa ";
      
      #ifdef BME280_SEA_LEVEL_PRESSURE
        sprintf(charBuffer, "%2.1f", BME280_altitude);
        tmp += (String) charBuffer + " mtr ";
      #endif
    #endif

    #ifndef DISPLAY_SYSTEM_INFO
      tmp += "<br>version " + (String) _VERSION + " - " + (String) _DATE + " by " + (String) _AUTHOR;
    #endif

    if (alarmTriggered)
      tmp += "<br>* Alarm " + (String) (alarmNum + 1) + " triggered *";
    else if (forceFirstAlarm)
      tmp += "<br>* Alarm " + (String) (alarmNum + 1) + " testing *";
  
    if (alarmRepeat)
      tmp += " recurring *";

  } else if (type == 1) {
    tmp+= "ericBclock - " + (String) _LANGUAGE + " - version " + (String) _VERSION + " - " + (String) _DATE + " by " + (String) _AUTHOR + "<br>" + setFeatures;
    tmp+= "-UI controls: " + (String) UI_numOfControls;
  
    if (UI_controls_extended || UI_controls_minimal || UI_controls_extended_sliders) {
      tmp+= " (";
      if (UI_controls_extended) tmp+= "ext";
      if (UI_controls_minimal)  tmp+= "min";
      if (UI_controls_extended_sliders) {
        if (UI_controls_extended || UI_controls_minimal)
          tmp+= "-sli";
        else
          tmp+= "sli";
      }
      tmp+= ")";  
    }
    
    tmp+= "<br>Wifi SSID: " + (String) UI_ssid + " IP: " + WiFi.softAPIP().toString();
    tmp+= "<br>lan: " + lanIP.toString();
  }
  
  return tmp;
}

void UI_callTab(Control *sender, int type) {
  DEBUGPRINTLN("UI - Selected tab " + (String) sender->id);
  UI_activeTab = sender->id;  
  UI_rereadTab();
}

void UI_callButton(Control *sender, int type) { 
  if (type == B_UP) {
    if (sender->id == UI_ID_sleepStart && sleeping) {   // when clicking the start sleep mode button and already sleeping, end sleepmode and return to prevent retriggering
      playBuzzer(20); 
      endSleepMode(true);
      return;
    }
    
    endSleepMode(true);
    
    if (sender->id == UI_ID_saveToEEPROM) {
      DEBUGPRINTLN("UI - Save button clicked");
      eepromWrite();
      endSleepMode(false);
	    playBuzzer(21);
//      ESPUI.updateText(UI_ID_info, UI_msg_txt_saved);
      
    } else if (sender->id == UI_ID_resetWifi) {
      DEBUGPRINTLN("UI - Reset wifi button clicked");
      UI_sw_resetEEPROM();
      playBuzzer(20);
//      ESPUI.updateText(UI_ID_info, msgUIwifiReset);      
//      displayMessageOutLoop(0, msgUIwifiReset, false);  // @EB-todo
      ESP.restart();
      
    } else if (sender->id == UI_ID_interruptAlarm) {
      DEBUGPRINTLN("UI - Alarm interrupt button clicked");
      forceFirstAlarm = false;
      UI_alarmToInterrupt = true;
      playBuzzer(20);
      
    } else if (sender->id == UI_ID_testAlarm) {
      DEBUGPRINTLN("UI - Test alarm button clicked " + (String) UI_alarmNum);
      if (alarmTriggered)
        UI_alarmToInterrupt = true;  
      else {
        if (forceFirstAlarm)
          forceFirstAlarm = false;
        else {
          forceFirstAlarm = true;
          alarmNum = UI_alarmNum;
        }
      }
      playBuzzer(20);
      
    } else if (sender->id == UI_ID_alarmsReset) {
      DEBUGPRINTLN("UI - Alarms reset button clicked");
      eepromInitialize(1);
      eepromRead();
//      UI_rereadTab();
//      ESPUI.updateText(UI_ID_info, UI_msg_txt_reset);
      playBuzzer(20);
            
    } else if (sender->id == UI_ID_resync) { 
      DEBUGPRINTLN("UI - Resync time button clicked");
      playBuzzer(20);
      UI_displayTimeRead  = true;
      UI_timeToResync = true;
      
    } else if (sender->id == UI_ID_restart) {
      DEBUGPRINTLN("UI - Restart button clicked");
      playBuzzer(20);
      ESP.restart();
      
    } else if (sender->id == UI_ID_sleepStart) {
      DEBUGPRINTLN("UI - Start sleep mode button clicked");
      playBuzzer(20); 
      
      if (sleeping)
        forceEndSleepMode = true;
      else
        forceStartSleepMode = true;
    }

  }
}

void UI_callText(Control *sender, int type) {
  eepromChanged();
  endSleepMode(true);
  
  if (sender->id == UI_ID_message) {    
    sender->value.toCharArray(alarmList[UI_alarmNum].message, (alarmMsgLength - 2)); 
    ESPUI.updateText(UI_ID_message, (String) alarmList[UI_alarmNum].message);  
  } else if (sender->id == UI_ID_customAnnouncement) { 
    sender->value.toCharArray(customAnnouncement, (customAnnouncementLength - 2)); 
  }
}

void UI_callNumber(Control *sender, int type) {
  #ifdef UI_CONTROLS_EXTENDED_SLIDERS
    // @EB-todo 20201113: ESPUI slider bug, the callback function is called 11 times !
    // so check previous call id and value to prevent retriggering
    static int prevID, prevValue;
    if (prevID == sender->id && prevValue == sender->value.toInt())
      return;
    prevID = sender->id;
    prevValue = sender->value.toInt();
  #endif
  
  eepromChanged();
  endSleepMode(true);
  
  if      (sender->id == UI_ID_timeZone)             { timeZone = UI_validateNum(-11, 14, sender->value); timeManualSet = false; UI_timeToResync = true; }
  else if (sender->id == UI_ID_displayInfoInterval)  { displayInfoInterval = UI_validateNum(30, 32766, sender->value); displayInfoStartTime = millis(); }
  else if (sender->id == UI_ID_sleepDelay)           { sleepDelay = UI_validateNum(0, 32766, sender->value);                                }  
  else if (sender->id == UI_ID_hour)                 { alarmList[UI_alarmNum].hour = UI_validateNum(0, 23, sender->value);                  }
  else if (sender->id == UI_ID_min)                  { alarmList[UI_alarmNum].min = UI_validateNum(0, 59, sender->value);                   }
  else if (sender->id == UI_ID_sec)                  { alarmList[UI_alarmNum].sec = UI_validateNum(0, 59, sender->value);                   }
  else if (sender->id == UI_ID_month)                { alarmList[UI_alarmNum].month = UI_validateNum(0, 12, sender->value);                 }
  else if (sender->id == UI_ID_duration)             { alarmList[UI_alarmNum].duration = UI_validateNum(0, 32766, sender->value);           }
  else if (sender->id == UI_ID_weekDay)              { alarmList[UI_alarmNum].weekDay = UI_validateNum(0, 7, sender->value);                alarmRecTriggered[UI_alarmNum] = false; }
  else if (sender->id == UI_ID_recurringType)        { alarmList[UI_alarmNum].recurringType = UI_validateNum(0, 5, sender->value);          alarmRecTriggered[UI_alarmNum] = false; }
  else if (sender->id == UI_ID_recurringInterval)    { alarmList[UI_alarmNum].recurringInterval = UI_validateNum(0, 32766, sender->value);  alarmRecTriggered[UI_alarmNum] = false; }
  else if (sender->id == UI_ID_buzzer)               { alarmList[UI_alarmNum].buzzer = UI_validateNum(0, 19, sender->value);                }
  else if (sender->id == UI_ID_fx)                   { alarmList[UI_alarmNum].fx = UI_validateNum(0, 1, sender->value);                     }
  else if (sender->id == UI_ID_actionType)           { alarmList[UI_alarmNum].actionType = UI_validateNum(0, FN_ARRAY_SIZE(UI_msg_actionTypes), sender->value); initAction(UI_alarmNum); }

  else if (sender->id == UI_ID_actionNum || sender->id == UI_ID_actionType) {
    bool valid = true;
    if (alarmList[UI_alarmNum].actionType == actionGPIOhigh || alarmList[UI_alarmNum].actionType == actionGPIOlow) {
      switch (sender->value.toInt()) {
        case PIN_CS: case PIN_BUZZER: case RE_PINA: case RE_PINB: case RE_SWITCH: case PIN_LS:
          #ifdef ENABLE_PIR
            case PIN_PIR: 
          #endif
          #ifdef ENABLE_NEOPIXELS
            case PIN_NEOPIXELS:
          #endif
        
          valid = false;
      }      
    } else if (alarmList[UI_alarmNum].actionType == actionNeoOn) {
      #ifdef ENABLE_NEOPIXELS
        alarmList[UI_alarmNum].actionNum = UI_validateNum(0, numOfNeoPalette, sender->value);        
      #endif        
    }
    
    if (valid) {      
      alarmList[UI_alarmNum].actionNum = UI_validateNum(0, 99, sender->value);      
      initAction(UI_alarmNum);        
    } else {
      playBuzzer(20);
      ESPUI.updateSlider(UI_ID_actionNum, alarmList[UI_alarmNum].actionNum);
    }    
  }
  
  else if (sender->id == UI_ID_brightness) {   
    ledBrightness= UI_validateNum(ls_minValue, ls_maxValue, sender->value);
    ledBrightnessManualSet = true;
    matrix.setIntensity(ledBrightness);

    #ifdef ENABLE_LIGHTSENSOR
      ESPUI.updateSwitcher(UI_ID_brightnessAuto, !ledBrightnessManualSet);
    #endif
  }

  else if (sender->id == UI_ID_day) {
    if (alarmList[UI_alarmNum].month)
      alarmList[UI_alarmNum].day = UI_validateNum(0, getDaysInMonth(timeData.tm_year, alarmList[UI_alarmNum].month), sender->value);
    else
      alarmList[UI_alarmNum].day = UI_validateNum(0, 31, sender->value);
  }

  #ifdef ENABLE_NEOPIXELS
    else if (sender->id == UI_ID_neoBacklight) { 
      neoBacklight = UI_validateNum(0, numOfNeoPalette, sender->value);
      neopixelsPresetOn(neoBacklight);
    }
  #endif

//  UI_rereadTab();
}

int UI_validateNum(int minVal, int maxVal, String value) {
  int tmpInt = value.toInt();
//  tmpInt = max(minVal, tmpInt);
//  tmpInt = min(maxVal, tmpInt);

  if (tmpInt < minVal) {
    tmpInt = minVal;
    playBuzzer(21);
  }
  if (tmpInt > maxVal) {
    tmpInt = maxVal;
    playBuzzer(21);
  }
  return tmpInt;
}

void UI_callOption(Control *sender, int value) {
  static int prevAnnounceItem = 0;
  eepromChanged();    
  endSleepMode(true);
  
  if (sender->id == UI_ID_DSTmode) { 
    DSTmode = sender->value.toInt(); 
    timeManualSet = false; 
    UI_timeToResync = true;   
  } else if (sender->id == UI_ID_timeDisplayMode) { 
    timeDisplayMode = sender->value.toInt(); 
    refreshTimeDisplay = true;
    refreshTempSwapTime = 2;
  } 

  #ifdef ENABLE_NEOPIXELS
    if (sender->id == UI_ID_announcements) { 
      announceItem = sender->value.toInt();
      if (announceItem != prevAnnounceItem) {
        customAnnouncement[0] = 0; 
        ESPUI.updateText(UI_ID_customAnnouncement, (String) customAnnouncement);
      }
    }
  #endif
}

void UI_callSwitch(Control *sender, int value) {
  eepromChanged();
  endSleepMode(true);
  
  bool result = (value == S_ACTIVE ? true : false);
  
  if      (sender->id == UI_ID_ampmMode)            { ampmMode = result; getPMhour(); refreshTimeDisplay = true; }
  else if (sender->id == UI_ID_displayInfo)         { displayInfo = result;                                }
  else if (sender->id == UI_ID_suspendMode)         { suspendMode = result;                                }
  else if (sender->id == UI_ID_sleepMode)           { sleepMode = result; endSleepMode(true);              }
  else if (sender->id == UI_ID_brightnessAuto)      { ledBrightnessManualSet = !result;                    }
  else if (sender->id == UI_ID_weekDays)            { alarmList[UI_alarmNum].weekDays = result;            alarmRecTriggered[UI_alarmNum] = false;  }
  else if (sender->id == UI_ID_weekendDays)         { alarmList[UI_alarmNum].weekendDays = result;         alarmRecTriggered[UI_alarmNum] = false;  }
  else if (sender->id == UI_ID_bright)              { alarmList[UI_alarmNum].bright = result;              }
  else if (sender->id == UI_ID_displayDaylight)     { displayDaylight = result;                            }
  else if (sender->id == UI_ID_displayBME280)       { BME280_display = result; BME280_startTime = 0;       }
  else if (sender->id == UI_ID_BME280_fahrenheid)   { BME280_fahrenheid = result; BME280_startTime = 0;    }
  else if (sender->id == UI_ID_BME280_tempTimeSwap) { BME280_tempTimeSwap = result;                        } 
  
  else if (sender->id == UI_ID_announceMode) {
    announceMode = result;
    if (strlen(customAnnouncement)) {
      customAnnounceMsg = true;
    } else 
      customAnnounceMsg = false;
  }
    
  else if (sender->id == UI_ID_alarmActive) {
    alarmList[UI_alarmNum].active = result;
    if (result) {

      if (UI_controls_minimal) {                      // if the alarm is set active and not all attributes are displaying, set them to 0 to avoid unexpected problems
        alarmList[UI_alarmNum].month = 0;
        alarmList[UI_alarmNum].day = 0;
        alarmList[UI_alarmNum].recurringType = 0;
        alarmList[UI_alarmNum].recurringInterval = 0;
      }
      
      alarmRecTriggered[UI_alarmNum] = false;
      initAction(UI_alarmNum);
    } else {
      if (alarmTriggered && alarmNum == UI_alarmNum)
        UI_alarmToInterrupt = true;
    }
  }
  
  else if (sender->id == UI_ID_sleepMode)
    DEBUGPRINTLN(sleepMode ? "UI - Sleep mode activated" : "UI - Sleep mode deactivated");
    
  else if (sender->id == UI_ID_suspendMode)
    DEBUGPRINTLN(suspendMode ? "UI - Suspend mode activated" : "UI - Suspend mode deactivated");
}

void UI_callPad(Control *sender, int value) {
  endSleepMode(true);
  
  switch (value) {
    case P_LEFT_UP:
      if (UI_alarmNum == 0)
        UI_alarmNum = maxAlarms - 1;
      else
        UI_alarmNum--;
      break;
      
    case P_RIGHT_UP:
      UI_alarmNum++;
      UI_alarmNum = UI_alarmNum % maxAlarms;
      break;
      
    case P_FOR_UP:
      UI_alarmNum = 0;
      break;
      
    case P_BACK_UP:
      UI_alarmNum = maxAlarms - 1;
      break;
  }

  UI_rereadAlarms();
}
