#include "switch.h"
#include <avr/io.h>

/*
 * Initializes pull-up resistor on PB3 and sets it into input mode
 */
void initSwitchPB3(){
    //Set up PB3 as input
    DDRB &= ~(1 << DDB3);

    // Enable pull-up resisitor on PB3 (pin 50)
    PORTB |= (1 << PORTB3);

    //Enable interrupt 0 on pin-change interrupt control register
    PCICR |= (1 << PCIE0);

    //Set up pin-change interrupt on PB3 in pin-change mask register 0
    PCMSK0 |= (1 << PCINT3);
}
