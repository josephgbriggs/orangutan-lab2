Experiments and Report
----------------------
Run a series of experiments as described below and answer the following questions. For each experiment:

* Zero all toggle counters (>za).
* Toggle the LEDs for approximately 1 minute.
* Record the number of toggles for all LEDs (>pa).

Experiments:

1. Use your original version of toggling the red LED that uses for-loops. Toggle all 3 at 1Hz. (Do not type in any menu options while you are toggling until the 1 minute is up). How good was your WCET analysis of the for loop? If it is very far off, adjust it. Why did I not want you to use the menu while running the experiment?

	> My WCET was a little bit off. The [for-loop listing](bare-for-loop_annotated-2.lss "Listing") 
	> which I annotated with the number of system clock cycles per instruction showed 33 ticks. That
	> results in 6061 loops (approx.) to delay 10 ms. My timing showed that 5714 loops (implying 
	> 35 ticks per loop) was a better estimate.
	>
	> 20,000 ticks / ms x 1 loop / 35 ticks =  571.4286 loops / 1 ms = 5714 loops / 10 ms
	> 
	> Using the menu would have thrown off the timing as this requires interrupts to fire and 
	> would delay other work being done in the main software thread.
	>
	> *Note:* I did not have serial communication working and most interrupts were not working
	> when I conducted this experiment, so typing in menu options and having it interfere was
	> not even possible. See the [experiment1 branch](https://github.com/josephgbriggs/orangutan-lab2/blob/experiment1/lab2.c) of the code for the state of the system
	> when this experiment was run.

2. Use your software timer to toggle the red LED. Toggle all 3 at 1Hz. Simply observe the final toggle count. All should be about 60 (maybe the red is off by 1). If this is not the case, you probably set something up wrong, and you should fix it.

	> I was a hair early when submitting the 'pa' command, so I got counts of R:60, Y:59 and G:59.
	> The video below shows this experiment:
	>
	> [![Experiment 2 Video](http://img.youtube.com/vi/xwaJ2xWF5oY/0.jpg)](http://www.youtube.com/watch?v=xwaJ2xWF5oY)
	>

3. Set all LEDs to toggle at 2Hz (500ms). Place a 90ms busy-wait for-loop into the ISR for the green LED. Toggle for 1 minute and record results. Now place a 90ms busy-wait for-loop into the ISR for the yellow LED. Toggle for 1 minute and record results. What did you observe? Did the busy-wait disrupt any of the LEDs? Explain your results.

	> Toggling all LEDs to toggle at 2Hz (500ms) with no busy-wait for-loops, the toggle counts 
	> were R:119, Y:119 and G:119.
	>
	> With a 90ms busy-wait loop in the green ISR I recorded R:98, Y:120, G:119.
	>
	> With a 90ms busy-wait loop in the yellow ISR I recorded R:13, Y:119, G:119.
	>
	> The green ISR task has a period of 500ms and the toggling of the LED is handled by hardware,
	> So it will not be affected by the busy-wait delay. There seems to be some impact on the red
	> LED since it is running in the software cyclic executive and has the lowest priority.
	> 
	> The yellow ISR task has a period of 100 ms. Inserting a 90ms delay still seems to not affect
	> the yellow LED -- probably because the other instructions do not take more than 10ms. However,
	> there is a large impact on the red LED. The green ISR has a period of 500ms, so an additional
	> 90ms of utilization is taken up every 500ms. But the yellow ISR has a period of 100ms, so 
	> there is 450ms of utilization consumed every 500ms. This has a much larger impact on red.

4. Repeat #3, except use a 110ms busy-wait. You probably won’t be able to use the menu functions. If not, report that, and discuss what you observed from the blinking. Explain your results.

	> With a 110ms busy-wait loop in the green ISR I recorded R:94, Y:117, G:119.
	>
	> With a 110ms busy-wait loop in the yellow ISR I could not record the counts because the 
	> menu was unresponsive, but the green LED was blinking faster than the yellow LED and the
	> red LED blinked once.
	>
	> Since the yellow period is only 100ms, a 110ms delay starves the red LED task. I assume
	> that the yellow LED blnked slower than 2 Hz and the green LED was unaffected and continued
	> to blink at 2 Hz because it was using a hardware PWM to blink the LED.

5. Repeat #3, except use a 510ms busy-wait. Explain your results.

	> With a 510ms busy-wait loop in the green ISR the menu was unresponsive. The green LED
	> appeared to toggle at 2 Hz. The yellow and red LEDs turned on, but never toggled off.
	>
	> With a 510ms busy-wait loop in the yellow ISR the menu was unreponsive. The green LED
	> appeared to toggle at 2 Hz. The yellow LED appeared to toggle at a rate slightly 
	> slower than 1 Hz. The red LED never lit.
	>
	> These results are expected because the toggling of the green LED is generated in
	> the timer hardware and is unaffected by the delay in the yellow ISR. The yellow ISR
	> is affected because the shortest period it can have is 1020ms (two ISR firings, one
	> to turn the LED on and one to turn it off). The red LED was completely starved and did
	> not light within the first minute.

6. Repeat #5 (i.e. 2Hz toggle with 510ms busy-wait), except place an sei() at the top of the ISR with the for-loop in it. Explain your results.

	> With a 510ms busy-wait loop in the green ISR and 'nested' interrupts, the green LED
	> appeared to blink at 2 Hz, with the yellow LED blinking slightly slower. The red LED
	> lit once and never turned off during the minute timing. The menu was unresponsive.
	>
	> With a 510ms busy-wait loop in the yellow ISR and 'nested' interrupts, the results
	> appeared to be exactly as above.
	>
	> Allowing interrupts within the green ISR allowed the yellow ISR to fire with its
	> shorter period, unlike experiment 5. The red LED continued to be starved since it 
	> could not preempt an ISR since it was running in the cyclic executive.
	>
	> Allowing interrupts in the yellow ISR did not materially affect the green LED that 
	> was relying on hardware. It did allow the yellow ISR to interrupt itself and 
	> appeared to almost acheive a 2 Hz rate unlike experiment 5.