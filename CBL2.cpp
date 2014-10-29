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
	uint8_t msg_header[4];
	int length;
	int rval;
	int endpoint = 0x12;

	if (!callback_init***REMOVED***
		return -1;
	
	// See if there's a message coming
	int rval = get(msg_header, data_, length, maxlength_***REMOVED***;
	if (rval***REMOVED***
		return;			// No message coming
		
	// Deduce what kind of operation is happening
	// CBL2 responds to TI-82 as 0x12, "0x95" endpoint as 0x15
	endpoint = (msg_header[0] == COMP82***REMOVED***?0x12:0x15;
	
	// Now deal with the message
	switch(msg_header[1]***REMOVED*** {
		case ACK:
			break;						// Drop ACKs on the floor

		case RTS:
			memcpy(header_, data_, length***REMOVED***;		// Save the variable header
			
			// Send an ACK
			msg_header[0] = endpoint;
			msg_header[1] = ACK;
			msg_header[2] = msg_header[3] = 0x00;
			send(msg_header, NULL, 0***REMOVED***;
			
			// Send a CTS
			msg_header[0] = endpoint;
			msg_header[1] = CTS;
			msg_header[2] = msg_header[3] = 0x00;
			send(msg_header, NULL, 0***REMOVED***;
			
			break;
		
		case DATA:
			// Send an ACK
			msg_header[0] = endpoint;
			msg_header[1] = ACK;
			msg_header[2] = msg_header[3] = 0x00;
			send(msg_header, NULL, 0***REMOVED***;
			
			// Deliver the data to the callback
			rval = get_callback_(header_[1], length***REMOVED***;		// Ignore rval for now	
			break;
	
		case EOT:
			// Send an ACK
			msg_header[0] = endpoint;
			msg_header[1] = ACK;
			msg_header[2] = msg_header[3] = 0x00;
			send(msg_header, NULL, 0***REMOVED***;
			break;
			
	return -1;
}