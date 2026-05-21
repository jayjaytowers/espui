/*
***************************************************************************
  ebc_alarmclock - language settings - Nederlands
***************************************************************************
  last update 20210319 by ericBcreator
***************************************************************************
*/

//
// language settings
//

#ifndef _LANGUAGE
  #define _LANGUAGE "DUT"
  
  const String months[]                   = { "januari", "februari", "maart", "april", "mei", "juni", "juli", "augustus", "september", "oktober", "november", "december" };
  const String days[]                     = { "Zondag", "Maandag", "Dinsdag", "Woensdag", "Donderdag", "Vrijdag", "Zaterdag" };
  
  const char * msgAlarm0Active            = "Alarm 1 actief, ingesteld op";
  const char * msgAlarm0Inactive          = "Alarm 1 uitgeschakeld";
  const char * msgAlarm0Set               = "Alarm 1 ingesteld op";
  const char * msgAlarmOff                = "Alarm uit";
  const char * msgConnTSfailed            = "* Verbinding met de tijdserver is mislukt...";
  const char * msgConnWifiFailed          = "* Wifi verbinding is mislukt...";
  const char * msgDateSet                 = "Datum ingesteld op";
  const char * msgDaylightTime            = "uren daglicht";
  const char * msgDSTon                   = "Zomertijd";
  const char * msgDSToff                  = "Wintertijd";
  const char * msgLedBrightnessSet        = "Led felheid ingesteld op";
  const char * msgMenu                    = "Menu";
  const char * msgNewVersion              = "Nieuwe firmware versie ";
  const char * msgNoWifi                  = "Geen Wifi verbinding";
  const char * msgNoWifiNoRE              = "Waarschuwing: Wifi en enconder uitgeschakeld"; // no Wifi and no encoder so unable to change any settings
  const char * msgOn                      = "Aan"; // 3 characters
  const char * msgOff                     = "Uit"; // 3 characters
  const char * msgOTAupdateStart          = ">> Start draadloze firmware update ";
  const char * msgOTAupdateEnd            = "<< Firmware update geinstalleerd, start opnieuw op...";
  const char * msgOTAupdateError          = ">* Fout firmware update: ";
  const char * msgRecurring               = "Herhalend: ";
  const char * msgSunrise                 = "Zonsopkomst";
  const char * msgSunset                  = "Zonsondergang";
  const char * msgbmeTemperature          = "Temperatuur";
  const char * msgbmeHumidity             = "vochtigheid";
  const char * msgbmePressure             = "luchtdruk";
  const char * msgTimeSet                 = "Tijd ingesteld op";
  const char * msgTimeSynced              = "Tijd gelezen ,";
  const char * msgUIfilesystemInit0       = "Prepareer het bestandssysteem...";
  const char * msgUIfilesystemInit1       = "Het bestandssysteem is geprepareerd";
  const char * msgUIstart0                = "Web interface gestart. Verbind met SSID ";
  const char * msgUIstart1                = " en browse naar ";
  const char * msgUIstart2                = " of lan ";
  const char * msgUIwifiSetup0            = "Start WiFi web setup. Verbind met SSID ";
  const char * msgUIwifiSetup1            = " en browse naar ";
  const char * msgUIwifiReset             = "Wifi gereset, start opnieuw op...";
  const char * msgWakeupLight             = " - wake up light";
  const char * msgWakeupLightChain        = " - wake up light, koppel het volgende alarm";
  
  // menu items
  const String menu[] {
    "Stel alarm 1 in",
    "Led felheid",
    "Neopixel achtergrondverlichting",
    "Synchroniseer de tijd",
    "Slaap modus",
    "Pauzeer modus",
    "Info weergave",
    "Interval info weergave",
    "Tijd instelling",
    "Datum instelling",
    "Display modus",
  //  "UI minimal controls",  // @EB-todo disabled
    "Systeem informatie",
    "Instellingen opslaan",
    "Herstarten",
  #ifdef ANNOUNCE_MODE
    "Aankondigingen",
  #endif
    "Verlaat het menu"
  };
  
  const String timeDisplayModes[] {
    "UU:MM",
    "UU:MM groot",
    "UU MM ss",
    "UU MM am/pm",
    "uu mm ss",
    "uu mm:ss",
    #ifdef ENABLE_BME280
      "temp vocht"
    #endif
  };
  
  const char * msgMenuAlarm0              = "Stel de tijd in voor alarm 1";
  const char * msgMenuAlarm0wul           = "Wake up light";
  const char * msgMenuLedBrightness       = "Stel de led felheid in, -1 is automatisch (sensor)";
  const char * msgMenuNeoBacklight        = "Stel de neopixels achtergrondkleur in";
  const char * msgMenuSyncTime            = "Synchroniseer met een tijdserver";
  const char * msgMenuSleepMode           = "Slaap modus";
  const char * msgMenuSuspendMode         = "Pauzeer modus";
  const char * msgMenudisplayInfo         = "Info weergave";
  const char * msgMenuDisplayInfoInterval = "Stel het info weergave interval in (seconden)";
  const char * msgMenuRestart             = "Opnieuw opstarten";
  const char * msgMenuSave                = "Instellingen opgeslaan";
  const char * msgMenuSaved               = "Instellingen opgeslagen";
  const char * msgMenuSetTime0            = "Stel de tijd in: uren en minuten";
  const char * msgMenuSetTime1            = "seconden";
  const char * msgMenuSetDate0            = "Stel de datum in: jaar";
  const char * msgMenuSetDate1            = "maand en dag";
  const char * msgMenuTimeDisplayMode     = "Set the time display mode";
  const char * msgMenuUIminimal           = "Web interface minimal controls";
  const char * msgMenuAnnouncement0       = "Aankondiging modus, selecteer de tekst";
  const char * msgMenuAnnouncement1       = "Start de aankondiging modus";
  const char * msgMenuNotAvailable        = "Niet beschikbaar"; 
  
  // web user interface - wifi setup
  const char * UI_msg_sw_info =           "WiFi verbinding instellen";
  const char * UI_msg_sw_ssid =           "Selecteer het SSID";
  const char * UI_msg_sw_password =       "Wachtwoord";
  const char * UI_msg_sw_saveButton =     "Opslaan";
  const char * UI_msg_sw_restartButton =  "Herstarten";
   
  #define UI_sw_msgInfo0                  "Selecteer het SSID en toets het wachtwoord van het netwerk (standaard: max 20 selecties). <br><br> NB: de maximale lengte van het SSID en wachtwoord is "
  #define UI_sw_msgInfo1                  " karakters, langere ingaven worden afgekort. <br><br> Klik op Opslaan om de instellingen op te slaan en te herstarten.<br>Klik op Herstarten om te herstarten en de netwerk opnieuw in te lezen."
  #define UI_sw_msgRestarting             "Instellingen opgeslagen, start opnieuw op..."
  
  // web user interface
  const char * UI_msg_saveToEEPROM        = "Instellingen opslaan";
  const char * UI_msg_info0               = "Status";
  const char * UI_msg_info1               = "Systeem informatie";
  const char * UI_msg_resync              = "Synchroniseer met een tijdserver";
  const char * UI_msg_interruptAlarm      = "Alarm uit / stop";
  const char * UI_msg_timeZone            = "Tijdzone";
  
  const char * UI_msg_DSTmode             = "Zomertijd mode";
  const String UI_msg_DSTmodes[] {
    "Uit",
    "Aan",
    "Auto"
  };
  
  const char * UI_msg_timeDisplayMode     = "Tijd weergave modus";
  const String UI_msg_timeDisplayModes[] {
    "Uren en minuten (colon)",
    "Uren en minuten (groot)",
    "Uren, minuten en kleine seconden",
    "Uren, minuten en AM/PM",
    "Kleine uren, minuten en seconden",
    "Kleine uren, minuten en seconden (colon)",
    #ifdef ENABLE_BME280
      "Temperatuur en vochtigheid" 
    #endif
  };
  
  const char * UI_msg_ampmMode            = "AM/PM modus";
  const char * UI_msg_displayInfo         = "Info weergave";
  const char * UI_msg_displayInfoInterval = "Info weergave interval (in seconden)";
  const char * UI_msg_suspendMode         = "Pauzeer modus";
  const char * UI_msg_sleepMode           = "Slaap modus";
  const char * UI_msg_sleepDelay          = "Slaap wachttijd (in seconden)";
  const char * UI_msg_sleepStart          = "Start de slaap modus";
  const char * UI_msg_displayDaylight     = "Daglicht weergave";
  const char * UI_msg_displayBME280       = "Temperatuur weergave";
  const char * UI_msg_BME280_fahrenheid   = "Temperatuur in Fahrenheid";
  const char * UI_msg_BME280_tempTimeSwap = "Wissel tijd / temperatuur";
  const char * UI_msg_brightnessAuto      = "Led felheid automatisch";
  const char * UI_msg_brightness          = "Led felheid";
  const char * UI_msg_neoBacklight        = "Neopixel achtergrondkleur (0 is uit)";
  const char * UI_msg_resetWifi           = "Reset de Wifi instellingen";
  const char * UI_msg_restart             = "Herstarten";
  
  const char * UI_msg_pad                 = "Selecteer het alarm";
  const char * UI_msg_alarmNumber         = "Alarm nummer";
  const char * UI_msg_message             = "Tekst";
  const char * UI_msg_alarmActive         = "Alarm actief";
  const char * UI_msg_hour                = "Uur";
  const char * UI_msg_min                 = "Minuten";
  const char * UI_msg_sec                 = "Seconden";
  const char * UI_msg_month               = "Maand";
  const char * UI_msg_day                 = "Dag";
  const char * UI_msg_weekDay             = "Dag van de week";
  const char * UI_msg_duration            = "Tijdsduur (in seconden)" ;
  const char * UI_msg_weekDays            = "Weekdagen";
  const char * UI_msg_weekendDays         = "Weekend dagen";
  
  const char * UI_msg_recurringType       = "Herhaal soort";
  const String UI_msg_recurringTypes[] {
    "(uit)",
    "Seconden",
    "Minuten",
    "Uren",
    "Dagen",
    "Maanden"
  };
  
  const char * UI_msg_recurringInterval   = "Herhaal interval";
  const char * UI_msg_buzzer              = "Buzzer";
  const char * UI_msg_bright              = "Fel";
  const char * UI_msg_fx                  = "Effect";
  const char * UI_msg_fx0                 = "(uit)";
  const char * UI_msg_fx1                 = "Knipperende tekst / neopixels";
  
  const char * UI_msg_actionType          = "Actie soort";
  const String UI_msg_actionTypes[] {
    "(uit)",                                                              // 0
    "Schakel pin actie nummer hoog (tijdsduur 0 is permanent)",           // 1
    "Schakel pin actie nummer laag (tijdsduur 0 is permanent)",           // 2
    "Neopixel wakeup light",                                              // 3
    "Neopixel wakeup light, koppelt het volgende alarm",                  // 4
    "Neopixels aan, het actie nummer selecteert de kleur",                // 5
    "Neopixels uit (of achtergrond kleur)"                                // 6
  };
  
  #define actionGPIOhigh     1
  #define actionGPIOlow      2
  #define actionNeoWUL       3
  #define actionNeoWULchain  4
  #define actionNeoOn        5
  #define actionNeoOff       6
  
  const char * UI_msg_actionNum           = "Actie nummer";
  const char * UI_msg_testAlarm           = "Test het alarm";
  const char * UI_msg_alarmsReset         = "Reset alle alarmen";
  
  const char * UI_msg_announcements       = "Aankondiging lijst";
  const char * UI_msg_customAnnouncement  = "Vrije aankondiging";
  const char * UI_msg_announceMode        = "Aankondiging modus actief";
  
  const char * UI_msg_tab_general         = "Hoofdinstellingen";
  const char * UI_msg_tab_alarms          = "Alarmen";
  const char * UI_msg_tab_announcements   = "Aankondigingen";
  
  const char * UI_msg_click               = "OK";
  const char * UI_msg_txt_saved           = "OPGESLAGEN";
  const char * UI_msg_txt_invalid         = "ONGELDIG";
  const char * UI_msg_txt_reset           = "ALARMEN GERESET";

  const char * UI_msg_info_manualSet      = "handmatig";
  const char * UI_msg_info_sleeping       = "slapend";
  const char * UI_msg_info_timeZone       = "tijdzone";
  const char * UI_msg_info_DST            = "zomertijd";
#endif
