/*
***************************************************************************
  ebc_alarmclock - wifi setup with user interface
***************************************************************************
  last update 20210319 by ericBcreator
***************************************************************************
*/

uint16_t UI_ID_sw_info;
uint16_t UI_ID_sw_ssid;
uint16_t UI_ID_sw_password;
uint16_t UI_ID_sw_saveButton;
uint16_t UI_ID_sw_restartButton;

unsigned long UI_sw_startTime;

const int maxNumOfSSIDs = 20;             // @EB-custom: max num of ssids
String ssids[maxNumOfSSIDs];
int numOfssids;

void UI_sw_setupWifi(char * UI_title) {
  UI_sw_scanSSIDs();
  
  WiFi.mode(WIFI_AP);                     // @EB-todo
  WiFi.softAPConfig(UI_apIP, UI_apIP, IPAddress(255, 255, 255, 0));
  #ifdef ESP32
    WiFi.setHostname(UI_ssid_ca);
  #else
    WiFi.hostname(UI_ssid_ca);
  #endif
  DEBUGPRINTLN(UI_ssid_ca);

  WiFi.softAP(UI_ssid, UI_password, UI_channel);
  DEBUGPRINTLN("Access point started");
  
  UI_dnsServer.start (UI_DNS_PORT, "*", UI_apIP);
  DEBUGPRINTLN("DNS server started");

  /// setup elements

  String UI_lb_sw_info = UI_sw_msgInfo0 + (String) (UI_sw_length - 1) + UI_sw_msgInfo1;  
  UI_ID_sw_info = ESPUI.addControl(ControlType::Label, UI_msg_sw_info, UI_lb_sw_info, ControlColor::Wetasphalt, Control::noParent);

  UI_ID_sw_ssid = ESPUI.addControl(ControlType::Select, UI_msg_sw_ssid, "", ControlColor::Peterriver, Control::noParent, &UI_sw_callSelect);
  for (int i = 0; i < numOfssids; i++) {
    charPtr = ssids[i].c_str();
    ESPUI.addControl(ControlType::Option, charPtr, (String) i, ControlColor::Peterriver, UI_ID_sw_ssid);
  }

  if (numOfssids)   // set ssid to first selection
    ssids[0].toCharArray(UI_sw_EEPROM.ssid, UI_sw_length);
    
  UI_ID_sw_password = ESPUI.addControl(ControlType::Text, UI_msg_sw_password, UI_sw_EEPROM.password, ControlColor::Peterriver, Control::noParent, &UI_sw_callText);
  UI_ID_sw_saveButton = ESPUI.addControl(ControlType::Button, UI_msg_sw_saveButton, UI_msg_sw_saveButton, ControlColor::Alizarin, Control::noParent, &UI_sw_callButton);
  UI_ID_sw_restartButton = ESPUI.addControl(ControlType::Button, UI_msg_sw_restartButton, UI_msg_sw_restartButton, ControlColor::Alizarin, Control::noParent, &UI_sw_callButton);

  ///

  UI_dnsServer.start(UI_DNS_PORT, "*", UI_apIP);
  ESPUI.begin(UI_title);
  UI_sw_updatePassword();   // mark with asterisks
  UI_sw_startTime = millis();

  while(1) {                // stay in loop until save is clicked or after a timeout
    UI_dnsServer.processNextRequest();

    if (wifiSetupTimeout) {
      if (((millis() - UI_sw_startTime) / 1000) >= wifiSetupTimeout) {
	    DEBUGPRINTLN("Wifi setup timeout, restarting");
        ESP.restart();
	    }
    }
    
    delay(40);                        // a little delay

    reSwitch = readRotEncSwitch();    // if the encoder switch is long pressed: restart
    if (reSwitch == 2) {
      DEBUGPRINTLN("Rotary encoder switch long press, restarting");
      ESP.restart();
    }
  }
}

void UI_sw_callSelect(Control *sender, int type) {
  UI_sw_startTime = millis();
  if (sender->id == UI_ID_sw_ssid) {
    int value = sender->value.toInt();
    ssids[value].toCharArray(UI_sw_EEPROM.ssid, UI_sw_length);
  }
}

