/*
***************************************************************************
  ebc_alarmclock - display functions
***************************************************************************
  last update 20210320 by ericBcreator
***************************************************************************
*/

void displayTime(bool updateDisplay) {
  static unsigned long startTime = millis();
  char timeArray[7], bmeArray[7];
  int xOffset = 0, displayTemp;
  activeTimeDisplayMode = timeDisplayMode;

  if (updateDisplay)
    readCurrentTime();

  if (refreshTempSwapTime > 0 && !animationStarted) {               // after sleepmode ended, reset the time/temp swap start time
    if (refreshTempSwapTime == 2)                                   // reset to time display when changing the displaymode from the ESPUI
      BME280_tempTimeSwitch = false;
    refreshTempSwapTime = 0;
    refreshTimeDisplay = true;
    startTime = millis();
  }
 
  #ifndef NO_WIFI                                                   // only resync when wifi and timeserver are enabled
    #ifndef DEBUG_NO_TIMESERVER
      if (timeData.tm_hour == 3 && timeData.tm_min == 0 && timeData.tm_sec == 0) {         // @EB-todo reread the time every day at 3:00 AM to synchronize and check DST
        if (!timeManualSet) {
          DEBUGPRINT("3 AM, resyncing the time");
          getTimeFromServer();
        }
      }
    #endif
  #endif

  if (refreshTimeDisplay) {
    matrix.fillScreen(LOW);
    refreshTimeDisplay = false;
    
    if (!animationStarted)
      updateDisplay = true;

//    if (!timeManualSet) {                    // @EB-todo: trying to capture the exact start of the second: not working, time info from the time server
//                                             //           is second based, so there is no way of knowing the exact start of the second
//      int tmpSec = timeData.tm_sec;
//      
//      while (timeData.tm_sec == tmpSec) {    // when refreshing, wait until a new second has started before continuing (to sync multiple clocks)
//        readCurrentTime();
//        delay(20);
//      };
//    }
  }

  String (fillZero(getPMhour()) + fillZero(timeData.tm_min) + fillZero(timeData.tm_sec)).toCharArray(timeArray, 7);      

  #ifdef ENABLE_BME280  
    if (LED_NUM_OF_HOR_DISPLAYS < 8) {                              // don't alternate when there are 8 (or more) matrices
      if (BME280_tempTimeSwap) {                                    // alternate between time and temp display if selected
        if (BME280_tempTimeSwitch) {
          if (millis() - startTime > BME280_dispTempInterval) {
            BME280_tempTimeSwitch = false;
  //        if (!animationStarted) displayAnimPacMan(true);         // @EB-todo
            matrix.fillScreen(LOW);
            startTime = millis();
          }
        } else if (millis() - startTime > BME280_dispTimeInterval) {
          BME280_tempTimeSwitch = true;
  //      if (!animationStarted) displayAnimPacMan();               // @EB-todo
          matrix.fillScreen(LOW);
          startTime = millis();
        }
  
        if (BME280_tempTimeSwitch)
          activeTimeDisplayMode = 6;
      }
    }
  #endif
  
  switch(activeTimeDisplayMode) {
    case 0: // HH:MM
      for (int i = 0; i < 4; i++)
        matrix.drawChar(charPos[i], yOffset, timeArray[i], HIGH, LOW, 1);
      break;

    case 1: // HH:MM big font
      for (int i = 0; i < 4; i++)
        displayCharAltFont(0, charPosBigFont[i], timeArray[i]);
      break;
      
    case 2: // HH MM ss
      for (int i = 0; i < 4; i++) {
        if (ampmMode) {
          if (i > 0 || timeArray[i] > 48)
            matrix.drawChar(charPosSec[i], yOffset, timeArray[i], HIGH, LOW, 1);
        } else
          matrix.drawChar(charPosSec[i], yOffset, timeArray[i], HIGH, LOW, 1);
      }
              
      for (int i = 4; i < 6; i++)
        displayTimeSmall(3, charPosSec[i], timeArray[i]);
      break;

    case 3: // HH MM ap
      for (int i = 0; i < 4; i++) {
        if (ampmMode) {
          if (i > 0 || timeArray[i] > 48)
            matrix.drawChar(charPosSec[i], yOffset, timeArray[i], HIGH, LOW, 1);
        } else
          matrix.drawChar(charPosSec[i], yOffset, timeArray[i], HIGH, LOW, 1);
      }
              
      if (pm) displayTimeSmall(3, charPosSec[4], 59); // P
      else    displayTimeSmall(3, charPosSec[4], 58); // A
      displayTimeSmall(3, charPosSec[5], 60);         // M
      break;

    case 4: // hh mm ss
      for (int i = 0; i < 6; i++)
        if (ampmMode) {
          if (i > 0 || timeArray[i] > 48)
            displayTimeSmall(4, charPosSmallPM[i], timeArray[i]);
        } else
          displayTimeSmall(4, charPosSmall[i], timeArray[i]);
      break;

    case 5: // hh mm:ss
      for (int i = 0; i < 6; i++)
        if (ampmMode) {
          if (i > 0 || timeArray[i] > 48)
            displayTimeSmall(4, charPosSmallColPM[i], timeArray[i]);
        } else
          displayTimeSmall(4, charPosSmallCol[i], timeArray[i]);
      break;
  }
    
  #ifdef ENABLE_BME280
    if (activeTimeDisplayMode == 6 || LED_NUM_OF_HOR_DISPLAYS >= 8) {      // temp and humidity
      if (BME280_temperature < 0)
        displayTemp = (BME280_temperature - .5);
      else
        displayTemp = (BME280_temperature + .5);
        
      String (fillSpace(displayTemp, 3) + (BME280_fahrenheid ? "F" : "C") + fillZero((BME280_humidity + .5))).toCharArray(bmeArray, 7);

      #ifdef AMPM_2ND_MATRICES
        if (LED_NUM_OF_HOR_DISPLAYS >= 8 && ampmMode) {
          if (BME280_temperature < -9 or BME280_temperature > 99)         // temp is 3 chars so use temp symbol as well
            xOffset = 8;
          else
            xOffset = 3;
            
          for (int i = 0; i < 3; i++)
            displayTimeSmall(4, xOffset + xOffset2ndMatrixSet + charPosBME[i], bmeArray[i]);
  
          if (xOffset == 3) {                                             // only display the temp symbol when it fits
            xOffset = 1;
    
            if (BME280_fahrenheid)
              displayTimeSmall(4, xOffset + xOffset2ndMatrixSet + charPosBME[3], 59);
            else
              displayTimeSmall(4, xOffset + xOffset2ndMatrixSet + charPosBME[3], 58);
          }
        } else {
      #endif
        for (int i = 0; i < 3; i++)
          displayTimeSmall(4, xOffset2ndMatrixSet + charPosBME[i], bmeArray[i]);      
  
        if (BME280_fahrenheid)
          displayTimeSmall(4, xOffset2ndMatrixSet + charPosBME[3], 59);
        else
          displayTimeSmall(4, xOffset2ndMatrixSet + charPosBME[3], 58);
          
      #ifdef AMPM_2ND_MATRICES
        }
      #endif
              
      for (int i = 4; i < 6; i++)
        displayTimeSmall(3, xOffset2ndMatrixSet + charPosBME[i], bmeArray[i]);
    }
  #endif

  displayPixelModes();
  displayTimeColon();
  if (activeTimeDisplayMode != 3) 
    displayPM();

  if (updateDisplay)
    matrix.write();
}

