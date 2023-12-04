#include <stdio.h> 
#include <avr/io.h> 
#include <avr/interrupt.h> 
#include <stdlib.h>
#include <Arduino.h>
#include "Timer.hpp"
#include "Motor.hpp"

// initialise macros
#define CW 1
#define CCW -1
#define SLOW 5 // 5 increment/decrement (play with this)

// initialise counter variables: 
volatile int counter = 0;
volatile int counterMatch = 100;
int outer_motor_duty = 400;
int laser_motor_duty = 400;
int top_count = 5000;
int state = 0;

// define pointers for PWM registers:
volatile uint16_t* outer_motor_pwm = &OCR1A;
volatile uint16_t* laser_motor_pwm = &OCR1B;

// timer ISR: 
ISR(TIMER1_OVF_vect) {
  counter++;
  TCNT1 = 0;  // reset timer
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
    if (counter == counterMatch) {
      // randomise position of motors 1 and 2:
      *outer_motor_pwm = timer.RandomNumber(200, 800);
      *laser_motor_pwm = timer.RandomNumber(200, 800);
      counterMatch = timer.RandomNumber(20, 50); // randomise time interval 
      counter = 0;
    }
  }
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