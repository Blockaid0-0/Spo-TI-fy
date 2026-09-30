import time
import spotipy
import serial
from spotipy.oauth2 import SpotifyOAuth

file = open("../../CONFIG")
content = file.readlines()
comID = content[1].strip('\n')
arduino = serial.Serial(port = comID, baudrate=115200, timeout = .1)

def writeRead(x):
    arduino.write((x+'\n').encode('utf-8'))
    time.sleep(0.05)
    data = arduino.readline()
    return data
clientID = content[3].strip('\n')
clientSEC = content[5].strip('\n')

scope = "user-read-currently-playing"

sp = spotipy.Spotify(
    auth_manager=SpotifyOAuth(
        scope=scope,
        redirect_uri="http://127.0.0.1:8888/callback",
        client_id=clientID,
        client_secret=clientSEC,
    )
)

while True:
    line = arduino.readline().decode(errors="ignore").strip()
    spot = sp.currently_playing()
    if spot is None or spot.get("item") == False:
        vals = "Not playing|No one|00:00/00:00|0|0"
        if line == "PLAY":
            sp.start_playback()
    else:
        isPlaying = spot.get("is_playing", False)

        if line == "PLAY":
            if isPlaying:
                sp.pause_playback()
            else:
                sp.start_playback()

        if line == "SKIP":
            sp.next_track()

        if line == "REPLAY":
            sp.previous_track()

        song = spot["item"]["name"]
        artist = spot["item"]["artists"][0]["name"]

        progressSec = spot.get("progress_ms", 0) // 1000
        durationSec = spot["item"]["duration_ms"] // 1000

        progress = "%02d:%02d" % (progressSec // 60, progressSec % 60)
        duration = "%02d:%02d" %(durationSec // 60,durationSec % 60)

        timeElapsed = f'{progress}/{duration}'

        if durationSec > 0:
            elapsePer = int((progressSec/durationSec) * 100)
        else:
            elapsePer = 0

        song = song[:26]
        artist = artist[:26]

        status = "1" if isPlaying else "0"

        vals = f'''{song}|{artist}|{timeElapsed}|{elapsePer}|{status}'''

    print(vals.strip())
    writeRead(vals.strip())