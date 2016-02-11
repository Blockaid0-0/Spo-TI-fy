/*************************************************
 *  SimpleIO.ino                                 *
 *  Example from the ArTICL library              *
 *           Created by Christopher Mitchell,    *
 *           2011-2016, all rights reserved.     *
 *                                               *
 *  This demo communicates as if it was a CBL2   *
 *  device. Use Send({0}***REMOVED*** to send a 1-element    *
 *  list to the Arduino and control the state    *
 *  of digital output lines. You can expand      *
 *  this demo to read or write any GPIO lines.   *
 *                                               *
 *  In its current state, takes a 5-element list,*
 *  the elements of which respectively turn a    *
 *  red, green, and blue LED on and off (0 or 1***REMOVED***,*
 *  an extra data line on and off (used in our   *
 *  demo for a pink LED, but you can use it for  *
 *  anything you like***REMOVED***, and set a motor's speed  *
 *  (0-255***REMOVED***. A 2-element list can be requested   *
 *  from the Arduino, indicating the state of    *
 *  two digital inputs, meant to be connected    *
 *  to a pushbutton and an SPST switch,          *
 *  respectively.                                *
 *                                               *
 *  This example is intended to function out of  *
 *  the box with rfdave's Arduino globalCALCnet  *
 *  shield; see https://www.cemetech.net/forum/  *
 *  viewtopic.php?t=10694 . If you're using this *
 *  example with another shield or without a     *
 *  shield, remember to adjust lineRed and       *
 *  lineWhite.                                   *
 *************************************************/

#include "CBL2.h"
#include "TIVar.h"

CBL2 cbl;
const int lineRed = 7;
const int lineWhite = 6;

// Specify the pins for input and output
// 
#if defined(__MSP432P401R__***REMOVED***    // MSP432 target
#define LED_PIN_R 75
#define LED_PIN_G 76
#define LED_PIN_B 77
#define LED_PIN_EXTRA 77
#define MOTOR_PIN 11
#define BUTTON_PIN 73
#define SWITCH_PIN 12
#else                           // Arduino target
#define LED_PIN_R 8
#define LED_PIN_G 9
#define LED_PIN_B 10
#define LED_PIN_EXTRA 4
#define MOTOR_PIN 5
#define BUTTON_PIN 3
#define SWITCH_PIN 2
#endif

// Lists are 2 + (9 * dimension***REMOVED*** bytes,
// so incidentally a 255-byte max data length
// limits this demo's lists to 28 elements.
#define MAXDATALEN 255

uint8_t header[16];
uint8_t data[MAXDATALEN];

