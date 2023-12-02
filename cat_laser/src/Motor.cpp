#include <stdio.h> 
#include <avr/io.h> 
#include <avr/interrupt.h> 
#include <stdlib.h>
#include <Arduino.h>
#include "Motor.hpp"

// constructor:
Motor::Motor(int pin_register, volatile uint16_t* timer_register, int motor_duty) {
    this->pin_register = pin_register;
    this->angle = 0;

    DDRB |= (1<<pin_register); // set pin as output
    *timer_register = motor_duty; // starting duty cycle for motor
}
