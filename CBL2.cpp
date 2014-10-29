/*************************************************
 *  CBL2.h - Library for emulating CBL2 devices  *
 *           or CBL-connected calculators with   *
 *           Arduinos.                           *
 *           Created by Christopher Mitchell,    *
 *           2011-2014, all rights reserved.     *
 *************************************************/

#include "Arduino.h"
#include "CBL2.h"

// Constructor with default communication lines
CBL2::CBL2(***REMOVED*** :
	TILP(***REMOVED***
{
	return;
}

// Constructor with custom communication lines. Fun
// fact: You can use this and multiple TILP objects to
// talk to multiple endpoints at the same time.
CBL2::CBL2(int tip, int ring***REMOVED*** :
	TILP(tip, ring***REMOVED***
{
	return;
}

int CBL2::getFromCBL2(uint8_t type, uint8_t* header, uint8_t* data, int* datalength, int maxlength***REMOVED*** {
	return -1;
}

int CBL2::sendToCBL2(uint8_t type, uint8_t* header, uint8_t* data, int datalength***REMOVED*** {
	return -1;
}

int CBL2::setupCallbacks(uint8_t* header, uint8_t* data, int maxlength,
				   int (*get_callback***REMOVED***(uint8_t, int***REMOVED***,
				   int (*send_callback***REMOVED***(uint8_t, int****REMOVED******REMOVED***
{
	header_ = header;
	data_ = data;
	maxlength_ = maxlength;
	get_callback_ = get_callback;
	send_callback_ = send_callback;
	callback_init = true;
}

int CBL2::eventLoopTick(***REMOVED*** {
	if (!callback_init***REMOVED***
		return -1;
		
	return -1;
}