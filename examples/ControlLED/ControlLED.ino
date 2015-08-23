/*************************************************
 *  ControlLED.ino                               *
 *  Example from the ArTICL library              *
 *           Created by Christopher Mitchell,    *
 *           2011-2014, all rights reserved.     *
 *                                               *
 *  This demo communicates as if it was a CBL2   *
 *  device. Use Send({0}***REMOVED*** to send a 1-element    *
 *  list to the Arduino and control the state    *
 *  of digital output lines. You can expand      *
 *  this demo to read or write any GPIO lines.   *
 *                                               *
 *  In its current state, send a value between   *
 *  0 and 15 as the single element of a 1-       *
 *  element list to control digital pins 8, 9,   *
 *  10, and 13 (on an Arduino***REMOVED*** or the four on-   *
 *  board LEDs (on the MSP432 Launchpad***REMOVED***.        *
 *************************************************/

#include "CBL2.h"
#include "TIVar.h"

CBL2* cbl;
const int lineRed = DEFAULT_TIP;
const int lineWhite = DEFAULT_RING;

#if defined(__MSP432P401R__***REMOVED***		// MSP432 target
#define LED_PIN_COUNT 4
const int ledPins[LED_PIN_COUNT] = {78, 75, 76, 77};
#else								// Arduino target
#define LED_PIN_COUNT 4
const int ledPins[LED_PIN_COUNT] = {8, 9, 10, 13};
#endif

#define MAXDATALEN 255
uint8_t header[16];
uint8_t data[MAXDATALEN];

void setup(***REMOVED*** {
  for(int i = 0; i < LED_PIN_COUNT; i++***REMOVED*** {
    pinMode(ledPins[i], OUTPUT***REMOVED***;
	digitalWrite(ledPins[i], LOW***REMOVED***;
  }

  Serial.begin(9600***REMOVED***;
  cbl = new CBL2(lineRed, lineWhite***REMOVED***;
  cbl->resetLines(***REMOVED***;
  // cbl->setVerbosity(true, &Serial***REMOVED***;			// Comment this in for mesage information
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
  
  // Turn the LEDs on or off
  int list_len = data[0] | (data[1] << 8***REMOVED***;
  if (list_len == 1***REMOVED*** {
    // It is a 1-element list now
	int value = (int***REMOVED***TIVar::realToFloat8x(&data[2], model***REMOVED***;
    Serial.print("Received value "***REMOVED***;
    Serial.println(value***REMOVED***;
    for(int i = 0; i < LED_PIN_COUNT; i++***REMOVED*** {
      digitalWrite(ledPins[i], (value >> i***REMOVED*** & 0x01***REMOVED***;
    }
  } else {
    Serial.println("Must send a 1-element list!"***REMOVED***;
  }
  return 0;
}

int onSendAsCBL2(uint8_t type, enum Endpoint model, int* headerlen,
                 int* datalen, data_callback* data_callback***REMOVED***
{
  Serial.print("Got request for variable of type "***REMOVED***;
  Serial.print(type***REMOVED***;
  Serial.print(" from endpoint of type "***REMOVED***;
  Serial.println((int***REMOVED***model***REMOVED***;
  return -1;
}