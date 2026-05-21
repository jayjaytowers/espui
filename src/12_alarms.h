/*
***************************************************************************
  ebc_alarmclock - alarm functions
***************************************************************************
  last update 20210319 by ericBcreator
***************************************************************************
*/

//
// functions
//

bool checkRecur(int i) {
  if (currentTime >= alarmRecNext[i])
    return true;
  else
    return false;
}

bool checkNonRecur(int i) {
  static int prevAlarm = 99;
  static unsigned long prevTrigTime;
  bool alarmToTrigger = false;
    
  if (timeData.tm_hour == alarmList[i].hour && timeData.tm_min == alarmList[i].min && timeData.tm_sec == alarmList[i].sec) {   // hour, min and sec match
    
    if (prevAlarm != i || (millis() - prevTrigTime) > 1000) {                               // prevent checking the same alarm at the same sec
      
      alarmToTrigger = true;
      prevAlarm = i;
      prevTrigTime = millis();
      
      if (alarmList[i].month > 0 && timeData.tm_mon != (alarmList[i].month - 1)) {          // month set, no match. Note: timeDate.tm_mon ranges 0 to 11 !!
        alarmToTrigger = false;
        DEBUGPRINTLN("Alarm " + fillSpace(i, 2) + "   no match for the month");
      }
    
      if (alarmList[i].day > 0 && timeData.tm_mday != alarmList[i].day) {                   // day set, no match
        alarmToTrigger = false;
        DEBUGPRINTLN("Alarm " + fillSpace(i, 2) + "   no match for the day");
      }
    
      if (alarmList[i].weekDay > 0 && timeData.tm_wday != (alarmList[i].weekDay - 1)) {     // weekday set, no match
        alarmToTrigger = false;
        DEBUGPRINTLN("Alarm " + fillSpace(i, 2) + "   no match for the day of the week");
      }

      if (alarmList[i].weekDays && alarmList[i].weekendDays) {                              // if both weekDays and weekendDays are set then don't check each
        DEBUGPRINTLN("Alarm " + fillSpace(i, 2) + "   triggers week and weekend days");
      } else {
        if (alarmList[i].weekDays)
          if (timeData.tm_wday == 0 || timeData.tm_wday == 6) {                             // weekdays set, no match
            alarmToTrigger = false;
            DEBUGPRINTLN("Alarm " + fillSpace(i, 2) + "   no match for weekdays");
          }
        
        if (alarmList[i].weekendDays)
          if (timeData.tm_wday != 0 && timeData.tm_wday != 6) {                             // weekenddays set, no match
            alarmToTrigger = false;
            DEBUGPRINTLN("Alarm " + fillSpace(i, 2) + "   no match for weekenddays");
          }
      }
    }
  }

  return alarmToTrigger;
}

void saveRecNext(int i) {  
  switch (alarmList[i].recurringType) {
    case 1: // sec
      alarmRecNext[i] = currentTime + alarmList[i].recurringInterval; 
      break;
      
    case 2: // min
      alarmRecNext[i] = currentTime + (alarmList[i].recurringInterval * 60);
      break;
      
    case 3: // hour
      alarmRecNext[i] = currentTime + (alarmList[i].recurringInterval * 3600); 
      break;
      
    case 4: // day
      alarmRecNext[i] = currentTime + (alarmList[i].recurringInterval * 24 * 3600); 
      break;
      
    case 5: // month
      byte daysInMonth = 0;
      byte month = timeData.tm_mon + 1;     // convert 0-11 to 1-12
      alarmRecNext[i] = currentTime;
            
      for (int j = 0; j < alarmList[i].recurringInterval; j++) {
        daysInMonth = getDaysInMonth(timeData.tm_year, month);
        alarmRecNext[i] += (daysInMonth * 24 * 3600);
        month++;
        if (month > 12)
          month = 1;
      }
      break;
  }

  DEBUGPRINT("Alarm " + fillSpace(i, 2) + "   current time " + timeString);
  DEBUGPRINTLN(", recurring at "+ (String) alarmRecNext[i]);
}

