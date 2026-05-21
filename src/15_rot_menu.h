/*
***************************************************************************
  ebc_alarmclock - rotary encoder menu
***************************************************************************
  last update 20210319 by ericBcreator
***************************************************************************
*/

//
// rotary encoder menu - functions
//

void encoderSetAlarm0() {
  String msgItem = fillZero(alarmList[0].hour) + fillZero(alarmList[0].min);
  
  displayMessage(msgMenuAlarm0);
  encoderDisplayItem(msgItem);

  alarmList[0].hour = encoderSetValue(alarmList[0].hour, 0, 23, 0, 0, NULL, 200);
  if (returnCode == 9) return;
  alarmList[0].min = encoderSetValue(alarmList[0].min, 0, 59, 2, 0, NULL, 500);
  if (returnCode == 9) return;
  
  alarmList[0].sec = 0;      

  if (alarmList[0].duration == 0) {                   // set to the default values if no duration is set
    alarmList[0].duration = defaultAlarmDuration;
    alarmList[0].buzzer = 4;
    alarmList[0].bright = false;
    alarmList[0].fx = 1;
  }

  #ifdef ENABLE_NEOPIXELS
    msgItem = "Wu" + (String) (sleepMode ? msgOn : msgOff);
    displayMessage(msgMenuAlarm0wul);
    encoderDisplayItemOnOff(msgItem);
    
    bool wul = (alarmList[0].actionType == actionNeoWUL || alarmList[0].actionType == actionNeoWULchain);
    wul = encoderSetValue(wul, 0, 1, 2, 10, NULL, 0);
    if (returnCode == 9) return;
    displayMessage((String) msgMenuAlarm0wul + " " + (wul ? msgOn : msgOff));

    if (wul)
      alarmList[0].actionType = actionNeoWUL;         // @EB-todo: setting wul chain not implemented in the menu yet
    else
      alarmList[0].actionType = 0;
  #endif

  displayAlarmTime(msgAlarm0Set);
}

void encoderSetLedBrightness() {                      // note: setting the brightness manually will disable the light sensor, if present
                                                      //       setting the brightness to -1 will re enable the light sensor
  int newLedBrightness = ledBrightness;
  int prevLedBrightness = ledBrightness;
  String msgItem = "LB" + fillZero(ledBrightness);
  
  displayMessage(msgMenuLedBrightness);
  encoderDisplayItem(msgItem);

  newLedBrightness = encoderSetValue(newLedBrightness, -1, ls_maxValue, 2, 0, encoderSetLedBrightnessCallback, 0);
  if (returnCode == 9) return;
 
  if (newLedBrightness == -1) {
    ledBrightness = prevLedBrightness;
    ledBrightnessManualSet = false;
  } else {
    if (newLedBrightness < ls_minValue)
      newLedBrightness = ls_minValue;
    ledBrightness = newLedBrightness;
    ledBrightnessManualSet = true;
  }

  matrix.setIntensity(ledBrightness);

  #ifdef ENABLE_LIGHTSENSOR
    if (ledBrightnessManualSet)
      displayMessage((String) msgLedBrightnessSet + " " + (String) ledBrightness);  
    else
      displayMessage((String) msgLedBrightnessSet + " auto");
  #else
    displayMessage((String) msgLedBrightnessSet + " " + (String) ledBrightness);
  #endif
}

void encoderSetLedBrightnessCallback(int value) {
  if (value >= 0)
    matrix.setIntensity(value);
}

void encoderSetNeoBacklight() {
  #ifdef ENABLE_NEOPIXELS
    String msgItem = fillSpace(neoBacklight, 4);
    displayMessage(msgMenuNeoBacklight);
    encoderDisplayItemLong(msgItem);
    neoBacklight = encoderSetValue(neoBacklight, 0, numOfNeoPalette, 0, 1, encoderSetNeoBacklightCallback, 200);
    if (returnCode == 9) return;
    displayMessage((String) msgMenuNeoBacklight + " " + (String) neoBacklight);
  #else
    displayMessage(msgMenuNotAvailable);
  #endif
}

