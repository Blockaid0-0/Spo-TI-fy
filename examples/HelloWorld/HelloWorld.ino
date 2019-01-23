/*************************************************
 *  HelloWorld.ino                               *
 *  Example from the ArTICL library              *
 *           Created by Alex Cordonnier, 2017    *
 *                                               *
 *  Use Get(Str1***REMOVED*** to request a string from the   *
 *  Arduino. Use Send(Str1***REMOVED*** to send a string to  *
 *  the Arduino.                                 *
 *************************************************/

#include "CBL2.h"
#include "TIVar.h"

CBL2 cbl;
const int lineRed = DEFAULT_TIP;
const int lineWhite = DEFAULT_RING;

#define MAXDATALEN 255

uint8_t header[16];
uint8_t data[MAXDATALEN];

void setup(***REMOVED*** {
    Serial.begin(9600***REMOVED***;
    cbl.setLines(lineRed, lineWhite***REMOVED***;
    cbl.resetLines(***REMOVED***;
    cbl.setupCallbacks(header, data, MAXDATALEN, onReceived, onRequest***REMOVED***;
}

void loop(***REMOVED*** {
    cbl.eventLoopTick(***REMOVED***;
}

int onReceived(uint8_t type, enum Endpoint model, int datalen***REMOVED*** {
    if (type != VarTypes82::VarString***REMOVED*** {
        return -1; // Can only accept strings
    }

    String str = TIVar::strVarToString8x(data, model***REMOVED***;
    Serial.println("Received: " + str***REMOVED***;
    return 0;
}

int onRequest(uint8_t type, enum Endpoint model, int* headerlen,
              int* datalen, data_callback* data_callback***REMOVED*** {
    if (type != VarTypes82::VarString***REMOVED*** {
        return -1; // Can only return strings
    }

    String hello = "Hello, world! :***REMOVED***";
    int rval = TIVar::stringToStrVar8x(hello, data, model***REMOVED***;
    if (rval < 0***REMOVED*** {
        return -1;
    }
    *datalen = rval;

    memset(header, 0, sizeof(header***REMOVED******REMOVED***;
    TIVar::intToSizeWord(rval, header***REMOVED***;
    header[2] = VarTypes82::VarString; // Variable type
    header[3] = 0xAA; // Variable name (Str1***REMOVED***
    header[4] = 0x00; // ^
    *headerlen = 13;

    Serial.println("Sending: " + hello***REMOVED***;

    return 0;
}