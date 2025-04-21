#include <Arduino.h>
#include "timer.h"
#include "switch.h"
#include "SPI.h"
#include "I2C.h"
#include "PWM.h"
#include <avr/io.h>
#include <avr/interrupt.h>

typedef enum
{
  SMILEY,
  FROWN,
  DEBOUNCE_PRESS,
  WAIT
} StateType;

typedef enum stateName
{
  WAIT_PRESS,
  DEBOUNCE_PRESS,
  WAIT_RELEASE,
  DEBOUNCE_RELEASE
} State;

volatile State buttonState = WAIT_PRESS;
volatile StateType state = SMILEY;
volatile bool smiley = true;

int main () {
  Serial.begin(9600);
  Serial.flush();
  Serial.println("Starting...");
    // Initialize the timer and PWM timer
    initTimer1();
    initPWMTimer3();
    // Initialize the MAX7219
    initMAX7219();

    initSwitchPD2();

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

      default:
        Serial.println("In defauly LED state.");
        break;
    }
    
	}

  return 0;
}

volatile bool buttonPressed = false;


ISR(PCINT0_vect){
  Serial.println("Interrupt triggered");
  delayMs(10); // Small debounce delay
    
    if (!(PINB & (1 << PD2))) { // Logic on PD2 is LOW (button pressed)
        // Flag that the button was pressed!!
        buttonPressed = true;
    } 
    
    else { // Logic on PD2 is high (button released)
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