void displayTimeSmall(byte charWidth, byte column, unsigned char c) {
  unsigned int line;
  unsigned char newC = c;

  if (c == 45) newC = 60;   // - char
  
  for (int i = 0; i < charWidth; i++ ) {
    if (newC == 32) {
      line = 0;
    } else {
      if (charWidth == 3)     
        line = numberFont3[(newC - 48) * 4 + i + 1];
      else if (charWidth == 4)
        line = numberFont4[(newC - 48) * 5 + i + 1];
    }
        
    for (int j = 0; j < 8; j++, line >>= 1) {
      if (line & 1)
        matrix.drawPixel(column + i, yOffset + j, HIGH);
      else
        matrix.drawPixel(column + i, yOffset + j, LOW);
    }
  } 
}

void displayCharAltFont(byte font, int startColumn, unsigned char c, bool continuePrevColumn) {
  static int prevColumn = 0;  
  unsigned int maxCharWidth, charWidth, charStartPos;
  unsigned char newC = c;
  int line, column;

  if (continuePrevColumn)
    prevColumn +=2;
  else
    prevColumn = startColumn;

  if (font == 0) {      // big number font
    maxCharWidth = numberFontBig[0];
  }
  
  if (newC == 32) 
    charWidth = 1;
  else {
    charStartPos = (newC - 48) * (maxCharWidth + 1) + 1;

    if (font == 0) {    // big number font
      charWidth = numberFontBig[charStartPos];
    }
  }

  for (int i = 0; i < charWidth; i++ ) {
    column = prevColumn + i;
    line = 0;
    
    if (newC != 32) {      
      if (font == 0) {  // big number font
        line = numberFontBig[(charStartPos + i + 1)];
      }
    }

    for (int j = 0; j < 8; j++, line >>= 1) {      
      if (line & 1)
        matrix.drawPixel(column, yOffset + j, HIGH);
      else
        matrix.drawPixel(column, yOffset + j, LOW);
    }
  } 

  prevColumn = column;
}

