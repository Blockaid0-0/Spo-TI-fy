/*************************************************
 *  TypeLetter.ino                               *
 *  Example from the ArTICL library              *
 *           Created by Christopher Mitchell,    *
 *           2011-2015, all rights reserved.     *
 *                                               *
 *  This demo communicates use the silent-       *
 *  linking commands to type the letter M on a   *
 *  connected calculator.                        *
 *************************************************/

#include "TICL.h"

TICL* ticl;
int lineRed = DEFAULT_TIP;
int lineWhite = DEFAULT_RING;

void setup(***REMOVED*** {
  Serial.begin(9600***REMOVED***;
  ticl = new TICL(lineRed, lineWhite***REMOVED***;
  ticl->resetLines(***REMOVED***;
  ticl->setVerbosity(true, &Serial***REMOVED***;
}

void loop(***REMOVED*** {
  int rlen = 0;
  int rval = 0;
  uint8_t header[4] = {COMP83P, KEY, 0xA6, 0x00};
  rval = ticl->send(header, NULL, 0***REMOVED***;              // Send KEY message
  if (rval != 0***REMOVED*** {
    Serial.print("Send returned "***REMOVED***;
    Serial.println(rval***REMOVED***;
  } else {
    rval = ticl->get(header, NULL, &rlen, 0***REMOVED***;             // Get ACK
    if (rval != 0***REMOVED*** {
      Serial.print("Get returned "***REMOVED***;
      Serial.println(rval***REMOVED***;
    } else {
      rval = ticl->get(header, NULL, &rlen, 0***REMOVED***;              // Get key process notification
      if (rval != 0***REMOVED*** {
        Serial.print("Get returned "***REMOVED***;
        Serial.println(rval***REMOVED***;
      }
	}
  }
  delay(500***REMOVED***;      // 2 'M's per second
}

