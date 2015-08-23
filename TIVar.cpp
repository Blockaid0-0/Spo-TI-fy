/***************************************************
 * tivar.cpp - Library for converting TI-OS var    *
 *             types to/from POSIX var types.      *
 *             Part of the ArTICL linking library. *
 *             Created by Christopher Mitchell,    *
 *             2011-2014, all rights reserved.     *
 ***************************************************/

#include "TIVar.h"

double TIVar::realToFloat8x(uint8_t* real, enum Endpoint model***REMOVED*** {
    const double ieee_lut[10] = {0.f, 1.f, 2.f, 3.f, 4.f, 5.f, 6.f, 7.f, 8.f, 9.f};
    int32_t dec_exp;
	double ieee_acc = 0;
	
	// Figure out what type it is
	enum RealType type = modelToType(model***REMOVED***;
	if (type == REAL_89***REMOVED*** {
		return NAN;			// TI-89/TI-92 not yet implemented! TODO
    }

	// Convert the exponent
	if (type == REAL_82***REMOVED*** {
		dec_exp = ((int16_t***REMOVED***real[1] - 0x80***REMOVED*** - 13;		// decimal point is followed by 13 digits
	} else if (type == REAL_85***REMOVED*** {
		int32_t raw_exp = (int32_t***REMOVED***TIVar::sizeWordToInt(&real[1]***REMOVED***;
		raw_exp -= 0x00fc00;
		dec_exp = (int16_t***REMOVED***raw_exp;
	}

	// Convert the mantissa
	const uint8_t mantissa_offset = (type == REAL_82***REMOVED***?2:3;
	for(uint8_t i = 0; i < 14; i++***REMOVED*** {
		float digit = ieee_lut[0x0f & (real[mantissa_offset + (i >> 1***REMOVED***] >> ((i & 0x01***REMOVED***?0:4***REMOVED******REMOVED***];
		ieee_acc = (10 * ieee_acc***REMOVED*** + digit;
	}
	
	// Raise mantissa to a positive exponent
	while(dec_exp > 0***REMOVED*** {
		ieee_acc *= 10;
		dec_exp--;
	}
	
	// Lower mantissa to a negative exponent
	while(dec_exp < 0***REMOVED*** {
		ieee_acc *= 0.1f;
		dec_exp++;
	}
	
	// Negate the number, if necessary
	if (real[0] & 0x80***REMOVED*** {
		ieee_acc *= -1;
	}
	
	return ieee_acc;
}

int TIVar::longToReal8x(long long int n, uint8_t* real, enum Endpoint model***REMOVED*** {
	int16_t exp = 13;

	// Figure out what type it is
	enum RealType type = modelToType(model***REMOVED***;
	if (type == REAL_89***REMOVED***
		return -1;			// TI-89/TI-92 not yet implemented! TODO
    
	// Set sign bit and get absolute value
	real[0] = (n >= 0***REMOVED***?0x00:0x80;
	n = (n > 0***REMOVED***?n:-n;
	
	// Bring large numbers down
	while(n != 0 && n >= 10e13***REMOVED*** {
		n /= 10;
		exp += 1;
	}
	
	// Bring small numbers up
	while(n != 0 && n < 1e13***REMOVED*** {
		n *= 10;
		exp -= 1;
	}

	// Extract the digits
	const uint8_t mantissa_offset = (type == REAL_82***REMOVED***?2:3;
	for(int8_t i=13; i >= 0; i--***REMOVED*** {
		uint8_t cdigit = (uint8_t***REMOVED***(n % 10***REMOVED***;

		if ((i & 0x01***REMOVED*** == 1***REMOVED*** {
			real[mantissa_offset + (i >> 1***REMOVED***] = cdigit;
		} else {
			real[mantissa_offset + (i >> 1***REMOVED***] |= (cdigit << 4***REMOVED***;
		}
		n /= 10;
	}
	
	// Set the exponent
	if (type == REAL_82***REMOVED*** {
		exp += 0x80;
		real[1] = (uint8_t***REMOVED***exp;

	} else if (type == REAL_85***REMOVED*** {
		int32_t temp_exp = (int32_t***REMOVED***exp;
		temp_exp += 0x00fc00;
		real[1] = (uint8_t***REMOVED***(temp_exp & 0x00ff***REMOVED***;
		real[2] = (uint8_t***REMOVED***((temp_exp >> 8***REMOVED*** & 0x00ff***REMOVED***;
	}

	return TIVar::sizeOfReal(model***REMOVED***;		// Success: inserted data length
}

int TIVar::floatToReal8x(double f, uint8_t* real, enum Endpoint model***REMOVED*** {
	int16_t exp = 13;
	
	// Figure out what type it is
	enum RealType type = modelToType(model***REMOVED***;
	if (type == REAL_89***REMOVED*** {
		return -1;			// TI-89/TI-92 not yet implemented! TODO
    }

	// Set sign bit and get absolute value
	real[0] = (f >= 0***REMOVED***?0x00:0x80;
	f = (f > 0***REMOVED***?f:-f;
	
	// Bring large numbers down
	while(f != 0 && f >= 10.e13***REMOVED*** {
		f *= 0.1f;
		exp += 1;
	}
	
	// Bring small numbers up
	while(f != 0 && f < 1.e13***REMOVED*** {
		f *= 10.f;
		exp -= 1;
	}
	
	// Extract the digits
	const uint8_t mantissa_offset = (type == REAL_82***REMOVED***?2:3;
	for(int8_t i=13; i >= 0; i--***REMOVED*** {
        double digit, odigit;
        digit = odigit = fmod(f, 10.***REMOVED***;
		uint8_t cdigit = 0;
		while(digit > 0.5***REMOVED*** {
			cdigit++;
			digit -= 1.f;
		}
		
		if ((i & 0x01***REMOVED*** == 1***REMOVED*** {
			real[mantissa_offset + (i >> 1***REMOVED***] = cdigit;
		} else {
			real[mantissa_offset + (i >> 1***REMOVED***] |= (cdigit << 4***REMOVED***;
		}
		f = (f - odigit***REMOVED*** / 10.f;
	}
	
	// Set the exponent
	// Set the exponent
	if (type == REAL_82***REMOVED*** {
		exp += 0x80;
		real[1] = (uint8_t***REMOVED***exp;

	} else if (type == REAL_85***REMOVED*** {
		int32_t temp_exp = (int32_t***REMOVED***exp;
		temp_exp += 0x00fc00;
		real[1] = (uint8_t***REMOVED***(temp_exp & 0x00ff***REMOVED***;
		real[2] = (uint8_t***REMOVED***((temp_exp >> 8***REMOVED*** & 0x00ff***REMOVED***;
	}

	return TIVar::sizeOfReal(model***REMOVED***;		// Success: inserted data length
}

enum RealType TIVar::modelToType(enum Endpoint model***REMOVED*** {
	switch(model***REMOVED*** {
		case COMP82:
		case CBL82:
		case CALC82:
			return REAL_82;
			break;
		case COMP83:
		case COMP83P:
		case CALC83P:
		case CALC83:
			return REAL_83;
			break;
		case COMP85:
		case CBL85:
		case CALC85a:
		case CALC85b:
			return REAL_85;
			break;
		case COMP86:
			return REAL_86;
			break;
		case COMP89:
		case CBL89:
		case CALC89:
			return REAL_89;
			break;
		default:
			return REAL_INVALID;
			break;
	}
}

uint16_t TIVar::sizeWordToInt(uint8_t* ptr***REMOVED*** {
	return ((uint16_t***REMOVED***ptr[0]***REMOVED*** | (((uint16_t***REMOVED***ptr[1]***REMOVED*** << 8***REMOVED***;
}

void TIVar::intToSizeWord(uint16_t size, uint8_t* ptr***REMOVED*** {
	ptr[0] = (uint8_t***REMOVED***(size & 0x00ff***REMOVED***;
	ptr[1] = (uint8_t***REMOVED***(size >> 8***REMOVED***;
	return;
}

int TIVar::sizeOfReal(enum Endpoint model***REMOVED*** {
	enum RealType type = modelToType(model***REMOVED***;
	switch(type***REMOVED*** {
		case REAL_82:
			return 9;
			break;
		case REAL_85:
			return 10;
			break;
		case REAL_89:
		case REAL_INVALID:
			return -1;
			break;
	}
	return -1;
}