void UI_sw_callText(Control *sender, int type) {
  UI_sw_startTime = millis();
  if (sender->id == UI_ID_sw_password) {
    sender->value.toCharArray(UI_sw_EEPROM.password, UI_sw_length);
    UI_sw_updatePassword();
  }
}

void UI_sw_updatePassword() {
  UI_sw_startTime = millis();
  ESPUI.updateText(UI_ID_sw_password, UI_sw_fillAsterisk(UI_sw_EEPROM.password, (UI_sw_length) - 1));
}

String UI_sw_fillAsterisk(String value, byte valLength) {
  String tmpStr = "";
  while (tmpStr.length() < value.length())
    tmpStr += "*"; 
  return String(tmpStr);
}

void UI_sw_callButton(Control *sender, int type) {
  UI_sw_startTime = millis();
  if (type == B_UP) {
    if (sender->id == UI_ID_sw_saveButton) {
      UI_sw_EEPROM.idStart = '$';
      UI_sw_EEPROM.idEnd = '*';
      UI_sw_writeEEPROM();

//        ESPUI.print(UI_ID_sw_info, UI_sw_msgRestarting);
//        UI_dnsServer.processNextRequest();    
//        displayMessageOutLoop(0, UI_sw_msgRestarting, false);  // @EB-todo
      ESP.restart();
    } else if (sender->id == UI_ID_sw_restartButton) {
      ESP.restart();
    }
  }
}

bool UI_sw_readEEPROM() {
  EEPROM.begin(sizeof(UI_sw_EEPROM));
  EEPROM.get(0, UI_sw_EEPROM);

  if (UI_sw_EEPROM.idStart == '$' && UI_sw_EEPROM.idEnd == '*') {
    return true;
  } else {
    DEBUGPRINTLN("Wifi setup EEPROM identifiers failed " + (String) UI_sw_EEPROM.idStart + " " + (String) UI_sw_EEPROM.idEnd + ", initializing");
    UI_sw_resetEEPROM();
    return false;
  }
}

void UI_sw_resetEEPROM() {
  UI_sw_EEPROM.idStart = ' ';
  for (int i = 0; i < UI_sw_length; i++) {
    UI_sw_EEPROM.ssid[i] = 0;
    UI_sw_EEPROM.password[i] = 0;
  }
  UI_sw_EEPROM.idEnd = ' ';
  UI_sw_writeEEPROM();
}

void UI_sw_writeEEPROM() {  
  EEPROM.begin(eepromBufSize);
  EEPROM.put(0, UI_sw_EEPROM);
  eepromWriteInitAndAlarms();     // @EB-todo needed to keep the init and alarms settings otherwise those settings will be erased!
  EEPROM.commit();

  DEBUGPRINT("EEPROM written:, SSID: " + (String) UI_sw_EEPROM.ssid);
  DEBUGPRINTLN(" password: " + (String) UI_sw_EEPROM.password);
  
  playBuzzer(21);
}

void UI_sw_scanSSIDs() {
  DEBUGPRINT("Scanning for networks...");
  numOfssids = 0;
    
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);
  
  int foundSSIDs = WiFi.scanNetworks();
  DEBUGPRINTLN("found " + (String) foundSSIDs + " networks");

  if (foundSSIDs) {
    for (int i = 0; i < foundSSIDs; ++i) {

//      if ((i < maxNumOfSSIDs) && (WiFi.SSID(i).length() < WIFI_CRED_LENGTH)) { // @EB-todo. NOTE: if skipping ssids, ssids[i] won't match anymore!!

      if ((i < maxNumOfSSIDs)) {
        numOfssids++;
        ssids[numOfssids - 1] = WiFi.SSID(i);
      
//      Serial.print(WiFi.RSSI(i));
//      Serial.println((WiFi.encryptionType(i) == ENC_TYPE_NONE) ? " " : "*");
      }
    }
  }
}
