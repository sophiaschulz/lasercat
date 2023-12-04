#include <stdio.h> 
#include <avr/io.h> 
#include <avr/interrupt.h> 
#include <stdlib.h>
#include <Arduino.h>
#include "Motor.hpp"

// constructor:
Motor::Motor(int pin_register, volatile uint16_t* timer_register, int motor_duty) {
    this->pin_register = pin_register;
    this->position = motor_duty;
    this->timer_register = timer_register;

    DDRB |= (1<<pin_register); // set pin as output
    *timer_register = motor_duty; // starting duty cycle for motor
}

void Motor::CalibrateMove(int direction, int speed) { // waits for button interrupt
    while ((this->position != LOWER_LIMIT) || (this->position != UPPER_LIMIT)) {
        *timer_register = this->position; // move motor to "position" by setting duty cycle
        this->position += direction*speed; // increment or decrement position (duty cycle) depending on direction
    } // need to switch directions somehow

    // alternatively, with a for loop:
    // int span = 0;
    // while (button not pressed):
    // if direction = 1:
        // span = UPPER_LIMIT - this->position
        // for (int i = 0, i < span, i+=speed) {
            // *timer_register = this->position;
            // this->position += speed;
        // } // now at UPPER_LIMIT
        // span = abs(this->position - LOWER_LIMIT)
        // for (int i = 0, i < span, i+=speed) {
            // *timer_register = this->position;
            // this->position -= speed;
        // } // now at LOWER_LIMIT
    // else if direction = -1:
        // span = this->position - LOWER_LIMIT
        // for (int i = 0, i < span, i+=speed) {
            // *timer_register = this->position;
            // this->position -= speed;
        // } // now at LOWER_LIMIT
        // span = abs(this->position - UPPER_LIMIT)
        // for (int i = 0, i < span, i+=speed) {
            // *timer_register = this->position;
            // this->position += speed;
        // } // now at UPPER_LIMIT
}