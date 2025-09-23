# Changelog

## 2025.9.x.x
- **BREAKING** Top button long press switched to double press. Works better with the tactile button
- Added SOAS version as text sensor
- Refined code due to the introduction of `music_on` in version 2025.8.25.1
- `music_on` now handles `sleep_timer_on`
- If `contrast_force` is being set, the contrast page will not be displayed anymore
- Configuration errors removed from the examples
- Click prevention in Arduino framework
- Documentation updates
- Resolved snooze issue [#33](https://github.com/Skons/SOAS/issues/33)
- Added wifi detection with global variables
- Music and alarm do not wait until time has synced, but wifi has been connected

## 2025.9.18.2
- Fix for issue [#30](https://github.com/Skons/SOAS/issues/30)

## 2025.9.18.1
- **BREAKING** SOAS now supports Arduino and the ESP-IDF framework. You will need to configure your SOAS accordingly. Please review the modules sestion
- **BREAKING** Home Assistant # time, snooze time and sleep timer are changed to datetime or time components
- RTTTL module support, thanks @popy24
- DS1307 module support, thanks @popy24
- GL5516 module support (Contrast based on room brightness), thanks @popy24
- Support for local alarm on, even when the alarm is sounding, see `Alarm on local after seconds`
- Improved `night_mode` detection
- Audio Server, thanks @randellmatt
- Documentation updates
- Examples added to the documentation

## 2025.9.7.1
- **ATTENTION** Avoid using GPIO3, which is a JTAG strapping pin. You must attach the wire from MAX98357a LRC <-> from ESP32 GPIO3 to GPIO12 for audio to work again.
- Configurable max alarm off button time
- Write preferences (like alarm_on) immediately to flash when alarm goes off (alarm_on set to true).
  If the clock would crash, it will restore alarm_on (true) and restores the alarm.
- Optional GPS time sync with NEO-6M
- Optional RTC time sync/backup with DS1307
- Night mode switch with Night mode automatic switch
- `time_sync_done` is now set by checking the local clock
- `check_alarms` script is now single mode
- Display on switch with Display on/off automatically switch
- Smooth display dimming on contrast change
- Documentation updates

## 2025.8.25.2
  - Fixed that the sleep timer disabled the alarm, not the new music switch

## 2025.8.25.1
  - **BREAKING** Switch to esp-idf framework
  - **BREAKING** With the switch to esp-idf pls stream urls do not work anymore
  - **BREAKING** on_boot is removed from the yaml, see `esphome:` above
  - **BREAKING** Flathead long press removed
  - **BREAKING** Music is added to distinguish between alarm sound and playing music (for sleep timer and the hardware button). If music is streamed to the device, it's not treated as an alarm anymore
  - Ability to increase the volume after a period of sounding the alarm [#5](https://github.com/Skons/SOAS/issues/5)
  - Local file can be added for when internet and/or home assistant is not available
  - Cosmetic updates to the yaml
  - When music is streamed to the clock, music_on will be switched on, enabling local controls
  - Volume is set to alarm_volume on stream stop, this is because of volume increase on alarm
  - Added I2C to SPI documentation, see issue [#2](https://github.com/Skons/SOAS/issues/2). Thanks @popy2k14
  - Language support for weekdays [#3](https://github.com/Skons/SOAS/issues/3)
  - Documentation updates

## 2025.7.14.1
 - ESP PSRAM implementation
 - Hide clock feature

## 2025.4.24.1
 - Time handling improvements
 - Solved issue #1
 - PIN assignment not hardcoded anymore

## 2025.4.8.1
 - Alarms are now more resillient, it runs as soon as it is missed
 - Multiple small improvements

## 2025.2.23.1
 - Multiple switches and numbers require time to be synced before their actions will take place, this makes sure settings will survive a reboot
 - Snooze and the sleep timer are more resillient when they miss their schedule due to a reboot
 - Multiple small bug fixes

## 2025.2.15.1
 - Playback will be restored after reboot

## 2025.2.14.1
- Number cleanup
- Switched to sliders instead of boxes for numbers
- Alarm volume number is not internal anymore

## 2025.2.13.1
- Initial commit