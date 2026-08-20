//
// Created by Blockaid on 8/19/2026.
//

#include "include/Serial.h"
#include "include/Globals.h"
#include <HardwareSerial.h>

void SerialCom::serialCommuncation() {
    while (Serial.available()) {
        static String majorStr, minorStr[5];
        char c = Serial.read();
        if (c == '\r') continue;
        if (c != '\n') {
            majorStr += c;
            continue;
        }
        majorStr.trim();
        int count = 0;
        int start = 0;
        while (count < 5) {
            int commaPos = majorStr.indexOf('|', start);
            if (commaPos < 0) {
                minorStr[count++] = majorStr.substring(start);
                break;
            }
            minorStr[count++] = majorStr.substring(start, commaPos);
            start = commaPos + 1;
        }
        mesg[0] = (count > 0) ? minorStr[0] : "loading";
        mesg[1] = (count > 1) ? minorStr[1] : "loading.";
        mesg[2] = (count > 2) ? minorStr[2] : "loading..";
        mesg[3] = (count > 3) ? minorStr[3] : "0";
        mesg[4] = (count > 4) ? minorStr[4] : "1";
        majorStr.clear();
    }
}
