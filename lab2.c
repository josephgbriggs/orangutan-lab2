#define ECHO2LCD

#include <pololu/orangutan.h>

#include "led.h"

// System tasks
#include "menu.c"
#include "tasks.c"

//Gives us uintX_t (e.g. uint32_t - unsigned 32 bit int)
//On the ATMega128 int is actually 16 bits, so it is better to use
//  the int32_t or int16_t so you know exactly what is going on
#include <inttypes.h> //gives us uintX_t

// useful stuff from libc
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*-------------------------------------------------------------
This program teaches you about the various ways to "schedule" tasks.
You can think of the three blinking LEDs as separate tasks with 
strict timing constraints.

As you build your program, think about the guarantees that you can 
make regarding the meeting of deadlines. Think about how the CPU
is "communicating" with the LEDs. Obviously, everything is output,
but what if it was input? Also think about how data can be passed
among tasks and how conflicts might arise.

TASKS

Establish WCET analysis on a for loop to use for timing.
Use the for loop to blink the red LED.

Set up a system 1 ms software timer (16-bit) and use that to "schedule" 
tasks.

Toggle the red LED inside a cyclic executive.

Toggle the yellow LED using a separate timer (8-bit) with a 100ms resolution.
Toggle the LED inside the ISR.

Toggle the green LED by toggling the output on a pin using
a Compare Match. This is the creation of a PWM signal with a very long period.
Count toggles inside the corresponding ISR (don't forget to turn this
off when the green LED is turned off.

Scheduler releases the menu task.
The menu allows the user to modify the period of each task.

--------------------------------------------------------------*/


// GLOBALS
// tick count for scheduler, yellow task, green task
volatile uint32_t G_msTicks = 0;

// shared variables with ISRs, including 
// release flags, task period, toggle counts
volatile uint32_t G_redToggles = 0;
volatile uint32_t G_yellowToggles = 0;
volatile uint32_t G_greenToggles = 0;

volatile uint16_t G_redPeriod = 0;
volatile uint16_t G_yellowPeriod = 0;
volatile uint16_t G_greenPeriod = 0;

volatile uint8_t G_redTaskRelease;
volatile uint8_t G_menuTaskRelease;

volatile uint16_t G_yellowBuffer = 0;

void configuration_check() {
	// Turn all LEDs on for a second or two then turn off to confirm working properly
	INIT_LEDS();
	
	// Send a message to LCD to confirm proper start
	print("LED blink test...");
	
	LED_ON(GREEN);
	delay_ms(200);
	LED_ON(YELLOW);
	delay_ms(200);
	LED_ON(RED);
	delay_ms(200);
	
	for (int i=0; i<10; i++) {
		LED_TOGGLE(GREEN);
		LED_TOGGLE(YELLOW);
		LED_TOGGLE(RED);
		delay_ms(100);
	}
	
	LED_OFF(GREEN);
	LED_OFF(YELLOW);
	LED_OFF(RED);
	
	lcd_goto_xy(0, 1);	// go to start of second LCD row
	print("...done");
	delay_ms(200);
	clear();	// clear the LCD
	
	// Send a message through serial comm to confirm working properly
	print("Waiting for");
	lcd_goto_xy(0, 1);
	print(" serial conn...");
	print_usb("Welcome to lab2!\r\n", 18);	// this will block if no serial
	clear();	// clear the LCD
}

int main(void) {
	
	configuration_check();
	
	// Initialize All Tasks
	initialize_scheduler();
	initialize_green_task();
	initialize_yellow_task();
	init_menu();
	
	//enable interrupts
	sei();
	
	while (1) {
		
		// ---- Have scheduler release tasks using user-specified period
		// --------------- RED task
		if (G_redTaskRelease) {
			G_redTaskRelease = 0;
			LED_TOGGLE(RED);
			// count only the times we turn the light on
			if (PORTA & BIT_RED) {
				G_redToggles++;
			}
		}
		
		// --------------- MENU task
		if (G_menuTaskRelease) {
			G_menuTaskRelease = 0;
			serial_check();
			check_for_new_bytes_received();
			
		}
		
		/*
		// Used to print to serial comm window
		char tempBuffer[32];
		int length = 0;
		if (G_msTicks % 100 == 0) {

			length = sprintf( tempBuffer, "Ticks %u, ", G_msTicks );
			print_usb( tempBuffer, length );
			length = sprintf( tempBuffer, "R per %u, ", G_redPeriod );
			print_usb( tempBuffer, length );
			length = sprintf( tempBuffer, "R tog %u; ", G_redToggles );
			print_usb( tempBuffer, length );
			length = sprintf( tempBuffer, "Y buf %u, ", G_yellowBuffer );
			print_usb( tempBuffer, length );
			length = sprintf( tempBuffer, "Y per %u, ", G_yellowPeriod );
			print_usb( tempBuffer, length );
			length = sprintf( tempBuffer, "Y tog %u, ", G_yellowToggles );
			print_usb( tempBuffer, length );
			length = sprintf( tempBuffer, "G per %u, ", G_greenPeriod );
			print_usb( tempBuffer, length );
			length = sprintf( tempBuffer, "G tog %u\r\n", G_greenToggles );
			print_usb( tempBuffer, length );
		}
		*/
				
	} //end while loop
} //end main

