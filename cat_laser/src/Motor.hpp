#ifndef MOTOR_HPP
#define MOTOR_HPP
#include <stdio.h> 
#include <avr/io.h> 
#include <avr/interrupt.h> 
#include <stdlib.h>
#include <Arduino.h>

class Motor {
    private:
        int pin_register;
        int angle;
    public:
        Motor(int pin_register, volatile uint16_t* timer_register, int motor_duty); // constructor
};

#endif