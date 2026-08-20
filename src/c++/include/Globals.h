//
// Created by Blockaid on 8/19/2026.
//

#pragma once
#include "CBL2.h"
#include "TIVar.h"

#define MAXDATALEN 255

extern uint8_t header[16];
extern uint8_t data[MAXDATALEN];

extern int redLine;
extern int whiteLine;

extern String mainStr;
extern String sendStr[5];
extern String mesg[5];

extern long long mainNum;
extern int strVar;

