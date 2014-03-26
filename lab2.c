#define ECHO2LCD

#include <pololu/orangutan.h>

// System tasks
//#include "menu.h"

//Gives us uintX_t (e.g. uint32_t - unsigned 32 bit int)
//On the ATMega128 int is actually 16 bits, so it is better to use
//  the int32_t or int16_t so you know exactly what is going on
#include <inttypes.h> //gives us uintX_t

// useful stuff from libc
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// GLOBALS
// tick count for scheduler, yellow task, green task
volatile uint32_t G_msTicks = 0;

// shared variables with ISRs, including 
// release flags, task period, toggle counts

int main(void) {
	// -------------------------------------------------------------
	// This program teaches you about the various ways to "schedule" tasks.
	// You can think of the three blinking LEDs as separate tasks with 
	// strict timing constraints.
	//
	// As you build your program, think about the guarantees that you can 
	// make regarding the meeting of deadlines. Think about how the CPU
	// is "communicating" with the LEDs. Obviously, everything is output,
	// but what if it was input? Also think about how data can be passed
	// among tasks and how conflicts might arise.
	//
	// TASKS
	//
	// Establish WCET analysis on a for loop to use for timing.
	// Use the for loop to blink the red LED.
	//
	// Set up a system 1 ms software timer (16-bit) and use that to "schedule" 
	// tasks.
	//
	// Toggle the red LED inside a cyclic executive.
	//
	// Toggle the yellow LED using a separate timer (8-bit) with a 100ms resolution.
	// Toggle the LED inside the ISR.
	//
	// Toggle the green LED by toggling the output on a pin using
	// a Compare Match. This is the creation of a PWM signal with a very long period.
	// Count toggles inside the corresponding ISR (don't forget to turn this
	// off when the green LED is turned off.
	//
	// Scheduler releases the menu task.
	// The menu allows the user to modify the period of each task.
	//
	// --------------------------------------------------------------

	int i;

	// Used to print to serial comm window
	char tempBuffer[32];
	int length = 0;
	
	// Turn all LEDs on for a second or two then turn off to confirm working properly
	// Send a message to LCD to confirm proper start. ( interrupts might need to be on for this ?? )
	// Send a message through serial comm to confirm working properly.

	// Initialize All Tasks

	clear();	// clear the LCD

	//enable interrupts
	sei();
	
	while (1) {
		/* BEGIN with a simple toggle using for-loops. No interrupt timers */

		// --------- blink LED by using a busy-wait delay implemented with an empty for-loop
/*
		LED_TOGGLE(RED);
		G_redToggles++;
		length = sprintf( tempBuffer, "R toggles %d\r\n", G_redToggles );
		print_usb( tempBuffer, length );

 #ifdef ECHO2LCD
		lcd_goto_xy(0,0);
		printf("R:%d ",G_redToggles);
#endif

		// create a for-loop to kill approximately 1 second
		for (i=0;i<100;i++) {
			WAIT_10MS;
		}
*/	
		// ----------- COMMENT OUT above implementation of toggle and replace with this...
		// ------------------- Have scheduler release tasks using user-specified period



		// --------------- RED task
		
		// --------------- MENU task
/*
		serial_check();
		check_for_new_bytes_received();
*/
					
	} //end while loop
} //end main

