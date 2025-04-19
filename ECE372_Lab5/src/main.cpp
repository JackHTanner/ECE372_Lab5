#include <Arduino.h>
#include "timer.h"
#include "switch.h"
#include "SPI.h"
#include "I2C.h"
#include <avr/io.h>
#include <avr/interrupt.h>

typedef enum {SMILEY, FROWN, DEBOUNCE_PRESS, WAIT} StateType;

volatile StateType state = SMILEY;
volatile bool smiley = true;

int main () {
  Serial.begin(9600);
  Serial.flush();
  Serial.println("Starting...");
    // Initialize the timer
    initTimer1();
    // Initialize the MAX7219
    initMAX7219();

    initSwitchPB3();

    sei(); // Enable global interrupts
    
    //start with smiley face
    displaySmileyFace();
    
  while (1) {
    
    switch (state) {
      case SMILEY:
        displaySmileyFace();
        Serial.println("Smiley face displayed");
        break;

      case FROWN:
        displayFrownyFace();
        Serial.println("Frowny face displayed");
        break;

      case DEBOUNCE_PRESS:
        Serial.println("In Debounce Press");
        delayMs(10);  //delay for debouncing
          state = WAIT;
        break;
/*
      case WAIT:
        Serial.println("In Wait");
        break;
        */
    }
    
	}

  return 0;
}

volatile bool buttonPressed = false;


ISR(PCINT0_vect){
  Serial.println("Interrupt triggered");
  delayMs(10); // Small debounce delay
    
    if (!(PINB & (1 << PB3))) { // Logic on PB3 is LOW (button pressed)
        // Flag that the button was pressed!!
        buttonPressed = true;
    } 
    
    else { // Logic on PB3 is high (button released)
        // Only toggle face on release, if the button was previously pressed
        if (buttonPressed) {
            state = (state == SMILEY) ? FROWN : SMILEY; // Toggle between SMILEY and FROWN
            buttonPressed = false; // Reset the flag
            }
            
        if (state == SMILEY) {   //Logic for changing display
          clearDisplay();
          displaySmileyFace();
        }

        else {
          clearDisplay();
          displayFrownyFace();
        }
      }
}
