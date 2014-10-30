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
	TICL(***REMOVED***
{
	return;
}

// Constructor with custom communication lines.
CBL2::CBL2(int tip, int ring***REMOVED*** :
	TICL(tip, ring***REMOVED***
{
	return;
}

int CBL2::getFromCBL2(uint8_t type, uint8_t* header, uint8_t* data, int* datalength, int maxlength***REMOVED*** {
	// Step 1: Send REQ, wait for ACK and VAR
	
	// Step 2: ACK VAR, send CTS
	
	// Step 3: Receive CTS ACK and DATA
	
	// Step 4: ACK DATA (do NOT perform EOT***REMOVED***
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
	return 0;
}

int CBL2::eventLoopTick(***REMOVED*** {
	uint8_t msg_header[4];
	int length;
	int rval;
	int endpoint = 0x12;

	if (!callback_init***REMOVED***
		return -1;
	
	// See if there's a message coming
	rval = get(msg_header, data_, &length, maxlength_***REMOVED***;
	if (rval***REMOVED*** {
		if (serial_***REMOVED*** {
			serial_->print("No incoming message: code "***REMOVED***;
			serial_->println(rval***REMOVED***;
		}
		return 0;			// No message coming
	}

	// Deduce what kind of operation is happening
	// CBL2 responds to TI-82 as 0x12, "0x95" endpoint as 0x15
	endpoint = (msg_header[0] == CALC82***REMOVED***?0x12:0x15;
	
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
			rval = get_callback_(header_[3], length***REMOVED***;		// Ignore rval for now	
			break;
	
		case EOT:
			// Send an ACK
			msg_header[0] = endpoint;
			msg_header[1] = ACK;
			msg_header[2] = msg_header[3] = 0x00;
			send(msg_header, NULL, 0***REMOVED***;
			break;
		
		case REQ:
			// Send an ACK
			msg_header[0] = endpoint;
			msg_header[1] = ACK;
			msg_header[2] = msg_header[3] = 0x00;
			send(msg_header, NULL, 0***REMOVED***;
			
			// Get the header and data from the callback
			send_callback_(header_[3], &datalength_***REMOVED***;
			
			// Send the VAR message
			msg_header[0] = endpoint;
			msg_header[1] = VAR;
			msg_header[2] = 0x0B;
			msg_header[3] = 0x00;
			send(msg_header, header_, 0x0B***REMOVED***;
			
			break;
			
		case CTS:
			// Send an ACK
			msg_header[0] = endpoint;
			msg_header[1] = ACK;
			msg_header[2] = msg_header[3] = 0x00;
			send(msg_header, NULL, 0***REMOVED***;
			
			// Send the DATA
			msg_header[0] = endpoint;
			msg_header[1] = VAR;
			msg_header[2] = (datalength_ & 0x00ff***REMOVED***;
			msg_header[3] = (datalength_ >> 8***REMOVED***;
			send(msg_header, data_, datalength_***REMOVED***;
			
			break;
	}
			
	return 0;
}