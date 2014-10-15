#include "TILP.h"

int lineRed = 2;
int lineWhite = 3;

TILP tilp = TILP(lineRed, lineWhite***REMOVED***

void setup(***REMOVED*** {
	tilp.begin(***REMOVED***;
}

void loop(***REMOVED*** {
	uint8_t header[4] = {COMP83P, KEY, 0xA6, 0x00};
	int rlen = 0;
	tilp.send(header, NULL, 0***REMOVED***;
	tilp.get(header, NULL, &rlen, 0***REMOVED***;
}