void checkRecurSkipped () {
  for (int i = 0; i < maxAlarms; i++) {      
    if (alarmList[i].active && alarmList[i].recurringType > 0 && alarmList[i].recurringInterval > 0) {
      if (alarmRecTriggered[i]) {
        if (checkRecur(i))
          saveRecNext(i);
      } else if (checkNonRecur(i)) {
        saveRecNext(i);
        alarmRecTriggered[i] = true;
      }
    }
  }
}

void alarmInterrupted() {
  alarmIsInterrupted = true;
  alarmEnded();
  alarmIsInterrupted = false;
  DEBUGPRINTLN("Alarm " + fillSpace(alarmNum, 2) + "   interrupted at " + timeString);
  displayMessage(msgAlarmOff);
}

void alarmEnded() {
  #ifndef NO_BUZZER
    #ifdef ESP32
      ledcWrite(0, LOW);
      delayAndCheckEnc(50);
      ledcDetachPin(PIN_BUZZER);
    #else
      noTone(PIN_BUZZER);
    #endif
    pinMode(PIN_BUZZER, INPUT);
  #endif

  displayClock(false);
    
  alarmTriggered = false;
  alarmMessage = false;
  alarmRepeat = false;

  if (alarmList[alarmNum].bright)                 // if set, reset led brightness
    matrix.setIntensity(ledBrightness);

  if (alarmList[alarmNum].duration > 0 || alarmIsInterrupted) {    // if duration is > 0 or the alarm is interrupted, reset gpio or neopixels otherwise keep it in the current state
    switch (alarmList[alarmNum].actionType) {     // reset the GPIO pin or turn the neopixels off
      case actionGPIOhigh:
        digitalWrite(alarmList[alarmNum].actionNum, LOW);
        DEBUGPRINTLN("           Pin " + (String) alarmList[alarmNum].actionNum + " set low");
        break;
      case actionGPIOlow:
        digitalWrite(alarmList[alarmNum].actionNum, HIGH);
        DEBUGPRINTLN("           Pin " + (String) alarmList[alarmNum].actionNum + " set high");
        break;
  
      #ifdef ENABLE_NEOPIXELS
        case actionNeoWUL:
        case actionNeoWULchain:
        case actionNeoOn:
          neopixelsOff();
          DEBUGPRINTLN("           Neopixels off");
          neopixelsPresetOn(neoBacklight);
          break;
      #endif
    }
  }
  
  if (alarmList[alarmNum].actionType)       // play the buzzer again at the end
    playBuzzer(alarmList[alarmNum].buzzer);

  if (sleepMode)
    sleepStartTime = millis();
  readCurrentTime();
  
  displayInfoStartTime = millis();
  
  if (!alarmIsInterrupted) {                // neopixel wakeup light chain next alarm, only chain when the alarm is not interrupted
    if (alarmList[alarmNum].actionType == actionNeoWULchain) {
      forceFirstAlarm = true;
      alarmNum++; 
      if (alarmNum == maxAlarms)
        alarmNum = 0;
      DEBUGPRINTLN("           Chaining next alarm " + (String) alarmNum);
    }
  }
}

void initActions() {  
  for (int i = 0; i < maxAlarms; i++)  
    initAction(i);  
}

