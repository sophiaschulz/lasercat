#ifndef MOTOR_HPP
#define MOTOR_HPP
#include <stdio.h> 
#include <avr/io.h> 
#include <avr/interrupt.h> 
#include <stdlib.h>
#include <Arduino.h>

#define LOWER_LIMIT 200
#define UPPER_LIMIT 800

class Motor {
    public:
        int pin_register;
        volatile uint16_t* timer_register;
        int position; // current position in terms of duty cycle
        Motor(int pin_register, volatile uint16_t* timer_register, int motor_duty); // constructor
        void CalibrateMove(int direction, int speed);
};

#endif