#ifndef __LEDs_H
#define __LEDs_H

#include <avr/io.h>         //gives us names for registers
#include <avr/interrupt.h>

#include <inttypes.h> //gives us uintX_t

#include <string.h>

/* 
	IO Port Information starts on pg 72 of the Atmega datasheet.
	DDRn : data direction register
	PORTn : write to port  (when configured as output)
	PINn : read from port  (when configured as input)

	LEDS should be on the following pins:
		Yellow : Port A Pin 0
		Green  : Port D Pin 5 ( OC1A )
		Red    : Port A Pin 2

*/

/*
// define the output ports by which you send signals to the LEDs
#define PORT_RED     
#define PORT_YELLOW  
#define PORT_GREEN   

// define the bit-masks for each port that the LEDs are attached to
#define BIT_RED      ( 1 << )
#define BIT_YELLOW   ( 1 << )
#define BIT_GREEN    ( 1 << )

// define "function" calls for turning LEDs on and off
#define LED_ON(x)     (PORT_##x |= BIT_##x)
#define LED_OFF(x)    (PORT_##x &= ~(BIT_##x))
#define LED_TOGGLE(x) (PORT_##x ^= BIT_##x)

 */

#endif

