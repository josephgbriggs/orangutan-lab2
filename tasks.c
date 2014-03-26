#include "tasks.h"

// NOTE: MENU task is in separate file menu.c

// extern GLOBALS


//--------------------------- SCHEDULER ------------------------------//
	// 1 ms Software Clock Using Timer/Counter 3.
	// Uses flags to release red toggle and serial communication (i.e. menu task)


// -------------------------  RED ------------------------------------//
	// Scheduler releases with flag. Executes inside  cyclic executive.
	// Keep count of the number of toggles.


//--------------------------- YELLOW ---------------------------------//
	// 100 ms Softward Clock using 8-bit timer.
	// Period of task is multiple of 100ms.
	// Yellow toggled inside ISR.
	// Keep count of the number of toggles.
	//
	// You could use a single timer to schedule everything, but we are experimenting here.


//--------------------------- GREEN ----------------------------------//

	// Toggle with PWM signal on OCR1A. The period of the PWM IS the period of the toggle.
	// Toggle period is limited by the range of the counter. Set prescaler and counter to maximize range.
	// Use the corresponding interrupt to keep count of toggles.
	//
	// Don't forget to stop counting toggles when green turned off.


// When setting up timers or PWM signals, remember...
/* 
 Set WGM bits in TCCR registers
 Set Clock Select Bits in TCCR registers
 Set COM bits, if needed, in TCCR registers
 Set OCRxx
 Enable Interrupt (if using)
 Define ISR() (if using)
 */


// You don't have to use the code below, but this functionality is needed.
/*
void setPeriod(char task, int ms) {
		
	if (~((ms%100)==0)) {
		ms = ms - (ms%100);
		printf("Converted to period: %d.\n",ms);
	}
	
	// For each task, if ms is 0, turn it off, else turn it on and set appropriately
	
	if ( ( task ==' R' ) || ( task == 'A' ) ) {
		if ( ms == 0 )
			
			else
				
				G_redPeriod = ms;
	}
	
	if ( ( task == 'Y') || ( task =='A')) {
		if ( ms == 0 )
			
			else
				
				G_yellowPeriod = ms;
	}
	
	if (( task == 'G') || ( task == 'A' )) {
		if ( ms == 0 )
			
			else
				
				
				// green has a limit on its period.
				if ( ms > 4000) ms = 4000;
		G_greenPeriod = ms;
		
		// set requested frequency.
		
	}
}
*/
