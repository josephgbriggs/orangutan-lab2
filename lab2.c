#include <pololu/orangutan.h>

/*
 * Lab 2: for the Orangutan SVP.
 * Joseph G. Briggs
 *
 */

int main() {
  /*  Set up to flash LED at 2 Hz using PWM on Timer1, Channel A (PD5)
   *     MCU CLOCK    | PRESCALER |   1/TOP   |     Hz
   * =========================================================
   * 20,000,000 ticks    1 count      1 int      2 ints
   * ---------------- *  -------  * --------- = ------- = 2 Hz
   *       1 sec         N ticks     M counts     1 sec
   *
   * N = 256  	--> clk(T1) = clk(i/o)/256
   * M = 39,062	--> TOP value for pulse wave generation
   *
   */
  // set the clock tick prescaler bits to use clkio/256
  TCCR1B |= 1 << CS12;

  // Set control register bits for 'mode 14' 
  // - Fast PWM with ICR1 to define the TOP
  TCCR1B |= ((1 << WGM13) | (1 << WGM12));
  TCCR1A |= (1 << WGM11);

  // set ICR for TOP value for wave
  ICR1 = 39062;

  // create the wave, set the comparison to 50% of TOP for a 50% duty cycle
  OCR1A = 39062 / 2;

  // Set to Clear OC1A on Compare Match, set OC1A at BOTTOM (non-inverting mode)
  TCCR1A |= 1 << COM1A1;

  // set the data direction register for PortD Pin5 where we are putting our
  // green LED
  DDRD |= 1 << DDD5;

  // set the I-flag in the Status Register (interrupts globally enabled),
  sei();

  while (1) {
    // no work to do, it is all handled by the timer/clock
  }
}
