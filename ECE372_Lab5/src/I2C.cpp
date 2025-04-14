#include "I2C.h"
#include <avr/io.h>

#define wait_for_completion while(!(TWCR & (1 << TWINT)));

void initI2C() {
    // Set prescalar TWPS to 1 in TWSR register
    TWSR |= (1 << TWPS0);
    TWSR &= ~(1 << TWPS1);

    /* Set two wire interface bit rate register TWBR
        Using formula:
        TWBR =   (CPU Clock Freq/SCL Clock Freq) - 16
                ---------------------------------------
                            2 * 4^(TWPS)

        Assuming SCL Clock Freq = 10 kHz

         TWBR =   (16 * 10^6/10 * 10^3) - 16
                -------------------------------
                            2 * 4^(1)

         = 198
         = 0xC6
    */

    TWBR = 0xC6;

    // Enable two wire interface
    TWBR = (1 << TWEN);
}

void StartI2C_Trans(unsigned char SLA) {
    // Clear TWINT, initiate start condition, initiate enable
    TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);

    wait_for_completion;

    // Set two wire data register to the SLA, write bit
    TWDR = (SLA << 1) | 0x00;

    // Trigger action: Clear TWINT and initiate enable
    TWCR = (1 << TWINT) | (1 << TWEN);

    wait_for_completion;
}

void StopI2C_Trans() {
    // Trigger action, stop condition
    TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);
}


void Write(unsigned char data) {
    // Set two wire data register equal to incoming data
    TWDR = data;

    // Trigger action
    TWCR = (1 << TWINT) | (1 << TWEN);

    wait_for_completion;
}

void Read_from(unsigned char SLA, unsigned char MEMADDRESS) {
    // Start a transmission to the SLA
    StartI2C_Trans(SLA);

    // Write to the MEMADDRESS
    Write(MEMADDRESS);

    // Clear TWINT, initiate start condition, initiate enable
    TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);

    wait_for_completion;

    // Set two wire data register to the SLA, read bit
    TWDR = (SLA << 1) | 0x01;

    // Trigger action, master acknowledge bit
    TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWEA);
    
    wait_for_completion;

    // Trigger action
    TWCR = (1 << TWINT) | (1 << TWEN);

    wait_for_completion;

    // Trigger action, stop condition
    TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);
}

unsigned char Read_data() {
    // Return TWDR
    return TWDR;
}
