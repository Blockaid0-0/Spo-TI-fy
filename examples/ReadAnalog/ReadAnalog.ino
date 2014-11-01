/*************************************************
 *  ReadAnalog.ino                               *
 *  Example from the ArTICL library              *
 *           Created by Christopher Mitchell,    *
 *           2011-2014, all rights reserved.     *
 *                                               *
 *  This demo reads the Arduino's six analog     *
 *  pins whenever the calculator requests a      *
 *  list, and returns the results as a six-      *
 *  element list with values between 0 and 1023. *
 *************************************************/

#include "CBL2.h"
#include "TIVar.h"

CBL2* cbl;
int lineRed = 7;
int lineWhite = 6;

#define MAXDATALEN 255
uint8_t header[16];
uint8_t data[MAXDATALEN];

void setup(***REMOVED*** {
  Serial.begin(9600***REMOVED***;
  cbl = new CBL2(lineRed, lineWhite***REMOVED***;
  cbl->resetLines(***REMOVED***;
  cbl->setVerbosity(true, &Serial***REMOVED***;			// Comment this in for mesage information
  cbl->setupCallbacks(header, data, MAXDATALEN,
                      onGetAsCBL2, onSendAsCBL2***REMOVED***;
}

void loop(***REMOVED*** {
  int rval;
  rval = cbl->eventLoopTick(***REMOVED***;
  if (rval && rval != ERR_READ_TIMEOUT***REMOVED*** {
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
  
  if (type != 0x01***REMOVED***
    return -1;
  
  // Compose the VAR header
  *datalen = 2 + 9 * 6;
  header[0] = *datalen;			// Two bytes for the element count, 6 9-byte reals
  header[1] = 0;
  header[2] = 0x04;
  header[3] = 0x01;
  header[4] = 0x41;				// See http://www.cemetech.net/forum/viewtopic.php?p=224739#224739
  header[5] = 0x00;
  
  // Compose the body of the variable
  data[0] = 6;
  data[1] = 0;
  for(int i = 0; i < 6; i++***REMOVED*** {
	float value = analogRead(i***REMOVED***;
	TIVar::floatToReal8x(value, &data[2 + 9 * i]***REMOVED***;
  }
  for(int i = 0; i < *datalen; i++***REMOVED*** {
    Serial.print(data[i], HEX***REMOVED***;
	Serial.print(" "***REMOVED***;
  }
  Serial.println("]]"***REMOVED***;

  return 0;
}