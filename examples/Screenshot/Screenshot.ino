/*************************************************
 *  Screenshot.ino                               *
 *  Example from the ArTICL library              *
 *           Created by Christopher Mitchell,    *
 *           2011-2015, all rights reserved.     *
 *                                               *
 *  This demo communicates use the silent-       *
 *  linking commands to dump a screenshot from   *
 *  a connected calculator to the Arduino's      *
 *  serial console.                              *
 *************************************************/

#include <TICL.h>

#if defined(__MSP432P401R__***REMOVED***        // MSP432 target
#define TRIGGER_PRESSED LOW
#define TRIGGER_BUTTON 73
#else                                // Arduino target
#define TRIGGER_PRESSED LOW
#define TRIGGER_BUTTON 4
#endif

TICL ticl = TICL(DEFAULT_TIP, DEFAULT_RING***REMOVED***;

void setup(***REMOVED*** {
  pinMode(TRIGGER_BUTTON, INPUT_PULLUP***REMOVED***;
  Serial.begin(9600***REMOVED***;
  ticl.resetLines(***REMOVED***;
  // ticl.setVerbosity(true, &Serial***REMOVED***;
}

void loop(***REMOVED*** {
  if (TRIGGER_PRESSED == digitalRead(TRIGGER_BUTTON***REMOVED******REMOVED*** {
    Serial.println("Starting transfer..."***REMOVED***;
    uint8_t screen[768 + 2];
    int rlen = 0, rval = 0;
    
    // Request the screen image
    uint8_t msg[4] = {CALC83P, SCR, 0x00, 0x00};
    rval = ticl.send(msg, NULL, 0***REMOVED***;
    if (rval***REMOVED*** {
      Serial.print("Failed to send SCR request: "***REMOVED***;
      Serial.println(rval***REMOVED***;
      return;
    }
    
    // Wait for ack
    ticl.resetLines(***REMOVED***;
    delay(100***REMOVED***;
    rval = ticl.get(msg, NULL, &rlen, 0***REMOVED***;
    if (rval***REMOVED*** {
      Serial.print("Failed to get SCR ack: "***REMOVED***;
      Serial.println(rval***REMOVED***;
      return;
    }
    
    // Wait for screen image
    rval == ticl.get(msg, screen, &rlen, 768+2***REMOVED***;
    if (rval***REMOVED*** {
      Serial.print("Failed to get SCR: "***REMOVED***;
      Serial.println(rval***REMOVED***;
      return;
    }
    
    // Send an ack
    rval = ticl.send(msg, NULL, 0***REMOVED***;
    if (rval***REMOVED*** {
      Serial.print("Failed to send ack: "***REMOVED***;
      Serial.println(rval***REMOVED***;
      return;
    }
    
    // Dump the screen to the serial console
    for (int i = 0; i < 768; i++***REMOVED*** {
      for (int j = 7; j >= 0; j--***REMOVED*** {
        if (screen[i] & (1 << j***REMOVED******REMOVED*** {
          Serial.write('#'***REMOVED***;
        } else {
          Serial.write('.'***REMOVED***;
        }
      }
      if (i % 12 == 11***REMOVED*** {
        Serial.println(***REMOVED***;
      }
    }
  }
}