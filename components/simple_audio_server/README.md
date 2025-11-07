# Simple audio server

This component leverages the standard webserver into a local simple media server. Its created to provide local access to a media file for SOAS in case of internet failure. Make sure your file media is at least `256KB`.

``` yaml
substitutions:
  alarm_file: alarm.flac

external_components:
  - source:
      type: git
      url: https://github.com/Skons/SOAS
    components: [simple_audio_server]

webserver: #required

simple_audio_server:
  id: audio_server
  alarm_file: ${alarm_file}

on_...:
  - media_player.play_media:
      id: media_player_alarm
      media_url: "http://127.0.0.1/sas/${alarm_file}"
```