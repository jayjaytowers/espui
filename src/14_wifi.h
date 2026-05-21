/*
***************************************************************************
  ebc_alarmclock - wifi setup and timeserver 
***************************************************************************
  last update 20201130 by ericBcreator
***************************************************************************
*/

void addChipidToUI_SSID() {
//  Serial.printf(" ESP32 Chip ID = %04X",(uint16_t)(chipid>>32));  // print High 2 bytes
//  Serial.printf("%08X  ",(uint32_t)chipid);                       // print Low 4 bytes.

  byte startPos = strlen(UI_ssid);
  if (startPos > (21 - 1 - 5))
    startPos = 21 - 1 - 5;

  #ifdef ESP32
    String idLow4 = String((uint16_t) ESP.getEfuseMac(), HEX);
  #else
    String idLow4 = String((uint16_t) ESP.getChipId(), HEX);
  #endif
  idLow4.toUpperCase();  

  UI_ssid_ca[startPos] = '-';
  for (int i = 0; i < 5; i++)
    UI_ssid_ca[startPos + 1 + i] = idLow4[i];

  DEBUGPRINT("Web interface SSID set to ");
  DEBUGPRINTLN(UI_ssid_ca);
}

void setupWifiConnection() {
  unsigned long wifiStartTime = millis();
  bool connectStat = true;
  matrix.drawChar(3,  yOffset, 'W', HIGH, LOW, 1);
  matrix.drawChar(8,  yOffset, 'i', HIGH, LOW, 1);
  matrix.drawChar(12, yOffset, 'f', HIGH, LOW, 1);
  matrix.drawChar(17, yOffset, 'i', HIGH, LOW, 1);
  matrix.write();

  DEBUGPRINT("Connecting to Wifi ");

  #ifndef NO_WIFI
    WiFi.mode(WIFI_AP_STA);                 // @EB-todo

    #ifdef ESP32
      WiFi.setHostname(UI_ssid_ca);
    #else
      WiFi.hostname(UI_ssid_ca);
    #endif
    DEBUGPRINT("Hostname set to ");
    DEBUGPRINTLN(UI_ssid);

    WiFi.begin(WN_ssid, WN_password);
  
    while (WiFi.status() != WL_CONNECTED && ((millis() - wifiStartTime) < (wifiTimeout * 1000))) {
      reSwitch = readRotEncSwitch();      
      if (reSwitch == 2) {
        DEBUGPRINTLN("* switch pressed, not connecting *");
        return;
      }  

      displayChar(23, fontCharWifi, (connectStat ? HIGH : LOW), (connectStat ? false : true));
      matrix.write();
      connectStat = !connectStat;
      Serial.print(">");
      delay(500);
    }
  
    if (WiFi.status() == WL_CONNECTED) {  
      wifiConnected = true;
      lanIP = WiFi.localIP();
      DEBUGPRINT("connected to Wifi at ");
      DEBUGPRINTLN(lanIP);

      displayChar(23, fontCharWifi, HIGH, false);
      
      #ifndef DEBUG_FAST_START
        while ((millis() - wifiStartTime) < startupAnimDelay) { delay(2); }         // display the wifi logo a min time
      #endif
      
    } else {    
      DEBUGPRINTLN(msgConnWifiFailed);

      #ifndef DEBUG_FAST_START
        displayMessage(msgConnWifiFailed);
      #endif
    }
  #endif
}

void getTimeFromServer() {
  // void configTime(int timezone, int daylightOffset_sec, const char* server1, const char* server2, const char* server3)
  DEBUGPRINT("Connecting to a timeserver ");  
  configTime(timeZone * 3600, (abs(DST) * 3600) + timeOffsetSec, timeserver1, timeserver2, timeserver3);
  gtfsWaitForTime();
  checkDST();
  configTime(timeZone * 3600, (abs(DST) * 3600) + timeOffsetSec, timeserver1, timeserver2, timeserver3);

  if (!timeServerRead)
    return;

  String tmpMsg = msgTimeSynced;
  if (DST == false) 
    tmpMsg = msgDSToff;
  else 
    tmpMsg = msgDSTon;    
  if (DSTmode == 2) 
    tmpMsg += " (auto)";

  #ifndef DEBUG_FAST_START
    if (UI_displayTimeRead) {
      displayMessage(tmpMsg);
      UI_displayTimeRead = false;
    }
  #endif

  timeManualSet = false;
  refreshTimeDisplay = true;

  DEBUGPRINTLN(tmpMsg);
}

void gtfsWaitForTime() {
  static bool firstCall = true;
  unsigned long tsStartTime = millis();
  unsigned int clockAnimation = 0;
  int startColumn = 0;  
  currentTime = 0;

  if (LED_NUM_OF_HOR_DISPLAYS >= 8)
    startColumn = 32;

  time(&currentTime);
  if (currentTime < 30000 || firstCall) {          // only display when it takes more 'time' ;-) and at first boot    
    if (!firstCall || (firstCall && LED_NUM_OF_HOR_DISPLAYS < 8))
      matrix.fillScreen(LOW);
      
    matrix.drawChar((startColumn + 1),  yOffset, 'T', HIGH, LOW, 1);
    matrix.drawChar((startColumn + 6),  yOffset, 'i', HIGH, LOW, 1);
    matrix.drawChar((startColumn + 11), yOffset, 'm', HIGH, LOW, 1);
    matrix.drawChar((startColumn + 17), yOffset, 'e', HIGH, LOW, 1);
    matrix.write();
    
    while((currentTime < 30000 || firstCall) && ((millis() - tsStartTime) < (timeServerTimeout * 1000))) {
      reSwitch = readRotEncSwitch();      
      if (reSwitch > 0) {
        DEBUGPRINTLN("* switch pressed, aborting *");
        return;
      }  
  
      time(&currentTime);
      DEBUGPRINT(".");
      displayChar((startColumn + 24), fontCharClock[clockAnimation], HIGH, false);
      matrix.write();
      clockAnimation++;
      clockAnimation = clockAnimation % 4;
      delay(250);

      #ifdef OTA_UPDATE
        ArduinoOTA.handle();
      #endif
    }

    #ifndef DEBUG_FAST_START
      if (firstCall) {
        while ((millis() - tsStartTime) < startupAnimDelay) { delay(2); }         // display the time logo a min time
      }
    #endif
    
    firstCall = false;
    matrix.fillScreen(LOW);
  }  

  if (currentTime < 30000) {
    DEBUGPRINTLN(msgConnTSfailed);

    #ifndef DEBUG_FAST_START
      displayMessage(msgConnTSfailed);
    #endif
  } else {
    timeServerRead = true;
    DEBUGPRINT("time read from server: ");
    DEBUGPRINTLN(currentTime);
  }
}
