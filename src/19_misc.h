/*
***************************************************************************
  ebc_alarmclock - misc functions
***************************************************************************
  last update 20210327 by ericBcreator
***************************************************************************
*/

uint16_t getVersionChecksum() {
  unsigned long checkSum = 0;
  
  #ifdef _VERSION
    String ebcVersion = (String) _VERSION;
    if (ebcVersion.length() != 6)
      DEBUGPRINTLN("Version check: length mismatch");
    else {
      byte checksumChar, counter = 0;
      
      for (int i = 0; i < 6; i++) {
        if (i != 2) {
          checksumChar = ebcVersion[i];
          if (checksumChar == 32)             // convert spaces to 0
            checksumChar = 48;

          //checkSum += (checksumChar - 48) * pow(10, (3 - counter));
          checksumChar = (57 - checksumChar);
          checkSum += checksumChar * pow(10, (4 - counter));
          counter++;
        }
      }

      checkSum %= 65535;
    }
  #endif

  return (uint16_t) checkSum;
}

// returns: 0 no user action, -1 rotary left, 1 rotary right, 11 rotary switch, 12 rotary switch long, 21 PIR motion
int handleEvents() {
  #ifdef ENABLE_PIR
    if (digitalRead(PIN_PIR) != PIR_DEFAULT) {
      digitalWrite(PIN_PIR, PIR_DEFAULT);
      return 21;
    }
  #endif
  
  #ifndef NO_ENCODER
    reEnc = readRotEnc();

    if (reEnc < 0)
      return -1;
    else if (reEnc > 1)
      return 1;

    reSwitch = readRotEncSwitch();

    if (reSwitch == 1)
      return 11;
    else if (reSwitch == 2)
      return 12;
  #endif

  #ifdef OTA_UPDATE                           // @EB-todo
    if ((millis() - OTA_startTime) > OTA_interval) {
      ArduinoOTA.handle();
      OTA_startTime = millis();
    }
  #endif
  
  if (UI_enabled) {                           // @EB-todo
    if ((millis() - startUIupdateTime) >= UI_updateDelay) {             // reread the settings after x milliseconds      
      UI_rereadTop();
      startUIupdateTime = millis();
    }
    UI_dnsServer.processNextRequest();
  }

  return 0;
}

int delayAndHandle(int delayTime) {
  static unsigned long startTimeHandleEvents = millis();  
  unsigned long startDelayTime = millis();
  int eventResult;
  
  while (millis() - startDelayTime < delayTime) {
    if (millis() - startTimeHandleEvents > handleEventsDelay) { 
      startTimeHandleEvents = millis();    
      
      eventResult = handleEvents();
      if (eventResult)
        return eventResult;        
    }
    
    #ifndef NO_WIFI
      if (UI_enabled) {
        if ((millis() - startUIupdateTime) >= UI_updateDelay) {             // reread the settings after x milliseconds      
          UI_rereadTop();
          startUIupdateTime = millis();
        }
        UI_dnsServer.processNextRequest();
      }
    #endif
    delay(1);         // @EB-todo: needed?
  }

  return 0;
}

int checkAndHandleEvents() {
  readCurrentTime();
  checkAlarms();
  return handleEvents();
}

