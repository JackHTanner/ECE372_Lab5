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
volatile uint8_t toggleState = 0;
volatile uint16_t tickCounter = 0;
volatile uint8_t overflowCount = 0;
volatile bool useHighFreq = true;
bool chirp = false;
int main () {

  Serial.begin(9600);
  Serial.flush();
  //Serial.println("Starting...");
    // Initialize the timer and PWM timer
    initTimer1();
    initTimer2();
    initPWMTimer3();
    turnOffDutyCycle();
    // Initialize the MAX7219
    initMAX7219();
    InitI2C();
  
    int16_t ax, ay, az;

    initSwitchPB3();

    sei(); // Enable global interrupts
    
    //start with smiley face

    while (1) {
    displaySmileyFace();

  while (1) {
    ReadAccelData(&ax, &ay, &az);

    // Now we calculate tilt
    float ax_g = ax / 16384.0;
    float ay_g = ay / 16384.0;
    float az_g = az / 16384.0;

    float pitch = atan2(ax_g, sqrt(ay_g * ay_g + az_g * az_g)) * (180.0 / 3.14159);
    float roll  = atan2(ay_g, sqrt(ax_g * ax_g + az_g * az_g)) * (180.0 / 3.14159);

    int isTilted = (fabs(pitch) >= 45.0) || (fabs(roll) >= 45.0);

    Serial.print(ax);
    Serial.print(" ");
    Serial.print(ay);
    Serial.print(" ");
    Serial.print(az);
    Serial.print(" ");

    if (isTilted){
      Serial.println("IT's TILTING!!!!");
      state = FROWN; 
    }

    else{
      Serial.println("All is stable");
      state = SMILEY;
    }

    switch (state) {
      case SMILEY:
        displaySmileyFace();
        break;

      case FROWN:
        displayFrownyFace();
        chirp = true;
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
      turnOnDutyCycle();
    }
    else{
      turnOffDutyCycle();
    }



	}




  return 0;
}
}

ISR(PCINT0_vect){
  Serial.println("Interrupt triggered");
  chirp = false;
  state = SMILEY;
  if(buttonState == WAIT_PRESS){
    buttonState = DEBOUNCE_PRESS;
  }
  else if(buttonState == WAIT_RELEASE){
    buttonState = DEBOUNCE_RELEASE;
  }
}

ISR(TIMER2_OVF_vect) {

overflowCount;

for ()




  overflowCount++;
  if (overflowCount >= 25) { // ~100ms if each overflow is ~4ms
      overflowCount = 0;
    if (useHighFreq) {
      for (int i = 99; i < 199; i++){
      ICR3 = i;
      OCR3B = ICR3 / 2; // 50% duty
      }
    }
      /*
      useHighFreq = !useHighFreq;

      if (useHighFreq) {
          ICR3 = 99;
      } else {
          ICR3 = 199;
      }

      */
      
  }
}
