#include "TILP.h"

TILP* tilp;
int lineRed = 7;
int lineWhite = 6;

void setup(***REMOVED*** {
  Serial.begin(9600***REMOVED***;
  tilp = new TILP(lineRed, lineWhite***REMOVED***;
  tilp->resetLines(***REMOVED***;
  tilp->setVerbosity(true, &Serial***REMOVED***;
}

void loop(***REMOVED*** {
  int rlen = 0;
  int rval = 0;
  uint8_t header[4] = {COMP83P, KEY, 0xA6, 0x00};
  rval = tilp->send(header, NULL, 0***REMOVED***;              // Send KEY message
  if (rval != 0***REMOVED*** {
    Serial.print("Send returned "***REMOVED***;
    Serial.println(rval***REMOVED***;
  } else {
    tilp->get(header, NULL, &rlen, 0***REMOVED***;             // Get ACK
    if (rval != 0***REMOVED*** {
      Serial.print("Get returned "***REMOVED***;
      Serial.println(rval***REMOVED***;
    }
    tilp->get(header, NULL, &rlen, 0***REMOVED***;              // Get key process notification
    if (rval != 0***REMOVED*** {
      Serial.print("Get returned "***REMOVED***;
      Serial.println(rval***REMOVED***;
    }
  }
  delay(500***REMOVED***;      // 2 'M's per second7
}