void encoderSetNeoBacklightCallback(int value) {
  #ifdef ENABLE_NEOPIXELS
    neopixelsPresetOn(value);
  #endif
}

void encoderSyncTime() {  
  #ifdef NO_WIFI    
    displayMessage(msgNoWifi);
  #else
    displayMessage(msgMenuSyncTime);
    UI_displayTimeRead = true;
    getTimeFromServer();
  #endif
}

void encoderSetSleepMode() {
  String msgItem = "Sl" + (String) (sleepMode ? msgOn : msgOff);
  displayMessage(msgMenuSleepMode);
  encoderDisplayItemOnOff(msgItem);
  sleepMode = encoderSetValue(sleepMode, 0, 1, 2, 10, NULL, 0);
  if (returnCode == 9) return;
  displayMessage((String) msgMenuSleepMode + " " + (sleepMode ? msgOn : msgOff));
}

void encoderSetSuspendMode() {
  String msgItem = "Su" + (String) (suspendMode ? msgOn : msgOff);
  displayMessage(msgMenuSuspendMode);
  encoderDisplayItemOnOff(msgItem);
  suspendMode = encoderSetValue(suspendMode, 0, 1, 2, 10, NULL, 0);
  if (returnCode == 9) return;
  displayMessage((String) msgMenuSuspendMode + " " + (suspendMode? msgOn : msgOff));
}

void encoderSetdisplayInfo() {
  String msgItem = "Da" + (String) (displayInfo ? msgOn : msgOff);
  displayMessage(msgMenudisplayInfo);
  encoderDisplayItemOnOff(msgItem);
  displayInfo = encoderSetValue(displayInfo, 0, 1, 2, 10, NULL, 0);
  if (returnCode == 9) return;
  displayMessage((String) msgMenudisplayInfo + " " + (displayInfo ? msgOn : msgOff));
}

void encoderSetdisplayInfoDelay() {
  String msgItem = fillSpace(displayInfoInterval, 4);
  displayMessage(msgMenuDisplayInfoInterval);
  encoderDisplayItemLong(msgItem);
  displayInfoInterval = encoderSetValue(displayInfoInterval, 10, 9999, 0, 1, NULL, 200);
  if (returnCode == 9) return;
  displayMessage((String) msgMenuDisplayInfoInterval + " " + (String) displayInfoInterval);
}

void encoderDisplayInfo() {
//  String tmpMsg = msgUIstart0 + (String) UI_ssid + msgUIstart1 + WiFi.softAPIP().toString();
//  tmpMsg += " - Version " + (String) _VERSION;
//displayMessage(tmpMsg);

  String tmpMsg = "ericBclock - version " + (String) _VERSION + " - " + (String) _DATE + " by " + (String) _AUTHOR;
  tmpMsg += " - Wifi SSID " + (String) UI_ssid + " IP " + WiFi.softAPIP().toString();
  tmpMsg += " - lan " + lanIP.toString();
  DEBUGPRINTLN(tmpMsg);
  displayIntro(tmpMsg);
}

void encoderSetTime() {
  int newHour = timeData.tm_hour;
  int newMin = timeData.tm_min;
  int newSec = timeData.tm_sec;
 
  String msgItem = fillZero(newHour) + fillZero(newMin);
  displayMessage(msgMenuSetTime0);
  encoderDisplayItem(msgItem);
  newHour = encoderSetValue(newHour, 0, 23, 0, 0, NULL, 200);
  if (returnCode == 9) return;
  newMin = encoderSetValue(newMin, 0, 59, 2, 0, NULL, 200);
  if (returnCode == 9) return;
  
  msgItem = "  " + fillZero(newSec);
  displayMessage(msgMenuSetTime1);
  encoderDisplayItem(msgItem);
  newSec = encoderSetValue(newSec, 0, 59, 2, 0, NULL, 200);
  if (returnCode == 9) return;

  setTime(newHour, newMin, newSec, timeData.tm_mday, (timeData.tm_mon + 1), (timeData.tm_year + 1900));    // important: offset the year of the tm structure by 1900 and month by 1
  currentTime = now();     
  timeData = *localtime(&currentTime);
  timeManualSet = true;

  msgItem = (String) msgTimeSet + " " + fillZero(newHour) + ":" + fillZero(newMin) + ":" + fillZero(newSec);
  displayMessage(msgItem);  
}

