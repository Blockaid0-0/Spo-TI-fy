import time
import spotipy
import serial
from spotipy.oauth2 import SpotifyOAuth


arduino = serial.Serial(port = 'COM3', baudrate=115200, timeout = .1)

def writeRead(x):
    arduino.write((x+'\n').encode('utf-8'))
    time.sleep(0.05)
    data = arduino.readline()
    return data

scope = "user-read-currently-playing"

sp = spotipy.Spotify(
    auth_manager=SpotifyOAuth(
        scope=scope,
        redirect_uri="http://127.0.0.1:8888/callback",
        client_id="e5dd75f52cd14e88a56adeb534d7f417",
        client_secret="d177b7dc500b47c5b91a631473ceda37",
    )
)

while True:
    line = arduino.readline().decode(errors="ignore").strip()
    spot = sp.currently_playing()
    if spot is None or spot['is_playing'] == False:
        vals = "Not playing|No one|00:00/00:00|0|1"
        if line == "PLAY":
            sp.start_playback()
    else:
        if line == "PLAY":
            sp.pause_playback()
        if line == "SKIP":
            sp.next_track()
        if line == "REPLAY":
            sp.previous_track()
        song = spot["item"]["name"]
        artist = spot["item"]["artists"][0]["name"]
        progressSec = spot["progress_ms"] // 1000
        progress = "%02d:%02d" % (progressSec // 60,progressSec % 60)
        durationSec = spot["item"]["duration_ms"] // 1000
        duration = "%02d:%02d" %(durationSec // 60,durationSec % 60)
        timeElapsed = f'{progress}/{duration}'
        if len(song) > 26:
            song = f"{song[:26]}"
        if len(artist) > 26:
            artist = f"{artist[:26]}"
        vals = f'''{song}|{artist}|{timeElapsed}|{progressSec}|{durationSec}'''
    print(vals.strip())
    writeRead(vals.strip())
