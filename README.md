# Switchless Mod

PIC Microcontroller code for implementing switchless console modifications.

## Supported Consoles

* Nintendo Super NES/Super Famicom
* Sega Mega Drive/Genesis
* Sega Saturn

## Hardware

Implementaions for all consoles use the same PIC16F630 microcontroller and the
same pinout, as illustrated below.

```
                        ______________
                      _|     \__/     |_
               +5V   [_| 1 Vdd Vss 14 |_]   Ground
            GPIO 3   [_| 2 RA5 RA0 13 |_]   Reset button (IN)
            GPIO 2   [_| 3 RA4 RA1 12 |_]   50/60Hz (OUT)
            GPIO 4   [_| 4 RA3 RA2 11 |_]   Reset console (OUT)
   LED Green (OUT)   [_| 5 RC5 RC0 10 |_]   GPIO 0
     LED Red (OUT)   [_| 6 RC4 RC1  9 |_]   GPIO 1
     LED type (IN)   [_| 7 RC3 RC2  8 |_]   LED Blue (OUT)
                       |______________|

```

* *+5V/Ground*:
  DC power. This is hooked up to the console's internal power supply.

* *Reset button/Reset console*:
  The console's reset button, usually isolated form the system, is connected to
  the _Reset button_ pin. When pressed briefly (by default < 500ms) the _Reset
  console_ pin is activated, resetting the console as normal. Holding the reset
  button cycles through the available modes.

* *50/60Hz Out
  Sets the console to 50Hz or 60Hz as determined by the currently selected
  mode.

* GPIO 0-4
  Used to set the console's region (or other features) depending on the
  currently selected mode. The state of each pin varies per console (see
  below).

* LED Red/Green/Blue
  Individual pins to connect to the red, green and blue components of a
  multi-coloured LED (through a suitable resistor). This LED is used to display
  the currently selected mode.

* LED type
  Used to select the type of LED used. For a common cathode LED, connect to
  ground. For common anode, connect to +5V, preferably via a pull-up resistor.

## Console Configuration

The following default modes are preconfigured in the C source file. These can
of course be edited before compilation.

### Nintendo Super NES/Super Famicom
For this console, the SuperCIC (https://sd2snes.de/blog/cool-stuff/supercic)
is recommended instead. Otherwise, this code offers four modes which mirror
the states of the traditional switched mod. This allows independent switching
between 50Hz and 60Hz modes, as well as enabling/disabling the region lockout
chip (CIC).

#### PAL (Europe) Consoles

| LED colour | Video | CIC enabled |
|------------|-------|-------------|
| Red        | 50Hz  | Yes         |
| Yellow     | 50Hz  | No          |
| Green      | 60Hz  | No          |
| Blue       | 60Hz  | Yes         |

#### NTSC (US/JP) Consoles

| LED colour | Video | CIC enabled |
|------------|-------|-------------|
| Red        | 60Hz  | Yes         |
| Yellow     | 60Hz  | No          |
| Green      | 50Hz  | No          |
| Blue       | 50Hz  | Yes         |

### Sega Mega Drive/Genesis
For this console, it's possible to switch languages (English/Japanese) and
video mode (50/60Hz).

| LED colour | Region | Video | Language |
|------------|--------|-------|----------|
| Blue       | USA    | 60Hz  | English  |
| Red        | Japan  | 60Hz  | Japanese |
| Green      | Europe | 50Hz  | English  |

If your console has an active high reset, this can be catered for at compile
time using the `RESET_ACTIVE` define.

### Sega Saturn
The Saturn offers a great number of region and video mode combinations. The
most useful are enabled by default.

| LED colour | Region | Video |
|------------|--------|-------|
| Blue       | USA    | 60Hz  |
| Red        | Japan  | 60Hz  |
| Green      | Europe | 50Hz  |
| Yellow     | Europe | 60Hz  |