void encoderSetDate() {
  int newYear = timeData.tm_year + 1900;                  // important: offset the year of the tm structure with 1900 !
  int newMon = timeData.tm_mon + 1;                       // important: tm month is from 0 to 11 so add 1 !
  int newMDay = timeData.tm_mday;
  int maxDays;

  String msgItem = fillSpace(newYear, 5);
  displayMessage(msgMenuSetDate0);
  encoderDisplayItemLong(msgItem);
  newYear = encoderSetValue(newYear, 1900, 2345, 0, 1, NULL, 200);
  if (returnCode == 9) return;

  msgItem = fillZero(newMon) + fillZero(newMDay);
  displayMessage(msgMenuSetDate1);
  encoderDisplayItem(msgItem);
  newMon = encoderSetValue(newMon, 1, 12, 0, 0, NULL, 200);
  if (returnCode == 9) return;
  
  maxDays = getDaysInMonth(newYear, newMon);  
  newMDay = encoderSetValue(newMDay, 1, maxDays, 2, 0, NULL, 200);
  if (returnCode == 9) return;

  readCurrentTime();  
  setTime(timeData.tm_hour, timeData.tm_min, timeData.tm_sec, newMDay, newMon, newYear);
  timeManualSet = true;

  msgItem = (String) msgDateSet + " " + fillZero(newMDay) + "-" + fillZero(newMon) + "-" + fillZero(newYear);
  displayMessage(msgItem);  

  checkDST();
  displayMessage((DST ? "DST on" : "DST off"));
}

void encoderTimeDisplayMode() {
  displayMessage(msgMenuTimeDisplayMode);  
  timeDisplayMode = encoderSetValue(timeDisplayMode, 0, (FN_ARRAY_SIZE(timeDisplayModes) - 1), 0, 20, encoderTimeDisplayModeUpdate, 200);
  if (returnCode == 9) return;
  displayMessage((String) msgMenuTimeDisplayMode + " " + timeDisplayModes[timeDisplayMode]);
}

void encoderTimeDisplayModeUpdate(int item) {
  String tmpMsg = (String) item + "|" + timeDisplayModes[item];
  displayMessage(timeDisplayModes[item]);
  displayMessageShort(tmpMsg);    
}

//// disabled

void encoderUIminimal() {
  bool UIminimal = UI_controls_minimal;
  String msgItem = "Um" + (String) (UIminimal ? msgOn : msgOff);
  displayMessage(msgMenuUIminimal);
  encoderDisplayItemOnOff(msgItem);
  UIminimal = encoderSetValue(UIminimal, 0, 1, 2, 10, NULL, 0);
  if (returnCode == 9) return;
  displayMessage((String) msgMenuUIminimal + " " + (UIminimal ? msgOn : msgOff));
  
  if (UIminimal) {
    UI_controls_minimal = true;
    UI_controls_extended_sliders = true;
    UI_dnsServer.stop();
    UI_setup();
  }
}

////

void encoderSave() {
  eepromWrite();
  playBuzzer(21);
  displayMessage((String) msgMenuSaved);
}

void encoderRestart() {
  DEBUGPRINTLN("Menu item restart selected");  
  displayMessage((String) msgMenuRestart);  
  playBuzzer(21);
  ESP.restart();
}

void encoderAnnounce() {
  displayMessage(msgMenuAnnouncement0);  
  announceItem = encoderSetValue(announceItem, 0, (FN_ARRAY_SIZE(announcements) - 1), 0, 20, encoderAnnounceDisplay, 200);

  if (announceItem == (FN_ARRAY_SIZE(announcements) - 1) || returnCode == 9)
    announceMode = false;
  else
    announceMode = true;  
}

