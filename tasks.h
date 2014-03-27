/*******************************************
*
* Header file for Task Code
*
*******************************************/
#ifndef __TASK_H
#define __TASK_H

#include <inttypes.h> //gives us uintX_t
#include "led.h"

/* number of empty for loops to eat up about 10 ms
 *
 * 33 ticks to execute instruction for the empty for-loop 
 * according to dissassembled code
 * 
 * Calculations below adjusted based on empirical testing
 * 1 sec = 1000 ms / sec 
 * 20,000,000 ticks / 1 sec = 20,000,000 / 1000 ms = 20,000 ticks/ 1ms 
 * 20,000 ticks / ms x 1 loop / 35 ticks =  571.4286 loops / 1 ms = 5714 loops / 10 ms
 */

#define FOR_COUNT_10MS 5714

volatile uint32_t __ii;

#define WAIT_10MS { for ( __ii=0; __ii < FOR_COUNT_10MS; __ii++ ); }


#endif

