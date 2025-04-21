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
bool chirp = false;

int main () {
  Serial.begin(9600);
  Serial.flush();
  Serial.println("Starting...");
    // Initialize the timer and PWM timer
    initTimer1();
    initPWMTimer3();
    // Initialize the MAX7219
    initMAX7219();
    InitI2C();
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
        chirp = true;
        Serial.println("Frowny face displayed");
        break;

      default:
        Serial.println("In default LED state.");
        break;
    }

    switch(buttonState){
      case WAIT_PRESS:
        break;
      case DEBOUNCE_PRESS:
        delayMs(1);
        buttonState = WAIT_RELEASE;
        break;
      case WAIT_RELEASE:
        break;
      case DEBOUNCE_RELEASE:
        delayMs(1);
        chirp = false;
         buttonState = WAIT_PRESS;
        break;
      default:
        break;
    }

    if (chirp == true){
      changeDutyCycle(512);
      delayMs(500);
      changeDutyCycle(0);
      delayMs(500);
    }
    else{
      changeDutyCycle(0);
    }
    
	}

  return 0;
}

volatile bool buttonPressed = false;


ISR(INT0_vect){
  Serial.println("Interrupt triggered");
  if(buttonState == WAIT_PRESS){
    Serial.println("Pressed");
    buttonState = DEBOUNCE_PRESS;
  }
  else if(buttonState == WAIT_RELEASE){
    Serial.println("Removed finger");
    buttonState = DEBOUNCE_RELEASE;
  }
}