void encoderAnnounceDisplay(int item) {
  String tmpMsg = (String) item + "|" + announcements[item];
  displayMessage(announcements[item]);
  displayMessageShort(tmpMsg);  
}

//
// display and set functions
//

void encoderDisplayItem(String item) {
  for (int i = 0; i < 4; i++)
    matrix.drawChar(charPos[i], yOffset, item[i], HIGH, LOW, 1);
  matrix.write();
}

void encoderDisplayItemLong(String item) {
  for (int i = 0; i < 5; i++)
    matrix.drawChar(charPosLong[i], yOffset, item[i], HIGH, LOW, 1);
  matrix.write();
}

void encoderDisplayItemOnOff(String item) {
  for (int i = 0; i < 5; i++)
    matrix.drawChar(charPosOnOff[i], yOffset, item[i], HIGH, LOW, 1);
  matrix.write();
}

int encoderSetValue(int value, int minValue, int maxValue, int startPos, byte inputType, callBackFunctionInt callbackFunction, unsigned long delayTime) {
  // inputType: 0 is number (2 digits), 1 is number (5 digits), 10 is bool (on/off), 20 is text (no display, use callbackfunction)
  // byte returnCode is set 0 when selected, 9 for timeOut

  returnCode = 0;
  unsigned long startTime = millis();
  unsigned long startBlinkTime = 0;
  bool blinkState = true;
  int reResult;
  int newValue, prevValue = value;
  String charValue, onOff;

  if (inputType == 10) {
    minValue = 0;
    maxValue = 1;

    newValue = value;
    if (newValue == -1)
      newValue = 1;
  } else {
    newValue = constrain(value, minValue, maxValue);
  }

  if (inputType == 20 && callbackFunction != NULL)
    callbackFunction(newValue);

  while(1) {
    if (millis() - startBlinkTime > 250) {
      switch (inputType) {
        case 0:
          charValue = fillZero(newValue);
          for (int i = 0; i < 2; i++)
            matrix.drawChar(charPos[startPos + i], yOffset, charValue[i], (blinkState ? HIGH : LOW), LOW, 1);
          break;

        case 1:
          charValue = fillSpace(newValue, 5);
          for (int i = 0; i < 5; i++)
            matrix.drawChar(charPosLong[startPos + i], yOffset, charValue[i], (blinkState ? HIGH : LOW), LOW, 1);
          break;

        case 10:
          onOff = (String) (newValue ? msgOn : msgOff);
          for (int i = 0; i < 3; i++) 
            matrix.drawChar(charPosOnOff[startPos + i], yOffset, onOff[i], (blinkState ? HIGH : LOW), LOW, 1);
          break;

        case 20:
          delay(20);
      }
      
      if (inputType != 20)
        matrix.write();

      blinkState = !blinkState;
      startBlinkTime = millis();
    }

    reResult = readRotEnc();

    if (reResult < 0) {
      newValue--;
      if (newValue < minValue)
        newValue = maxValue;
    }
    else if (reResult > 0) {
      newValue++;
      if (newValue > maxValue)
        newValue = minValue;
    }

    if (reResult != 0) {      
      blinkState = true;
      startTime = millis();
      startBlinkTime = millis() - 250;

      if (callbackFunction != NULL)
        callbackFunction(newValue);
    }

    if (readRotEncSwitch() != false) 
      break;

    if (millis() - startTime > (1000 * menuTimeout)) {
      DEBUGPRINTLN("Menu timeout");
      newValue = prevValue;
      returnCode = 9;      
      break;
    }
  }

  if (inputType == 0) {
    matrix.drawChar(charPos[startPos + 0], yOffset, charValue[0], HIGH, LOW, 1);
    matrix.drawChar(charPos[startPos + 1], yOffset, charValue[1], HIGH, LOW, 1);
    matrix.write();
  }

  if (inputType == 10 && newValue != 0)
    newValue = -1;  

  delay(delayTime);
    
  return newValue;
}

