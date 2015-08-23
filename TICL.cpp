/*************************************************
 * TICL.cpp - Core of ArTICL library for linking *
 *            TI calculators and Arduinos.       *
 *            Created by Christopher Mitchell,   *
 *            2011-2014, all rights reserved.    *
 *************************************************/

#include "Arduino.h"
#include "TICL.h"

// Constructor with default communication lines
TICL::TICL(***REMOVED*** {
	tip_ = DEFAULT_TIP;
	ring_ = DEFAULT_RING;
	serial_ = NULL;
}

// Constructor with custom communication lines. Fun
// fact: You can use this and multiple TICL objects to
// talk to multiple endpoints at the same time.
TICL::TICL(int tip, int ring***REMOVED*** {
	tip_ = tip;
	ring_ = ring;
	serial_ = NULL;
}

// This should be called during the setup(***REMOVED*** function
// to set the communication lines to their initial values
void TICL::begin(***REMOVED*** {
	resetLines(***REMOVED***;
}

// Determine whether debug printing is enabled
void TICL::setVerbosity(bool verbose, HardwareSerial* serial***REMOVED*** {
	if (verbose***REMOVED*** {
		serial_ = serial;
	} else {
		serial_ = NULL;
	}
}

// Send an entire message from the Arduino to
// the attached TI device, byte by byte
int TICL::send(uint8_t* header, uint8_t* data, int datalength, uint8_t(*data_callback***REMOVED***(int***REMOVED******REMOVED*** {
	if (serial_***REMOVED*** {
		serial_->print("snd type 0x"***REMOVED***;
		serial_->print(header[1], HEX***REMOVED***;
		serial_->print(" as EP 0x"***REMOVED***;
		serial_->print(header[0], HEX***REMOVED***;
		serial_->print(" len "***REMOVED***;
		serial_->println(datalength***REMOVED***;
	}

	// Send all of the bytes in the header
	for(int idx = 0; idx < 4; idx++***REMOVED*** {
		int rval = sendByte(header[idx]***REMOVED***;
		if (rval != 0***REMOVED***
			return rval;
	}
	
	// If no data, we're done
	if (datalength == 0***REMOVED*** {
		return 0;
	}
	
	// These  also indicate that there are 
	// no data bytes to be sent
	if (header[1] == CTS ||
		header[1] == VER ||
		header[1] == ACK ||
		header[1] == ERR ||
		header[1] == RDY ||
		header[1] == SCR ||
		header[1] == KEY ||
		header[1] == EOT***REMOVED***
	{
		return 0;
	}
	
	// Send all of the bytes in the data buffer
	uint16_t checksum = 0;
	for(int idx = 0; idx < datalength; idx++***REMOVED*** {
		uint8_t outbyte;
		// Get a byte if we need
		if (data_callback != NULL***REMOVED*** {
			outbyte = data_callback(idx***REMOVED***;
		} else {
			outbyte = data[idx];
		}
		// Try to send this byte
		int rval = sendByte(outbyte***REMOVED***;
		if (rval != 0***REMOVED***
			return rval;
		checksum += outbyte;
	}
	
	// Send the checksum
	int rval = sendByte(checksum & 0x00ff***REMOVED***;
	if (rval != 0***REMOVED***
		return rval;
	rval = sendByte((checksum >> 8***REMOVED*** & 0x00ff***REMOVED***;
	return rval;
}

// Send a single byte from the Arduino to the attached
// TI device, returning nonzero if a failure occurred.
int TICL::sendByte(uint8_t byte***REMOVED*** {
	unsigned long previousMicros = 0;

	// Send all of the bits in this byte
	for(int bit = 0; bit < 8; bit++***REMOVED*** {
		
		// Wait for both lines to be high before sending the bit
		previousMicros = micros(***REMOVED***;
		while (digitalRead(ring_***REMOVED*** == LOW || digitalRead(tip_***REMOVED*** == LOW***REMOVED*** {
			if (micros(***REMOVED*** - previousMicros > TIMEOUT***REMOVED*** {
				resetLines(***REMOVED***;
				return ERR_WRITE_TIMEOUT;
			}
		}
		
		// Pull one line low to indicate a new bit is going out
		bool bitval = (byte & 1***REMOVED***;
		int line = (bitval***REMOVED***?ring_:tip_;
		pinMode(line, OUTPUT***REMOVED***;
		digitalWrite(line, LOW***REMOVED***;
		
		// Wait for peer to acknowledge by pulling opposite line low
		line = (bitval***REMOVED***?tip_:ring_;
		previousMicros = micros(***REMOVED***;
		while (digitalRead(line***REMOVED*** == HIGH***REMOVED*** {
			if (micros(***REMOVED*** - previousMicros > TIMEOUT***REMOVED*** {
				resetLines(***REMOVED***;
				return ERR_WRITE_TIMEOUT;
			}
		}

		// Wait for peer to indicate readiness by releasing that line
		resetLines(***REMOVED***;
		previousMicros = micros(***REMOVED***;
		while (digitalRead(line***REMOVED*** == LOW***REMOVED*** {
			if (micros(***REMOVED*** - previousMicros > TIMEOUT***REMOVED*** {
				resetLines(***REMOVED***;
				return ERR_WRITE_TIMEOUT;
			}
		}
		
		// Rotate the next bit to send into the low bit of the byte
		byte >>= 1;
	}
	
	return 0;
}

// Returns 0 for a successfully-read message or non-zero
// for failure. If return value is 0 and datalength is zero,
// then the message is just a 4-byte message in the header
// buffer. If the 
int TICL::get(uint8_t* header, uint8_t* data, int* datalength, int maxlength***REMOVED*** {
	int rval;

	// Get the 4-byte header: sender, message, length
	for(int idx = 0; idx < 4; idx++***REMOVED*** {
		rval = getByte(&header[idx]***REMOVED***;
		if (rval***REMOVED***
			return rval;
	}
	*datalength = (int***REMOVED***header[2] | ((int***REMOVED***header[3] << 8***REMOVED***;
	
	if (serial_***REMOVED*** {
		serial_->print("Recv typ 0x"***REMOVED***;
		serial_->print(header[1], HEX***REMOVED***;
		serial_->print(" from EP 0x"***REMOVED***;
		serial_->print(header[0], HEX***REMOVED***;
		serial_->print(" len "***REMOVED***;
		serial_->println(*datalength***REMOVED***;
	}

	if (*datalength == 0***REMOVED***
		return 0;

	// These  also indicate that there are 
	// no data bytes to be received
	if (header[1] == CTS ||
		header[1] == VER ||
		header[1] == ACK ||
		header[1] == ERR ||
		header[1] == RDY ||
		header[1] == SCR ||
		header[1] == KEY ||
		header[1] == EOT***REMOVED***
	{
		return 0;
	}
	
	// Check if this is a data-free message
	if (*datalength > maxlength***REMOVED*** {
		if (serial_***REMOVED*** {
			serial_->print("Msg buf ovfl: "***REMOVED***;
			serial_->print(*datalength***REMOVED***;
			serial_->print(" > "***REMOVED***;
			serial_->println(maxlength***REMOVED***;
		}
		return ERR_BUFFER_OVERFLOW;
	}
	
	// Get the data bytes, if there are any.
	uint16_t checksum = 0;
	for(int idx = 0; idx < *datalength; idx++***REMOVED*** {
		// Try to get all the bytes, or fail if any of the
		// individual byte reads fail
		rval = getByte(&data[idx]***REMOVED***;
		if (rval != 0***REMOVED***
			return rval;
			
		// Update checksum
		checksum += data[idx];
	}
	
	// Receive and check the checksum
	uint8_t recv_checksum[2];
	for(int idx = 0; idx < 2; idx++***REMOVED*** {
		rval = getByte(&recv_checksum[idx]***REMOVED***;
		if (rval***REMOVED***
			return rval;
	}
	
	// Die on a bad checksum
	if (checksum != (uint16_t***REMOVED***(((int***REMOVED***recv_checksum[1] << 8***REMOVED*** | (int***REMOVED***recv_checksum[0]***REMOVED******REMOVED***
		return ERR_BAD_CHECKSUM;
	
	return 0;
}

// Receive a single byte from the attached TI device,
// returning nonzero if a failure occurred.
int TICL::getByte(uint8_t* byte***REMOVED*** {
	unsigned long previousMicros = 0;
	*byte = 0;
	
	// Pull down each bit and store it
	for (int bit = 0; bit < 8; bit++***REMOVED*** {
		int linevals;

		previousMicros = 0;
		while ((linevals = ((digitalRead(ring_***REMOVED*** << 1***REMOVED*** | digitalRead(tip_***REMOVED******REMOVED******REMOVED*** == 0x03***REMOVED*** {
			if (micros(***REMOVED*** - previousMicros > GET_ENTER_TIMEOUT***REMOVED*** {
				resetLines(***REMOVED***;
				if (serial_***REMOVED*** { serial_->print("died waiting for bit "***REMOVED***; serial_->println(bit***REMOVED***; }
				return ERR_READ_ENTER_TIMEOUT;
			}
		}
		
		// Store the bit, then acknowledge it
		*byte = (*byte >> 1***REMOVED*** | ((linevals == 0x01***REMOVED***?0x80:0x00***REMOVED***;
		int line = (linevals == 0x01***REMOVED***?tip_:ring_;
		pinMode(line, OUTPUT***REMOVED***;
		digitalWrite(line, LOW***REMOVED***;
		
		// Wait for the peer to indicate readiness
		line = (linevals == 0x01***REMOVED***?ring_:tip_;		
		previousMicros = 0;
		while (digitalRead(line***REMOVED*** == LOW***REMOVED*** {            //wait for the other one to go high again
			if (micros(***REMOVED*** - previousMicros > TIMEOUT***REMOVED*** {
				resetLines(***REMOVED***;
				if (serial_***REMOVED*** { serial_->print("died waiting for bit ack "***REMOVED***; serial_->println(bit***REMOVED***; }
				return ERR_READ_TIMEOUT;
			}
		}

		// Now set them both high and to input
		resetLines(***REMOVED***;
	}
	if (serial_***REMOVED*** { serial_->print("Got byte "***REMOVED***; serial_->println(*byte***REMOVED***; }
	return 0;
}

void TICL::resetLines(void***REMOVED*** {
	pinMode(ring_, INPUT_PULLUP***REMOVED***;           // set pin to input with pullups
	pinMode(tip_, INPUT_PULLUP***REMOVED***;            // set pin to input with pullups
}
