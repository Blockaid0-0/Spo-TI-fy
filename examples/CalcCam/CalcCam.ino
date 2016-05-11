/*************************************************
 *  CalcCam.ino                                  *
 *  Example from the ArTICL library              *
 *           Created by Christopher Mitchell,    *
 *           2011-2014, all rights reserved.     *
 *                                               *
 *  This demo lets you capture pictures using a  *
 *  graphing calculator and a Gameboy camera.    *
 *  It also lets a connected calculator get or   *
 *  set the Gameboy camera's control registers,  *
 *  to control things like gain, exposure time,  *
 *  and more. The following commands are         *
 *  interpreted:                                 *
 *  - GetCalc(Pic1***REMOVED***: Returns a 265x165 15-color  *
 *    image in Pic1-Pic9 or Pic0 to a TI-84+CSE, *
 *    or a 96x64 monochrome image to a TI-83+/   *
 *    TI-84+.                                    *
 *  - Send(L1***REMOVED***: Sends an 8-element list          *
 *    containing the camera settings. Reset to   *
 *    defaults if the Arduino loses power.       *
 *  - Get(L1***REMOVED***: Gets the 8-element list           *
 *    containing the camera settings.            *
 *                                               *
 *  The original Gameboy camera (M64282FP***REMOVED*** code  *
 *  used in this demo was created by Laurent     *
 *  Saint-Marcel (lstmarcel@yahoo.fr***REMOVED***, released  *
 *  2005-07-05. Adapted to the Arduino by Google *
 *  Code user shimniok@gmail.com, and further    *
 *  modified by Christopher Mitchell.            *
 *************************************************/

/* Camera connections:
    READ  -- D8,  PB0
    XCK   -- D9,  PB1
    XRST  -- D10, PB2
    LOAD  -- D11, PB3
    SIN   -- D12, PB4
    START -- D13, PB5
    VOUT  -- A3,  PC3
*/

// Includes
#include <Wire.h>

#include <avr/interrupt.h>
#include <avr/io.h>
#include <compat/deprecated.h>

#include "CBL2.h"
#include "TIVar.h"

// Defines
#define CAM_DATA_PORT     PORTB
#define CAM_DATA_DDR      DDRB
#define CAM_READ_PIN      8       // Arduino D8

#define CAM_LED_DDR       DDRB
#define CAM_LED_PORT      PORTB

// CAM_DATA_PORT
#define CAM_START_BIT 5
#define CAM_SIN_BIT   4
#define CAM_LOAD_BIT  3
#define CAM_RESET_BIT 2
#define CAM_CLOCK_BIT 1
#define CAM_READ_BIT  0
// CAM_LED_PORT
#define CAM_LED_BIT   4
// PORT C: Analogic/digital converter
#define CAM_ADC_PIN   3

#define TWI_CAMERA 0x01 // TWI (I2C***REMOVED*** address of the camera

// modes of the camera
enum CamMode {
	CAM_MODE_STANDARD,
	CAM_MODE_DOWNSCALE
};