void initAction(int value) {    
  if (alarmList[value].active || forceFirstAlarm) {
    if (alarmList[value].actionType == actionGPIOhigh || alarmList[value].actionType == actionGPIOlow || forceFirstAlarm) {
      DEBUGPRINTLN("Init action " + (String) alarmList[value].actionNum);
      switch (alarmList[value].actionNum) {
        case PIN_CS: case PIN_BUZZER: case RE_PINA: case RE_PINB: case RE_SWITCH: case PIN_LS: 
          #ifdef ENABLE_PIR
            case PIN_PIR: 
          #endif
          #ifdef ENABLE_NEOPIXELS
            case PIN_NEOPIXELS:
          #endif
        
          DEBUGPRINTLN("** Pin " + fillZero(alarmList[value].actionNum) + " in use by one of the defined pins, skipped");
          break;
          
        default:  
          pinMode(alarmList[value].actionNum, OUTPUT);
          DEBUGPRINTLN("Pin " + fillZero(alarmList[value].actionNum) + "     set to output");
          
          switch (alarmList[value].actionType) {
            case actionGPIOhigh:
              digitalWrite(alarmList[value].actionNum, LOW);
              DEBUGPRINTLN("Pin " + fillZero(alarmList[value].actionNum) + "     set low");
              break;
            case actionGPIOlow:
              digitalWrite(alarmList[value].actionNum, HIGH);
              DEBUGPRINTLN("Pin " + fillZero(alarmList[value].actionNum) + "     set high");
              break;
          }
      }
    }
  }
}

