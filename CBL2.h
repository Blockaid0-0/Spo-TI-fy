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
#include "TICL.h"

namespace VarTypes82 { enum VarTypes82 {
	VarReal = 0,
	VarRList = 1,
	VarMatrix = 2,
	VarYVar = 3,
	VarString = 4,
	VarProgram = 5,
	VarProtProg = 6,
	VarPic = 7,
	VarGDB = 8,
	VarWindow = 0x0B,
	VarComplex = 0x0C,
}; };
namespace VarTypes85 { enum VarTypes85 {
	VarReal = 0,
	VarCplx = 1,
	VarRVec = 2,
	VarCVec = 3,
	VarRList = 4,
	VarCList = 5,
	VarRMat = 6,
	VarCMat = 7,
	VarRConst = 8,
	VarCConst = 9,
	VarEqu = 0x0A,
	VarString = 0x0C
}; };

typedef uint8_t(*data_callback***REMOVED***(int***REMOVED***;

class CBL2: public TICL {
	public:
		CBL2(***REMOVED***;
		CBL2(int tip, int ring***REMOVED***;

		// Methods for emulating a calculator, talking to a CBL2
		int getFromCBL2(uint8_t type, uint8_t* header, uint8_t* data, int* datalength, int maxlength***REMOVED***;
		int sendToCBL2(uint8_t type, uint8_t* header, uint8_t* data, int datalength***REMOVED***;
		
		// Methods for emulating a CBL2, talking to a calculator
		int setupCallbacks(uint8_t* header, uint8_t* data, int maxlength,
		                   int (*get_callback***REMOVED***(uint8_t, enum Endpoint, int***REMOVED***,
						   int (*send_callback***REMOVED***(uint8_t, enum Endpoint, int*, int*, data_callback****REMOVED******REMOVED***;
		int eventLoopTick(bool quick_fail = false***REMOVED***;				// Usually called in loop(***REMOVED***

	private:
		bool verbose_;
		bool callback_init;
		uint8_t* header_;							// Variable header, not msg header, returned to callbacks!
		uint8_t* data_;								// Variable data returned to callbacks
		int datalength_;
		int maxlength_;
		data_callback data_callback_;
		int (*get_callback_***REMOVED***(uint8_t, enum Endpoint, int***REMOVED***;	// Called when data received from calculator
		int (*send_callback_***REMOVED***(uint8_t, enum Endpoint, int*, int*, data_callback****REMOVED***;	// Called when calculator wants to get data
};

#endif	// CBL2_H