void displayTimeColon() {
  static unsigned int prevSec = 0;

  if (timeData.tm_sec != prevSec) {
    if (!blinkingColon)
      colon = HIGH;
    else {
      if (timeData.tm_sec % 2 == 0)
        colon = HIGH;
      else
        colon = LOW;
    }
    
    if (activeTimeDisplayMode == 0) {
      matrix.drawPixel(15, 2, colon);
      matrix.drawPixel(15, 4, colon);
    } else if (activeTimeDisplayMode == 1) {
      matrix.drawPixel(31, 2, colon);
      matrix.drawPixel(31, 3, (colon ? colon : !colon));
      matrix.drawPixel(31, 4, (colon ? colon : !colon));
      matrix.drawPixel(31, 5, !colon);
    } else if (activeTimeDisplayMode == 5) {
      matrix.drawPixel(21, 2, colon);
      matrix.drawPixel(21, 4, colon);
    }

    #ifdef AMPM_2ND_MATRICES
      if (activeTimeDisplayMode == 6 || (LED_NUM_OF_HOR_DISPLAYS >= 8 && !ampmMode))
    #endif
        matrix.drawPixel(xOffset2ndMatrixSet + 16, 0, colon);

    prevSec = timeData.tm_sec;
  }
}

void displayPixelModes() {
  matrix.drawPixel(0,  0, (suspendMode ? HIGH : LOW));                            // top left suspend mode
  matrix.drawPixel(0,  7, (sleepMode ? HIGH : LOW));                              // bottom left sleep mode
  matrix.drawPixel((matrixWidth - 1), 0, (alarmList[0].active ? HIGH : LOW));     // top right alarm 1 active
  matrix.drawPixel((matrixWidth - 1), 7, (sleeping ? HIGH : LOW));                // bottom right sleeping
}

void displayAnnounceMode(byte displayMode) {
  matrix.fillScreen(LOW);
  matrix.drawPixel(14,  3, HIGH);    
  matrix.drawPixel(18,  3, HIGH);    

  if (displayMode == 1)
    matrix.drawPixel(16,  3, HIGH);    
    
  matrix.write();  
}

void displayEndAnnounceMode() {
  matrix.fillScreen(LOW);
  matrix.write();  
}

void displayPM() {
  bool highLow = LOW;

  #ifdef AMPM_2ND_MATRICES
    if (LED_NUM_OF_HOR_DISPLAYS >= 8 && ampmMode) {                 // when AM/PM mode is active, display AM/PM on the right set of 4 matrices
      if (activeTimeDisplayMode == 3)                               // AM/PM is already displayed in this mode so exit
        return;
        
      if (pm) displayTimeSmall(3, 8 + charPosSec[4], 59);  // P
      else    displayTimeSmall(3, 8 + charPosSec[4], 58);  // A      
      displayTimeSmall(3, 8 + charPosSec[5], 60);          // M
      return;
    }
  #endif
  
  if ((activeTimeDisplayMode == 4 || activeTimeDisplayMode == 5) && !ampmMode) // prevent the am/pm pixels from interfering with the left char
    return;
    
  if (ampmMode && pm)
    highLow = HIGH;
        
  for (int i = 3; i < 5; i++)
    matrix.drawPixel(0, i, highLow);  
}

