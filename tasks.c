#include "tasks.h"

// NOTE: MENU task is in separate file menu.c

// extern GLOBALS
extern volatile uint32_t G_msTicks;
extern volatile uint8_t G_redTaskRelease;
extern volatile uint8_t G_menuTaskRelease;

extern volatile uint32_t G_redToggles;
extern volatile uint32_t G_yellowToggles;
extern volatile uint32_t G_greenToggles;

extern volatile uint16_t G_redPeriod;
extern volatile uint16_t G_yellowPeriod;
extern volatile uint16_t G_greenPeriod;

extern volatile uint16_t G_yellowBuffer;

//--------------------------- SCHEDULER ------------------------------//
/* 1 ms Software Clock Using (Timer/Counter 2)
 * Uses flags to release red toggle and serial communication (i.e. menu task)
 *
 * Timer/Counter 2 is a 8-bit counter, so the largest unsigned value it can
 * hold is 255. See data sheet on p.161 for prescalers
 * 
 * We want 1 ms precision, so that equates to 1000 Hz
 * Based on our 20 MHz processor, here are the closest approximations
 *
 *  PRESCALER	      Hz		255 counts is	Closest approx to 1 ms
 *  -----------	-------------	-------------	----------------------
 * 	clkT2S/1	20,000,000 Hz	 0.0128 ms		*overflow*
 * 	clkT2S/8	 2,500,000 Hz	 0.102 ms		*overflow*
 * 	clkT2S/32	   625,000 Hz	 0.408 ms		*overflow*
 * 	clkT2S/64	   312,500 Hz	 0.816 ms		*overflow*
 * 	clkT2S/128	   156,250 Hz	 1.632 ms		156 counts --> 0.9984 ms
 * 												157 counts --> 1.005 ms
 * 	clkT2S/256	    78,125 Hz	 3.264 ms		78 counts  --> 0.9984 ms
 * 												79 counts  --> 1.0112 ms
 * 	clkT2S/1024	    19,531 Hz	13.056 ms		20 counts  --> 1.024 ms
 * 
 */
#define SCHEDULER_CLK_BITS ((1 << CS22) | (1 << CS20))
void initialize_scheduler(void) {
	// set the clock tick prescaler bits to use clkio/128
	TCCR2B |= SCHEDULER_CLK_BITS;

	// Set control register bits for 'mode 7' 
	// - Fast PWM with OCRA to define the TOP
	TCCR2B |= (1 << WGM22);
	TCCR2A |= ((1 << WGM21) | (1 << WGM20));

	// set the top of the wave to fire the ISR every millisecond
	OCR2A = 156;
	
	// turn the interrupt on
	TIMSK2 |= (1 << OCIE2A);
	
}


// -------------------------  RED ------------------------------------//
	// Scheduler releases with flag. Executes inside  cyclic executive.
	// Keep count of the number of toggles.
ISR(TIMER2_COMPA_vect) {
	G_msTicks++;

	// fire the release at half the period since we are toggling
	if (G_redPeriod == 0) {
		// ignore
		G_redTaskRelease = 0;
	}
	else if ( G_msTicks % (G_redPeriod / 2)  == 0 ) {
		G_redTaskRelease = 1;
	}
	
	// always release menu task
	G_menuTaskRelease = 1;
}


//--------------------------- YELLOW ---------------------------------//
	// 100 ms Software Clock using 16-bit timer (Timer/Counter 3)
	// Period of task is multiple of 100ms.
	// Yellow toggled inside ISR.
	// Keep count of the number of toggles.
	//
	// You could use a single timer to schedule everything, but we are experimenting here.
/* 16-bit count up to 65,535
 *
 * (20MHz) * (1/PRESCALE) * (1/C) = 1 ISR / 100 ms
 * 
 *  PRESCALER	   Hz		 C (100 ms)	Cmax (T ms)
 *  -----------	---------	----------	------	
 * 	clkT3S/256	78,125 Hz	7812		7812 * 8  = 62,469	-> 8*100ms = 800 ms
 * 	clkT3S/1024	19,531 Hz	1953 		1953 * 33 = 64,449 -> 33*100ms = 3300 ms
 */
#define YELLOW_CLK_BITS ((1 << CS32) | (1 << CS30))

