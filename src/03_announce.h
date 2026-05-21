/*
***************************************************************************
  ebc_alarmclock - announcements
***************************************************************************
  last update 20200915 by ericBcreator
***************************************************************************
*/

const String announcements[] {                                  // @EB-custom
  "Went to the carwash",
  "Walking with the dog",
  "Walked the dog",
  "Went to the gym",
  "Going running",
  "-exit mode"                                                  // needed to exit announce mode in the rotary menu
};

const byte announceStartDelay = 10;                             // @EB-custom initial delay time in seconds before checking for motion
const byte announceWaitDelay = 5;                               // @EB-custom delay time in seconds before restarting the announce loop
const byte announceRepeat = 3;                                  // @EB-custom the number of times to display the announcement
const byte customAnnouncementLength = 41;                       // @EB-custom the max length of an announcement

struct tm announcementTime;
char customAnnouncement[customAnnouncementLength] = "";
bool customAnnounceMsg = false;
