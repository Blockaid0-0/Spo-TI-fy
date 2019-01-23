/*************************************************
 *  WhackAMole.ino                               *
 *  Example from the ArTICL library              *
 *           Created by Timothy Keller,          *
 *           With Christopher Mitchell,          *
 *           2011-2015, all rights reserved.     *
 *                                               *
 *  This demo allows interfacing with a MSP432   *
 *  device designed as a Whack A Mole Board, by  *
 *  emulating a CBL2 Style API.                  *
 *  Pins 2 and 23-30 are connected to CDS cells  *
 *  that act as our "buttons" for this demo.     *
 *                                               *
 *************************************************/
 
#include <CBL2.h>
#include <TIVar.h>

//#define VERBOSE

CBL2* cbl;
const int lineRed = 4;
const int lineWhite = 3;

#define ANALOG_PIN_COUNT 9
const int analogPins[ANALOG_PIN_COUNT] = {30, 29, 28, 27, 26, 25, 24, 23, 2};

#define RGB_RED 40
#define RGB_GREEN 39
#define RGB_BLUE 38
#define MULTIPLEXER_PIN_COUNT 9
const int multiplexer_pins[MULTIPLEXER_PIN_COUNT] = {37, 36, 35, 34, 33, 32, 31, 11, 12};

#define MAXDATALEN 255
uint8_t header[16];
uint8_t data[MAXDATALEN];

// Forward function definitions.
int onGetAsCBL2(uint8_t type, enum Endpoint model, int datalen***REMOVED***;
int onSendAsCBL2(uint8_t type, enum Endpoint model, int* headerlen,
                 int* datalen, data_callback* data_callback***REMOVED***;

// Set up serial for debugging, and CBL2 for communication
void setup(***REMOVED*** {
	Serial.begin(9600***REMOVED***;
	cbl = new CBL2(lineRed, lineWhite***REMOVED***;
	cbl->resetLines(***REMOVED***;
#ifdef VERBOSE
	cbl->setVerbosity(true, &Serial***REMOVED***;  // Comment this in for message information
#endif
	cbl->setupCallbacks(header, data, MAXDATALEN, onGetAsCBL2, onSendAsCBL2***REMOVED***;                      
}

// Main loop: Let CBL2 event handler do the work
void loop(***REMOVED*** {
	int rval;
	rval = cbl->eventLoopTick(***REMOVED***;
	if (rval && rval != ERR_READ_TIMEOUT***REMOVED*** {
		Serial.print("Failed to run eventLoopTick: code "***REMOVED***;
		Serial.println(rval***REMOVED***;
	}
	
}

// Takes Led = LED 1-9
void SetRgbActive(int led***REMOVED*** {
	turnOffAllRGBLEDs(***REMOVED***;
	digitalWrite(multiplexer_pins[led - 1], HIGH***REMOVED***;
}

void turnOffAllRGBLEDs(***REMOVED*** {
	for(int i = 0; i < MULTIPLEXER_PIN_COUNT; i++***REMOVED*** {
		digitalWrite(multiplexer_pins[i], LOW***REMOVED***;
	}
}

int onGetAsCBL2(uint8_t type, enum Endpoint model, int datalen***REMOVED*** {
#ifdef VERBOSE
	Serial.print("Got variable of type "***REMOVED***;
	Serial.print(type***REMOVED***;
	Serial.print(" from endpoint of type "***REMOVED***;
	Serial.println((int***REMOVED***model***REMOVED***;
#endif
	
	if (type != 0x5D***REMOVED*** { //VarTypes82::VarRList***REMOVED*** ???
		return -1;  //If you are not a list we do not want you ABORT
	}
	int list_len = data[0] | (data[1] << 8***REMOVED***;
	switch(list_len***REMOVED*** {
		case 1: {  //If we are a control structure
			int value = (int***REMOVED***TIVar::realToFloat8x(&data[2], model***REMOVED***;  // Get element
			switch(value***REMOVED*** {
				case 0:{  // Turns off all LEDS
#ifdef VERBOSE
					Serial.println("Turning off all LEDs"***REMOVED***;
#endif
					turnOffAllRGBLEDs(***REMOVED***;
					return 0;
				}
				case 10:  // Unimplemented yet. But room to grow for single commands is here. 
				case 11:  //
				default:
					return -1;
			}
		}
		case 4: {  // RGB Control structure
		           // {N,R,G,B} Where N is LED and RGB=Analog Values for each color
			turnOffAllRGBLEDs(***REMOVED***;  //First we turn off the multiplexer
				
			int offset = 2;  //Start us after the size bytes
			int new_led = (int***REMOVED***TIVar::realToFloat8x(&data[offset], model***REMOVED***;  // Get element
				
			offset += TIVar::sizeOfReal(model***REMOVED***;
			int val_red = (int***REMOVED***TIVar::realToFloat8x(&data[offset], model***REMOVED***;
			offset += TIVar::sizeOfReal(model***REMOVED***;
			int val_green = (int***REMOVED***TIVar::realToFloat8x(&data[offset], model***REMOVED***;
			offset += TIVar::sizeOfReal(model***REMOVED***;
			int val_blue = (int***REMOVED***TIVar::realToFloat8x(&data[offset], model***REMOVED***;
				
#ifdef VERBOSE
			Serial.print("Setting LED "***REMOVED***;
			Serial.print(new_led***REMOVED***;
			Serial.print(" to color ("***REMOVED***;
			Serial.print(val_red***REMOVED***;
			Serial.print(','***REMOVED***;
			Serial.print(val_green***REMOVED***;
			Serial.print(','***REMOVED***;
			Serial.print(val_blue***REMOVED***;
			Serial.println("***REMOVED***"***REMOVED***;
#endif
			// I don't know why this is inverted. It works.
			analogWrite(RGB_RED, 255 - val_red***REMOVED***;
			analogWrite(RGB_GREEN, 255 - val_green***REMOVED***;
			analogWrite(RGB_BLUE, 255 - val_blue***REMOVED***;
				
			SetRgbActive(new_led***REMOVED***;  //Turn on the new LED
			return 0;
		}    
		default: {  // Gotta Catch Em All!
				return -1;
		}    
	}
	
	return -1;
}

int onSendAsCBL2(uint8_t type, enum Endpoint model, int* headerlen,
                 int* datalen, data_callback* data_callback***REMOVED***
{
	Serial.print("Got request for variable of type "***REMOVED***;
	Serial.print(type***REMOVED***;
	Serial.print(" from endpoint of type "***REMOVED***;
	Serial.println((int***REMOVED***model***REMOVED***;

	if (type != VarTypes82::VarRList***REMOVED*** {
		return -1; //If we are not a list we do not want you ABORT
	}

	// Compose the VAR header
	*datalen = 2 + TIVar::sizeOfReal(model***REMOVED*** * ANALOG_PIN_COUNT;
	TIVar::intToSizeWord(*datalen, &header[0]***REMOVED***;	// Two bytes for the element count, ANALOG_PIN_COUNT Reals
	                                            // This sets header[0] and header[1]
	header[2] = 0x04;				// RealList (if you're a TI-85. Bleh.***REMOVED***
	header[3] = 0x01;				// Name length
	header[4] = 0x41;				// "A", as per "standard" See http://www.cemetech.net/forum/viewtopic.php?p=224739#224739
	header[5] = 0x00;				// Zero terminator (remainder of header is ignored***REMOVED***
	*headerlen = 11;
	
	// Compose the body of the variable
	data[0] = ANALOG_PIN_COUNT;
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