void initialize_yellow_task(void) {
	
	// set the clock tick prescaler bits to use clkio/1024
	TCCR3B |= YELLOW_CLK_BITS;
	
	// Compare Output pins disconnected (OC3A/B not used)
	// Leave default COM settings in TCCR3A
	
	// Set waveform generation to CTC 'mode 4', OCR3A to define the TOP
	TCCR3B |= (1 << WGM32);

	OCR3A = 1953; // 100 ms TOP
	
	// set clock off initially
	TCCR3B &= ~YELLOW_CLK_BITS;
	
	// turn on the interrupt
	TIMSK3 |= 1 << OCIE3A;
}

ISR(TIMER3_COMPA_vect) {
	G_yellowBuffer += 100;
	
	if (G_yellowBuffer == G_yellowPeriod) {
		G_yellowBuffer = 0;
		G_yellowToggles++;
		LED_ON(YELLOW);
	}
	else if (G_yellowBuffer >= (G_yellowPeriod / 2)) {
		LED_OFF(YELLOW);
	}
	
}


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
/* 16-bit count up to 65,535
 *
 * (20MHz) * (1/PRESCALE) * (1s/1000ms) * (1/65,535) = 1 ISR / N ms
 * 
 *  PRESCALER	      Hz		 N (16-bit ovrflw)
 *  -----------	-------------	------------------	
 * 	clkT1S/64	   312,500 Hz	 209.71 ms	
 * 	clkT1S/256	    78,125 Hz	 838.84 ms	
 * 	clkT1S/1024	    19,531 Hz	3355.43 ms	
 *
 * counts/ms = 19.5
 */
#define GREEN_PERIOD_SCALER 39/2
#define GREEN_CLK_BITS ((1 << CS12) | (1 << CS10))
void initialize_green_task(void) {
	// set the clock tick prescaler bits to use clkio/1024
	TCCR1B |= GREEN_CLK_BITS;

	// Set control register bits for 'mode 14' 
	// - Fast PWM with ICR1 to define the TOP
	TCCR1B |= ((1 << WGM13) | (1 << WGM12));
	TCCR1A |= (1 << WGM11);

	// set ICR for TOP value for wave
	uint16_t waveTop = G_greenPeriod * GREEN_PERIOD_SCALER;
	ICR1 = waveTop;

	// 50% duty cycle for the light
	OCR1A = waveTop / 2;

	// Set to Clear OC1A on Compare Match, set OC1A at BOTTOM (non-inverting mode)
	TCCR1A |= 1 << COM1A1;

	// set the data direction register for PortD Pin5 where we are putting our
	// green LED
	DDRD |= 1 << DDD5;

	// set the clock off initially
	TCCR1B &= ~GREEN_CLK_BITS;

	// turn on the interrupt
	TIMSK1 |= 1 << OCIE1A;
}


ISR(TIMER1_COMPA_vect) {
	G_greenToggles++;
}

// You don't have to use the code below, but this functionality is needed.

void setPeriod(char task, int ms) {
		
	if (~((ms%100)==0)) {
		ms = ms - (ms%100);
		printf("Converted to period: %d.\n",ms);
	}
	
	// For each task, if ms is 0, turn it off, else turn it on and set appropriately
	
	if ( ( task == 'R' ) || ( task == 'A' ) ) {
		
		G_redPeriod = ms;
	}
	
	if ( ( task == 'Y') || ( task =='A')) {
		if ( ms == 0 ) {
			// turn Y off
			TCCR3B &= ~YELLOW_CLK_BITS;
		}
		else {
			TCCR3B |= YELLOW_CLK_BITS;
			G_yellowPeriod = ms;
		}
	}
	
	if (( task == 'G') || ( task == 'A' )) {
		if ( ms == 0 ){
			// turn G off
			TCCR1B &= ~GREEN_CLK_BITS;
		}
		else {
			TCCR1B |= GREEN_CLK_BITS;
			// green has a limit on its period.
			if ( ms > 3300) { 
				ms = 3300; 
				printf("Converted to period: %d.\n",ms);
			}
			
			G_greenPeriod = ms;
			
			// set ICR for TOP value for wave
			uint16_t waveTop = G_greenPeriod * GREEN_PERIOD_SCALER;
			ICR1 = waveTop;

			// 50% duty cycle for the light
			OCR1A = waveTop / 2;
		}
	}
	// set requested frequency.
		
}
