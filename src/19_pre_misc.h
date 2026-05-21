//
// div
//

String fillZero(int value) {
  int tmpValue;
  
  if (value < 0)
    return String(value);

  tmpValue = constrain(value, 0, 99);
    
  if (tmpValue < 10)
    return '0' + String(tmpValue);
  else
    return String(tmpValue);
}

String fillSpace(int value, int valLength) {
  String tmpStr = String(value);
  while (tmpStr.length() < valLength)
    tmpStr = " " + tmpStr;
  return tmpStr;
}

int delayAndCheckEnc(unsigned int delayTime) {
  unsigned long startTime = millis();
  unsigned long startDelayTime;  
  int returnValue = 0;

  if (announceMode) {                         // don't check in announce mode, it will interfere with delayHandleEvents()
    startDelayTime = millis();
    while (millis() - startDelayTime < delayTime) { }
    return returnValue;
  }

  #ifndef NO_WIFI                             // only resync when wifi and timeserver are enabled
    #ifndef DEBUG_NO_TIMESERVER
      if (UI_timeToResync) {
        getTimeFromServer();
        matrix.fillScreen(0);
        UI_timeToResync = false;
      }
    #endif
  #endif
  
  while ((millis() - startTime) < delayTime) {
    if (alarmTriggered)
      checkRecurSkipped();                    // @EB-todo needed?????

    #ifndef NO_WIFI
      if (UI_enabled) {                                                     // handle the UI if the webinterface is enabled
        if ((millis() - startUIupdateTime) >= UI_updateDelay) {             // reread the settings after x milliseconds
//          UI_rereadTab(); // @EB-todo reread
          UI_rereadTop();
          startUIupdateTime = millis();
        }
        UI_dnsServer.processNextRequest();
      }
    #endif

    #ifdef ENABLE_PIR
      if (prevPIRstate == true) {
        if (millis() - PIRstartTime > (PIRdelay * 1000)) {                // wait before checking the PIR to prevent retriggering
          prevPIRstate = false;
        }
      } else {
        if (digitalRead(PIN_PIR) != PIR_DEFAULT) {
          returnValue = 21;
          digitalWrite(PIN_PIR, PIR_DEFAULT);
          prevPIRstate = true;
          PIRstartTime = millis();
          #ifdef DEBUG_PIR
            DEBUGPRINTLN("Motion detected at " + timeString);
          #endif

          if (sleeping)
            endSleepMode(true);
          else      
            endSleepMode(false);
        }
      }
    #endif

    #ifdef ENABLE_LIGHTSENSOR
      checkLightSensor();
    #endif

    #ifndef NO_ENCODER
      reSwitch = readRotEncSwitch();
      reEnc = readRotEnc();
      
      if (reSwitch || reEnc) {
        DEBUGPRINT("Rotary encoder used: ");

        if (reEnc < 0)          returnValue = -1;
        else if (reEnc > 1)     returnValue =  1;
        if (reSwitch == 1)      returnValue = 11;
        else if (reSwitch == 2) returnValue = 12;

        DEBUGPRINTLN(returnValue);

        if (sleeping) {
          endSleepMode(true);
        } else {
          if (alarmTriggered || forceFirstAlarm) {
            if (forceFirstAlarm)
              forceFirstAlarm = false;
            alarmInterrupted();
          } else {
            if (reSwitch == 1) {
              alarmList[0].active = !alarmList[0].active;
              alarmRecTriggered[0] = false;
              eepromChanged();
  
              if (UI_enabled)
                UI_rereadTab(-1);
  
              displayPixelModes();
              matrix.write();
              
              if (alarmList[0].active)
                displayAlarmTime(msgAlarm0Active); 
              else
                displayMessage(msgAlarm0Inactive);
              return 0;
            } else if (reSwitch == 2) {
              //playBuzzer(20);   @EB-todo
              encoderMenu();
            }
          }
        }
      }
    #endif // NO_ENCODER

//    delay(delayAndCheckEncDelay);             // @EB-todo: strange bug: the delay function causes problems when called from the UI webinterface 
                                                //           (the playTone function after saving) so replaced with empty while loop
    
    startDelayTime = millis();
    while (millis() - startDelayTime < delayAndCheckEncDelay) { 
      #ifndef NO_WIFI
        if (UI_enabled)
          UI_dnsServer.processNextRequest();
      #endif
    }

    /*
	nope: don't return early, it will mess up playtone delays...
	if (returnValue)
      return returnValue;                       // event occurred so break the loop and return the value
	*/
  }

  return 0;
}

//
// buzzer
//

void playTone(int freq, int duration, int postDelay) {
  unsigned int startDelayTime;
  
  if (freq > 0) {
    #ifdef ESP32
      ledcWriteTone(0, freq);
    #else
      tone(PIN_BUZZER, freq);
    #endif
  } 

//  startDelayTime = millis();
//  while ((millis() - startDelayTime) < duration) { };
  delayAndCheckEnc(duration);
  
  #ifdef ESP32
    ledcWriteTone(0, 0);
    ledcWrite(0, LOW);
  #else
    noTone(PIN_BUZZER);
  #endif  

  if (postDelay) {
//    startDelayTime = millis();
//    while ((millis() - startDelayTime) < postDelay) { };
    delayAndCheckEnc(postDelay);
  }
}

