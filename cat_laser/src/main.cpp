#include <stdio.h> 
#include <avr/io.h> 
#include <avr/interrupt.h> 
#include <stdlib.h>
#include <Arduino.h>
#include "Timer.hpp"
#include "Motor.hpp"

// initialise macros:
#define SLOW 5 // 5 increment/decrement (play with this)
#define START 0
#define CALIBRATE_Y 1
#define CALIBRATE_X 2
#define FREERUN 3

// initialise counter variables: 
volatile int counter = 0;
volatile int counterMatch = 100;
int outer_motor_duty = 400;
int laser_motor_duty = 400;
int top_count = 5000;

// initialise state variables:
int currentState = START;
int nextState = START;
bool isCalPressed = false;
bool isMovePressed = false;

// initialise limit variables:
int lowLim_x = 200;
int lowLim_y = 200;
int uppLim_x = 800;
int uppLim_y = 800;

// define pointers for PWM registers:
volatile uint16_t* outer_motor_pwm = &OCR1A;
volatile uint16_t* laser_motor_pwm = &OCR1B;

// TODO: define pins for input buttons?

// timer ISR: 
ISR(TIMER1_OVF_vect) {
  counter++;
  TCNT1 = 0;  // reset timer
}

// calibrate button ISR:
ISR(INT0_vect) { // which pin is this again??
  isCalPressed = true;
}

// move button ISR:
ISR(INT1_vect) { // which pin is this again??
  isMovePressed = true;
}

int main(void) {

  cli(); // disable interrupts

  // Timer 1 (16-bit):
  class Timer timer(top_count, counterMatch);

  // Servo 1 - outer motor: OC1A (PB1) and Servo 2 - laser motor: OC1B (PB2) controlled by PWM output from Timer 1
  class Motor outer_motor(DDB1, outer_motor_pwm, outer_motor_duty);
  class Motor laser_motor(DDB2, laser_motor_pwm, laser_motor_duty);

  sei(); // enable interrupts

  while(1) { // superloop
    switch (currentState) {
      case START:
        // TODO: turn on laser (need input pin)

        // reset calibrated limits:
        lowLim_x = 200;
        lowLim_y = 200;
        uppLim_x = 800;
        uppLim_y = 800;

        // state transitions:
        if (isMovePressed) {
          currentState = FREERUN;
          isMovePressed = false;
        }
        if (isCalPressed) {
          currentState = CALIBRATE_Y;
          isCalPressed = false;
        }
        break;

      case CALIBRATE_Y:
        int span = 0; 
        while (!isCalPressed) { // move motor until button is pressed to set UPPER LIMIT
          // rotate CW:
          span = abs(UPPER_LIMIT - outer_motor.position);
          for (int i = 0; i < span; i+=SLOW) {
            *outer_motor_pwm = outer_motor.position;
            outer_motor.position += SLOW;
            delay(SLOW);
          } // now at UPPER_LIMIT
          // rotate CCW:
          span = abs(outer_motor.position - LOWER_LIMIT);
          for (int i = 0; i < span; i+=SLOW) {
            *outer_motor_pwm = outer_motor.position;
            outer_motor.position -= SLOW;
            delay(SLOW);
          } // now at LOWER_LIMIT
        }
        lowLim_y = outer_motor.position;
        isCalPressed = false;
        while (!isCalPressed) { // move motor until button is pressed again
          // rotate CCW:
          span = abs(outer_motor.position - lowLim_y);
          for (int i = 0; i < span; i+=SLOW) {
            *outer_motor_pwm = outer_motor.position;
            outer_motor.position -= SLOW;
            delay(SLOW);
          } // now at lowLim_y
          // rotate CW:
          span = abs(UPPER_LIMIT - outer_motor.position);
          for (int i = 0; i < span; i+=SLOW) {
            *outer_motor_pwm = outer_motor.position;
            outer_motor.position += SLOW;
            delay(SLOW);
          } // now at UPPER_LIMIT
        }
        uppLim_y = outer_motor.position;
        isCalPressed = false;
        break;

      case CALIBRATE_X:
        int span = 0; 
        while (!isCalPressed) { // move motor until button is pressed
          // rotate CCW:
          span = abs(laser_motor.position - LOWER_LIMIT);
          for (int i = 0; i < span; i+=SLOW) {
            *laser_motor_pwm = laser_motor.position;
            laser_motor.position -= SLOW;
            delay(SLOW);
          } // now at LOWER_LIMIT
          // rotate CW:
          span = abs(UPPER_LIMIT - laser_motor.position);
          for (int i = 0; i < span; i+=SLOW) {
            *laser_motor_pwm = laser_motor.position;
            laser_motor.position += SLOW;
            delay(SLOW);
          } // now at UPPER_LIMIT
        }
        lowLim_x = laser_motor.position;
        isCalPressed = false;
        while (!isCalPressed) { // move motor until button is pressed again
          // rotate CW:
          span = abs(UPPER_LIMIT - laser_motor.position);
          for (int i = 0; i < span; i+=SLOW) {
            *laser_motor_pwm = laser_motor.position;
            laser_motor.position += SLOW;
            delay(SLOW);
          } // now at UPPER_LIMIT
          // rotate CCW:
          span = abs(laser_motor.position - lowLim_x);
          for (int i = 0; i < span; i+=SLOW) {
            *laser_motor_pwm = laser_motor.position;
            laser_motor.position -= SLOW;
            delay(SLOW);
          } // now at lowLim_x
        }
        uppLim_x = laser_motor.position;
        isCalPressed = false;
        currentState = FREERUN;
        break;

      case FREERUN:
        // state transition:
        if (isMovePressed) {
          currentState = START;
          isMovePressed = false;
        }
        
        // TODO: randomise position of outer and laser motors

        // old randomise code:
        if (counter == counterMatch) {
          // randomise position of motors 1 and 2:
          *outer_motor_pwm = timer.RandomNumber(200, 800);
          *laser_motor_pwm = timer.RandomNumber(200, 800);
          counterMatch = timer.RandomNumber(20, 50); // randomise time interval 
          counter = 0;
        }
        break;
    }
  }

    // old randomise code:
    // if (counter == counterMatch) {
    //   // randomise position of motors 1 and 2:
    //   *outer_motor_pwm = timer.RandomNumber(200, 800);
    //   *laser_motor_pwm = timer.RandomNumber(200, 800);
    //   counterMatch = timer.RandomNumber(20, 50); // randomise time interval 
    //   counter = 0;
    // }
}

