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

// can probably get rid of this method altogether
void Motor::CalibrateMove(int target) {
    *timer_register = target; // set duty cycle to target position
}

    // while ((this->position != LOWER_LIMIT) || (this->position != UPPER_LIMIT)) {
    //     *timer_register = this->position; // move motor to "position" by setting duty cycle
    //     this->position += direction*speed; // increment or decrement position (duty cycle) depending on direction
    // } // need to switch directions somehow

    // note that idk how fast this will spin with the delays and increments i've set lol

    // alternatively, with a for loop:
    // int span = 0; 
    // bool isPressed = false; // triggered by interrupt or smth; either this or while state isn't changed
    //     // how tf is this gonna inherit the state from main.cpp
    // while (!isPressed) { 
    //     if (direction == 1) {
    //         span = UPPER_LIMIT - this->position;
    //         for (int i = 0; i < span; i+=speed) {
    //             *timer_register = this->position;
    //             this->position += speed;
    //             delay(speed);
    //         } // now at UPPER_LIMIT
    //         span = abs(this->position - LOWER_LIMIT);
    //         for (int i = 0; i < span; i+=speed) {
    //             *timer_register = this->position;
    //             this->position -= speed;
    //             delay(speed);
    //         } // now at LOWER_LIMIT
    //     } else if (direction == -1) {
    //         span = this->position - LOWER_LIMIT;
    //         for (int i = 0; i < span; i+=speed) {
    //             *timer_register = this->position;
    //             this->position -= speed;
    //             delay(speed);
    //         } // now at LOWER_LIMIT
    //         span = abs(this->position - UPPER_LIMIT);
    //         for (int i = 0; i < span; i+=speed) {
    //             *timer_register = this->position;
    //             this->position += speed;
    //             delay(speed);
    //         } // now at UPPER_LIMIT
    //     }
    // }
    // return this->position; // returns position when button was pressed (i hope. actually idk if this works)
    //     // could prob just use a setter function within the ISR instead honestly - actually nvm this wouldn't work