void testBuzzer() {  
  for (int i = 100; i < 10000; i += 100) {
    DEBUGPRINTLN(i);
    playTone(true, i, 500);
  }
  playTone(false, 0, 0);
}

#ifdef NO_BUZZER
  void playBuzzer(int numOfBuzz) {                      // no buzzer so just delay numOfBuzz * 100 ms
    if (numOfBuzz < 11) {
      for (int i = 0; i < numOfBuzz; i++) {
        delayAndCheckEnc(100);
      }
    } else
      delayAndCheckEnc(100);
  }
#else
  void playBuzzer(int numOfBuzz) {
    #ifdef ESP32                                        // ESP32
      ledcSetup(0, 0, 8);
      ledcAttachPin(PIN_BUZZER, 0);
    #endif
    
    if (numOfBuzz < 11) {
      for (int i = 0; i < numOfBuzz; i++) {
        playTone(2000, 50, 50);
      }
    } else {
      switch (numOfBuzz) {
        case 11:                                  // big ben ECDG(-1) G(-1)DEC
          playTone(659, 500); playTone(523, 500); playTone(587, 500); playTone(392, 1000, 500);
          playTone(392, 500); playTone(587, 500); playTone(659, 500); playTone(523, 1000, 500);
          break;

        case 12:                                  // Avicii
          //127 BPM = 1/8 Note: 236.22, 1/16 Note: 118.11, 1/32 Note: 59.055, 1/64 Note: 29.528, 1/128: 14.764 
          //sustain: 1/16+1/32+1/64+1/128 = 221ms //release: 1/128 = 15ms 
          
          playTone(1480, 221, 15); playTone(1319, 221, 15); playTone(1319, 221, (15 + 236)); 
          playTone(1319, 221, 15); playTone(1319, 221, 15); playTone(1319, 221, 15); 
          playTone(1319, 221, 15); playTone(1245, 221, 15); playTone(1245, 221, 15); 
          playTone(1319, 221, 15); playTone(1319, 221, (15 + 236));
          playTone(2217, 221, 15); playTone(1976, 221, 15); playTone(1661, 221, 15);
          playTone(1480, 221, 15); playTone(1319, 221, 15); playTone(1319, 221, (15 + 236));
          playTone(1319, 221, 15); playTone(1319, 221, 15); playTone(1319, 221, 15);
          playTone(1319, 221, 15); playTone(1109, 221, 15); playTone(1109, 221, 15);
          playTone(988, 221, 15);  playTone(988, 221, (15 + 236));
          playTone(2217, 221, 15); playTone(1976, 221, 15); playTone(1661, 221, 15);
          break;
          
        case 20: playTone(2250, 50); break;                                           // UI button clicked
        case 21: for (int i = 0; i < 2; i++) { playTone(2250,  50,  50); }   break;   // save
        case 22: for (int i = 0; i < 2; i++) { playTone(2500,  50,  50); }   break;   // firmware update start
        case 23: for (int i = 0; i < 3; i++) { playTone(2500,  50,  50); }   break;   // firmware update finish
        case 24: for (int i = 0; i < 2; i++) { playTone(1000, 100, 100); }   break;   // firmware update error
        case 31: for (int i = 0; i < 5; i++) { playTone(1750,  50,  50); }   break;   // announcement
      }
    }

    #ifdef ESP32
      ledcDetachPin(PIN_BUZZER);
    #endif
    pinMode(PIN_BUZZER, INPUT);                   // make sure the buzzer is silent ;-)
  }
#endif

//
// BME280
//

#ifdef ENABLE_BME280
  void readBME280() {
    BME280_temperature = BME280.readTemperature();                        // get temperature in degree Celsius
    
    if (BME280_fahrenheid) {                                              // convert to Fahrenheid if necessary
      BME280_temperature = (BME280_temperature * 9 / 5 + 32);
    }
      
    BME280_humidity = BME280.readHumidity();                              // get humidity in rH%
    BME280_pressure = BME280.readPressure() / 100;                        // get pressure in Pa   

    #ifdef BME280_SEA_LEVEL_PRESSURE
      BME280_altitude = BME280.readAltitude(BME280_SEA_LEVEL_PRESSURE);   // get altitude in mtr. BME280_SEA_LEVEL_PRESSURE has to be set correctly to get a accurate reading    
    #endif

    BME280_temperature *= BME280_temperatureCalFactor;
    BME280_temperature += BME280_temperatureCalValue;
    BME280_humidity    *= BME280_humidityCalFactor;
    BME280_humidity    += BME280_humidityCalValue;
    BME280_pressure    *= BME280_pressureCalFactor;
    BME280_pressure    += BME280_pressureCalValue;
  }
#endif