void checkAlarms() {
  static unsigned long alarmTriggerTime = 0;
  static unsigned long prevBuzzerTime = 0;
  static unsigned long prevBlinkTime = 0;
  static bool alarmBlink = false;
  bool alarmToTrigger = false;
  bool firstAlarmSet = false;

  // if the alarm is not already triggered, check if an alarm is ready to trigger
  if (!alarmTriggered) {                          // check for a newly activated alarm when the alarm is not already triggered 
    if (!forceFirstAlarm) {                       // and not forced to trigger
      for (int i = 0; i < maxAlarms; i++) {
        if (alarmList[i].active) {                // only check active alarms                      
          if (alarmList[i].recurringType > 0 && alarmList[i].recurringInterval > 0) {
            if (alarmRecTriggered[i]) {           // recurring: 1st trigger normal, then check for recurring
              alarmToTrigger = checkRecur(i);
              if (alarmToTrigger)
                alarmRepeat = true;
            } else 
              alarmToTrigger = checkNonRecur(i);
              
            if (alarmToTrigger) {
              saveRecNext(i);
              alarmRecTriggered[i] = true;
            }
            
          } else 
            alarmToTrigger = checkNonRecur(i);
  
          if (alarmToTrigger) {
            if (alarmSuspended || suspendMode)    // do not trigger if in suspendMode or alarmSuspended (active when using the encoder)
              DEBUGPRINTLN("Alarm " + fillSpace(i, 2) + "   suspended at " + timeString);
            else {
              if (!firstAlarmSet) {               // loop through all alarms but only respond to the 1st triggered
                alarmNum = i;
                firstAlarmSet = true;
              }
            }
          }
        }
      }
    }

    if (forceFirstAlarm) {
      if (alarmList[alarmNum].duration == 666 || alarmList[alarmNum].duration == 777) {
        displayTime();
        displayAnimPacMan();
        if (alarmList[alarmNum].duration == 666)
          delayAndCheckEnc(250);
        displayAnimPacMan(true);
        
        if (alarmList[alarmNum].duration == 666)
          forceFirstAlarm = false;
        else
          return;
      }
    }

    if (firstAlarmSet || forceFirstAlarm) {     // 1st alarm activated or forced      
      if (sleepMode) {
        if (sleeping)
          endSleepMode(true);
        else
          sleepStartTime = millis();
      }

      initAction(alarmNum);

      alarmTriggered = true;
      alarmTriggerTime = millis();
      alarmBlink = false;
      forceFirstAlarm = false;
      displayInfoStartTime = millis();

      displayClock(false);

      if (alarmRepeat)
        DEBUGPRINT("Recurring: ");

      readCurrentTime();
      DEBUGPRINTLN("Alarm " + fillSpace(alarmNum,2) + "   started at " + timeString);

      if (alarmList[alarmNum].bright)               // if set, turn leds bright
        matrix.setIntensity(ledBrightnessAlarm);

      switch (alarmList[alarmNum].actionType) {     
        case actionGPIOhigh:                        // set GPIO pin high
          digitalWrite(alarmList[alarmNum].actionNum, HIGH);
          DEBUGPRINTLN("           Pin " + (String) alarmList[alarmNum].actionNum + " set high");
          break;
        case actionGPIOlow:                         // set GPIO pin low
          digitalWrite(alarmList[alarmNum].actionNum, LOW);
          DEBUGPRINTLN("           Pin " + (String) alarmList[alarmNum].actionNum + " set low");
          break;

        #ifdef ENABLE_NEOPIXELS
          case actionNeoWUL:
          case actionNeoWULchain:
            neopixelsFade(0, true);                 // 1st call neopixel wake up light
            DEBUGPRINTLN("           Neopixels wake up light started");
            break;

          case actionNeoOn:
            neopixelsPresetOn(alarmList[alarmNum].actionNum);  // neopixels on, actionNum refers to neoPalette
            DEBUGPRINTLN("           Neopixels on: palette " + (String) alarmList[alarmNum].actionNum);
            break;

          case actionNeoOff:
            neopixelsOff();                         // neopixels off
            DEBUGPRINTLN("           Neopixels off");
            break;
            
        #endif
      }

      // for gpio action types only play the buzzer at the start
      if (alarmList[alarmNum].actionType == actionGPIOhigh || alarmList[alarmNum].actionType == actionGPIOlow)
        playBuzzer(alarmList[alarmNum].buzzer);
      
      if (strlen(alarmList[alarmNum].message) > 0)
        alarmMessage = true;      
    }
  }

  // alarm triggered loop
    
  if (alarmTriggered) {                                               // loop code
    if (alarmList[alarmNum].duration == 666) {
      displayTime();
      displayAnimPacMan();
      UI_alarmToInterrupt = true;
    }
    
    if (!alarmMessage && alarmList[alarmNum].fx == 1) {
      if ((millis() - prevBlinkTime) >= 500) {                        // blink every 500 ms
        alarmBlink = !alarmBlink;
        prevBlinkTime = millis();

        if (alarmBlink) {          
          displayTime();
		  
    		  if (alarmList[alarmNum].buzzer)                             // sync the buzzer with the blinking
    		    prevBuzzerTime = 0;

          #ifdef ENABLE_NEOPIXELS                                     // blink neopixels if set
            if (alarmList[alarmNum].actionType == actionNeoOn)
              neopixelsPresetOn(alarmList[alarmNum].actionNum);
          #endif
        } else {
          matrix.fillScreen(LOW);
          matrix.write();

          #ifdef ENABLE_NEOPIXELS
            if (alarmList[alarmNum].actionType == actionNeoOn);
              neopixelsOff();
              neopixelsPresetOn(neoBacklight);
          #endif
        }
      }
    }

    if (alarmTriggered) {                                                                           // check if alarm is still triggered
      if (alarmList[alarmNum].actionType == 0 || alarmList[alarmNum].actionType == actionNeoOn) {   // don't play the buzzer in the loop for actions
        if (alarmList[alarmNum].buzzer > 0 && !alarmMessage) {                                      // play the buzzer every other sec when it is set
          if ((millis() - prevBuzzerTime) >= 2000) {                                                // and there is no message set
            prevBuzzerTime = millis();
            playBuzzer(alarmList[alarmNum].buzzer);
          }
        }
      }
    }

    #ifdef ENABLE_NEOPIXELS
      switch (alarmList[alarmNum].actionType) {
        case actionNeoWUL:
        case actionNeoWULchain:
          neopixelsFade(0, false); break;                       // loop neopixel wake up light
      }
    #endif

    if (alarmMessage)
      displayMessage((alarmRepeat ? msgRecurring : "") + fillZero(timeData.tm_hour) + ":" + fillZero(timeData.tm_min) + ":" + fillZero(timeData.tm_sec) + " " + alarmList[alarmNum].message);

    if ((millis() - alarmTriggerTime) >= (alarmList[alarmNum].duration * 1000)) {
      DEBUGPRINTLN("Alarm " + fillSpace(alarmNum, 2)+ "   ended   at " + timeString);
      alarmEnded();
    }

    if (UI_alarmToInterrupt) {
      alarmInterrupted();
      UI_alarmToInterrupt = false;
    }
  }
}