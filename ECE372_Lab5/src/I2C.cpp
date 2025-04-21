#include "I2C.h"
#include <avr/io.h>

#define wait_for_completion while(!(TWCR & (1 << TWINT)));
#define I2C_WRITE 0
#define I2C_READ 1
#define unsigned char WHO_AM_I 0x68

void InitI2C () {
    StartI2C_Trans(WHO_AM_I); // Wake up I2C module on mega 2560 I2C address is 0b1101000
    StartI2C_Trans(0x6B); 
    Write(0x00); // Power Managment register is 6B and I must set every bit in that register to 0
    StopI2C_Trans();
    
    //- Accelerometre Configuration regiser is 1C set all the bits to 0
    //- Convert accelerometre value from LSB/g to g by dividing the value of the output reg by the LSB Sensitivity
    //?- Read from the Accelerometre register 
    //3B XOUT_high, 3C XOUT_low, 3D YOUT_high, 3E YXOUT_low, 3F ZOUT_high, 40 ZXOUT_low,
    TWSR = ((1<<TWPS1) | (1<<TWPS0)); //- Set prescaler TWPS to Power Managment register is 6B 
    TWBR = 18; // 100k Hz- Set two wire interface bit rate register TWBR
    TWCR = (1 << TWEN); //- Enable two wire interface
}

void StartI2C_Trans(unsigned char SLA) {
    // set the start condition
    TWCR = ((1 << TWEN) | (1 << TWINT) | (1 << TWSTA));
    wait_for_completion;
    // send the address
    TWDR =  SLA + I2C_WRITE; // SLA+W, address + write bit
    TWCR = ((1 << TWEN | (1 << TWINT))); // trigger I2C action
    wait_for_completion;
}


void StopI2C_Trans() {

    TWCR = ((1 << TWEN) | (1 << TWINT) | (1 << TWSTO)); // trigger I2C action
    wait_for_completion;

}

void Write(unsigned char data) {

    TWDR = data; // register value in the data register
    TWCR = ((1 << TWEN | (1 << TWINT))); // trigger I2C action
    wait_for_completion;

}

void Read_from(unsigned char SLA, unsigned char MEMADDRESS) {

    StartI2C_Trans(SLA);
    Write(MEMADDRESS);
    // switch master to read (receiver) mode and slave to transmitter
    TWCR = ((1 << TWEN) | (1 << TWINT) | (1 << TWSTA)); // set another start
    wait_for_completion;
    TWDR = (SLA << 1) + I2C_READ; // SLA+R, switch to reading
    //TWCR = ((1 << TWEN | (1 << TWINT))); // trigger I2C action
    //wait_for_completion;
    // perform first read to get the MSB

    TWCR = ((1 << TWINT) | (1 << TWEN) | (1 << TWEA)); // with ACK set
    wait_for_completion;
    
    // the received byte is now in the TWDR data register
  //  Read_data(); 
    // second read to get LSB
    TWCR = ((1 << TWINT) | (1 << TWEN)); // no acknowledge bit set, NOT ACK
    wait_for_completion;
    // the second byte is now in TWDR
  ;

}

unsigned char Read_data() {
    return TWDR;
}

/*
uint16_t accVal () {


// specify the register
TWDR = 0x03; // register value in the data register
TWCR = ((1 << TWEN | (1 << TWINT))); // trigger I2C action
wait_for_completion;
// switch master to read (receiver) mode and slave to transmitter
TWCR = ((1 << TWEN) | (1 << TWINT) | (1 << TWSTA)); // set another start
wait_for_completion;
TWDR = 0x6C + I2C_READ; // SLA+R, switch to reading
TWCR = ((1 << TWEN | (1 << TWINT))); // trigger I2C action
wait_for_completion;
 // perform first read to get the MSB
TWCR = ((1 << TWINT) | (1 << TWEN) | (1 << TWEA)); // with ACK set
wait_for_completion;
// the received byte is now in the TWDR data register
register_value = (TWDR << 8); // put value in top half of variable
// second read to get LSB
TWCR = ((1 << TWINT) | (1 << TWEN)); // no acknowledge bit set, NOT ACK
wait_for_completion;
// the second byte is now in TWDR
register_value += TWDR;

return register_value;
}

*/
