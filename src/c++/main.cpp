#include "include/Globals.h"
#include "include/Serial.h"

SerialCom ser;
CBL2 cbl;

int requestStr(uint8_t type, enum Endpoint model, int* headerLen, int* dataLen, data_callback* data_callback) {
    String text;
    int num = text.toInt();
    memset(header, 0, sizeof(header));
    int rVal;
    switch (strVar) {
        case 1: header[4] = 0x00;
            header[3] = 0xAA;
            header[2] = VarTypes82::VarString;
            text = mesg[0];
            text.toUpperCase();
            sendStr[0] = text;
            rVal = TIVar::stringToStrVar8x(text, data, model);
            break;
        case 2: header[4] = 0x01;
            header[3] = 0xAA;
            header[2] = VarTypes82::VarString;
            text = mesg[1];
            text.toUpperCase();
            sendStr[1] = text;
            rVal = TIVar::stringToStrVar8x(text, data, model);
            break;
        case 3: header[4] = 0x02;
            header[3] = 0xAA;
            header[2] = VarTypes82::VarString;
            text = mesg[2];
            text.toUpperCase();
            sendStr[2] = text;
            rVal = TIVar::stringToStrVar8x(text, data, model);
            break;
        case 4: header[4] = 0x00;
            header[3] = 'A';
            header[2] = VarTypes82::VarReal;
            text = mesg[3];
            num = text.toInt();
            sendStr[3] = text;
            rVal = TIVar::longToReal8x(num, data, model);
            break;
        case 5: header[4] = 0x00;
            header[3] = 'B';
            text = mesg[4];
            header[2] = VarTypes82::VarReal;
            num = text.toInt();
            sendStr[4] = text;
            rVal = TIVar::longToReal8x(num, data, model);
            break;
        default: header[4] = 0x00;
            header[3] = 0xAA;
            header[2] = VarTypes82::VarString;
            text = "error";
            rVal = TIVar::stringToStrVar8x(text, data, model);
            text.toUpperCase();
    }
    *dataLen = rVal;
    TIVar::intToSizeWord(rVal, header);
    *headerLen = 13;
    return 0;
}


int recieveStr(uint8_t type, enum Endpoint model, int dataLen) {
    mainStr = TIVar::strVarToString8x(data, model);
    mainNum = TIVar::realToLong8x(data, model);
    if (mainStr == "PLAY") Serial.println("PLAY");
    if (mainStr == "SKIP") Serial.println("SKIP");
    if (mainStr == "REPLAY") Serial.println("REPLAY");
    if (mainStr == "STR1" || mainStr == sendStr[0]) strVar = 1;
    else if (mainStr == "STR2" || mainStr == sendStr[1]) strVar = 2;
    else if (mainStr == "STR3" || mainStr == sendStr[2]) strVar = 3;
    else if (mainNum == 0 || mainNum == sendStr[3].toInt()) strVar = 4;
    else if (mainNum == 1 || mainNum == sendStr[4].toInt()) strVar = 5;
    return 0;
}

void setup() {
    Serial.begin(115200);
    strVar = 1;
    cbl.setLines(redLine, whiteLine);
    cbl.resetLines();
    cbl.setupCallbacks(header, data, MAXDATALEN, recieveStr, requestStr);
}
void loop() {
    SerialCom::serialCommuncation();
    cbl.eventLoopTick();
}