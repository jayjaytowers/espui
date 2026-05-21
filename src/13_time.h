//
// time functions
//

void readCurrentTime() {
  // note: when timeManualSet is true, the time is read from the internal ESP clock, when false from the NTP server
  //       timeManualSet is set true when the time is set with the rotary encoder and false when the time is read with getTimeFromServer()
  
  if (timeManualSet) {
    currentTime = now();
  } else {
    time(&currentTime);     // @EB-todo: connects to timeserver with every call ???
  }
  
  timeData = *localtime(&currentTime);
  
//  timeString = String(ctime(&currentTime));
//  timeString.trim();

  int year = timeData.tm_year;
  if (year < 1900) year += 1900;
    
  timeString = days[timeData.tm_wday] + " "+ fillZero(timeData.tm_mday) + " " + months[timeData.tm_mon]  + " " + (String) year + " ";
  timeString += fillZero(getPMhour()) + ":" + fillZero(timeData.tm_min) + ":" + fillZero(timeData.tm_sec);

  if (ampmMode) {
    if (pm)  timeString += " PM";
    else     timeString += " AM";
  }
}

int getPMhour() {
  int pmHour = timeData.tm_hour;
  pm = false;
  if (pmHour > 11) pm = true;

  if (ampmMode) {         // displaying 12:00-12:59 and 1:00-11:59 AM or PM
    if (pmHour == 0)      pmHour += 12;
    else if (pmHour > 12) pmHour -= 12;    
  }

  return pmHour;
}

bool checkLeapYear (int the_year) {
  if (the_year % 4 != 0) return false;
  if (the_year % 100 == 0 && the_year %400 != 0) return false;
  if (the_year % 400 == 0) return true;
  return true;
}

byte getDaysInMonth (int year, int month) { 
  byte daysInMonth = 31;  
  switch (month) {
    case 2:
      if (checkLeapYear(year))
        daysInMonth = 29;
      else
        daysInMonth = 28;
      break;
    case 4:
    case 6:
    case 9:
    case 11:
      daysInMonth = 30;
      break;
  }
  return daysInMonth;
}

//
// checkDST
//

void checkDST() {
  if (DSTmode == 0) { DST = false; return; }  // DST mode set to off so return false
  if (DSTmode == 1) { DST = true;  return; }  // DST mode set to on so return true

  DST = false;                                // defaults to false

  if (timeManualSet)
    currentTime = now();
  else
    time(&currentTime);
  
  timeData = *localtime(&currentTime);

  int y = timeData.tm_year - 100;             // tm_year returns the year after 1900 so subtract 100 to get the year after 2000
  int x1 = (y + y / 4 - 2) % 7;               // identifies Sunday offset for March
  int x2 = (y + y / 4 + 2) % 7;               // identifies Sunday offset for October
  
  int mon = timeData.tm_mon + 1;              // important: the tm_mon variable ranges from 0 to 11 so add 1 to get 1 to 12 !!

  // DST: begins last Sunday of March 02:00
  if ((mon == 3) && (timeData.tm_mday == (31 - x1)) && (timeData.tm_hour >= 2))
    DST = true;
  if (((mon == 3) && (timeData.tm_mday > (31 - x1))) || (mon  > 3))
    DST = true;

  // DST: ends last Sunday of Oct 02:00
  if ((mon == 10) && (timeData.tm_mday == (31 - x2)) && (timeData.tm_hour >= 2))
    DST = false;
  if (((mon == 10) && (timeData.tm_mday > (31 - x2))) || (mon > 10) || (mon < 3))
    DST = false;
}
