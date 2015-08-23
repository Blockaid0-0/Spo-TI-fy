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
 *  On the MSP432 Launchpad, it returns the      *
 *  values of A0, A1, A3, A4, A5, A6 instead.    *
 *************************************************/

#include "CBL2.h"
#include "TIVar.h"

CBL2* cbl;
const int lineRed = DEFAULT_TIP;
const int lineWhite = DEFAULT_RING;

#define MAXDATALEN 255
uint8_t header[16];
uint8_t data[MAXDATALEN];

#if defined(__MSP432P401R__***REMOVED***		// MSP432 target
#define ANALOG_PIN_COUNT 6
const int analogPins[ANALOG_PIN_COUNT] = {30, 29, 12, 33, 13, 28};

#else								// Arduino target
#define ANALOG_PIN_COUNT 6
const int analogPins[ANALOG_PIN_COUNT] = {0, 1, 2, 3, 4, 5};

#endif

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

int onGetAsCBL2(uint8_t type, enum Endpoint model, int datalen***REMOVED*** {
  Serial.print("Got variable of type "***REMOVED***;
  Serial.print(type***REMOVED***;
  Serial.print(" from endpoint of type "***REMOVED***;
  Serial.println((int***REMOVED***model***REMOVED***;
  return 0;
}

int onSendAsCBL2(uint8_t type, enum Endpoint model, int* headerlen,
                 int* datalen, data_callback* data_callback***REMOVED***
{
  Serial.print("Got request for variable of type "***REMOVED***;
  Serial.print(type***REMOVED***;
  Serial.print(" from endpoint of type "***REMOVED***;
  Serial.println((int***REMOVED***model***REMOVED***;
  
  if (type != 0x01***REMOVED***
    return -1;
  
  // Compose the VAR header
  *datalen = 2 + TIVar::sizeOfReal(model***REMOVED*** * 6;
  TIVar::intToSizeWord(*datalen, &header[0]***REMOVED***;	// Two bytes for the element count, 6 Reals
  header[1] = 0;
  header[2] = 0x04;
  header[3] = 0x01;
  header[4] = 0x41;				// See http://www.cemetech.net/forum/viewtopic.php?p=224739#224739
  header[5] = 0x00;
  *headerlen = 11;
  
  // Compose the body of the variable
  data[0] = 6;
  data[1] = 0;
  int offset = 2;
  for(int i = 0; i < ANALOG_PIN_COUNT; i++***REMOVED*** {
	long value = analogRead(analogPins[i]***REMOVED***;
	// Convert the value, get the length of the inserted data or -1 for failure
	int rval = TIVar::longToReal8x(value, &data[offset], model***REMOVED***;
	if (rval < 0***REMOVED*** {
		return -1;
	}
	offset += rval;
  }
  for(int i = 0; i < *datalen; i++***REMOVED*** {
    Serial.print(data[i], HEX***REMOVED***;
	Serial.print(" "***REMOVED***;
  }
  Serial.println("]]"***REMOVED***;

  return 0;
}