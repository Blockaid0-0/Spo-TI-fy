/*************************************************
 *  CBL2.h - Library for emulating CBL2 devices  *
 *           or CBL-connected calculators with   *
 *           Arduinos.                           *
 *           Created by Christopher Mitchell,    *
 *           2011-2014, all rights reserved.     *
 *************************************************/

#include "Arduino.h"
#include "CBL2.h"
#include "TIVar.h"

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
	uint8_t msg_header[4];
	uint8_t endpoint = (type == 0x01***REMOVED***?CALC85b:CALC82;	// CALC82 for strings and other types, CALC85b for lists
	int length;
	
	// Step 1: Send REQ, wait for ACK and VAR
	// We will assume that the CBL2 can use 11-byte (TI-82/TI-83/TI-85-style***REMOVED***
	// variable headers when we send messages with a CALC82 endpoint
	msg_header[0] = endpoint;
	msg_header[1] = REQ;
	TIVar::intToSizeWord(11, &msg_header[2]***REMOVED***;
	send(msg_header, header, 11***REMOVED***;
	
	if (get(msg_header, NULL, &length, 0***REMOVED*** || msg_header[1] != ACK***REMOVED*** {
		// Either the message was not an ACK, or we didn't even get a message
		return -1;
	}
	
	if (get(msg_header, header, &length, 11***REMOVED*** || msg_header[1] != VAR***REMOVED*** {
		// Either the message was not a VAR, or we didn't even get a message
		return -1;
	}

	// Step 2: ACK VAR, send CTS
	msg_header[0] = endpoint;
	msg_header[1] = ACK;
	msg_header[2] = msg_header[3] = 0;
	send(msg_header, NULL, 0***REMOVED***;
	
	msg_header[1] = CTS;
	send(msg_header, NULL, 0***REMOVED***;

	// Step 3: Receive CTS ACK and DATA
	if (get(msg_header, NULL, &length, 0***REMOVED*** || msg_header[1] != ACK***REMOVED*** {
		// Either the message was not an ACK, or we didn't even get a message
		return -1;
	}

	if (get(msg_header, NULL, datalength, maxlength***REMOVED*** || msg_header[1] != DATA***REMOVED*** {
		// Either the message was not a DATA, or we didn't even get a message
		return -1;
	}
	
	// Step 4: ACK DATA (do NOT perform EOT***REMOVED***
	msg_header[0] = endpoint;
	msg_header[1] = ACK;
	msg_header[2] = msg_header[3] = 0;
	return send(msg_header, NULL, 0***REMOVED***;
}

int CBL2::sendToCBL2(uint8_t type, uint8_t* header, uint8_t* data, int datalength***REMOVED*** {
	uint8_t msg_header[4];
	uint8_t endpoint = (type == 0x01***REMOVED***?CALC85b:CALC82;	// CALC82 for strings and other types, CALC85b for lists
	int length;

	// Step 1: Send RTS, wait for RTS ACK
	// We will assume that the CBL2 can use 11-byte (TI-82/TI-83/TI-85-style***REMOVED***
	// variable headers when we send messages with a CALC82 endpoint
	msg_header[0] = endpoint;
	msg_header[1] = RTS;
	msg_header[2] = 11;
	msg_header[3] = 0;
	send(msg_header, header, 11***REMOVED***;
	
	if (get(msg_header, NULL, &length, 0***REMOVED*** || msg_header[1] != ACK***REMOVED*** {
		// Either the message was not an ACK, or we didn't even get a message
		return -1;
	}

	// Step 2: Wait for CTS, send CTS ACK
	if (get(msg_header, NULL, &length, 0***REMOVED*** || msg_header[1] != CTS***REMOVED*** {
		// Either the message was not a CTS, or we didn't even get a message
		return -1;
	}
	
	msg_header[0] = endpoint;
	msg_header[1] = ACK;
	msg_header[2] = msg_header[3] = 0;
	send(msg_header, NULL, 0***REMOVED***;

	// Step 3: Send DATA, wait for DATA ACK
	msg_header[0] = endpoint;
	msg_header[1] = DATA;
	TIVar::intToSizeWord(11, &msg_header[2]***REMOVED***;
	send(msg_header, data, datalength***REMOVED***;
	
	if (get(msg_header, NULL, &length, 0***REMOVED*** || msg_header[1] != ACK***REMOVED*** {
		// Either the message was not an ACK, or we didn't even get a message
		return -1;
	}

	// Step 4: Send EOT and wait for EOT ACK
	msg_header[0] = endpoint;
	msg_header[1] = EOT;
	msg_header[2] = msg_header[3] = 0;
	send(msg_header, NULL, 0***REMOVED***;
	
	if (get(msg_header, NULL, &length, 0***REMOVED*** || msg_header[1] != ACK***REMOVED*** {
		// Either the message was not an ACK, or we didn't even get a message
		return -1;
	}
	return 0;
}

int CBL2::setupCallbacks(uint8_t* header, uint8_t* data, int maxlength,
				   int (*get_callback***REMOVED***(uint8_t, enum Endpoint, int***REMOVED***,
				   int (*send_callback***REMOVED***(uint8_t, enum Endpoint, int*, int*, data_callback****REMOVED******REMOVED***
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
	int endpoint = CBL82;

	if (!callback_init***REMOVED***
		return -1;
	
	// See if there's a message coming
	rval = get(msg_header, data_, &length, maxlength_***REMOVED***;
	if (rval***REMOVED*** {
		if (serial_***REMOVED*** {
			serial_->print("No msg: code "***REMOVED***;
			serial_->println(rval***REMOVED***;
		}
		return 0;			// No message coming
	}

	// Deduce what kind of operation is happening
	// CBL2 responds to TI-82 as 0x12, "0x95" endpoint as 0x15
	enum Endpoint model = (enum Endpoint***REMOVED***msg_header[0];
	bool type_as_cbl = true;
	switch(model***REMOVED*** {
		case CALC82:
			endpoint = CBL82;
			break;
		case CALC85a:
		case CALC85b:
			endpoint = CBL85;
			break;
		case CALC89:
			endpoint = CBL89;
			break;
		case COMP83P:
			type_as_cbl = false;
			endpoint = CALC83P;
			break;
		default:
			return -1;				// Unknown endpoint
	};
	
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
			rval = get_callback_(header_[3], model, length***REMOVED***;	// Ignore rval for now	
			break;
	
		case EOT:
			// Send an ACK
			msg_header[0] = endpoint;
			msg_header[1] = ACK;
			msg_header[2] = msg_header[3] = 0x00;
			send(msg_header, NULL, 0***REMOVED***;
			break;
		
		case REQ: {
			memcpy(header_, data_, length***REMOVED***;		// Save the variable header

			// Send an ACK
			msg_header[0] = endpoint;
			msg_header[1] = ACK;
			msg_header[2] = msg_header[3] = 0x00;
			send(msg_header, NULL, 0***REMOVED***;
			
			// Get the header and data from the callback
			data_callback_ = NULL;
			int headerlength = 11;
			send_callback_(header_[type_as_cbl ? 3 : 2], model,
			               &headerlength, &datalength_, &data_callback_***REMOVED***;
			
			// Send the VAR message
			msg_header[0] = endpoint;
			msg_header[1] = VAR;
			msg_header[2] = headerlength;
			msg_header[3] = 0x00;
			send(msg_header, header_, headerlength***REMOVED***;
		  }
		  break;
			
		case CTS:
			// Send an ACK
			msg_header[0] = endpoint;
			msg_header[1] = ACK;
			msg_header[2] = msg_header[3] = 0x00;
			send(msg_header, NULL, 0***REMOVED***;
			
			// Send the DATA
			msg_header[0] = endpoint;
			msg_header[1] = DATA;
			msg_header[2] = (datalength_ & 0x00ff***REMOVED***;
			msg_header[3] = (datalength_ >> 8***REMOVED***;
			send(msg_header, data_, datalength_, data_callback_***REMOVED***;
			
			break;
	}
			
	return 0;
}