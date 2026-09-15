#include "include/Globals.h"
#include "include/Serial.h"

SerialCom ser;
CBL2 cbl;

int requestStr(uint8_t type, enum Endpoint model, int* headerLen, int* dataLen, data_callback* data_callback***REMOVED*** {
    String text;
    int num = text.toInt(***REMOVED***;
    memset(header, 0, sizeof(header***REMOVED******REMOVED***;
    int rVal;
    switch (strVar***REMOVED*** {
        case 1: header[4] = 0x00;
            header[3] = 0xAA;
            header[2] = VarTypes82::VarString;
            text = mesg[0];
            text.toUpperCase(***REMOVED***;
            sendStr[0] = text;
            rVal = TIVar::stringToStrVar8x(text, data, model***REMOVED***;
            break;
        case 2: header[4] = 0x01;
            header[3] = 0xAA;
            header[2] = VarTypes82::VarString;
            text = mesg[1];
            text.toUpperCase(***REMOVED***;
            sendStr[1] = text;
            rVal = TIVar::stringToStrVar8x(text, data, model***REMOVED***;
            break;
        case 3: header[4] = 0x02;
            header[3] = 0xAA;
            header[2] = VarTypes82::VarString;
            text = mesg[2];
            text.toUpperCase(***REMOVED***;
            sendStr[2] = text;
            rVal = TIVar::stringToStrVar8x(text, data, model***REMOVED***;
            break;
        case 4: header[4] = 0x00;
            header[3] = 'A';
            header[2] = VarTypes82::VarReal;
            text = mesg[3];
            num = text.toInt(***REMOVED***;
            sendStr[3] = text;
            rVal = TIVar::longToReal8x(num, data, model***REMOVED***;
            break;
        case 5: header[4] = 0x00;
            header[3] = 'B';
            text = mesg[4];
            header[2] = VarTypes82::VarReal;
            num = text.toInt(***REMOVED***;
            sendStr[4] = text;
            rVal = TIVar::longToReal8x(num, data, model***REMOVED***;
            break;
        default: header[4] = 0x00;
            header[3] = 0xAA;
            header[2] = VarTypes82::VarString;
            text = "error";
            rVal = TIVar::stringToStrVar8x(text, data, model***REMOVED***;
            text.toUpperCase(***REMOVED***;
    }
    *dataLen = rVal;
    TIVar::intToSizeWord(rVal, header***REMOVED***;
    *headerLen = 13;
    return 0;
}


int recieveStr(uint8_t type, enum Endpoint model, int dataLen***REMOVED*** {
    mainStr = TIVar::strVarToString8x(data, model***REMOVED***;
    mainNum = TIVar::realToLong8x(data, model***REMOVED***;
    if (mainStr == "PLAY"***REMOVED*** Serial.println("PLAY"***REMOVED***;
    if (mainStr == "SKIP"***REMOVED*** Serial.println("SKIP"***REMOVED***;
    if (mainStr == "REPLAY"***REMOVED*** Serial.println("REPLAY"***REMOVED***;
    if (mainStr == "STR1" || mainStr == sendStr[0]***REMOVED*** strVar = 1;
    else if (mainStr == "STR2" || mainStr == sendStr[1]***REMOVED*** strVar = 2;
    else if (mainStr == "STR3" || mainStr == sendStr[2]***REMOVED*** strVar = 3;
    else if (mainNum == 0 || mainNum == sendStr[3].toInt(***REMOVED******REMOVED*** strVar = 4;
    else if (mainNum == 1 || mainNum == sendStr[4].toInt(***REMOVED******REMOVED*** strVar = 5;
    return 0;
}

void setup(***REMOVED*** {
    Serial.begin(115200***REMOVED***;
    strVar = 1;
    cbl.setLines(redLine, whiteLine***REMOVED***;
    cbl.resetLines(***REMOVED***;
    cbl.setupCallbacks(header, data, MAXDATALEN, recieveStr, requestStr***REMOVED***;
}
void loop(***REMOVED*** {
    SerialCom::serialCommuncation(***REMOVED***;
    cbl.eventLoopTick(***REMOVED***;
}