# [System Of Alarm Sound - A cheap full featured alarm clock](https://community.home-assistant.io/t/soas-full-featured-alarm-clock-with-home-assistant-integration/847108)

|                                                                     |                             |
|---------------------------------------------------------------------|-----------------------------|
| <img src="images/SOASClocks.jpeg" alt="SOAS" style="width:700px;"/> | ![Wake UP](images/soas.png) |

* [What?](#What)
* [Why?](#Why)
* [Features](#Features)
	* [Optional Features](#OptionalFeatures)
* [Requirements](#Requirements)
* [Optional](#Optional)
* [Installation](#Installation)
	* [Modules](#Modules)
		* [ESP-IDF Framework](#ESP-IDFFramework)
		* [Arduino Framework](#ArduinoFramework)
		* [GPS (Optional)](#GPSOptional)
		* [DS1307 (Optional)](#DS1307Optional)
		* [RTTTL (Optional)](#RTTTLOptional)
* [Usage](#Usage)
	* [Time page](#Timepage)
		* [Flathead short press](#Flatheadshortpress)
		* [Rotary single click](#Rotarysingleclick)
		* [Rotary double click](#Rotarydoubleclick)
		* [Rotary triple click](#Rotarytripleclick)
		* [Display](#Display)
	* [Alarm page](#Alarmpage)
		* [Rotary single click](#Rotarysingleclick-1)
		* [Rotary double click](#Rotarydoubleclick-1)
	* [Home Assitant page 1,2,3 and 4](#HomeAssitantpage123and4)
		* [Rotary single click](#Rotarysingleclick-1)
		* [Rotary double click](#Rotarydoubleclick-1)
		* [Rotary triple click](#Rotarytripleclick-1)
	* [Sleep timer page](#Sleeptimerpage)
		* [Rotary single click](#Rotarysingleclick-1)
		* [Rotary double click](#Rotarydoubleclick-1)
	* [Radio page](#Radiopage)
	* [Volume and contrast pages](#Volumeandcontrastpages)
	* [Restart page](#Restartpage)
	* [Options not available on the alarm itself](#Optionsnotavailableonthealarmitself)
* [Display mode](#Displaymode)
* [FAQ](#FAQ)
	* [SH1107 SPI/I2C](#SH1107SPII2C)
* [ToDo](#ToDo)
* [Known issues](#Knownissues)
* [Changelog](#Changelog)
	* [2025.x.x.x](#x.x.x)
	* [2025.9.7.1](#)
	* [2025.8.25.2](#-1)
	* [2025.8.25.1](#-1)
	* [2025.7.14.1](#-1)
	* [2025.4.24.1](#-1)
	* [2025.4.8.1](#-1)
	* [2025.2.23.1](#-1)
	* [2025.2.15.1](#-1)
	* [2025.2.14.1](#-1)
	* [2025.2.13.1](#-1)

## <a name='What'></a>What?
ESPHome with Home Assistant integration?! "No shit, Sherlock". Well yes, all ESPHome has got HA integration, but SOAS has features that enables you to have HA automations based on your alarm time. So automations can be triggered based on the alarm time set on the alarm clock. There are 4 switches that will switch relative to the alarm time, you have the choice to enable the HA switch whether the alarm will sound or not. So the alarm does not have to sound for the HA automation to be triggered.

## <a name='Why'></a>Why?
This alarm clock is customizable, full featured and smart for under €35,-. It's a clock that can be managed through Home Assistant and (mostly) on the clock itself. The clock can be set while your partner is asleep, a tradional clock could be making noise. And, because of the smart features, you could for instance enable the heating in your home 15 minutes before the alarm sets off. Especially when your wake up schedule is not regular, this clock can make automations efficient by associating the triggers with your alarm time.

## <a name='Features'></a>Features
* Alarm based on time.
* Different display modes so there is more or less light emitted by the clock.
* Contrast based on day or night with smooth dimming
* 4 Home Assistant integrated switches that switch based on the alarm time, alarm does not have to sound. These switches can be switched manually on the clock itself.
* Customizable sleep timer.
* Customizable snooze timer.
* Online radio streams.
* Decent sound.
* Ability to hide the clock
* Volume increase of the alarm after a defined time of alarming
* Local file as fallback when internet is not available
* Ability to switch off display completely
  - Display is switched on every alarm.
* Ability to "Display on/off automatically"
  - Display is switched on when rotary or alarm button input or when alarm fires
  - Display is switched off after 10 seconds of no rotary or alarm button input.
    It will not switch off when alarm_on, snooze_on or music_on


### <a name='OptionalFeatures'></a>Optional Features
* Time sync with GPS for when internet is not available
* Time backup with DS1307 rtc module internet is not available
* Local buzzer playback with service and as fallback when internet is not availabl

## <a name='Requirements'></a>Requirements
* < €25,-
* 3d printer (not included in the price)
* [ESP32-S3-N16R8](https://nl.aliexpress.com/item/1005006418608267.html) ~ €6,-
* Oled screen
  - [SH1106](https://nl.aliexpress.com/item/1005007253095259.html) (128x64) ~ €2,50
  - [SH1107](https://nl.aliexpress.com/item/1005005313150711.html) (128x128) ~ €6,-
* MAX98357a amplifier ~ €3,-
* [3W speaker](https://nl.aliexpress.com/item/32593991938.html) ~ €3,-
* [Rotary button](https://nl.aliexpress.com/item/1005001877184897.html) < €1,-
* [Flat head button](https://nl.aliexpress.com/item/1005003400929705.html) ~ €1,50
* Dupont cables
* PLA
* Glue

## <a name='Optional'></a>Optional

* A bit of soldering is not required, but the ground has to be shared so it is nice to solder that one. Depending on your rotary button, you maybe also need to do a little bit of soldering
* [NEO-6M](https://nl.aliexpress.com/item/1005006816514975.html) ~3,- for GPS time sync, this link requires soldering
* [DS1307](https://de.aliexpress.com/item/1005006984190682.html) ~ €0,50/Pc
* [RTTTL buzzer](https://de.aliexpress.com/item/1005009658713423.html) ~ €0,16/Pc

## <a name='Installation'></a>Installation

3d print [these](https://www.thingiverse.com/thing:7091731) files.

Connect all dupont cables corresponding the schema's below:

| SH1106 / SH1107 | ESP32  |
|-----------------|--------|
| VCC             | 3v     |
| GND             | GND    |
| SCL             | GPIO47 |
| SDA             | GPIO48 |

| Rotary Button | ESP32  |
|---------------|--------|
| A             | GPIO9  |
| B             | GPIO10 |
| C             | GND    |
| D             | GND    |
| E             | GPIO8  |

| MAX98357a | ESP32   |
|-----------|---------|
| LRC       | GPIO12  |
| BLCK      | GPIO1   |
| DIN       | GPIO2   |
| GND       | GND     |
| Vin       | 5v / 3v |

| Flat head button | ESP32 |
|------------------|-------|
| Switch           | GPIO4 |
| GND              | GND   |

Include the config below in your YAML. This one is made for a `ESP32-S3-N16R8`:

```yaml
substitutions:
  display_model: "SH1107 128x128" #use "SH1106 128x64" for SH1106
  display_rotation: "270" #use 0 for SH1106
  sun_latitude: 52.37°
  sun_longitude: 4.89°
  i2c_sda: GPIO48
  i2c_scl: GPIO47
  i2s_lrclk_pin: GPIO12
  i2s_bclk_pin: GPIO1
  i2s_dout_pin: GPIO2
  pin_a: GPIO9
  pin_b: GPIO10
  alarm_off_button_pin: GPIO4
  rotary_button_pin: GPIO8
  language: "EN" #NL and DE are also supported
  alarm_off_button_single_click_time: 1s #max time in seconds, which the alarm off button can be pressed to recognize the press
  timezone: Europe/Amsterdam

packages:
  remote_package_files:
    url: https://github.com/skons/soas
    files: [alarm-clock-soas.yaml] #Review the modules section!

time:
  - id: !extend ntp
    timezone: ${timezone}
    servers:
      - 0.pool.ntp.org
      - 1.pool.ntp.org
      - 2.pool.ntp.org

select:
  - id: !extend alarm_stream_url
    options:
      - "mp3 url to radio" #AAC seams to be making SOAS crash, FLAC or WAV will probably also work
      - "mp3 url to radio #2"
  - id: !extend alarm_stream_name
    options:
      - "Friendly name of the radio station"
      - "Friendly name of the radio station #2"

esphome:
  name: alarm-clock-soas
  friendly_name: Alarm Clock SOAS
  platformio_options: #PSRAM stuff
    build_flags: "-DBOARD_HAS_PSRAM"
    board_build.arduino.memory_type: qio_opi

psram:
  mode: octal
  speed: 80MHz
```

Save the `fonts` folder into your ESPHome folder. The folder needs to be placed in the same directory as your YAML.

Edit the `select` options with a stream URL and the name of the stream. To get some streams, install [radio browser](https://www.home-assistant.io/integrations/radio_browser/). Select a stream, send it to the clock and look at the ESPHome logging to view the URL of that stream.

Edit `alarm_file` to have your own local alarm. You can use mp3, wav or a flac file.

### <a name='Modules'></a>Modules

It is required that you choose the ESP-IDF or the Arduino module, all other modules are optional

#### <a name='ESP-IDFFramework'></a>ESP-IDF Framework

The ESP-IDF framework provides an `media_player` component which allows for playing a local file, which is used when the configured stream does not play. The downside of this framework is that is has more issues playing a stream. Crashes can occur.

Edit the YAML and make sure the `packages` and `esp32` at least contains the code from below.

``` yaml
packages:
  remote_package_files:
    url: https://github.com/skons/soas
    files: [alarm-clock-soas.yaml, alarm-clock-espidf.yaml]

esp32:
  board: esp32-s3-devkitc-1
  framework:
    type: esp-idf
  flash_size: 16MB
```

Add this to the `substitutions:`

``` yaml
  alarm_file: alarm.flac
```

#### <a name='ArduinoFramework'></a>Arduino Framework

The Arduino framework provides a simpeler `media_player` component, so to have a fallback you will need something like a RTTL buzzer. Another downside is that it does not report errors the way the ESP-IDF framework reports errors. Therefor it's more difficult to detect if the stream is playing. With the `Alarm on local after seconds` number it is possible to force the local alarm after a period.

Edit the YAML and make sure the `packages` and `esp32` at least contains the code from below.

``` yaml
packages:
  remote_package_files:
    url: https://github.com/skons/soas
    files: [alarm-clock-soas.yaml, alarm-clock-arduino.yaml]

esp32:
  board: esp32-s3-devkitc-1
  framework:
    type: arduino
  flash_size: 16MB
```

#### <a name='GPSOptional'></a>GPS (Optional)

Instead of using internet time (NTP), it is possible to use GPS which does not require internet. The NEO-6M module is supported. Keep in mind that it's not the fastest method, it could take some time before the GPS sattelites have been found. It's accuracy is very high though.

| NEO-6M | ESP32       |
|--------|-------------|
| TX     | GPIO44 (RX) |
| RX     | GPIO43 (TX) |
| VCC    | 3v          |
| GND    | GND         |

Add this to the `substitutions:`

``` yaml
  #NEO-6M
  gps_tx_pin: GPIO43
  gps_rx_pin: GPIO44
  timezone: Europe/Amsterdam
```

Add `alarm-clock-gps.yaml` to the `files:` property of `packages`.

#### <a name='DS1307Optional'></a>DS1307 (Optional)

With the DS1307 rtc module the clock keeps the time, also when the power is lost.

**No recharge mod**

Those modules are usally designed to work with rechargeable LIR2032 batterys and on 5V only (DS1307 spec is 4,5V - 5,5V). You must modify the DS1307 module in order to work with non rechargable and cheap CR2032 coin cells and the 3V3 IIC of the ESP32. On some ESP32-S3 PCB's you have to bridge a jumper to get +5V on the ESP out pin! Meassure the voltage before connect it, it should be arround ~5V (sometimes 4,7 because there is a diode in series, which is ok).
[Pictures of modifications in images folder starting with ds1307_...](images/)

| DS1307   | ESP32  | Remarks                               |
|----------|--------|---------------------------------------|
| SCL      | GPIO47 | Shared with the display               |
| SDA      | GPIO48 | Shared with the display               |
| VCC      | 5V     | On some boards, this must be soldered |
| GND      | GND    |                                       |
| 3V3 wire | 3V3    | Only with no recharge mod             |


Add `alarm-clock-ds1307.yaml` to the `files:` property of `packages`.

#### <a name='RTTTLOptional'></a>RTTTL (Optional)

**Important: Only use this module in combination with the Arduino framework module**

With an added buzzer, you can use this to play nostalgia rtttl sound (like on older phones or pc system speaker). The main goal is to use it as an fallback when internet is down, or the music stream malfunctions.

| BUZZER | ESP32  |
|--------|--------|
| RED    | GPIO21 |
| BLACK  | GND    |

Add this to the `substitutions:`

``` yaml
  #RTTTL
  buzzer_pin: GPIO21
  buzzer_gain: 50%
```
The buzzer_gain value did not work for me, i have just added an 56 Ohm series resistor to make it quiter.

Add this to `select:` (after your stream urls)

``` yaml
    #buzzer sounds - do not use too much, this crash SOAS
  - id: !extend alarm_buzzer_sound
    options:
      - "Mario:d=4,o=5,b=100:16e6,16e6,32p,8e6,16c6,8e6,8g6,8p,8g,8p,8c6,16p,8g,16p,8e,16p,8a,8b,16a#,8a,16g.,16e6,16g6,8a6,16f6,8g6,8e6,16c6,16d6,8b,16p,8c6,16p,8g,16p,8e,16p,8a,8b,16a#,8a,16g.,16e6,16g6,8a6,16f6,8g6,8e6,16c6,16d6,8b,8p,16g6,16f#6,16f6,16d#6,16p,16e6,16p,16g#,16a,16c6,16p,16a,16c6,16d6,8p,16g6,16f#6,16f6,16d#6,16p,16e6,16p,16c7,16p,16c7,16c7,p,16g6,16f#6,16f6,16d#6,16p,16e6,16p,16g#,16a,16c6,16p,16a,16c6,16d6,8p,16d#6,8p,16d6,8p,16c6"
      - "The Simpsons:d=4,o=5,b=160:c.6,e6,f#6,8a6,g.6,e6,c6,8a,8f#,8f#,8f#,2g,8p,8p,8f#,8f#,8f#,8g,a#.,8c6,8c6,8c6,c6"
      - "Two short tones:d=4,o=5,b=100:16e6,16e6"
  - id: !extend alarm_buzzer_name
    options:
      - "Mario"
      - "The Simpsons"
      - "Two short tones"
```

For additional tunes, see `alarm-clock-rtttl-additional-tunes.txt`. You can add them instead of the above. Do not add too much, this can crash SOAS.

Add `alarm-clock-rtttl.yaml` to the `files:` property of `packages`.

##### Usage

###### Options not available on the alarm itself

* Automatic enable of buzzer after defined period - This enables the buzzer after a period the alarm has been on. Failure detection with the Arduino framework is difficult due to the lack of good error reporting

###### <a name='Flatheadshortpress'></a>Flathead long press

When the buzzer is sounding, you can disable the buzzer with a long press. This can be handy when the music is on, and you want to listen without the buzzer interrupting.

###### Test a tune

To test a tune, you can use the HA service "tune" to play directly on the buzzer of the device, like this example:

``` yaml
action: esphome.soas_tune
data:
  tune: Two short tones:d=4,o=5,b=100:16e6,16e6
```

## <a name='Usage'></a>Usage

The rotary button is the button for accessing and editing configuration. When on a page, and there is no blinking of a configuration, you will automatically be redirected to the time page after 5 seconds of inactivity. The edit mode, blinking of a configuration, needs to be exited to return back to the time page. Entering and exiting the edit mode is done by single clicking the rotary button.

### <a name='Timepage'></a>Time page

#### <a name='Flatheadshortpress'></a>Flathead short press
When the alarm, sleep timer and snooze are off, single press will switch the music on. If the sleep timer is enabled, the sleep timer will also switch to on.

If the sleep timer is on and the music is on, the music will be switched off.
When the alarm, sleep timer and snooze are off, single press will switch the music on. If the sleep timer is enabled, the sleep timer will also switch to on.

If the sleep timer is on and the music is on, the music will be switched off.

When the alarm is on, snooze will switch on and the alarm will go to off.

When snooze is on, the snooze will be switched off on single press.

#### <a name='Rotarysingleclick'></a>Rotary single click
Single click of the rotary button toggles the alarm.

#### <a name='Rotarydoubleclick'></a>Rotary double click
Double click will bring you to the contrast page.

#### <a name='Rotarytripleclick'></a>Rotary triple click
Triple click will bring you to the reboot page.

#### <a name='Display'></a>Display
Blinking of the alarm icon (most left icon) means that the radio is on. This happens when it is switched on manually or by the time set as alarm. When it is displayed, but not blinking it means the alarm is enabled. If the radio is on. the rotary button does not allow to switch pages but instead it will interact with the volume. If settings must be changed, the radio must be switched off.

The second icon is the snooze icon and tells you if snooze is on.

The Home Assistant icon is cut into 4 pieces. Left top is HA 1, right top is HA 2, left bottom is HA 3 and right bottom is HA 4. Is that part visible?, then that HA switch is enabled and it will go off relative to the alarm time. If it is blinking, then the HA switch is on. The on switch will be on for 1 minute and after that it will be automatically switched off.

Blinking of the sleep timer icon (second to last icon) means that the sleep timer is on. When it is displayed, but not blinking, the sleep timer is enabled.

The SH1107 display contains more pixel space, therefor the day of the week and date is displayed. The alarm time is also displayed, if snooze is on, then the time when the alarm will sound is displayed.

### <a name='Alarmpage'></a>Alarm page

#### <a name='Rotarysingleclick-1'></a>Rotary single click
The alarm can be edited by single clicking the rotary button. This will make the time blink that is in edit mode. Single again and it will bring you to the minute. The rotary button is used to edit the hour or minute.

#### <a name='Rotarydoubleclick-1'></a>Rotary double click
Double click will enable or disable the alarm.

### <a name='HomeAssitantpage123and4'></a>Home Assitant page 1,2,3 and 4

This page displays if the Home Assistant switch is enabled, if the Home Assistant switch is associated with the alarm enabled and it shows the time relative to the alarm time. If the Home Assistant switch is enabled, it will switch to on even if the alarm is disabled.

#### <a name='Rotarysingleclick-1'></a>Rotary single click
Single click of the rotary button will enable editing of the relative time, the rotary button can then be used to change the time.

#### <a name='Rotarydoubleclick-1'></a>Rotary double click
Double click will toggle the Home Assistant enabled switch and alarmed switch. The enabled switch (right top icon) means the HA switch will switch on relative to the alarm time. The alarmed switch (right bottom icon) means that is associated with the alarm being enabled. If HA is enabled, alarmed off, then the HA switch will switched based on the alarm time but the alarm does not have to be enabled. If HA is enabled and alarmed is switched on, then the HA switch will switch on only when the alarm is also enabled.

First double click will enable the enabled switch. The second double click will enable both enabled and alarmed switch. The third will disable both switches.

#### <a name='Rotarytripleclick-1'></a>Rotary triple click
Triple click will switch the Home Assistant switch to on. This way it is possible to trigger your Home Assistant automation anytime you want. Regardless of the enabled and alarmed switched, and regardless of the alarm time.

### <a name='Sleeptimerpage'></a>Sleep timer page

#### <a name='Rotarysingleclick-1'></a>Rotary single click
With a single click with the rotary button you can define how long the music will play until it goes off.

#### <a name='Rotarydoubleclick-1'></a>Rotary double click
Double click will enable or disable the sleep timer.

### <a name='Radiopage'></a>Radio page

Enter edit mode with a rotary button single click to select the station that will be used for the radio. Switching to a radio station means the radio will go on.

### <a name='Volumeandcontrastpages'></a>Volume and contrast pages

The pages that are accessible from the time page are editable by rotating the rotary button. The contrast is depending on the day or night time. There is a different setting for both, and that can only be configured during that time.

### <a name='Restartpage'></a>Restart page

Use the rotary button to select the correct option, single click the rotary button to restart or cancel your action.

### <a name='Optionsnotavailableonthealarmitself'></a>Options not available on the alarm itself

A few options are not (yet) available on the alarm self:
* Snooze duration
* Display Mode
* Hide clock
* Alarm volume increase - The increase of volume after `Alarm volume increase duration`. Set to 0 to disable this feature.
* Alarm volume increase duration - The seconds that need to pass before the alarm volume is increased with `Alarm volume increase`. Set to 0 to disable this feature.
* Alarm on local after seconds - This will force the local alarm, even if the normal alarm is sounding. Set to 0 to disable this feature.
* Night mode - This mode is enabled by the sun long lat by default. `Night mode automatically` switched off will not switch the mode without a Home Assistant automation

Use Home Assistant to configure these options.

## <a name='Displaymode'></a>Display mode

There are 3 display modes:
* Full
* Minimum night only
* Minimum

The `Full` means that all is visible. The second indicator `:` will only blink during day time.

The `Minimum night only` will have a smaller font for less light. The wifi icon will not be displayed unless there is no wifi. The second indicator does not blink. This will only be displayed during night time.

`Minimum` is the same as `Minimum night only`, except that it is the mode also during daytime.

## <a name='FAQ'></a>FAQ

### <a name='SH1107SPII2C'></a>SH1107 SPI/I2C

Some SH1107 display modules support both I2C and SPI interface modes (one mode at a time). To switch to IIC mode, follow [this](https://simple-circuit.com/interfacing-arduino-sh1107-oled-display-i2c-mode/) tutorial and review [this](https://github.com/Skons/SOAS/issues/2#issue-3286014273) post.

## <a name='ToDo'></a>ToDo

* Ability to save streamed url to local instead of having a list of streams (https://alshowto.com/home-assistant-and-esphome-how-to-series-1-step-3-make-a-simple-media-speaker/, see things that are quirky)

## <a name='Knownissues'></a>Known issues
* The ESP-IDF framework seems to be causing frequent crashes (https://github.com/esphome/esphome/issues/10451)
* The GPS module seems to be causing crashes, even if the ESP-IDF framework does not cause crashes

## <a name='Changelog'></a>Changelog

### <a name='x.x.x'></a>2025.x.x.x
- **BREAKING** SOAS now supports Arduino and the ESP-IDF framework. You will need to configure your SOAS accordingly. Please review the modules sestion
- RTTTL module support
- DS1307 module support
- Support for local alarm on, even when the alarm is sounding
- Improved `night_mode` detection
- Documentation updates

### <a name=''></a>2025.9.7.1
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

### <a name='-1'></a>2025.8.25.2
  - Fixed that the sleep timer disabled the alarm, not the new music switch

### <a name='-1'></a>2025.8.25.1
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

### <a name='-1'></a>2025.7.14.1
 - ESP PSRAM implementation
 - Hide clock feature

### <a name='-1'></a>2025.4.24.1
 - Time handling improvements
 - Solved issue #1
 - PIN assignment not hardcoded anymore

### <a name='-1'></a>2025.4.8.1
 - Alarms are now more resillient, it runs as soon as it is missed
 - Multiple small improvements

### <a name='-1'></a>2025.2.23.1
 - Multiple switches and numbers require time to be synced before their actions will take place, this makes sure settings will survive a reboot
 - Snooze and the sleep timer are more resillient when they miss their schedule due to a reboot
 - Multiple small bug fixes

### <a name='-1'></a>2025.2.15.1
 - Playback will be restored after reboot

### <a name='-1'></a>2025.2.14.1
- Number cleanup
- Switched to sliders instead of boxes for numbers
- Alarm volume number is not internal anymore

### <a name='-1'></a>2025.2.13.1
- Initial commit
