// Description: This file implements the initialization of an external
// switch.
//----------------------------------------------------------------------//

#include "switch.h"
#include <avr/io.h>

/*
 * Initializes pull-up resistor on PB3 and sets it into input mode
 */
void initSwitchPB3(){
 DDRB &= ~(1<<DDB3); // enables input on pin 50
 PORTB |= (1<<PORTB3); // enables pull up resistor on pin 50

 //Enable interrupt
 /*
 Group PCINT is enabled using Pin Change Interrupt Control Register 
    Bit 0 => enables PCINT 0-7 (PCIE0)
    Bit 1 => enables PCINT 8-15 (PCIE1)
    Bit 2 => enables PCINT 16-23 (PCIE2)
Pin Change Mask enables the specific PCINT pin
    Bit 0 => enables PCINT 0
    .
    .
    .
    Bit 7 => enables PCINT 7
*/
 PCICR |= (1 << PCIE0);
 PCMSK0 |= (1 << PCINT3);
}
