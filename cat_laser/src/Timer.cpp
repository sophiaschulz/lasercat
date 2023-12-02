#include <stdio.h> 
#include <avr/io.h> 
#include <avr/interrupt.h> 
#include <stdlib.h>
#include <Arduino.h>
#include "Timer.hpp"

// constructor:
Timer::Timer(int top_count, int counterMatch) {
    TCCR1A |= (1<<WGM11); // fast PWM mode with ICR1 to set TOP
    TCCR1B |= (1<<WGM12) | (1<<WGM13); // fast PWM mode with ICR1 to set TOP
    TCCR1A |= (1<<COM1A1) | (1<<COM1B1); // clear OC1A and OC1B on compare match
    TCCR1B |= (1<<CS11) | (1<<CS10); // setting prescalar of 64; see page 110
    ICR1 = top_count; // TOP value to set 50Hz
    TIMSK1 |= (1 << TOIE1); // Enable Timer1 overflow interrupt
}

void Timer::ResetTimer() {
    TCNT1 = 0; // reset timer
}

// random number generator function: 
int Timer::RandomNumber(int min, int max) {
  int randomNum = 0;
  randomSeed(millis());
  randomNum = random(max - min + 1) + min;
  return randomNum;
}

// randomise position of motor by changing duty cycle:
void Timer::RandomisePosition() {

}