/*************************************************
 *  CBL2.h - Library for emulating CBL2 devices  *
 *           or CBL-connected calculators with   *
 *           Arduinos.                           *
 *           Created by Christopher Mitchell,    *
 *           2011-2014, all rights reserved.     *
 *************************************************/

#ifndef CBL2_H
#define CBL2_H

#include "Arduino.h"
#include "TILP.h"

enum VarTypes {
	VarReal = 0,
	VarRList = 1,
	VarMatrix = 2,
	VarYVar = 3,
	VarString = 4,
	VarProgram = 5,
	VarProtProg = 6,
	VarPic = 7,
	VarGDB = 8,
	VarWindow = 0x0b,
	VarComplex = 0x0c,
};

class CBL2: public TILP {
	public:
		CBL2(***REMOVED***;
		CBL2(int tip, int ring***REMOVED***;

		// Methods for emulating a calculator, talking to a CBL2
		int getFromCBL2(uint8_t type, uint8_t* header, uint8_t* data, int* datalength, int maxlength***REMOVED***;
		int sendToCBL2(uint8_t type, uint8_t* header, uint8_t* data, int datalength***REMOVED***;
		
		// Methods for emulating a CBL2, talking to a calculator
		int setupCallbacks(uint8_t* header, uint8_t* data, int maxlength,
		                   int (*get_callback***REMOVED***(uint8_t, int***REMOVED***,
						   int (*send_callback***REMOVED***(uint8_t, int****REMOVED******REMOVED***;
		int eventLoopTick(***REMOVED***;						// Usually called in loop(***REMOVED***

	private:
		bool verbose_;
		bool callback_init;
		uint8_t* header_;							// Variable header, not msg header, returned to callbacks!
		uint8_t* data_;								// Variable data returned to callbacks
		int datalength_;
		int maxlength_;
		int (*get_callback_***REMOVED***(uint8_t, int***REMOVED***;			// Called when calculator wants to get data
		int (*send_callback_***REMOVED***(uint8_t, int****REMOVED***;		// Called when data received from calculator
};

#endif	// CBL2_H