int onSendAsCBL2(uint8_t type, enum Endpoint model, int* headerlen,
                 int* datalen, data_callback* data_callback***REMOVED***;
int onGetAsCBL2(uint8_t type, enum Endpoint model, int datalen***REMOVED***;

void setup(***REMOVED*** {
  // put your setup code here, to run once:
  pinMode(LED_PIN_R, OUTPUT***REMOVED***;
  pinMode(LED_PIN_G, OUTPUT***REMOVED***;
  pinMode(LED_PIN_B, OUTPUT***REMOVED***;
  pinMode(LED_PIN_EXTRA, OUTPUT***REMOVED***;
  pinMode(MOTOR_PIN, OUTPUT***REMOVED***;
  pinMode(BUTTON_PIN, INPUT***REMOVED***;
  pinMode(SWITCH_PIN, INPUT***REMOVED***;

  digitalWrite(LED_PIN_R, LOW***REMOVED***;
  digitalWrite(LED_PIN_G, LOW***REMOVED***;
  digitalWrite(LED_PIN_B, LOW***REMOVED***;
  digitalWrite(LED_PIN_EXTRA, LOW***REMOVED***;
  analogWrite(MOTOR_PIN, 255***REMOVED***;      // Active low
  digitalWrite(BUTTON_PIN, HIGH***REMOVED***;   // Pull-up resistor
  digitalWrite(SWITCH_PIN, HIGH***REMOVED***;   // Pull-up resistor

  Serial.begin(9600***REMOVED***;                           // Used for debugging
  cbl.setLines(lineRed, lineWhite***REMOVED***;
  cbl.resetLines(***REMOVED***;
  // cbl.setVerbosity(true, &Serial***REMOVED***;      // Comment this in for verbose message information
  
  // The following registers buffers for exchanging data, the maximum
  // allowed data length, and functions to call on Get(***REMOVED*** and Send(***REMOVED***.
  cbl.setupCallbacks(header, data, MAXDATALEN,
                      onGetAsCBL2, onSendAsCBL2***REMOVED***;
}

// Repeatedly check to see if the calculator has initiated a Get(***REMOVED***
// or a Send(***REMOVED*** operation yet. If it has, then onGetAsCBL2(***REMOVED*** or
// onSendAsCBL2(***REMOVED*** will be invoked, since they were registered in
// setup(***REMOVED*** above.
void loop(***REMOVED*** {
  int rval;
  rval = cbl.eventLoopTick(***REMOVED***;
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
  if (type != VarTypes82::VarRList && type != VarTypes82::VarURList &&
      type != VarTypes84PCSE::VarRList***REMOVED***
  {
    return -1;
  }

  // Turn the LEDs and motor on or off
  uint16_t list_len = TIVar::sizeWordToInt(&(data[0]***REMOVED******REMOVED***;    // Convert 2-byte size word to int
  if (list_len == 5***REMOVED*** {
    // It is indeed a 5-element list
    int size_of_real = TIVar::sizeOfReal(model***REMOVED***;
    int val_red   = TIVar::realToLong8x(&data[size_of_real * 0 + 2], model***REMOVED***; // First list element starts after 2-byte size word
    int val_green = TIVar::realToLong8x(&data[size_of_real * 1 + 2], model***REMOVED***;
    int val_blue  = TIVar::realToLong8x(&data[size_of_real * 2 + 2], model***REMOVED***;
    int val_extra = TIVar::realToLong8x(&data[size_of_real * 3 + 2], model***REMOVED***;
    int val_motor = TIVar::realToLong8x(&data[size_of_real * 4 + 2], model***REMOVED***;

    digitalWrite(LED_PIN_R, val_red***REMOVED***;
    digitalWrite(LED_PIN_G, val_green***REMOVED***;
    digitalWrite(LED_PIN_B, val_blue***REMOVED***;
    digitalWrite(LED_PIN_EXTRA, val_extra***REMOVED***;
    analogWrite(MOTOR_PIN, val_motor***REMOVED***;
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
  
  if (type != VarTypes82::VarRList***REMOVED***
    return -1;
  
  // Compose the VAR header
  *datalen = 2 + TIVar::sizeOfReal(model***REMOVED*** * 2;
  TIVar::intToSizeWord(*datalen, &header[0]***REMOVED***;  // Two bytes for the element count, ANALOG_PIN_COUNT Reals
                                                // This sets header[0] and header[1]
  header[2] = VarTypes85::VarRList;             // RealList (if you're a TI-85. Bleh.***REMOVED***
  header[3] = 0x01;                // Name length
  header[4] = 0x41;                // "A", as per "standard" See http://www.cemetech.net/forum/viewtopic.php?p=224739#224739
  header[5] = 0x00;                // Zero terminator (remainder of header is ignored***REMOVED***
  *headerlen = 11;
  
  // Compose the body of the variable
  data[0] = 2;   // Little-endian word for number of
  data[1] = 0;            // elements in this list
  int offset = 2;         // Offset past the count word

  // Convert the value, get the length of the inserted data or -1 for failure
  int rval;
  rval = TIVar::longToReal8x(digitalRead(BUTTON_PIN***REMOVED***, &data[offset], model***REMOVED***;
  if (rval < 0***REMOVED*** {
    return -1;
  }
  offset += rval;
  rval = TIVar::longToReal8x(digitalRead(SWITCH_PIN***REMOVED***, &data[offset], model***REMOVED***;
  if (rval < 0***REMOVED*** {
    return -1;
  }
  offset += rval;

  return 0;
}