int encoderMenuSelect(bool firstCall) {
  static int menuItem = 0;
  int reResult = 0;
  bool reSwitchPressed = false;
  unsigned long menuStartTime = 0, itemChangedStartTime = 0;

  if (firstCall) {
    menuItem = 0;
    displayMessage(msgMenu);
  }
  
  encoderDisplayMenuItemShort(menuItem);
  menuStartTime = millis();
  itemChangedStartTime = menuStartTime;
  
  do {
    reResult = readRotEnc();

    if (reResult < 0) {             // down
      --menuItem;
      if (menuItem < 0)
        menuItem = FN_ARRAY_SIZE(menu) - 1;
    } else if (reResult > 0) {      // up
      menuItem++;
      menuItem = menuItem % FN_ARRAY_SIZE(menu);
    }

    if (reResult != 0) {
      matrix.fillScreen(0);
      encoderDisplayMenuItemShort(menuItem);
      menuStartTime = millis();
      itemChangedStartTime = menuStartTime;
    }

    if (itemChangedStartTime) {
      if (millis() - itemChangedStartTime > 1500) {
        matrix.fillScreen(0);
        encoderDisplayMenuItem(menuItem);
        encoderDisplayMenuItemShort(menuItem);
        menuStartTime = millis();
        itemChangedStartTime  = 0;
      }
    }

    reSwitchPressed = readRotEncSwitch();

    delay(5);
  } while ((millis() - menuStartTime < (1000 * menuTimeout)) && reSwitchPressed == false);

  if (reSwitchPressed == false)
    return -1;
  else
    return menuItem;
}

void encoderMenu() {
  int menuItem;
  bool firstCall = true;
  
  DEBUGPRINTLN("Entering encoder menu");
  alarmSuspended = true;
  DEBUGPRINTLN("Alarm temporarily suspended");

  do {
    menuItem = encoderMenuSelect(firstCall);
    DEBUGPRINTLN("Menu selection " + (String) menuItem);
    
    switch (menuItem) {
      case  0: encoderSetAlarm0();           break;
      case  1: encoderSetLedBrightness();    break;
      case  2: encoderSetNeoBacklight();     break;
      case  3: encoderSyncTime();            break;
      case  4: encoderSetSleepMode();        break;
      case  5: encoderSetSuspendMode();      break;
      case  6: encoderSetdisplayInfo();      break;
      case  7: encoderSetdisplayInfoDelay(); break;
      case  8: encoderSetTime();             break;
      case  9: encoderSetDate();             break;
      case 10: encoderTimeDisplayMode();     break;
//      case 11: encoderUIminimal();           break; // @EB-todo disabled
      case 11: encoderDisplayInfo();         break;
      case 12: encoderSave();                break;
      case 13: encoderRestart();             break;

      #ifdef ANNOUNCE_MODE
        case 14: 
          encoderAnnounce();
          if (announceMode) {
            customAnnounceMsg = false;
            menuItem = -1;   
          }         
          break;
      #endif
    }    

    if (!announceMode)
      eepromChanged();
      
    firstCall = false;

    if (UI_enabled) {                        // handle the UI if the webinterface is enabled  @EB-todo: use interrupt? also no UI handling when displaying messages
      UI_rereadTab(-1);
      UI_dnsServer.processNextRequest();
    }

  } while (menuItem != -1 && menuItem != FN_ARRAY_SIZE(menu)- 1);

  matrix.fillScreen(0);
  alarmSuspended = false;
  sleepStartTime = millis();
  displayInfoStartTime = millis();
  sleeping = false;
  refreshTimeDisplay = true;
  DEBUGPRINTLN("Menu exited");  
  DEBUGPRINTLN("Alarm suspending ended");  
}
  
void encoderDisplayMenuItem(int menuItem) {
  String tmpMsg = menu[menuItem];
  displayMessage(tmpMsg);
}

void encoderDisplayMenuItemShort(int menuItem) {
  String tmpMsg = (String) menuItem + "|" + menu[menuItem];
  displayMessageShort(tmpMsg);
}