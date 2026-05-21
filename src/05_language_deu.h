/*
***************************************************************************
  ebc_alarmclock - language settings - Deutsch
***************************************************************************
  last update 20210320 by ericBcreator
***************************************************************************
*/

//
// language settings
//

#ifndef _LANGUAGE
  #define _LANGUAGE "DEU"
  
  const String months[]                   = { "Jaenner", "Februar", "Maerz", "April", "Mai", "Juni", "Juli", "August", "September", "Oktober", "November", "Dezember" };
  const String days[]                     = { "Sonntag", "Montag", "Dienstag", "Mittwoch", "Donnerstag", "Freitag", "Samstag" };
  
  const char * msgAlarm0Active            = "Alarm 1 aktiv, um";
  const char * msgAlarm0Inactive          = "Alarm 1 inaktiv";
  const char * msgAlarm0Set               = "Alarm 1 um";
  const char * msgAlarmOff                = "Alarm aus";
  const char * msgConnTSfailed            = "* Verbindung zum Zeitserver fehlgeschlagen...";
  const char * msgConnWifiFailed          = "* Wifi-Verbindung fehlgeschlagen...";
  const char * msgDateSet                 = "Datum gesetzt auf";
  const char * msgDaylightTime            = "Tageslicht";
  const char * msgDSTon                   = "Sommerzeit ein";
  const char * msgDSToff                  = "Sommerzeit aus";
  const char * msgLedBrightnessSet        = "Helligkeit gesetzt auf";
  const char * msgMenu                    = "Menue";
  const char * msgNewVersion              = "Neue Firmware Version ";
  const char * msgNoWifi                  = "Keine Wifi-Verbindung";
  const char * msgNoWifiNoRE              = "Warnung: Wifi und Encoder deaktiviert";           // no Wifi and no encoder so unable to change any settings
  const char * msgOn                      = "EIN"; // 3 characters
  const char * msgOff                     = "AUS"; // 3 characters
  const char * msgOTAupdateStart          = ">> Starte Firmware-Update OTA";
  const char * msgOTAupdateEnd            = "<< Firmware aktualisiert, Neustart...";
  const char * msgOTAupdateError          = ">* Fehler Firmware-Update: ";
  const char * msgRecurring               = "Wiederholung: ";
  const char * msgSunrise                 = "Sonnenaufgang";
  const char * msgSunset                  = "Sonnenuntergang";
  const char * msgbmeTemperature          = "Temperatur";
  const char * msgbmeHumidity             = "Luftfeuchtigkeit";
  const char * msgbmePressure             = "Luftdruck";
  const char * msgTimeSet                 = "Zeit gestellt auf";
  const char * msgTimeSynced              = "Lese Zeit,";
  const char * msgUIfilesystemInit0       = "Bereite Dateisystem vor...";
  const char * msgUIfilesystemInit1       = "Dateisystem vorbereitet";
  const char * msgUIstart0                = "Webinterface gestartet. Verbinde mit SSID ";
  const char * msgUIstart1                = " und gehe zu ";
  const char * msgUIstart2                = " oder LAN ";
  const char * msgUIwifiSetup0            = "Starte WiFi Web-Setup. Verbinde mit SSID ";
  const char * msgUIwifiSetup1            = " und gehe zu ";
  const char * msgUIwifiReset             = "Wifi-Reset, Neustart...";
  const char * msgWakeupLight             = " - Aufwachlicht";
  const char * msgWakeupLightChain        = " - Aufwachlicht, verknuepfe mit Naechstem";
  
  // menu items
  const String menu[] {
    "Alarm 1 einstellen",
    "Led Helligkeit",
    "Neopixel Hintergrundfarbe",
    "Sync Zeit",
    "Schlafmodus",
    "Pausenmodus",
    "Dat.anzeige ein/aus",
    "Intervall Info-Anzeige",
    "Zeit einstellen",
    "Dat. einstellen",
    "Displaymodus",
  //  "UI minimal controls",  // @EB-todo disabled
    "Systeminfo",
    "Einstellungen speichern",
    "Neustart",
  #ifdef ANNOUNCE_MODE
    "Ankuendigungen",
  #endif
    "Menue beenden"
  };
  
  const String timeDisplayModes[] {
    "HH:MM",
    "HH:MM große Schrift",
    "HH MM ss",
    "HH MM am/pm",
    "hh mm ss",
    "hh mm:ss",
    #ifdef ENABLE_BME280
      "tmp hu"
    #endif
  };
  
  const char * msgMenuAlarm0              = "Alarmzeit 1 einstellen";
  const char * msgMenuAlarm0wul           = "Wecklicht";
  const char * msgMenuLedBrightness       = "LED Helligkeit einstellen, -1 fuer Auto (Sensor)";
  const char * msgMenuNeoBacklight        = "Neopixels Farbe einstellen";
  const char * msgMenuSyncTime            = "Sync mit Zeitserver";
  const char * msgMenuSleepMode           = "Schlafmodus einstellen";
  const char * msgMenuSuspendMode         = "Pausenmodus einstellen";
  const char * msgMenudisplayInfo         = "Datum anzeigen";
  const char * msgMenuDisplayInfoInterval = "Interval Info-Anzeige einstellen (in Sekunden)";
  const char * msgMenuRestart             = "Restart";
  const char * msgMenuSave                = "Einstellungen speichern";
  const char * msgMenuSaved               = "Einstellungen gespeichert";
  const char * msgMenuSetTime0            = "Zeit einstellen: Stunden und Minuten";
  const char * msgMenuSetTime1            = "Sekunden";
  const char * msgMenuSetDate0            = "Datum einstellen: Jahr";
  const char * msgMenuSetDate1            = "Monat und Tag";
  const char * msgMenuTimeDisplayMode     = "Modus Zeitanzeige einstellen";
  const char * msgMenuUIminimal           = "Webinterface minimalistisch";
  const char * msgMenuAnnouncement0       = "Ankuendigungsmodus, Auswahl der Nachricht";
  const char * msgMenuAnnouncement1       = "Starte Ankuendigungsmodus";
  const char * msgMenuNotAvailable        = "nicht verfuegbar"; 
  
  // web user interface - wifi setup
  const char * UI_msg_sw_info =           "WiFi-Verbindung einstellen";
  const char * UI_msg_sw_ssid =           "Auswahl der SSID";
  const char * UI_msg_sw_password =       "Passwort eigeben";
  const char * UI_msg_sw_saveButton =     "Speichern";
  const char * UI_msg_sw_restartButton =  "Neustart";
   
  #define UI_sw_msgInfo0                  "Wähle deine SSID aus und gib das Passwort vom zu verbindenden Netzwerk ein (Standard: max 20 Einträge). <br><br> Hinweis: die max. Länge der SSID und des Passwortes ist "
  #define UI_sw_msgInfo1                  " Zeichen, längere Einträge werden gekürzt. <br><br> Drück den SPEICHERN Knopf um die neuen Einstellungen zu speichern und einen Neustart durchzuführen.<br>Drück den NEUSTART Knopf um neu zu starten und die Netzwerke neu einzulesen."
  #define UI_sw_msgRestarting             "Einstellungen gespeichert, Neustart..."
  
  // web user interface
  const char * UI_msg_saveToEEPROM        = "Speichern im EEPROM";
  const char * UI_msg_info0               = "Status";
  const char * UI_msg_info1               = "System-Info";
  const char * UI_msg_resync              = "Syncronisiere mit einem Zeitserver";
  const char * UI_msg_interruptAlarm      = "Alarm aus / Stop";
  const char * UI_msg_timeZone            = "Zeitzone";
  
  const char * UI_msg_DSTmode             = "Sommerzeit-Modus";
  const String UI_msg_DSTmodes[] {
    "aus",
    "ein",
    "Auto"
  };
  
  const char * UI_msg_timeDisplayMode     = "Zeitanzeige-Modus";
  const String UI_msg_timeDisplayModes[] {
    "Stunden und Minuten (Kolon)",
    "Stunden und Minuten, große Schrift",
    "Stunden, Minuten, kleine Sekunden",
    "Stunden, Minuten, AM/PM",
    "Kleine Stunden, Minuten und Sekunden",
    "Kleine Stunden, Minuten und Sekunden (Kolon)",
    #ifdef ENABLE_BME280
      "Temperatur und Luftfeuchtigkeit" 
    #endif
  };
  
  const char * UI_msg_ampmMode            = "AM/PM (12/24h) Modus";
  const char * UI_msg_displayInfo         = "Info-Anzeige";
  const char * UI_msg_displayInfoInterval = "Info-Anzeige Intervall (in Sekunden)";
  const char * UI_msg_suspendMode         = "Pausenmodus";
  const char * UI_msg_sleepMode           = "Schlafmodus";
  const char * UI_msg_sleepDelay          = "Schlafmodus (in Sekunden)";
  const char * UI_msg_sleepStart          = "Starte Schlafmodus";
  const char * UI_msg_displayDaylight     = "Tageslichtinfo anzeigen";
  const char * UI_msg_displayBME280       = "Temperaturinfo anzeigen";
  const char * UI_msg_BME280_fahrenheid   = "Temperatur in Fahrenheit";
  const char * UI_msg_BME280_tempTimeSwap = "Alternative Zeit / Temperatur";
  const char * UI_msg_brightnessAuto      = "Helligkeit Auto";
  const char * UI_msg_brightness          = "Helligkeit";
  const char * UI_msg_neoBacklight        = "Neopixel Hintergrundfarbe (0 ist aus)";
  const char * UI_msg_resetWifi           = "Wifi-Einstellungen zurücksetzen";
  const char * UI_msg_restart             = "Neustart";
  
  const char * UI_msg_pad                 = "Alarm auswählen";
  const char * UI_msg_alarmNumber         = "Alarm Nummer";
  const char * UI_msg_message             = "Nachricht";
  const char * UI_msg_alarmActive         = "Alarm aktiv";
  const char * UI_msg_hour                = "Stunde";
  const char * UI_msg_min                 = "Minuten";
  const char * UI_msg_sec                 = "Sekunden";
  const char * UI_msg_month               = "Monat";
  const char * UI_msg_day                 = "Tag";
  const char * UI_msg_weekDay             = "Wochentag";
  const char * UI_msg_duration            = "Dauer (in Sekunden)" ;
  const char * UI_msg_weekDays            = "Arbeitstage";
  const char * UI_msg_weekendDays         = "Wochenendtage";
  
  const char * UI_msg_recurringType       = "Wiederholungsart";
  const String UI_msg_recurringTypes[] {
    "(aus)",
    "Sekunde(n)",
    "Minute(n)",
    "Stunde(n)",
    "Tag(e)",
    "Monat(e)"
  };
  
  const char * UI_msg_recurringInterval   = "Wiederholungsintervall";
  const char * UI_msg_buzzer              = "Buzzer / Melodie";
  const char * UI_msg_bright              = "Hell";
  const char * UI_msg_fx                  = "Effekt";
  const char * UI_msg_fx0                 = "(aus)";
  const char * UI_msg_fx1                 = "Blinkender Text / blitzende Neopixel";
  
  const char * UI_msg_actionType          = "Aktionstyp";
  const String UI_msg_actionTypes[] {
    "(aus)",                                                         // 0
    "PIN-Aktionsnummer 'high' (Zeitdauer 0 hält es aktiv)",          // 1
    "PIN-Aktionsnummer 'low' (Zeitdauer 0 hält es passiv)",           // 2
    "Neopixel Wecklicht",                                         // 3
    "Neopixel Wecklicht, verknüpfe mit nächstem Alarm",           // 4
    "Neopixels an, die Aktionsnummer wählt die Farbe aus",           // 5
    "Neopixels aus (Hintergrundfarbe)"                               // 6
  };
  
  #define actionGPIOhigh     1
  #define actionGPIOlow      2
  #define actionNeoWUL       3
  #define actionNeoWULchain  4
  #define actionNeoOn        5
  #define actionNeoOff       6
  
  const char * UI_msg_actionNum           = "Aktionsnummer";
  const char * UI_msg_testAlarm           = "Teste den Alarm";
  const char * UI_msg_alarmsReset         = "Alle Alarme zurücksetzen";
  
  const char * UI_msg_announcements       = "Ankündigungsliste";
  const char * UI_msg_customAnnouncement  = "Individuelle Ankündigung";
  const char * UI_msg_announceMode        = "Ankündigungsmodus aktiv";
  
  const char * UI_msg_tab_general         = "Haupteinstellungen";
  const char * UI_msg_tab_alarms          = "Alarme";
  const char * UI_msg_tab_announcements   = "Ankündigungen";
  
  const char * UI_msg_click               = "OK";
  const char * UI_msg_txt_saved           = "GESPEICHERT";
  const char * UI_msg_txt_invalid         = "UNGÜLTIG";
  const char * UI_msg_txt_reset           = "ALARM RESET";

  const char * UI_msg_info_manualSet      = "manuell";
  const char * UI_msg_info_sleeping       = "schläft";
  const char * UI_msg_info_timeZone       = "Zeitzone";
  const char * UI_msg_info_DST            = "Sommerzeit";
#endif