void displayAlarmTime(String message) {  
  String strAlarmTime;
  strAlarmTime = fillZero(alarmList[0].hour) + ":" + fillZero(alarmList[0].min) + ":" + fillZero(alarmList[0].sec);

  if (alarmList[0].actionType == actionNeoWUL)
    strAlarmTime += msgWakeupLight;
  else if (alarmList[0].actionType == actionNeoWULchain)
    strAlarmTime += msgWakeupLightChain;
  
  displayMessage(message + " " + strAlarmTime);
}

#ifdef DAYLIGHTINFO
  void displayDaylightInfo(bool displayDaylightInfo) {
    String daylightInfoMessage;
    time_t t_today;
    
    char d2dSunrise[] = "00:00";
    char d2dSunset[] = "00:00";
    char d2dDaylightTime[] = "00:00";  
    Dusk2Dawn d2d(d2d_lattitude, d2d_longitude, d2d_timezone);
  
    if (timeManualSet) {
      t_today= now();
    } else {
      time(&t_today);     // @EB-todo: connects to timeserver with every call ???
    }
    
    int todaySunrise = d2d.sunrise(year(t_today), month(t_today), day(t_today), DST);
    int todaySunset = d2d.sunset(year(t_today), month(t_today), day(t_today), DST);
    int daylightTime = todaySunset - todaySunrise;
  
    time_t t_yesterday = (t_today - (24 * 60 * 60));
    int yesterdaySunrise = d2d.sunrise(year(t_yesterday), month(t_yesterday), day(t_yesterday), DST);
    int yesterdaySunset = d2d.sunset(year(t_yesterday), month(t_yesterday), day(t_yesterday), DST);
    int daylightTimeYesterday = yesterdaySunset - yesterdaySunrise;
  
    int sunriseDiff = yesterdaySunrise - todaySunrise;
    int sunsetDiff = todaySunset - yesterdaySunset;
    int daylightTimeDiff = daylightTime - daylightTimeYesterday;
  
    String sunriseDiffS = (sunriseDiff > 0 ? "+" : "");
    String sunsetDiffS = (sunsetDiff > 0 ? "+" : "");
    String dltDiffS = (daylightTimeDiff > 0 ? "+" : "");
    
    Dusk2Dawn::min2str(d2dSunrise, todaySunrise);
    Dusk2Dawn::min2str(d2dSunset, todaySunset);
    Dusk2Dawn::min2str(d2dDaylightTime, daylightTime);
  
  //  sprintf(pd_message2, "%s : %s %s, %s %s, %s %s (%s: %d min)", 
  //    d2d_City, msgSunrise, d2dSunrise, msgSunset, d2dSunset, msgDaylightTime, d2dDaylightTime, msgDaylightTimeDifference, daylightTimeDiff);

    daylightInfo =  (String) msgSunrise + (String) + " " + (String) d2dSunrise + (String) " (" + (String) sunriseDiffS + (String) sunriseDiff + (String) + " min), ";
    daylightInfo += "<br>";  // insert line break for the web interface
    daylightInfo += (String) msgSunset  + (String) + " " + (String) d2dSunset  + (String) " (" + (String) sunsetDiffS  + (String) sunsetDiff  + (String) + " min), ";
    daylightInfo += (String) msgDaylightTime + (String) + " " + (String) d2dDaylightTime + (String) " (" + (String) dltDiffS + (String) daylightTimeDiff + (String) + " min)";
  
    if (displayDaylightInfo) {
      daylightInfoMessage = d2d_City + (String) " : " + (String) daylightInfo;
      daylightInfoMessage.replace("<br>", "");  // remove the linebreaks for the display
      displayMessage(daylightInfoMessage);
    }
      
  }
#endif

#ifdef ENABLE_BME280
  void displayBME280() {
    char charBuffer[10];
    String BME280_info;

    sprintf(charBuffer, "%3.1f", BME280_temperature);
    BME280_info = (String) msgbmeTemperature + ' ' + (String) charBuffer + (BME280_fahrenheid ? "F " : "C ");
    BME280_info += (String) msgbmeHumidity + ' ' +  (String) int((BME280_humidity + .5)) + "% ";
    BME280_info += (String) msgbmePressure + ' ' +  (String) int((BME280_pressure + .5)) + " hPa ";

    #ifdef BME280_SEA_LEVEL_PRESSURE
      sprintf(charBuffer, "%2.1f", BME280_altitude);
      BME280_info += (String) charBuffer + " mtr ";
    #endif

    displayMessage(BME280_info);
  }
