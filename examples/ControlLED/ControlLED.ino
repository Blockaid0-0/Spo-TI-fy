#include "CBL2.h"

CBL2* cbl;
int lineRed = 7;
int lineWhite = 6;
int ledPin = 13;

#define MAXDATALEN 255
uint8_t header[16];
uint8_t data[MAXDATALEN];

void setup(***REMOVED*** {
  Serial.begin(9600***REMOVED***;
  cbl = new CBL2(lineRed, lineWhite***REMOVED***;
  cbl->resetLines(***REMOVED***;
  cbl->setVerbosity(true, &Serial***REMOVED***;
  cbl->setupCallbacks(header, data, MAXDATALEN,
                      onGetAsCBL2, onSendAsCBL2***REMOVED***;
}

void loop(***REMOVED*** {
  int rval;
  if (rval = cbl->eventLoopTick(***REMOVED******REMOVED*** {
    Serial.print("Failed to run eventLoopTick: code "***REMOVED***;
    Serial.println(rval***REMOVED***;
  }
}

int onGetAsCBL2(uint8_t type, int datalen***REMOVED*** {
  Serial.print("Got variable of type "***REMOVED***;
  Serial.print(type***REMOVED***;
  Serial.println(" from calculator."***REMOVED***;
  return 0;
}

int onSendAsCBL2(uint8_t type, int* datalen***REMOVED*** {
  Serial.print("Got request for variable of type "***REMOVED***;
  Serial.print(type***REMOVED***;
  Serial.println(" from calculator."***REMOVED***;
  return -1;
}