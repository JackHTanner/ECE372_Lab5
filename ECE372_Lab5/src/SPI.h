#ifndef SPI_H
#define SPI_H
#include <avr/io.h>
#include <Arduino.h>    //to use byte

void SendData(byte address, byte data);
void initMAX7219();
void displaySmileyFace();
void displayFrownyFace();
void clearDisplay();
void shutDown();
void wakeUp();



#endif