#endif

//
// display main functions
//

void displayMessage(String message) {
  unsigned long delayTime;
  int letter = 0;
  int x = 0;
  int y = yOffset + (matrix.height() - 8) / 2; 				// center the text vertically  

  if (alarmMessage)
    DEBUGPRINT("           Message at ");
  else
    DEBUGPRINT("Message    at ");

  readCurrentTime();
  DEBUGPRINTLN((String) currentTime + " - " + timeString + ": " + message);

 
  for (int i = 0 ; i < charDefaultWidth * message.length() + matrixWidth - charSpacer; i++) {
    letter = i / charDefaultWidth;
    x = (matrixWidth - 1) - i % charDefaultWidth;
    
    while (x + charDefaultWidth - charSpacer >= 0 && letter >= 0) {
      if (letter < message.length()) {
        if (message[letter] == '|')
          matrix.drawChar(x, y, ' ', HIGH, LOW, 1);
        else
          matrix.drawChar(x, y, message[letter], HIGH, LOW, 1);
      }
      letter--;
      x -= charDefaultWidth;
    }
    matrix.write();

    readCurrentTime();
    delayTime = millis();

    while ((millis() - delayTime) < scrollDelayTime) {
      if (alarmTriggered) {
        checkRecurSkipped();
        #ifdef ENABLE_NEOPIXELS
          switch (alarmList[alarmNum].actionType) {
            case actionNeoWUL: 
            case actionNeoWULchain:
              neopixelsFade(0, false); break;                       // loop neopixel wake up light
          }
        #endif
     
      } else {
        checkAlarms();
        if (alarmTriggered) 
          return;
      }

      if (UI_enabled) {                                             // @EB-todo
        if ((millis() - startUIupdateTime) >= UI_updateDelay) {     // reread the settings after x milliseconds      
          UI_rereadTop();
          startUIupdateTime = millis();
        }
        UI_dnsServer.processNextRequest();
      }
  
      if (UI_alarmToInterrupt) {
        UI_alarmToInterrupt = false;
        if (alarmTriggered) {                       // the Alarm off/stop button is clicked, check if an alarm is triggered. if so, interrupt the alarm
          alarmInterrupted();
        } else {                                    // if not, stop the message
          matrix.fillScreen(LOW);
          matrix.write();
        }
        return;
      }

      #ifndef NO_ENCODER
        reSwitch = readRotEncSwitch();
        if (reSwitch != false) {
          matrix.fillScreen(0);
          matrix.write();
          if (alarmTriggered)
            alarmInterrupted();
          return;
        }
    
        if (readRotEnc() != false) {
          matrix.fillScreen(0);
          matrix.write();
          return;
        }
      #endif

      delay(5);
    }
      
    #ifdef ENABLE_LIGHTSENSOR
      checkLightSensor();
    #endif
  }
}

void displayMessageShort(String message) {
  static int maxChars = LED_NUM_OF_HOR_DISPLAYS * 2 - 1;
  int x = 0;
  int y = yOffset + (matrix.height() - 8) / 2;

  readCurrentTime();  
  DEBUGPRINTLN("Short message " + (String) currentTime + " - " + timeString + ": " + message);

  for (int i = 0; i < maxChars; i++) {
    if (message[i] == '|' || message[i] == 32) {    // set spacing between chars:
      x++;                                          // 1 pixel
      if (LED_NUM_OF_HOR_DISPLAYS >= 8 )            // 2 pixels when using 8 (or more) matrices
        x++;
    } else if (message[i] == ':') {
      matrix.drawPixel(x, 2, HIGH);
      matrix.drawPixel(x, 4, HIGH);
      x += 2;
    } else {
      matrix.drawChar(x, y, message[i], HIGH, LOW, 1);
      x += charDefaultWidth + charSpacer - 1;
    }
  }
  matrix.write();
}

