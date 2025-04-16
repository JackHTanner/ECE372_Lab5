#include "timer.h"
#include <avr/io.h>

/* Initialize timer 1. Using CTC mode  .*/
void initTimer1(){
    TCCR1B |= (1 << WGM12);
    TCCR1B |= (1 << CS10);
    OCR1A = 15;
}


/* This delays the program an amount of microsecond specified by unsigned int delay.
* Used for timer 1. 
*/
void delayMs(unsigned int delay){
    unsigned int delaycount = 0;

    TCNT1 = 0; // Start the timer at 0
    TIFR1 |= (1 << OCF1A); // Set the compare flag to start the timer 

    while (delaycount < delay) {
        // If timer has reached the max value of OCR1A,
        // increment delaycount. 
        if (TIFR1 & (1 << OCF1A)) {
            delaycount++;
            TIFR1 |= (1 << OCF1A); // Restart the timer
        }
    }
}
