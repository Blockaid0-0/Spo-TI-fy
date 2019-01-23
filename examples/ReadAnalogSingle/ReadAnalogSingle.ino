/*************************************************
 *  ReadAnalogSingle.ino                         *
 *  Example from the ArTICL library              *
 *           Created by Christopher Mitchell,    *
 *           2011-2019, all rights reserved.     *
 *************************************************/
#include "CBL2.h"
#include "TIVar.h"

#define MAXDATALEN 255

uint8_t header[16];
uint8_t data[MAXDATALEN];

CBL2 cbl;
const int lineRed = 7;
const int lineWhite = 6;

// Forward declaration of onRequest(***REMOVED*** and onReceived(***REMOVED*** functions
int onReceived(uint8_t type, enum Endpoint model, int datalen***REMOVED***;
int onRequest(uint8_t type, enum Endpoint model, int* headerlen,
              int* datalen, data_callback* data_callback***REMOVED***;

void setup(***REMOVED*** {
    Serial.begin(9600***REMOVED***;
    cbl.setLines(lineRed, lineWhite***REMOVED***;
    cbl.resetLines(***REMOVED***;
    //cbl.setVerbosity(true, &Serial***REMOVED***;      // Comment this in for message information
    cbl.setupCallbacks(header, data, MAXDATALEN, onReceived, onRequest***REMOVED***;
}

void loop(***REMOVED*** {
    cbl.eventLoopTick(***REMOVED***;
}

int onReceived(uint8_t type, enum Endpoint model, int datalen***REMOVED*** {
    // do nothing
    return 0;
}

// This is triggered when the attached calculator calls Get(***REMOVED***
int onRequest(uint8_t type, enum Endpoint model, int* headerlen,
              int* datalen, data_callback* data_callback***REMOVED***
{
    if (type != VarTypes82::VarReal***REMOVED*** {
        Serial.println("Received request for invalid data type"***REMOVED***;
        return -1; // Can only return a real
    }

	// This is the value that we're going to send back
    int val = analogRead(0***REMOVED***;
	// Convert it into a TI-formatted real number for the correct type of calculator,
	// and get back how many bytes that real number takes up. We convert it directly
	// into the body of the packet we'll be returning to the calculator (data***REMOVED***.
    *datalen = TIVar::longToReal8x((long long int***REMOVED***val, data, model***REMOVED***;

	// Clear the header to be sent, then fill in the size of the real number
    memset(header, 0, sizeof(header***REMOVED******REMOVED***;
    TIVar::intToSizeWord(*datalen, header***REMOVED***;
	// Adjust the variable type and name
    header[2] = VarTypes82::VarReal; // Variable type
    header[3] = 'A'; // variable A
    header[4] = 0x00; // pointless zero termination
    *headerlen = 13;

	// ArTICL will take it from here.
    return 0;
}