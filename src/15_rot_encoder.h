//
// rotary encoder
//

int readRotEnc() {                                                              // returns -1 when turned left/down and +1 when turned right/up
  static int reLastPos = 0;
  int reCurrentPos;
  int reResult = 0;

  RE_encoder.tick();

  reCurrentPos = RE_encoder.getPosition();
  if (reCurrentPos < reLastPos)
    reResult = encLeftResult;
  else if (reCurrentPos > reLastPos)
    reResult = encRightResult;

  reLastPos = reCurrentPos;
  return reResult;
}

int readRotEncSwitch() {                                                        // returns 1 when pressed or 2 when pressed for 1 second
  static int reLastSwitchState = LOW;
  int reSwitchState = digitalRead(RE_SWITCH);

  if (reSwitchState == LOW && reLastSwitchState == HIGH) {
    unsigned long startTime = millis();
    
    while (digitalRead(RE_SWITCH) == LOW) {                                     // wait until the button is released or 1 second has passed
      reLastSwitchState = reSwitchState;
      if (millis() - startTime > 1000)
        break;
    }

    //delay(40);
    
    if (millis() - startTime > 1000)
      return 2;
    else
      return 1;
  }
  
  reLastSwitchState = reSwitchState;
  return false;
}
