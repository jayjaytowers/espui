/*
***************************************************************************
  ebc_alarmclock - language settings - Italian
***************************************************************************
  last update 20210320 by ericBcreator
***************************************************************************
*/

//
// language settings
//

#ifndef _LANGUAGE
  #define _LANGUAGE "ITA"
  
  const String months[]                   = { "Gennaio", "Febbraio", "Marzo", "Aprile", "Maggio", "Giugno", "Luglio", "Agosto", "Settembre", "Ottobre", "Novembre", "Dicembre" };
  const String days[]                     = { "Domenica", "Lunedi", "Martedi", "Mecoledi", "Giovedi", "Venerdi", "Sabato" };
  
  const char * msgAlarm0Active            = "Allarme 1 Attivato, alle ore";
  const char * msgAlarm0Inactive          = "Allarme 1 Dissattivato";
  const char * msgAlarm0Set               = "Allarme 1 impostato alle ore";
  const char * msgAlarmOff                = "Allarme Off";
  const char * msgConnTSfailed            = "* Connessione al timeserver fallita...";
  const char * msgConnWifiFailed          = "* Connessione Wifi fallita...";
  const char * msgDateSet                 = "Data impostata a";
  const char * msgDaylightTime            = "ore di luce del giorno";
  const char * msgDSTon                   = "DST on";
  const char * msgDSToff                  = "DST off";
  const char * msgLedBrightnessSet        = "Luminosità impostata a";
  const char * msgMenu                    = "Menu";
  const char * msgNewVersion              = "Nuova versione firmware ";
  const char * msgNoWifi                  = "Nessuna connessione Wifi";
  const char * msgNoWifiNoRE              = "Attenzione: Wifi and encoder disabilitati";           // no Wifi and no encoder so unable to change any settings
  const char * msgOn                      = "On "; // 3 characters
  const char * msgOff                     = "Off"; // 3 characters
  const char * msgOTAupdateStart          = ">> Inizio aggiornamento firmware over the air";
  const char * msgOTAupdateEnd            = "<< Firmware aggiornato, riavvio...";
  const char * msgOTAupdateError          = ">* Errore aggiornamento firmware: ";
  const char * msgRecurring               = "Ricorrente: ";
  const char * msgSunrise                 = "Alba";
  const char * msgSunset                  = "Tramonto";
  const char * msgbmeTemperature          = "Temperatura";
  const char * msgbmeHumidity             = "Umidità";
  const char * msgbmePressure             = "Pressione";
  const char * msgTimeSet                 = "Oraio impostato a";
  const char * msgTimeSynced              = "Tempo di lettura ,";
  const char * msgUIfilesystemInit0       = "Preparazione del filesystem...";
  const char * msgUIfilesystemInit1       = "Filesystem preparato";
  const char * msgUIstart0                = "Interfaccia Web partita. Connesso al SSID ";
  const char * msgUIstart1                = " e vai a ";
  const char * msgUIstart2                = " oppure lan ";
  const char * msgUIwifiSetup0            = "Avvio della configurazione web WiFi. Connetti a SSID ";
  const char * msgUIwifiSetup1            = " e vai a ";
  const char * msgUIwifiReset             = "Ripristino Wi-Fi, riavvio...";
  const char * msgWakeupLight             = " - luce sveglia"; // what do you mean? "turn on the light"?-----------------------
  const char * msgWakeupLightChain        = " - luce sveglia, prossima catena"; //........, prossima catena-------------------------
  
  // menu items
  const String menu[] {
    "Imposta sveglia 1",
    "Luminosità LED",
    "Retroilluminazione Neopixel",
    "Sincronizza l'ora",
    "Modalità risparmio",
    "Modalità di sospensione",
    "Visualizzazione della data",
    "Visualizzazione info intervallo",
    "Impostazione tempo",
    "Impostazione data",
    "Modalità display",
  //  "UI minimal controls",  // @EB-todo disabled
    "Informazioni di sistema",
    "Salva le impostazioni",
    "Riavvio",
  #ifdef ANNOUNCE_MODE
    "Annunci",
  #endif
    "Exit menu"
  };
  
  const String timeDisplayModes[] {
    "HH:MM",
    "HH:MM big font",
    "HH MM ss",
    "HH MM am/pm",
    "hh mm ss",
    "hh mm:ss",
    #ifdef ENABLE_BME280
      "tmp hu"
    #endif
  };
  
  const char * msgMenuAlarm0              = "Impostare l'ora per la sveglia 1";
  const char * msgMenuAlarm0wul           = "Luce sveglia";//-------------------------------------------------------------------
  const char * msgMenuLedBrightness       = "Impostare la luminosità LED, -1 è automatico (sensore)";
  const char * msgMenuNeoBacklight        = "Imposta colore retroilluminazione dei neopixel";
  const char * msgMenuSyncTime            = "Sincronizza con il timeserver";
  const char * msgMenuSleepMode           = "Imposta modalità risparmio";
  const char * msgMenuSuspendMode         = "Imposta modalità di sospensione";
  const char * msgMenudisplayInfo         = "Visualizza data";
  const char * msgMenuDisplayInfoInterval = "Imposta l'intervallo info sul display (in secondi)";
  const char * msgMenuRestart             = "Restart";
  const char * msgMenuSave                = "Salva impostazioni";
  const char * msgMenuSaved               = "Impostazioni salvate";
  const char * msgMenuSetTime0            = "Imposta l'ora: ore e minuti";
  const char * msgMenuSetTime1            = "secondi";
  const char * msgMenuSetDate0            = "Imposta la data: anno";
  const char * msgMenuSetDate1            = "mese e giorno";
  const char * msgMenuTimeDisplayMode     = "Imposta modalità di visualizzazione dell'ora";
  const char * msgMenuUIminimal           = "Controlli minimi dell'interfaccia web";
  const char * msgMenuAnnouncement0       = "Modalità annuncio, selezionare il messaggio";
  const char * msgMenuAnnouncement1       = "Avvio modalità di annuncio";
  const char * msgMenuNotAvailable        = "Non disponibile"; 
  
  // web user interface - wifi setup
  const char * UI_msg_sw_info =           "Imposta connessione WiFi";
  const char * UI_msg_sw_ssid =           "Seleziona the SSID";
  const char * UI_msg_sw_password =       "Scrivi la password";
  const char * UI_msg_sw_saveButton =     "Salva";
  const char * UI_msg_sw_restartButton =  "Riavvia";
   
  #define UI_sw_msgInfo0                  "Selezionare SSID e inserire la password di rete a cui connettersi (impostazione predefinita: max 20 caratteri). <br> <br> Nota: il max. la lunghezza dell'SSID e della password è "
  #define UI_sw_msgInfo1                  " caratteri, le voci più lunghe verranno troncate. <br> <br> Fare clic sul pulsante Salva per memorizzare le nuove impostazioni e riavviare. <br> Fare clic sul pulsante Riavvia per riavviare e rileggere le reti."
  #define UI_sw_msgRestarting             "Impostazioni salvate, riavvio in corso ...."
  
  // web user interface
  const char * UI_msg_saveToEEPROM        = "Salva in memoria";
  const char * UI_msg_info0               = "Stato";
  const char * UI_msg_info1               = "Informazioni di sistema";
  const char * UI_msg_resync              = "Sincronizza con il timeserver";
  const char * UI_msg_interruptAlarm      = "Allarme off / stop";
  const char * UI_msg_timeZone            = "Timezone";
  
  const char * UI_msg_DSTmode             = "Modo DST";
  const String UI_msg_DSTmodes[] {
    "Off",
    "On",
    "Auto"
  };
  
  const char * UI_msg_timeDisplayMode     = "Modalità visualizzazione del tempo";
  const String UI_msg_timeDisplayModes[] {
    "Ore, minuti (:)",
    "Ore, minuti carattere grande",
    "Ore, minuti e secondi piccoli",
    "Ore, minuti AM/PM",
    "Ore, minuti e secondi piccoli",
    "Ore, minuti e seconds piccole (:)",
    #ifdef ENABLE_BME280
      "Temperatura e umidità" 
    #endif
  };
  
  const char * UI_msg_ampmMode            = "AM/PM modo";
  const char * UI_msg_displayInfo         = "Visualizza informazioni";
  const char * UI_msg_displayInfoInterval = "Visualizza intervallo info (in secondi)";
  const char * UI_msg_suspendMode         = "Modalità sospensione";
  const char * UI_msg_sleepMode           = "Modalità risparmio";
  const char * UI_msg_sleepDelay          = "Ritardo mod. risparmio (in secondi)";
  const char * UI_msg_sleepStart          = "Inizio mod. risparmio";
  const char * UI_msg_displayDaylight     = "Visualizza info luce diurna";
  const char * UI_msg_displayBME280       = "Visualizza info temperatura";
  const char * UI_msg_BME280_fahrenheid   = "Temperatura in Fahrenheid";
  const char * UI_msg_BME280_tempTimeSwap = "Alterna tempo / temperatura";
  const char * UI_msg_brightnessAuto      = "Luminosità automatica";
  const char * UI_msg_brightness          = "Luminosità";
  const char * UI_msg_neoBacklight        = "Colore retroilluminazione Neopixel (0 è off)";
  const char * UI_msg_resetWifi           = "Ripristina impostazioni Wi-Fi";
  const char * UI_msg_restart             = "Riavvio";
  
  const char * UI_msg_pad                 = "Seleziona sveglia";
  const char * UI_msg_alarmNumber         = "Sveglia numero";
  const char * UI_msg_message             = "Messaggio";
  const char * UI_msg_alarmActive         = "Allarme attivo";
  const char * UI_msg_hour                = "Ora";
  const char * UI_msg_min                 = "Minuti";
  const char * UI_msg_sec                 = "Secondi";
  const char * UI_msg_month               = "Mese";
  const char * UI_msg_day                 = "Giorno";
  const char * UI_msg_weekDay             = "Giorno della Settimana";
  const char * UI_msg_duration            = "Durata (in secondi)" ;
  const char * UI_msg_weekDays            = "Giorni della settimana";
  const char * UI_msg_weekendDays         = "Giorni del fine settimana";
  
  const char * UI_msg_recurringType       = "Tipo ricorrente";
  const String UI_msg_recurringTypes[] {
    "(off)",
    "Secondo(i)",
    "Minuto(i)",
    "Ora(e)",
    "Giorno(i)",
    "Mese(i)"
  };
  
  const char * UI_msg_recurringInterval   = "Intervallo ricorrente";
  const char * UI_msg_buzzer              = "Cicalino";
  const char * UI_msg_bright              = "Luminosità";
  const char * UI_msg_fx                  = "Effetto";
  const char * UI_msg_fx0                 = "(off)";
  const char * UI_msg_fx1                 = "Testo lampeggiante / neopixel lampeggiante";
  
  const char * UI_msg_actionType          = "Tipo di azione";
  const String UI_msg_actionTypes[] {
    "(off)",                                                                  // 0
    "Imposta il numero di azione del pin alto (0 lo manterrà alto)",          // 1
    "Imposta il numero di azione del pin basso (0 lo manterrà basso)",        // 2
    "Luce sveglia Neopixel",                                                  // 3
    "Luce sveglia Neopixel, catena di allarme successivo",                    // 4
    "Neopixel on, il numero azione è il colore della tavolozza",              // 5
    "Neopixel off (colore retroilluminazione)"                                // 6
  };
  
  #define actionGPIOhigh     1
  #define actionGPIOlow      2
  #define actionNeoWUL       3
  #define actionNeoWULchain  4
  #define actionNeoOn        5
  #define actionNeoOff       6
  
  const char * UI_msg_actionNum           = "Numero azione";
  const char * UI_msg_testAlarm           = "Prova l'allarme";
  const char * UI_msg_alarmsReset         = "Resetta tutti gli allarmi";
  
  const char * UI_msg_announcements       = "Elenco annunci";
  const char * UI_msg_customAnnouncement  = "Annuncio personalizzato";
  const char * UI_msg_announceMode        = "Modalità annuncio attiva";
  
  const char * UI_msg_tab_general         = "Impostazioni principali";
  const char * UI_msg_tab_alarms          = "Allarmi";
  const char * UI_msg_tab_announcements   = "Annunci";
  
  const char * UI_msg_click               = "OK";
  const char * UI_msg_txt_saved           = "SALVATO";
  const char * UI_msg_txt_invalid         = "NON VALIDO";
  const char * UI_msg_txt_reset           = "RESET ALLARMI";

  const char * UI_msg_info_manualSet      = "impostazione manuale";
  const char * UI_msg_info_sleeping       = "risparmio";
  const char * UI_msg_info_timeZone       = "timezone";
  const char * UI_msg_info_DST            = "DST";
#endif
