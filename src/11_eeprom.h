/*
***************************************************************************
  ebc_alarmclock - eeprom functions
***************************************************************************
  last update 20201112 by ericBcreator
  
  note: eeprom structure:
  0   UI_sw_EEPROM
  +   eepromInit
  +   alarmList[x]  array of alarms, x = 0 to maxAlarms (set by code)
***************************************************************************
*/

void eepromSetVariables() {
  eepromInit.idStart = '$';
  eepromInit.displayInfo = displayInfo;
  eepromInit.displayInfoInterval = displayInfoInterval;
  eepromInit.suspendMode = suspendMode;
  eepromInit.sleepMode = sleepMode;
  eepromInit.sleepDelay = sleepDelay;
  eepromInit.displayDaylight = displayDaylight;
  eepromInit.ledBrightness = ledBrightness;
  eepromInit.ledBrightnessManualSet = ledBrightnessManualSet;
  eepromInit.ampmMode = ampmMode;
  eepromInit.timeDisplayMode = timeDisplayMode;
  eepromInit.fileSystemInitialized = fileSystemInitialized;
  eepromInit.versionChecksum = versionChecksum;
  eepromInit.neoBacklight = neoBacklight;
  eepromInit.timeZone = timeZone;
  eepromInit.DSTmode = DSTmode;
  eepromInit.BME280_display = BME280_display;
  eepromInit.BME280_fahrenheid = BME280_fahrenheid;
  eepromInit.BME280_tempTimeSwap = BME280_tempTimeSwap;
  eepromInit.idEnd = '*';
}

void eepromInitialize(byte initType) {                    // 0 is all, 1 is only alarms  
  int startAddress = sizeof(UI_sw_EEPROM) + 1;
  int bufSize = eepromBufSize - startAddress;

  if (initType == 1) {
    startAddress += sizeof(eepromInit);
    bufSize -= sizeof(eepromInit);
  }

  EEPROM.begin(bufSize);
  for (int i = 0 ; i < bufSize; i++)
    EEPROM.write(startAddress + i, 0);

  if (initType == 0) {
    initCustomSettings();
    eepromSetVariables();  
    EEPROM.put(startAddress, eepromInit);  
  }
  
  EEPROM.commit();    
  
  DEBUGPRINT("EEPROM initialized");

  if (initType == 1)
    DEBUGPRINTLN(" - only alarms");
  else
    DEBUGPRINTLN();
}

void eepromRead() {
  int startAddress = sizeof(UI_sw_EEPROM) + 1;
  int address = 0;
  
  EEPROM.begin((eepromBufSize - startAddress));
  EEPROM.get(startAddress + address, eepromInit);

  if (eepromInit.idStart == '$' && eepromInit.idEnd == '*') {   // if first char is $ and last is * assume it is initialized
    address = sizeof(eepromInit);
    
    displayInfo = eepromInit.displayInfo;
    displayInfoInterval = eepromInit.displayInfoInterval;
    suspendMode = eepromInit.suspendMode;
    sleepMode = eepromInit.sleepMode;
    sleepDelay = eepromInit.sleepDelay;
    displayDaylight = eepromInit.displayDaylight;
    ledBrightness = eepromInit.ledBrightness;
    ledBrightnessManualSet = eepromInit.ledBrightnessManualSet;
    ampmMode = eepromInit.ampmMode;
    timeDisplayMode = eepromInit.timeDisplayMode;
    fileSystemInitialized = eepromInit.fileSystemInitialized;
    versionChecksum = eepromInit.versionChecksum;
    neoBacklight = eepromInit.neoBacklight;    
    timeZone = eepromInit.timeZone;
    DSTmode = eepromInit.DSTmode;
    BME280_display = eepromInit.BME280_display;
    BME280_fahrenheid = eepromInit.BME280_fahrenheid;
    BME280_tempTimeSwap = eepromInit.BME280_tempTimeSwap;
  
    for (int i = 0; i < maxAlarms; i++) {      
      EEPROM.get(startAddress + address, alarmList[i]);
      address += sizeof(alarmList[0]);
    }

    matrix.setIntensity(ledBrightness);

    DEBUGPRINTLN();
    DEBUGPRINTLN("**EEPROM read");
    DEBUGPRINTLN("  Version checksum       " + (String) versionChecksum);
    DEBUGPRINTLN("  UI file initialized    " + (String) fileSystemInitialized);
    DEBUGPRINTLN("  Timezone               " + (String) timeZone);
    DEBUGPRINTLN("  DST mode               " + (String) DSTmode);
    DEBUGPRINTLN("  Time display mode      " + (String) timeDisplayMode);
    DEBUGPRINTLN("  AM/PM mode             " + (String) (ampmMode               ? msgOn : msgOff));    
    DEBUGPRINTLN("  Display info           " + (String) (displayInfo            ? msgOn : msgOff));
    DEBUGPRINTLN("  Display info interval  " + (String) displayInfoInterval);
    DEBUGPRINTLN("  Daylight info          " + (String) (displayDaylight        ? msgOn : msgOff));
    DEBUGPRINTLN("  BME280 info            " + (String) (BME280_display         ? msgOn : msgOff));
    DEBUGPRINTLN("  BME280 fahrenheid      " + (String) (BME280_fahrenheid      ? msgOn : msgOff));
    DEBUGPRINTLN("  BME280 time/temp swap  " + (String) (BME280_tempTimeSwap    ? msgOn : msgOff));
    DEBUGPRINTLN("  Suspend mode           " + (String) (suspendMode            ? msgOn : msgOff));
    DEBUGPRINTLN("  Sleep mode             " + (String) (sleepMode              ? msgOn : msgOff));
    DEBUGPRINTLN("  Sleep delay            " + (String) sleepDelay);
    DEBUGPRINTLN("  Brightness             " + (String) ledBrightness);
    DEBUGPRINTLN("  Brightness manual set  " + (String) (ledBrightnessManualSet ? msgOn : msgOff));    
    DEBUGPRINTLN("  Neopixel backlight     " + (String) neoBacklight);
    DEBUGPRINTLN("**");
    
  } else {                                                       // if not, initialize
    DEBUGPRINTLN();
    DEBUGPRINTLN("EEPROM identifiers failed " + (String) eepromInit.idStart + " " + (String) eepromInit.idEnd + ", initializing");
    eepromInitialize(0);
  }
}

void eepromWriteInitAndAlarms() {
  int startAddress = sizeof(UI_sw_EEPROM) + 1;
  int address = 0;
  eepromSetVariables();
  EEPROM.put(startAddress + address, eepromInit);
  
  address = sizeof(eepromInit);
  for (int i = 0; i < maxAlarms; i++) {
    EEPROM.put(startAddress + address, alarmList[i]);
    address += sizeof(alarmList[0]);      
  }
}

void eepromWrite() {
  #ifdef DEBUG_NO_WRITE_ALARMS_TO_EEPROM
    DEBUGPRINTLN("EEPROM writing skipped");
    eepromSavedTime = millis();
    return;
  #endif

  matrix.drawPixel(31, 6, HIGH);
  int startAddress = sizeof(UI_sw_EEPROM) + 1;
  EEPROM.begin((eepromBufSize - startAddress));
  eepromWriteInitAndAlarms();  
  EEPROM.commit();

  eepromValChanged = false;
  eepromSavedTime = millis();

  matrix.drawPixel(31, 6, LOW);
  DEBUGPRINTLN("EEPROM written");  
  //playBuzzer(21);
}

void eepromChanged() {
  eepromValChanged = true;
  eepromSavedTime = millis();
}