void displayMessageOutLoop(int xOffset, String message, bool reverse) {
  int letter, x;
  int y = yOffset + (matrix.height() - 8) / 2; // center the text vertically
  int xMax = matrix.width() - xOffset;

  int fgCol = HIGH, bgCol = LOW;
  if (reverse) {
    fgCol = LOW;
    bgCol = HIGH;    
  }

  message += " ";
  
  for (int i = 0 ; i < charDefaultWidth * message.length() + xMax - charSpacer; i++) {
    letter = i / charDefaultWidth;
    x = (xMax - 1) - i % charDefaultWidth;
        
    while (x + charDefaultWidth - charSpacer >= 0 && letter >= 0) {
      if (letter < message.length()) {
        if (x >= 0)
          matrix.drawChar(xOffset + x, y, message[letter], fgCol, bgCol, 1);
      }
      letter--;
      x -= charDefaultWidth;
    }
    matrix.write();
    delayAndHandle(30);
  }
}

void displayClock(bool reverse) {
  unsigned int line;
  int xOffset = 11;
  int fgCol = HIGH;
  int bgCol = LOW;

  if (reverse) {
    fgCol = LOW;
    bgCol = HIGH;
  }

  matrix.fillScreen(bgCol);
  displayChar(xOffset, fontCharLogo, fgCol, false);
  matrix.write();
  delayAndHandle(150);
  matrix.fillScreen(LOW);
}

void displayIntro(String message) {  
  unsigned int line;
  int x = 0, xOffset = 11;  
  String tmpMessage = message;
  bool reverse = true;
  
  if (tmpMessage == "")
    tmpMessage = "ericBclock"; 

  playBuzzer(20);

  for (x = (matrixWidth - 1); x >= 0; x--) {
    displayChar(x, fontCharLogo, LOW, false);    
    matrix.write();
    delay(30);
  }

  displayMessageOutLoop(xOffset, tmpMessage, reverse);

  if (!reverse) {
    for (int y = 0; y < 8; y++) {
      for (int x = 0; x < matrixWidth; x++) {
        matrix.drawPixel(x, y, HIGH);
      }
      matrix.write();
      delay(40);
    }
  }

  for (int y = 0; y < 8; y++) {
    for (int x = 0; x < matrixWidth; x++) {
      matrix.drawPixel(x, (7 - y), LOW);
    }
    matrix.write();
    delay(40);
  }

  delay(100);
}

void displayChar(int x, unsigned int fontChar[], int lowHigh, bool fill, bool reverse) {
  unsigned int line;
  int column;  
    
  for (int i = 1; i <= fontChar[0]; i++ ) {
    if (reverse)
      line = fontChar[fontChar[0] + 1 - i];
    else
      line = fontChar[i];
    column = x + i - 1;  
    
    if (column >= 0 && column < matrixWidth) {
      for (int j = 0; j < 8; j++, line >>= 1) {
        if (line & 1 || fill)
          matrix.drawPixel(column, yOffset + j, lowHigh);
        else
          matrix.drawPixel(column, yOffset + j, !lowHigh);
      }
    }
  }
}

void displayAnimPacMan(bool reverse) {  
  animationStarted = true;
  unsigned long startTime = millis();
  int state = 0, column, maxColumn, charWidth, events;
  charWidth = fontCharPacMan[0][0];
  maxColumn = matrixWidth - charWidth;
  readCurrentTime();
  
  for (int x = -charWidth; x <= matrixWidth; x++) {
    if (reverse) {
      column = matrixWidth - x - charWidth;      
    } else {
      column = x;
    }
    
  if (column < maxColumn) // @EB-todo
      displayTime(false);
      
    if (column >= 0 && column < maxColumn) {
      for (int xx = 0; xx <= column; xx++) {        
        for (int yy = 0; yy < 8; yy++)
          matrix.drawPixel(xx, yy, LOW);
      }
    }

    displayChar(column, fontCharPacMan[state], HIGH, false);
    
    matrix.write();
    
    if (x % 3 == 0) {
      state++;
      state = state % 2;
    }
    
    if (millis() - startTime > 500) {
      readCurrentTime();
      startTime = millis();
    }

    events = delayAndHandle(40);
    if (events != 0 && events != 21) {     // delay and abort if there was a user event (except for PIR movement)
      playBuzzer(20);
      return;
    }
    
    if (alarmTriggered)
      return;
  }

  animationStarted = false;
}
