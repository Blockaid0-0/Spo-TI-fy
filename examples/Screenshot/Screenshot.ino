#include <TILP.h>

TILP tilp = TILP(3, 2***REMOVED***;

void setup(***REMOVED*** {
  pinMode(4, INPUT_PULLUP***REMOVED***;
  Serial.begin(9600***REMOVED***;
  tilp.resetLines(***REMOVED***;
  delay(1000***REMOVED***;
}

void loop(***REMOVED*** {
  if (!digitalRead(4***REMOVED******REMOVED*** {
    Serial.println("Starting transfer..."***REMOVED***;
    uint8_t screen[768+2];
    uint8_t msg[4] = {0x73, 0x6D, 0x00, 0x00};
    tilp.send(msg, NULL, 0***REMOVED***;
    Serial.println("Sent request"***REMOVED***;
    int rlen = 0;
    tilp.get(msg, NULL, &rlen, 0***REMOVED***;
    Serial.println("Recieved acknowledgement"***REMOVED***;
    tilp.get(NULL, screen, &rlen, 768+2***REMOVED***;
    Serial.println("Recieved screenshot"***REMOVED***;
    tilp.send(msg, NULL, 0***REMOVED***;
    Serial.println("Sent acknowledgement"***REMOVED***;
    for (int i = 1; i <= 768; i++***REMOVED*** {
      for (int j = 7; j > -1; j--***REMOVED*** {
        if (screen[i+3] & (1 << j***REMOVED******REMOVED*** {
          Serial.write('#'***REMOVED***;
        } else {
          Serial.write('.'***REMOVED***;
        }
      }
      if (i%(12***REMOVED*** == 0***REMOVED*** {
        Serial.println(***REMOVED***;
      }
    }
  }
}