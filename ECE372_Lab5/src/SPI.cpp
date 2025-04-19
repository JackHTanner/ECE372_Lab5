#include "SPI.h"
#include <avr/io.h>
#include "timer.h"
#include <timer.h>
#include <Arduino.h>

//data for the 8x8 LED matrix
//Each hexadecimal number represents a row of the matrix
//Each bit in the hexadecimal number represents a pixel in the row
//1 means the pixel is on, 0 means the pixel is off
//The first element in the array represents the first row of the matrix
//The second element in the array represents the second row of the matrix and so on

//PROVIDED IN MAX7219 DATA SHEET
#define REG_NOOP        0x00
#define REG_DIGIT0      0x01
#define REG_DIGIT1      0x02
#define REG_DIGIT2      0x03
#define REG_DIGIT3      0x04
#define REG_DIGIT4      0x05
#define REG_DIGIT5      0x06
#define REG_DIGIT6      0x07
#define REG_DIGIT7      0x08
#define REG_DECODE      0x09
#define REG_INTENSITY   0x0A
#define REG_SCANLIMIT   0x0B
#define REG_SHUTDOWN    0x0C
#define REG_DISPLAYTEST 0x0F

// Pin definitions
#define CS_PIN   0  // PB0
#define CLK_PIN  1  // PB1
#define DIN_PIN  2  // PB2


// Smiley face pattern (each byte represents a row)
const byte smileyFace[8] = {
  0b00000000, // Row 0
  0b00100100, // Row 1
  0b00100100, // Row 2
  0b00100100, // Row 3
  0b00000000, // Row 4
  0b01000010, // Row 5
  0b00111100, // Row 6
  0b00000000  // Row 7
};

const byte frownyFace[8] = {
  0b00000000, // Row 0
  0b00100100, // Row 1
  0b00100100, // Row 2
  0b00100100, // Row 3
  0b00000000, // Row 4
  0b00111100, // Row 5
  0b01000010, // Row 6
  0b00000000  // Row 7
};


// Function to send data to MAX7219
void sendData(byte address, byte data) {
  // Pull CS low to start transmission
  PORTB &= ~(1 << CS_PIN);
  
  // Send address byte
  for (byte i = 0; i < 8; i++) {
    // Set CLK low
    PORTB &= ~(1 << CLK_PIN);
    
    // Set DIN high or low based on bit value
    if (address & (1 << (7 - i))) {
      PORTB |= (1 << DIN_PIN);
    } else {
      PORTB &= ~(1 << DIN_PIN);
    }
    
    // Pulse CLK high
    PORTB |= (1 << CLK_PIN);
  }
  
  // Send data byte
  for (byte i = 0; i < 8; i++) {
    // Set CLK low
    PORTB &= ~(1 << CLK_PIN);
    
    // Set DIN high or low based on bit value
    if (data & (1 << (7 - i))) {
      PORTB |= (1 << DIN_PIN);
    } else {
      PORTB &= ~(1 << DIN_PIN);
    }
    
    // Pulse CLK high
    PORTB |= (1 << CLK_PIN);
  }
  
  // Pull CS high to end transmission
  PORTB |= (1 << CS_PIN);
}

// Initialize MAX7219
void initMAX7219() {
  // Set pin directions
  DDRB |= (1 << CS_PIN) | (1 << CLK_PIN) | (1 << DIN_PIN);
  
  // Set initial pin states
  PORTB |= (1 << CS_PIN);  // CS high
  PORTB &= ~(1 << CLK_PIN); // CLK low
  
  // Wake up from shutdown mode
  sendData(REG_SHUTDOWN, 0x01);
  
  // Disable test mode
  sendData(REG_DISPLAYTEST, 0x00);
  
  // Set scan limit to display all digits (rows)
  sendData(REG_SCANLIMIT, 0x07);
  
  // Disable BCD decoding
  sendData(REG_DECODE, 0x00);
  
  // Set medium brightness
  sendData(REG_INTENSITY, 0x08);
  
  // Clear the display
  for (byte i = 1; i <= 8; i++) {
    sendData(i, 0x00);
  }
}

// Display the smiley face
void displaySmileyFace() {
  for (byte i = 0; i < 8; i++) {
    sendData(i + 1, smileyFace[i]);
  }
}

void displayFrownyFace() {
    for (byte i = 0; i < 8; i++) {
        sendData(i + 1, frownyFace[i]);
    }
}

void clearDisplay() {
  // Clear the display by sending 0 to all digit registers
  for (byte i = 1; i <= 8; i++) {
    sendData(i, 0x00);
  }
}

void shutDown() {
  // Put the MAX7219 in shutdown mode
  sendData(REG_SHUTDOWN, 0x00);
}

void wakeUp() {
  // Wake up the MAX7219 from shutdown mode
  sendData(REG_SHUTDOWN, 0x01);
}


