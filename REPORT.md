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
	> Using the menu would have thrown off the timing as other work was being done outside the loop.

2. Use your software timer to toggle the red LED. Toggle all 3 at 1Hz. Simply observe the final toggle count. All should be about 60 (maybe the red is off by 1). If this is not the case, you probably set something up wrong, and you should fix it.

	> I was a hair early when submitting the 'pa' command, so I got counts of R:60, Y:59 and G:59.
	> The video below shows this experiment:
	>
	> [![Experiment 2 Video](http://img.youtube.com/vi/xwaJ2xWF5oY/0.jpg)](http://www.youtube.com/watch?v=xwaJ2xWF5oY)
	>

3. Set all LEDs to toggle at 2Hz (500ms). Place a 90ms busy-wait for-loop into the ISR for the green LED. Toggle for 1 minute and record results. Now place a 90ms busy-wait for-loop into the ISR for the yellow LED. Toggle for 1 minute and record results. What did you observe? Did the busy-wait disrupt any of the LEDs? Explain your results.

4. Repeat #3, except use a 110ms busy-wait. You probably won’t be able to use the menu functions. If not, report that, and discuss what you observed from the blinking. Explain your results.

5. Repeat #3, except use a 510ms busy-wait. Explain your results.

6. Repeat #5 (i.e. 2Hz toggle with 510ms busy-wait), except place an sei() at the top of the ISR with the for-loop in it. Explain your results.