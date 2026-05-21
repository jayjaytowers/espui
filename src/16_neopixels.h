/*
***************************************************************************
  ebc_alarmclock - neopixel functions
***************************************************************************
  last update 20201115 by ericBcreator
***************************************************************************
*/

#ifdef ENABLE_NEOPIXELS

  void neopixelsFade(byte colorSet, bool firstCall) {
    static unsigned long delayTime = 0, prevCallTime = 0;
    static byte colorCode = 0, numOfColorCodes = 0, fadeStep = 0;
    static float brightnessFactor = 0, rStep, gStep, bStep;
    int neoCurColorRint, neoCurColorGint, neoCurColorBint;
      
    if (firstCall) {      
      colorCode = 0;
      neoCurColorR = 0;
      neoCurColorG = 0;
      neoCurColorB = 0;
      numOfColorCodes = sizeof(neoColorsSunrise) / sizeof(neoColorsSunrise[0]);

      delayTime = ((alarmList[alarmNum].duration * 1000) / numOfColorCodes / neoFadeNumOfSteps);
      
      if (delayTime < 100)
        delayTime = 100; // @EB-todo sanitize

//      DEBUGPRINTLN("delayTime " + (String) delayTime);
    }

    if ((millis() - prevCallTime < delayTime) || (colorCode >= numOfColorCodes))
      return;

    if (fadeStep > neoFadeNumOfSteps || firstCall) {
      if (!firstCall) {
        colorCode++;
        if (colorCode >= numOfColorCodes)
          return;
      }

      fadeStep = 0;
      neoCurBrightness = neoColorsSunrise[colorCode][3];
      brightnessFactor = (float) neoCurBrightness / 255;
  
      byte colorR = (float) (neoColorsSunrise[colorCode][0] * brightnessFactor);
      byte colorG = (float) (neoColorsSunrise[colorCode][1] * brightnessFactor);
      byte colorB = (float) (neoColorsSunrise[colorCode][2] * brightnessFactor);
      
      rStep = (float) (((float) colorR - (float) neoCurColorR) / (float) neoFadeNumOfSteps);
      gStep = (float) (((float) colorG - (float) neoCurColorG) / (float) neoFadeNumOfSteps);
      bStep = (float) (((float) colorB - (float) neoCurColorB) / (float) neoFadeNumOfSteps);

//      DEBUGPRINTLN("Neopixel colorcode " + (String) colorCode + " RGB " + (String) colorR + " " + (String) colorG + " " + (String) colorB + " " + (String) neoCurBrightness);
    }

    neoCurColorR += rStep;
    neoCurColorG += gStep;
    neoCurColorB += bStep;
   
    neoCurColorRint = constrain(neoCurColorR, 0, 255);
    neoCurColorGint = constrain(neoCurColorG, 0, 255);
    neoCurColorBint = constrain(neoCurColorB, 0, 255);

//    DEBUGPRINTLN("Neopixel colorcode " + (String) colorCode + " fadestep " + (String) fadeStep + " RGB " + (String) neoCurColorRint + " " + (String) neoCurColorGint + " " + (String) neoCurColorBint);

    for (int j = 0; j < NUM_OF_NEOPIXELS; j++)
      neopixels[j] = CRGB(neoCurColorRint, neoCurColorGint, neoCurColorBint);
    FastLED.show();
    
    prevCallTime = millis();
    fadeStep++;
  }  

  void neopixelsOn(byte RGB[]) {
    int red   = (RGB[0] * RGB[3] / 255);
    int green = (RGB[1] * RGB[3] / 255);
    int blue  = (RGB[2] * RGB[3] / 255);
    
    for (int j = 0; j < NUM_OF_NEOPIXELS; j++)
      neopixels[j] = CRGB(red, green, blue);
    FastLED.show();
  }

  void neopixelsPresetOn(int preset) {
    if (preset >= 0 && preset <= numOfNeoPalette) {
      neopixelsOn(neoPalette[preset]);
    }
  }
  
  void neopixelsOff() {
    for (int j = 0; j < NUM_OF_NEOPIXELS; j++)
      neopixels[j] = CRGB::Black;      
    FastLED.show();
  }

#endif
