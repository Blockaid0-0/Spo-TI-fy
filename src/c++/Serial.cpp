
#include "include/Serial.h"
#include "include/Globals.h"
#include <HardwareSerial.h>

void SerialCom::serialCommuncation(***REMOVED*** {
    while (Serial.available(***REMOVED******REMOVED*** {
        static String majorStr, minorStr[5];
        char c = Serial.read(***REMOVED***;
        if (c == '\r'***REMOVED*** continue;
        if (c != '\n'***REMOVED*** {
            majorStr += c;
            continue;
        }
        majorStr.trim(***REMOVED***;
        int count = 0;
        int start = 0;
        while (count < 5***REMOVED*** {
            int commaPos = majorStr.indexOf('|', start***REMOVED***;
            if (commaPos < 0***REMOVED*** {
                minorStr[count++] = majorStr.substring(start***REMOVED***;
                break;
            }
            minorStr[count++] = majorStr.substring(start, commaPos***REMOVED***;
            start = commaPos + 1;
        }
        mesg[0] = (count > 0***REMOVED*** ? minorStr[0] : "loading";
        mesg[1] = (count > 1***REMOVED*** ? minorStr[1] : "loading.";
        mesg[2] = (count > 2***REMOVED*** ? minorStr[2] : "loading..";
        mesg[3] = (count > 3***REMOVED*** ? minorStr[3] : "0";
        mesg[4] = (count > 4***REMOVED*** ? minorStr[4] : "1";
        majorStr.clear(***REMOVED***;
    }
}
