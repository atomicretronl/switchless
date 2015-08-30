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
   Reset type (IN)   [_| 4 RA3 RA2 11 |_]   Reset console (OUT)
   LED Green (OUT)   [_| 5 RC5 RC0 10 |_]   GPIO 0
     LED Red (OUT)   [_| 6 RC4 RC1  9 |_]   GPIO 1
     LED type (IN)   [_| 7 RC3 RC2  8 |_]   LED Blue (OUT)
                       |______________|

```

* _+5V/Ground_:
  DC power. This is hooked up to the console's internal power supply.

* _Reset button/Reset console_:
  The console's reset button, usually isolated form the system, is connected to
  the _Reset button_ pin. A pull-up/pull-down resistor may be necessary for
  stable operation, once the button is isolated. When pressed briefly (by
  default < 500ms) the _Reset console_ pin is activated, resetting the console
  as normal. Holding the reset button cycles through the available modes.

* _Reset type_:
  For consoles with active low reset, tie this pin to ground. For consoles with
  active high reset, connect to +5V, preferably via a pull-up resistor.

* _50/60Hz Out_:
  Sets the console to 50Hz or 60Hz as determined by the currently selected
  mode.

* _GPIO 0-3_:
  Used to set the console's region (or other features) depending on the
  currently selected mode. The state of each pin varies per console (see
  below).

* _LED Red/Green/Blue_:
  Individual pins to connect to the red, green and blue components of a
  multi-coloured LED (through a suitable resistor). This LED is used to display
  the currently selected mode.

* _LED type_:
  Used to select the type of LED used. For a common cathode LED, connect to
  ground. For common anode, connect to +5V, preferably via a pull-up resistor.

## Console Configuration

The following default modes are preconfigured in the C source file. These can
of course be edited before compilation.

### Nintendo Super NES/Super Famicom
For this console, the [SuperCIC](https://sd2snes.de/blog/cool-stuff/supercic)
mod is recommended as this offers full region-free operation. Otherwise, this
code offers four modes which mirror the states of the traditional switched mod.
This allows independent switching between 50Hz and 60Hz modes, as well as
enabling/disabling the region lockout chip (CIC).

#### PAL (Europe) Consoles

| LED colour | Video | CIC enabled | Description                    |
|------------|-------|-------------|--------------------------------|
| Red        | 50Hz  | Yes         | Factory default.               |
| Yellow     | 50Hz  | No          | Factory default, CIC disabled. |
| Green      | 60Hz  | No          | 60Hz mode, CIC disabled.       |
| Blue       | 60Hz  | Yes         | 60Hz mode, CIC enabled.        |

#### NTSC (US/JP) Consoles

| LED colour | Video | CIC enabled | Description                    |
|------------|-------|-------------|--------------------------------|
| Red        | 60Hz  | Yes         | Factory default.               |
| Yellow     | 60Hz  | No          | Factory default, CIC disabled. |
| Green      | 50Hz  | No          | 60Hz mode, CIC disabled.       |
| Blue       | 50Hz  | Yes         | 60Hz mode, CIC enabled.        |

#### Pin Functions
| Pin     | Description                                               |
|---------|-----------------------------------------------------------|
| 50/60Hz | Connect to PPU1 pin 24 and PPU2 pin 30 (lifted from PCB). |
| GPIO 0  | Connect to CIC pin 4 (lifted from PCB).                   |

### Sega Mega Drive/Genesis
For this console, it's possible to switch languages (English/Japanese) and
video mode (50/60Hz).

| LED colour | Region | Video | Language |
|------------|--------|-------|----------|
| Blue       | USA    | 60Hz  | English  |
| Red        | Japan  | 60Hz  | Japanese |
| Green      | Europe | 50Hz  | English  |

#### Pin Functions
| Pin     | Description                         |
|---------|-------------------------------------|
| 50/60Hz | Connect to common point of JP3/JP4. |
| GPIO 0  | Connect to common point of JP1/JP2. |

Existing connections on named jumpers must be cut. If your console has an
active high reset, this can be catered for by correctly connecting the _Reset
type_ pin.

### Sega Saturn
The Saturn offers a great number of region and video mode combinations. The
most useful are enabled by default.

| LED colour | Region | Video |
|------------|--------|-------|
| Blue       | USA    | 60Hz  |
| Red        | Japan  | 60Hz  |
| Green      | Europe | 50Hz  |
| Yellow     | Europe | 60Hz  |

#### Pin Functions
| Pin     | Description                               |
|---------|-------------------------------------------|
| 50/60Hz | Connect to IC14 pin 79 (lifted from PCB). |
| GPIO 0  | Connect to common point of JP6/JP7.       |
| GPIO 1  | Connect to common point of JP10/JP11.     |
| GPIO 2  | Connect to common point of JP12/JP13.     |

