#include "I2C.h"
#include <avr/io.h>

#define wait_for_completion while(!(TWCR & (1 << TWINT)));

void initI2C() {

}

void StartI2C_Trans(unsigned char SLA) {

}

void StopI2C_Trans() {

}


void Write(unsigned char data) {

}

void Read_from(unsigned char SLA, unsigned char MEMADDRESS) {
    //Start a transmission to the SLA
    StartI2C_Trans(SLA);

    //Write to the MEMADDRESS
    Write(MEMADDRESS);

    //Clear TWINT, initiate start condition, initiate enable
    TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);

    wait_for_completion;

    //Set two wire data register to the SLA, read bit
    TWDR = (SLA << 1) | 0x01;

    //Trigger action, master acknowledge bit
    TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWEA);
    wait_for_completion;

    //Trigger action
    TWCR = (1 << TWINT) | (1 << TWEN);

    wait_for_completion;

    //Trigger action, stop condition
    TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);

}

unsigned char Read_data() {

}