//WORKING CODE:

/* #include <Arduino.h>
#include <stdio.h> 
#include <avr/io.h> 
#include <avr/interrupt.h> 
#include <stdlib.h>

// initialise counter variables: 
volatile int counter = 0;
volatile int counterMatch = 100;

// random number generator function: 
int randomNumber(int min, int max) {
  int randomNum = 0;
  randomSeed(millis());
  randomNum = random(max - min + 1) + min;
  return randomNum;
}

// timer ISR: 
ISR(TIMER1_OVF_vect) {
  counter++;
  if (counter == counterMatch) {
    OCR1A = randomNumber(200, 800); // randomise position of Servo 1
    OCR1B = randomNumber(200, 800); // randomise position of Servo 2
    counterMatch = randomNumber(20, 50); // randomise time interval 
    counter = 0;
  } 
  TCNT1 = 0;  // reset timer 
}

int main(void) {

  cli(); // disable interrupts
  
  // Timer 1 (16-bit):
  TCCR1A |= (1<<WGM11); // fast PWM mode with ICR1 to set TOP
  TCCR1B |= (1<<WGM12) | (1<<WGM13); // fast PWM mode with ICR1 to set TOP
  TCCR1A |= (1<<COM1A1) | (1<<COM1B1); // clear OC1A and OC1B on compare match
  TCCR1B |= (1<<CS11) | (1<<CS10); // setting prescalar of 64; see page 110
  OCR1A = 400; // starting duty cycle (~1.5ms) for servo 1
  OCR1B = 400; // starting duty cycle (~1.5ms) for servo 2
  ICR1 = 5000; // TOP value to set 50Hz
  TIMSK1 |= (1 << TOIE1); // Enable Timer1 overflow interrupt

  // Servo 1: OC1A (PB1) and Servo 2: OC1B (PB2) controlled by PWM output from Timer 1
  DDRB |= (1<<DDB1); // set PB1 as output  
  DDRB |= (1<<DDB2); // set PB2 as output

  sei(); // enable interrupts

  while(1) { // superloop
    // no operation
  }
} */