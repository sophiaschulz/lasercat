#ifndef TIMER_HPP
#define TIMER_HPP

#include <stdio.h> 
#include <avr/io.h> 
#include <avr/interrupt.h> 
#include <stdlib.h>
#include <Arduino.h>

class Timer {
    public:
        Timer(int top_count, int counterMatch); // constructor
        void ResetTimer();
        int RandomNumber(int min, int max);
        void RandomisePosition();
};

#endif