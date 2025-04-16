#include "SPI.h"

void initSPI() {
    //Set DIN pin (MOSI) as output on pin PA0
    DDRA |= (1 << PA0);
    //Set CS pin as output on pin PA1
    DDRA |= (1 << PA1);
    //Set CLK pin (SCK) as output on pin PA2
    DDRA |= (1 << PA2);
}