#ifdef ENABLE_LIGHTSENSOR
  void checkLightSensor() {
    static bool firstCall = true;
    static unsigned long lsStartTime = 0;
    static int prevReadValue = 0;
    int readValue;
    int rangedValue;
    int mappedValue;

    if (ledBrightnessManualSet)                                             // don't use the light sensor when the brightness is set manually
      return;
  
    if (sleeping)
      return;
    
    if (lsStartTime != 0 && ((millis() - lsStartTime) < ls_delay)) {        // delay time before checking the light sensor
      return;
    }

    lsStartTime = millis();
    readValue = analogRead(PIN_LS);

    #ifdef LIGHTSENSOR_AVG                                                  // if defined, average with previous reading
      if (prevReadValue) {
        rangedValue = (readValue + prevReadValue) / 2;
      } else {
        rangedValue = readValue;
      }
    #else
      rangedValue = readValue;
    #endif

    prevReadValue = readValue;

    if (rangedValue < ls_minSensor)                                         // set out of range values to min or max values
      rangedValue = ls_minSensor;
    if (rangedValue > ls_maxSensor)
      rangedValue = ls_maxSensor;
    
    mappedValue = map(rangedValue, ls_minSensor, ls_maxSensor, ls_minValue, ls_maxValue);

    #ifdef LIGHTSENSOR_CURVE                                                // if defined, use slow curve
      mappedValue = int((ls_minMaxRange + .5 - (cos(3.14 / 2 / ls_minMaxRange * mappedValue) * ls_minMaxRange))) + ls_minValue;
    #endif

    #ifdef LIGHTSENSOR_OFFSET
      mappedValue += LIGHTSENSOR_OFFSET;
      mappedValue = constrain(mappedValue, ls_minValue, ls_maxValue);
    #endif

    #ifdef DEBUG_LIGHTSENSOR
      #ifdef LIGHTSENSOR_AVG
        DEBUGPRINT("Brightness read value " + (String) readValue + ", averaged, set in range " + (String) rangedValue + ", mapped to " + (String) mappedValue);
      #else
        DEBUGPRINT("Brightness read value " + (String) readValue + ", set in range " + (String) rangedValue + ", mapped to " + (String) mappedValue);
      #endif
    #endif

    if (firstCall) {
      ledBrightness = mappedValue;
      matrix.setIntensity(ledBrightness);
      #ifdef DEBUG_LIGHTSENSOR
        DEBUGPRINTLN(", set to " + (String) ledBrightness);  
      #endif
      firstCall = false;
      
    } else if (abs(mappedValue - ledBrightness) >= ls_triggerStep) {
      if (mappedValue < ledBrightness)
        ledBrightness--;      
      else
        ledBrightness++;
      matrix.setIntensity(ledBrightness);
      
      #ifdef DEBUG_LIGHTSENSOR
        DEBUGPRINTLN(", set to " + (String) ledBrightness);
      #endif

      if (UI_enabled) {
        UI_rereadLS();
      }
      
    } else {
      #ifdef DEBUG_LIGHTSENSOR
        DEBUGPRINTLN();
      #endif
    }
  }
#endif

void startSleepMode() {
  #ifdef NOPACMAN
    displayMessage(UI_msg_sleepMode);
    matrix.fillScreen(LOW);
  #else
    displayAnimPacMan();
  #endif
  
  sleeping = true;
  DEBUGPRINTLN("Sleep mode started at " + timeString);
  sleepStartedTime = millis();
  displayPixelModes();
  matrix.write();
}
  
void endSleepMode(bool resetInfoTime) {
  if (sleeping) {
    readCurrentTime();
    DEBUGPRINTLN("Sleep mode ended at " + timeString);
    bool prevDST = DST;
    checkDST();
    refreshTimeDisplay = true;
    refreshTempSwapTime = 1;
    sleeping = false;

    #ifndef NO_WIFI
      #ifndef DEBUG_NO_TIMESERVER
        if (DST != prevDST)     // reread the time if the DST changed while sleeping
          getTimeFromServer();
      #endif
    #endif

    #ifndef NOPACMAN
      displayAnimPacMan(true);
    #endif    
  }
  
  sleepStartTime = millis();

  if (resetInfoTime)
    displayInfoStartTime = millis();
}

void setupMatrix() {
  int i = LED_NUM_OF_HOR_DISPLAYS * LED_NUM_OF_VERT_DISPLAYS - 1;
  
  matrix.setIntensity(ledBrightness);
  if (i >= 0) matrix.setRotation(0, LED_MATRIX_ROTATION_0); // set the rotation as defined for each matrix
  if (i >= 1) matrix.setRotation(1, LED_MATRIX_ROTATION_1);
  if (i >= 2) matrix.setRotation(2, LED_MATRIX_ROTATION_2);
  if (i >= 3) matrix.setRotation(3, LED_MATRIX_ROTATION_3);
  if (i >= 4) matrix.setRotation(4, LED_MATRIX_ROTATION_4);
  if (i >= 5) matrix.setRotation(5, LED_MATRIX_ROTATION_5);
  if (i >= 6) matrix.setRotation(6, LED_MATRIX_ROTATION_6);
  if (i >= 7) matrix.setRotation(7, LED_MATRIX_ROTATION_7);
  matrix.fillScreen(LOW);
}