// Forward function definitions.
int onGetAsCBL2(uint8_t type, enum Endpoint model, int datalen***REMOVED***;
int onSendAsCBL2(uint8_t type, enum Endpoint model, int* headerlen,
                 int* datalen, data_callback* data_callback***REMOVED***;

/* ------------------------------------------------------------------------ */
/* GLOBALS                                                                  */
/* ------------------------------------------------------------------------ */

// ArTICL-related
CBL2 cbl;
int lineRed = 7;
int lineWhite = 6;

#define MAXDATALEN 255
uint8_t header[16];
uint8_t data[MAXDATALEN];

// default value for registers
// 155 1 0 30 1 0 1 7 
// no edge detection, exposure=0,30, offset=-27, vref=+1.0, gain = 1
unsigned char camReg[8]={ 155, 1, 0, 30, 1, 0, 1, 7 };

CamMode camMode             = CAM_MODE_STANDARD;
unsigned char camClockSpeed = 0x07; // was 0x0A

int x, y;                // the current x,y coordinate we're working on
int cx, cy;

/* ------------------------------------------------------------------------ */
/* MACROS                                                                   */
/* ------------------------------------------------------------------------ */

#define Serialwait(***REMOVED***   while (Serial.available(***REMOVED*** == 0***REMOVED*** ;

int dataIn;
int dataOut;
boolean dataReady;
unsigned char reg;

/* ------------------------------------------------------------------------ */
/* Initialize all components                                                */
/* ------------------------------------------------------------------------ */
void setup(***REMOVED***
{
  dataReady = false;
  Serial.begin(38400***REMOVED***;
  //Wire.begin(TWI_CAMERA***REMOVED***;
  //Wire.onReceive(recvByte***REMOVED***;
  //Wire.onRequest(sendByte***REMOVED***;
  camInit(***REMOVED***;

  /* enable interrupts */
  sei(***REMOVED***;

  cbl.setLines(lineRed, lineWhite***REMOVED***;
  cbl.resetLines(***REMOVED***;
  //cbl.setVerbosity(true, &Serial***REMOVED***;			// Comment this in for message information
  cbl.setupCallbacks(header, data, MAXDATALEN,
                      onGetAsCBL2, onSendAsCBL2***REMOVED***;

  Serial.println("Ready."***REMOVED***;
}

/* ------------------------------------------------------------------------ */
/* Program entry point                                                      */
/* ------------------------------------------------------------------------ */
void loop(***REMOVED*** { 
  int rval = 0;
  rval = cbl.eventLoopTick(***REMOVED***;
  if (rval && rval != ERR_READ_TIMEOUT***REMOVED*** {
    Serial.print("Failed to run eventLoopTick: code "***REMOVED***;
    Serial.println(rval***REMOVED***;
  }  
} // loop

///////////////////////////////////////////////////////////////////////////
// PRIVATE FUNCTIONS
///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
// TWI HANDLERS
void recvByte(int howMany***REMOVED*** {
  dataIn = Wire.read(***REMOVED***;
  dataReady = true;
}

void sendByte(void***REMOVED*** {
  Wire.write(dataOut***REMOVED***;
}

///////////////////////////////////////////////////////////////////////////
// CAM TIMING AND CONTROL

// cbi(port, bitId***REMOVED*** = clear bit(port, bitId***REMOVED*** = Set the signal Low
// sbi(port, bitId***REMOVED*** = set bit(port, bitId***REMOVED*** = Set the signal High

// Delay used between each signal sent to the AR (four per xck cycle***REMOVED***.
void camStepDelay(***REMOVED*** {
  	unsigned char u=camClockSpeed;
	while(u--***REMOVED*** {__asm__ __volatile__ ("nop"***REMOVED***;}
}
// Set the clock signal Low
inline void camClockL(***REMOVED***
{
	cbi(CAM_DATA_PORT, CAM_CLOCK_BIT***REMOVED***;
}
// Set the clock signal High
inline void camClockH(***REMOVED***
{
	sbi(CAM_DATA_PORT, CAM_CLOCK_BIT***REMOVED***;
}


// Initialise the IO ports for the camera
void camInit(***REMOVED***
{
  pinMode(8, INPUT***REMOVED***;   // READ
  pinMode(9, OUTPUT***REMOVED***;  // XCK
  pinMode(10, OUTPUT***REMOVED***;  // XRST
  pinMode(11, OUTPUT***REMOVED***; // LOAD
  pinMode(12, OUTPUT***REMOVED***; // SIN
  pinMode(13, OUTPUT***REMOVED***; // START
  
  cbi(CAM_DATA_PORT, CAM_CLOCK_BIT***REMOVED***;
  sbi(CAM_DATA_PORT, CAM_RESET_BIT***REMOVED***;  // Reset is active low
  cbi(CAM_DATA_PORT, CAM_LOAD_BIT***REMOVED***;
  cbi(CAM_DATA_PORT, CAM_START_BIT***REMOVED***;
  cbi(CAM_DATA_PORT, CAM_SIN_BIT***REMOVED***;
}


// Sends a 'reset' pulse to the AR chip.
// START:  XCK Rising Edge 
// FINISH: XCK Just before Falling Edge
void camReset(***REMOVED***
{
  camClockH(***REMOVED***; // clock high
  camStepDelay(***REMOVED***;
  camStepDelay(***REMOVED***;
 
  camClockL(***REMOVED***; // clock low
  camStepDelay(***REMOVED***;
  cbi(CAM_DATA_PORT, CAM_RESET_BIT***REMOVED***;
  camStepDelay(***REMOVED***;

  camClockH(***REMOVED***; // clock high
  camStepDelay(***REMOVED***;
  sbi(CAM_DATA_PORT, CAM_RESET_BIT***REMOVED***;
  camStepDelay(***REMOVED***;
}


// locally set the value of a register but do not set it in the AR chip. You 
// must run camSendRegisters1 to write the register value in the chip
void camStoreReg(unsigned char reg, unsigned char data***REMOVED*** 
{
  camReg[reg] = data;
}

// Reset the camera and set the camera's 8 registers
// from the locally stored values (see camStoreReg***REMOVED***
void camSetRegisters(void***REMOVED***
{
  for(reg=0; reg<8; ++reg***REMOVED*** {
    camSetReg(reg, camReg[reg]***REMOVED***;
  }
}

// Sets one of the 8 8-bit registers in the AR chip.
// START:  XCK Falling Edge 
// FINISH: XCK Just before Falling Edge
void camSetReg(unsigned char regaddr, unsigned char regval***REMOVED***
{
  unsigned char bitmask;

  // Write 3-bit address.
  for(bitmask = 0x4; bitmask >= 0x1; bitmask >>= 1***REMOVED***{
    camClockL(***REMOVED***;
    camStepDelay(***REMOVED***;
    // ensure load bit is cleared from previous call
    cbi(CAM_DATA_PORT, CAM_LOAD_BIT***REMOVED***;
    // Set the SIN bit
    if(regaddr & bitmask***REMOVED***
      sbi(CAM_DATA_PORT, CAM_SIN_BIT***REMOVED***;
    else
      cbi(CAM_DATA_PORT, CAM_SIN_BIT***REMOVED***;
    camStepDelay(***REMOVED***;

    camClockH(***REMOVED***;
    camStepDelay(***REMOVED***;
    // set the SIN bit low
    cbi(CAM_DATA_PORT, CAM_SIN_BIT***REMOVED***;
    camStepDelay(***REMOVED***;
  }

  // Write 7 most significant bits of 8-bit data.
  for(bitmask = 128; bitmask >= 1; bitmask>>=1***REMOVED***{
    camClockL(***REMOVED***;
    camStepDelay(***REMOVED***;
    // set the SIN bit
    if(regval & bitmask***REMOVED***
      sbi(CAM_DATA_PORT, CAM_SIN_BIT***REMOVED***;
    else
      cbi(CAM_DATA_PORT, CAM_SIN_BIT***REMOVED***;
    camStepDelay(***REMOVED***;
    // Assert load at rising edge of xck
    if (bitmask == 1***REMOVED***
      sbi(CAM_DATA_PORT, CAM_LOAD_BIT***REMOVED***;
    camClockH(***REMOVED***;
    camStepDelay(***REMOVED***;
    // reset the SIN bit
    cbi(CAM_DATA_PORT, CAM_SIN_BIT***REMOVED***;
    camStepDelay(***REMOVED***;
  }
}

// Take a picture, read it and send it trhough the serial port. 
//
// getPicture -- send the pixel data to the requester
//
// START:  XCK Falling Edge 
// FINISH: XCK Just before Rising Edge
void camStartPicture(***REMOVED*** {

  // Camera START sequence
  camClockL(***REMOVED***;
  camStepDelay(***REMOVED***;
  // ensure load bit is cleared from previous call
  cbi(CAM_DATA_PORT, CAM_LOAD_BIT***REMOVED***;
  // START rises before xck
  sbi(CAM_DATA_PORT, CAM_START_BIT***REMOVED***;
  camStepDelay(***REMOVED***;

  camClockH(***REMOVED***;
  camStepDelay(***REMOVED***;
  // start valid on rising edge of xck, so can drop now
  cbi(CAM_DATA_PORT, CAM_START_BIT***REMOVED***;
  camStepDelay(***REMOVED***;

  camClockL(***REMOVED***;
  camStepDelay(***REMOVED***;
  camStepDelay(***REMOVED***;
 
  // Wait for READ to go high
  while (1***REMOVED*** {
    camClockH(***REMOVED***;
    camStepDelay(***REMOVED***;
    // READ goes high with rising XCK
    //    if ( inp(CAM_READ_PIN***REMOVED*** & (1 << CAM_READ_BIT***REMOVED*** ***REMOVED***
    if (digitalRead(CAM_READ_PIN***REMOVED*** == HIGH***REMOVED*** // CAM pin on PB0/D8
      break;
    camStepDelay(***REMOVED***;

    camClockL(***REMOVED***;
    camStepDelay(***REMOVED***;
    camStepDelay(***REMOVED***;
  }
  
  //sbi(CAM_LED_PORT, CAM_LED_BIT***REMOVED***;

  camStepDelay(***REMOVED***;
   
  // Read pixels from cam until READ signal is low again
  // Set registers while reading the first 11-ish pixels
  // The camera seems to be spitting out 128x128 even though the final 5 rows are junk

  cx = 0;
  cy = 0;
}

inline uint8_t camGetPixel(void***REMOVED*** {
  uint8_t pixel;
  camClockL(***REMOVED***;
  camStepDelay(***REMOVED***;
  // get the next pixel, buffer it, and send it out to the attached calculator
  pixel = analogRead(CAM_ADC_PIN***REMOVED*** >> 2;

  camClockH(***REMOVED***;
  camStepDelay(***REMOVED***;
  camStepDelay(***REMOVED***;

  cx++;
  if (cx == 128***REMOVED*** {
    cx = 0;
    cy++;
  }
      
  if (cy >= 128***REMOVED*** {
    camClockL(***REMOVED***;
    camStepDelay(***REMOVED***;
    camStepDelay(***REMOVED***;
  }
  
  return pixel;
}

uint8_t sendPicDataByte(int idx***REMOVED*** {
  uint8_t outbyte;
  if (x < 0***REMOVED*** {
    uint16_t length = (camMode == CAM_MODE_STANDARD***REMOVED***?21945:756;
	if (x == -2***REMOVED*** {
	  outbyte = (uint8_t***REMOVED***(length & 0x0ff***REMOVED***;
	} else {
	  outbyte = (uint8_t***REMOVED***(length >> 8***REMOVED***;
	}
	x++;
	return outbyte;
  }
  if (camMode == CAM_MODE_STANDARD***REMOVED*** {
    outbyte = 0xbb;    // all white
    if (y >= 17 && y < 145 && x >= 68 && x < 196***REMOVED*** {
      for(uint8_t i = 0; i < 2; i++***REMOVED*** {
        uint8_t pixel = camGetPixel(***REMOVED***;
        outbyte <<= 4;
        if (pixel < 25***REMOVED*** {
        outbyte |= 0x03;
        } else if (pixel < 76***REMOVED*** {
          outbyte |= 0x0f;
        } else if (pixel < 127***REMOVED*** {
          outbyte |= 0x0e;
        } else if (pixel < 178***REMOVED*** {
          outbyte |= 0x0d;
        } else if (pixel < 229***REMOVED*** {
          outbyte |= 0x0c;
        } else {
          outbyte |= 0x0b;
        }
      }
    }
    x += 2;
    // Update coordinates
    if (x >= 266***REMOVED*** {
      x = 0;
      y++;
    }
  } else {
    outbyte = 0x00;	// 8 white pixels;
    if (x >= 16 && x < 80***REMOVED*** {
      for(uint8_t i = 0; i < 8; i++***REMOVED*** {
        uint8_t pixel = camGetPixel(***REMOVED***;
        outbyte <<= 1;
        outbyte |= (pixel >= 128***REMOVED***?0:1;
        camGetPixel(***REMOVED***;			// Skip one pixel
      }
    }

    x += 8;
    if (x >= 96***REMOVED*** {
      x = 0;
      y++;
      for(uint8_t i = 0; i < 128; i++***REMOVED*** {
        camGetPixel(***REMOVED***;		// Throw out one row
      }
	}
  }
  
  return outbyte;
}

int onGetAsCBL2(uint8_t type, enum Endpoint model, int datalen***REMOVED*** {
  //Serial.print("Got variable of type 0x"***REMOVED***;
  //Serial.print(header[2], HEX***REMOVED***;
  //Serial.print(" from endpoint of type 0x"***REMOVED***;
  //Serial.println((int***REMOVED***model, HEX***REMOVED***;

  if (header[2] != 0x01***REMOVED*** {
    // Only accept a list
    return -1;
  }
  if (8 != TIVar::sizeWordToInt(&data[0]***REMOVED******REMOVED*** {
    // Only accept an 8-element list
    return -1;
  }
  for(int i=0; i<8; i++***REMOVED*** {
    int value = TIVar::realToLong8x(&data[2 + TIVar::sizeOfReal(model***REMOVED*** * i], model***REMOVED***;  
    //Serial.print("Element "***REMOVED***;
    //Serial.print(i***REMOVED***;
    //Serial.print(" has value "***REMOVED***;
    //Serial.println(value, HEX***REMOVED***;
    camStoreReg(i, value***REMOVED***;
  }
  return 0;
}

int onSendAsCBL2(uint8_t type, enum Endpoint model, int* headerlen,
                 int* datalen, data_callback* data_callback***REMOVED***
{
  Serial.print("Req for var 0x"***REMOVED***;
  Serial.print(type, HEX***REMOVED***;
  Serial.print(" from EP 0x"***REMOVED***;
  Serial.println((int***REMOVED***model, HEX***REMOVED***;
  
  if (type == 0x01 || type == 0x5D***REMOVED*** {
    // Return the register values. First compose header...
    *headerlen = 11;
    *datalen = 2 + 8 * TIVar::sizeOfReal(model***REMOVED***;
    if (*datalen > MAXDATALEN***REMOVED*** {
      // Too big.
      return -1;
    }
    TIVar::intToSizeWord(*datalen, &header[0]***REMOVED***;
    // Do not change the rest of the header
    
    // ... then compose body
    TIVar::intToSizeWord(8, &data[0]***REMOVED***;
    int offset = 2;
    for(int i = 0; i < 8; i++***REMOVED*** {
  	// Convert the value, get the length of the inserted data or -1 for failure
  	int rval = TIVar::longToReal8x(camReg[i], &data[offset], model***REMOVED***;
  	if (rval < 0***REMOVED*** {
  		return -1;
  	}
  	offset += rval;
    }
    return 0;
    
  } else if (type == 0x07 || type == 0x60***REMOVED*** {
	// TI-84+CSE or TI-83+/TI-84+ picture
	
    // Compose the VAR header
    if (*headerlen == 13 && header[11] == 0x0A***REMOVED*** {  
      *datalen = 21945 + 2;                             // 165 * 266 / 2 (4 bits per pixel***REMOVED***
      // Leave the pic portion of the header as-is
      header[11] = 0x0A;      // Version
      header[12] = 0x80;      // Archived
      camMode = CAM_MODE_STANDARD;
    } else {
      *datalen = 756 + 2;
      TIVar::intToSizeWord(*datalen, &header[0]***REMOVED***;	// Two bytes for the element count, 6 Reals
	  header[2] = 0x07;		// Because the TI-OS makes no sense
      camMode = CAM_MODE_DOWNSCALE;
    }
	
	// Initialize the camera
    x = -2;
    y = 0;
    camReset(***REMOVED***;
    camSetRegisters(***REMOVED***;
    camStartPicture(***REMOVED***;

    // Do not compose the body of the variable!
    *data_callback = sendPicDataByte;
    return 0;
  } else {
    // Unknown type requested
    return -1;
  }
}
