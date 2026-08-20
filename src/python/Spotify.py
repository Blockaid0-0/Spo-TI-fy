***REMOVED***
***REMOVED***
***REMOVED***
***REMOVED***


arduino = serial.Serial(port = 'COM3', baudrate=115200, timeout = .1***REMOVED***

***REMOVED***
***REMOVED***
***REMOVED***
***REMOVED***
***REMOVED***

***REMOVED***

***REMOVED***
***REMOVED***
***REMOVED***
***REMOVED***
        client_id="e5dd75f52cd14e88a56adeb534d7f417",
        client_secret="d177b7dc500b47c5b91a631473ceda37",
***REMOVED***
***REMOVED***

***REMOVED***
    line = arduino.readline(***REMOVED***.decode(errors="ignore"***REMOVED***.strip(***REMOVED***
    spot = sp.currently_playing(***REMOVED***
***REMOVED***
***REMOVED***
***REMOVED***
            sp.start_playback(***REMOVED***
***REMOVED***
***REMOVED***
            sp.pause_playback(***REMOVED***
***REMOVED***
            sp.next_track(***REMOVED***
***REMOVED***
            sp.previous_track(***REMOVED***
***REMOVED***
***REMOVED***
***REMOVED***
        progress = "%02d:%02d" % (progressSec // 60,progressSec % 60***REMOVED***
***REMOVED***
        duration = "%02d:%02d" %(durationSec // 60,durationSec % 60***REMOVED***
***REMOVED***
        if len(song***REMOVED*** > 26:
***REMOVED***
        if len(artist***REMOVED*** > 26:
***REMOVED***
***REMOVED***
    print(vals.strip(***REMOVED******REMOVED***
    writeRead(vals.strip(***REMOVED******REMOVED***
