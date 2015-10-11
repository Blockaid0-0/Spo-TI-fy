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

// Specify the number of LEDs to control, and to which
// digital pin each LED is attached.
#if defined(__MSP432P401R__***REMOVED***		// MSP432 target
#define LED_PIN_COUNT 4
const int ledPins[LED_PIN_COUNT] = {78, 75, 76, 77};
#else								// Arduino target
#define LED_PIN_COUNT 4
const int ledPins[LED_PIN_COUNT] = {8, 9, 10, 13};
#endif

// Lists are 2 + (9 * dimension***REMOVED*** bytes,
// so incidentally a 255-byte max data length
// limits this demo's lists to 28 elements.
#define MAXDATALEN 255

uint8_t header[16];
uint8_t data[MAXDATALEN];

// Run once when the Arduino resets. This first sets
// the LED pins to OUTPUT, then initializes a new CBL2
// instance. This is responsible for dealing with the
// actual calculator<->Arduino message exchange.
void setup(***REMOVED*** {
  for(int i = 0; i < LED_PIN_COUNT; i++***REMOVED*** {
    pinMode(ledPins[i], OUTPUT***REMOVED***;
	digitalWrite(ledPins[i], LOW***REMOVED***;
  }

  Serial.begin(9600***REMOVED***;                           // Used for debugging
  cbl = new CBL2(lineRed, lineWhite***REMOVED***;
  cbl->resetLines(***REMOVED***;
  // cbl->setVerbosity(true, &Serial***REMOVED***;			// Comment this in for verbose message information
  
  // The following registers buffers for exchanging data, the maximum
  // allowed data length, and functions to call on Get(***REMOVED*** and Send(***REMOVED***.
  cbl->setupCallbacks(header, data, MAXDATALEN,
                      onGetAsCBL2, onSendAsCBL2***REMOVED***;
}

// Repeatedly check to see if the calculator has initiated a Get(***REMOVED***
// or a Send(***REMOVED*** operation yet. If it has, then onGetAsCBL2(***REMOVED*** or
// onSendAsCBL2(***REMOVED*** will be invoked, since they were registered in
// setup(***REMOVED*** above.
void loop(***REMOVED*** {
  int rval;
  rval = cbl->eventLoopTick(***REMOVED***;
  if (rval && rval != ERR_READ_TIMEOUT***REMOVED*** {
    Serial.print("Failed to run eventLoopTick: code "***REMOVED***;
    Serial.println(rval***REMOVED***;
  }
}

// Callback when the CBL2 class has successfully received a variable
// from the attached calculator.
int onGetAsCBL2(uint8_t type, enum Endpoint model, int datalen***REMOVED*** {
  Serial.print("Got variable of type "***REMOVED***;
  Serial.print(type***REMOVED***;
  Serial.print(" from endpoint of type "***REMOVED***;
  Serial.println((int***REMOVED***model***REMOVED***;
  
  // We only want to handle lists.
  if (type != VarTypes82::VarRList***REMOVED***
    return -1;

  // Turn the LEDs on or off
  uint16_t list_len = sizeWordToInt(&(data[0]***REMOVED******REMOVED***;			// Convert 2-byte size word to int
  if (list_len == 1***REMOVED*** {
    // It is indeed a 1-element list
	int value = (int***REMOVED***TIVar::realToFloat8x(&data[2], model***REMOVED***;	// First list element starts after 2-byte size word
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

// Callback when the CBL2 class notices the attached calculator
// wants to start a Get(***REMOVED*** exchange. The CBL2 class needs to get
// any data to send before continuing the exchange.
int onSendAsCBL2(uint8_t type, enum Endpoint model, int* headerlen,
                 int* datalen, data_callback* data_callback***REMOVED***
{
  Serial.print("Got request for variable of type "***REMOVED***;
  Serial.print(type***REMOVED***;
  Serial.print(" from endpoint of type "***REMOVED***;
  Serial.println((int***REMOVED***model***REMOVED***;
  return -1;		// -1 means we have no data to send.
}