/*
***************************************************************************
  ebc_alarmclock - debug set alarms
***************************************************************************
  last update 20201129 by ericBcreator
***************************************************************************
*/

// hour 6 min 10 duration 30 buzzer 4 effect 1 bright

void debugPrintAlarmlist() {
  byte tmpMaxAlarms = ((eepromBufSize - sizeof(UI_sw_EEPROM) - sizeof(eepromInit)) / sizeof(alarmList[0]));
  DEBUGPRINTLN();
  DEBUGPRINT("EEPROM size " + (String) eepromBufSize + " bytes, init size " + (String) (sizeof(UI_sw_EEPROM) + sizeof(eepromInit)));
  DEBUGPRINT (", alarm element size " + (String) sizeof(alarmList[0]) + " bytes, max number of alarms " + (String) tmpMaxAlarms);
  DEBUGPRINTLN();

  for (int tmpAlarm = 0; tmpAlarm < maxAlarms; tmpAlarm++) {
    DEBUGPRINT("Alarm " + fillSpace((tmpAlarm + 1), 2) + "    ");
    DEBUGPRINT(alarmList[tmpAlarm].active ? "active  ": "inactive");
    DEBUGPRINT(" set at " + fillZero(alarmList[tmpAlarm].hour) + ":" + fillZero(alarmList[tmpAlarm].min) + ":" + fillZero(alarmList[tmpAlarm].sec) + " ");
    DEBUGPRINT(" month " + fillSpace(alarmList[tmpAlarm].month, 2) + " day " + fillSpace(alarmList[tmpAlarm].day, 2) + " weekday " + (String) alarmList[tmpAlarm].weekDay);
    DEBUGPRINT(" duration " + fillSpace(alarmList[tmpAlarm].duration, 5) + " ");
    DEBUGPRINT("recurring " + (String) alarmList[tmpAlarm].recurringType + " " + fillSpace(alarmList[tmpAlarm].recurringInterval,5) + " ");
    DEBUGPRINT("buzzer " + fillSpace(alarmList[tmpAlarm].buzzer, 2) + " ");
    DEBUGPRINT("fx " + (String) alarmList[tmpAlarm].fx + " ");
    DEBUGPRINT("action " + (String) alarmList[tmpAlarm].actionType + " " + fillSpace(alarmList[tmpAlarm].actionNum, 2) + " ");
    DEBUGPRINT(alarmList[tmpAlarm].weekDays ? "weekdays " : "");
    DEBUGPRINT(alarmList[tmpAlarm].weekendDays ? "weekenddays " : "");
    DEBUGPRINT(alarmList[tmpAlarm].bright ? "bright message: " : "message: ");
    DEBUGPRINT(alarmList[tmpAlarm].message);
    DEBUGPRINTLN();
  }
  DEBUGPRINTLN();
}

void debugSetAlarms() {
  for (int tmpAlarm = 0; tmpAlarm < 8; tmpAlarm++) {
    alarmList[tmpAlarm].hour = timeData.tm_hour;
    alarmList[tmpAlarm].min = timeData.tm_min;
    int tmpSec = timeData.tm_sec + (60+30 * (tmpAlarm + 1));
    debugValidateTime(alarmList[tmpAlarm].hour, alarmList[tmpAlarm].min, tmpSec);
    alarmList[tmpAlarm].sec = tmpSec;

    alarmList[tmpAlarm].duration = defaultAlarmDuration;
    alarmList[tmpAlarm].buzzer = 2;
    alarmList[tmpAlarm].active = true;
    String tmpMsg = "";
  
    switch (tmpAlarm) {		
      case 1 :
        alarmList[tmpAlarm].actionType = actionGPIOhigh;
        alarmList[tmpAlarm].actionNum = 12; // 2 equals LED_BUILTIN 12 equals D6
        alarmList[tmpAlarm].recurringType = 1;
        alarmList[tmpAlarm].recurringInterval = 40;
        alarmList[tmpAlarm].buzzer = 0;
        alarmList[tmpAlarm].duration = 10;
        tmpMsg = "GPIO D6";
        break;
        
      case 2 : 
        alarmList[tmpAlarm].recurringType = 2;
        alarmList[tmpAlarm].recurringInterval = 2;
        alarmList[tmpAlarm].weekendDays = true;
        tmpMsg = "recur 2 min weekend";
        break;
        
      case 3 :
        alarmList[tmpAlarm].recurringType = 1;
        alarmList[tmpAlarm].recurringInterval = 135;
        tmpMsg = "recur 135 sec";
        break;
        
      case 4 :
        alarmList[tmpAlarm].recurringType = 4;
        alarmList[tmpAlarm].recurringInterval = 4;
        tmpMsg = "recur 4 day";
        break;

      case 5 :
        alarmList[tmpAlarm].recurringType = 5;
        alarmList[tmpAlarm].recurringInterval = 3;
        tmpMsg = "recur 3 months";
        break;

      case 6:              
        alarmList[tmpAlarm].recurringType = 2;
        alarmList[tmpAlarm].recurringInterval = 2;
        alarmList[tmpAlarm].day = 6;
        tmpMsg = "Sat recur 2 hour";
        break;
        
      case 7:  
        alarmList[tmpAlarm].weekDay = 7;
        alarmList[tmpAlarm].weekDays = true;
        alarmList[tmpAlarm].recurringType = 1;
        alarmList[tmpAlarm].recurringInterval = 5;
        tmpMsg =  "Sat & weekdays recur 5 min";
        break;
    }

    if (tmpAlarm > 1) {
      tmpMsg = "al " + (String) tmpAlarm + " " + tmpMsg;
      tmpMsg.toCharArray(alarmList[tmpAlarm].message, (alarmMsgLength - 2));
    }
  } 

  alarmList[0].buzzer = 4;
  alarmList[0].fx = 1;
  alarmList[0].bright = true;
  alarmList[0].weekDays = false;
  alarmList[0].weekendDays = false;

#ifndef DEBUG_NO_WRITE_ALARMS_TO_EEPROM
  eepromInitialize(0);
  eepromWrite();
#endif
}

void debugValidateTime(byte &vtHour, byte &vtMin, int &vtSec) {
  while (vtSec > 59) {
    vtMin++;
    vtSec -= 60;
  }
  
  while (vtMin > 59) {
    vtHour = min(vtHour + 1, 23);
    vtMin -= 60;
  }
}
