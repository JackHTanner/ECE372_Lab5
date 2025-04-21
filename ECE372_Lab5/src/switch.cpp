#include "switch.h"
#include <avr/io.h>

/*
 * Initializes pull-up resistor on PD2 and sets it into input mode
 */
void initSwitchPD2(){
    //Set up PD2 as input
    DDRD &= ~(1 << DDD2);

    // Enable pull-up resisitor on PD2 (digital pin 19)
    PORTD |= (1 << PORTD2);

    EICRA |= (1 << ISC20);
    EICRA &= ~(1 << ISC21);
    EIMSK |= (1 << INT2);
}
