/*
 * PIC Microcontroller code for implementing switchless console modifications.
 * Copyright (C) 2015  Steve Maddison <steve@atomicretro.nl>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#define _XTAL_FREQ 4000000
#include <pic.h>
#include <xc.h>

#pragma config WDTE=OFF
#pragma config PWRTE=OFF
#pragma config CP=OFF
#pragma config BOREN=ON
#pragma config MCLRE=OFF
#pragma config CPD=OFF
#pragma config FOSC=INTRCIO

/*
 * Compile-time configuration
 */

/* LED type */
#define LED_COMMON_CATHODE  0
#define LED_COMMON_ANODE    1

/* LED port */
#define LED_PORT            PORTC
#define LED_TRIS            TRISC

/* LED colours */
#define LED_RED             0x20
#define LED_GREEN           0x10
#define LED_BLUE            0x08

/* Reset properties */
#define RESET_ACTIVE_HIGH   1
#define RESET_ACTIVE_LOW    0

/* Reset port(s) and pins */
#define RESET_IN_PORT       PORTA
#define RESET_IN_TRIS       TRISA
#define RESET_IN            0x02

#define RESET_OUT_PORT      PORTA
#define RESET_OUT_TRIS      TRISA
#define RESET_OUT           0x04

/* Mode array uses these index values */
typedef enum {
  MODE_PORT_A = 0,
  MODE_PORT_C,
  MODE_PORTS,
} mode_port_t;
/* Map above indices to actual ports */
volatile unsigned char *mode_port[MODE_PORTS] = {
  &PORTA,
  &PORTC
};
/* Same for TRIS */
volatile unsigned char *mode_tris[MODE_PORTS] = {
  &TRISA,
  &TRISC
};
#define PORT_MASK 0x3f
#define TRIS_MASK PORT_MASK

/*
 * Helper macros
 */
#define LED_MASK    (LED_RED|LED_GREEN|LED_BLUE)
#define LED_YELLOW  (LED_RED|LED_GREEN)
#define LED_CYAN    (LED_GREEN|LED_BLUE)
#define LED_MAGENTA (LED_RED|LED_BLUE)
#define LED_WHITE   (LED_RED|LED_GREEN|LED_BLUE)
#define LED_OFF     0x00

/*
 * Modes
 */
typedef enum {
    REGION_USA = 0,
    REGION_JAPAN,
    REGION_EUROPE,
    REGION_EURO60,
} region_t;

#ifdef CONSOLE_MEGA_DRIVE
/*
 * Sega Mega Drive/Genesis
 *
 * As the Mega Drive/Genesis doesn't have "regions" as such (just languages)
 * a 60Hz "European" mode is equivalent to USA.
 */
#define MODES       3

#define MD_ENGLISH  0x01
#define MD_JAPANESE 0x00
#define MD_PAL      0x00
#define MD_NTSC     0x02

const unsigned char mode[MODES][MODE_PORTS] = {
    /* PORTA bits,   PORTC bits */
    { (MD_NTSC), (MD_ENGLISH)  }, /* USA */
    { (MD_NTSC), (MD_JAPANESE) }, /* Japan */
    { (MD_PAL),  (MD_ENGLISH)  }  /* Europe */
};
const unsigned char colour[MODES] = {
    (LED_BLUE),
    (LED_RED),
    (LED_GREEN),
};

#elif defined CONSOLE_SATURN
/*
 * Sega Saturn
 */
#define MODES          4

#define SATURN_JP6  0x01
#define SATURN_JP10 0x02
#define SATURN_JP12 0x04

#define SATURN_PAL  0x00
#define SATURN_NTSC 0x02

const unsigned char mode[MODES][MODE_PORTS] = {
    /* PORTA bits,   PORTC bits */
    { (SATURN_NTSC), (SATURN_JP10)             }, /* USA */
    { (SATURN_NTSC), (SATURN_JP6)              }, /* Japan */
    { (SATURN_PAL),  (SATURN_JP10|SATURN_JP12) }, /* Europe */
    { (SATURN_NTSC), (SATURN_JP10|SATURN_JP12) }, /* Europe 60Hz */
};
const unsigned char colour[MODES] = {
    (LED_BLUE),
    (LED_RED),
    (LED_GREEN),
    (LED_YELLOW),
};
#else
#error "No CONSOLE type defined."
#endif

unsigned char mode_mask[MODE_PORTS];

/*
 * Defaults
 */
#ifndef LED_MODE
#define LED_MODE LED_COMMON_CATHODE
#endif

#ifndef RESET_ACTIVE
#define RESET_ACTIVE RESET_ACTIVE_LOW
#endif

/*
 * Set the LED to a specific colour (see the LED_* macros).
 */
void set_led(unsigned char colour) {
    unsigned char state = LED_PORT;
    unsigned char led_bits = colour;

    if( LED_MODE == LED_COMMON_ANODE ) {
        led_bits = ~led_bits;
    }
    led_bits &= LED_MASK;

    state &= ~LED_MASK;
    state |= led_bits;

    LED_PORT = (state & PORT_MASK);
}

/*
 * Should an unrecoverable error occur, blink the LED. This should of course
 * never happen...
 */
void error(unsigned char code) {
    int i = 0;
    int blinks = code;

    if( blinks < 1 ) {
        blinks = 1;
    }

    while(1) {
        set_led(LED_OFF);
        __delay_ms(1000);

        for ( i = 0 ; i < blinks ; i++ ) {
            set_led(LED_RED);
            __delay_ms(200);
            set_led(LED_OFF);
            __delay_ms(500);
        }
    }
}

/*
 * Set the mode outputs on all applicable ports.
 */
void set_mode(unsigned char m) {
    int p = 0;

    for( p = 0 ; p < MODE_PORTS ; p++ ) {
        unsigned char state = *mode_port[p];
        unsigned char mode_bits = mode[m][p] & mode_mask[p];

        state &= ~mode_mask[p];
        state |= mode_bits;

        *mode_port[p] = (state & PORT_MASK);
    }

    set_led(colour[m]);
}

void init_chip(void) {
    int p = 0;

    CMCON = 0x03; /* Disable comparator */

    for( p = 0 ; p < MODE_PORTS ; p++ ) {
        unsigned char outputs = mode_mask[p];

        if( mode_port[p] == &LED_PORT ) {
            outputs |= LED_MASK;
        }
        if( mode_port[p] == &RESET_IN_PORT ) {
            /* Leave as input */
        }
        if( mode_port[p] == &RESET_OUT_PORT ) {
            outputs |= RESET_OUT;
        }

        *mode_port[p] = 0;
        *mode_tris[p] = ((~outputs) & TRIS_MASK);
    }
}

void main(void) {
    int i = 0;
    int p = 0;

    /* Calculate the bitmasks for the pins used for mode output, based on
     * configuration. This could be done at compile time but this way is easy
     * and a bit dryer. */
    for( p = 0 ; p < MODE_PORTS ; p++ ) {
        mode_mask[p] = 0;
        for( i = 0 ; i < MODES ; i++ ) {
            mode_mask[p] |= mode[i][p];
        }
    }

    /* Initialisation */
    init_chip();

    while(1) {
        __delay_ms(1000);
    }
}

