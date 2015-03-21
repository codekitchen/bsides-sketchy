/*
    Filename: badge.h
    Purpose: Library for BSidesSLC 2015
    Created: March 16, 2013
    Original Author: d3c4f
*/

#ifndef BADGE_H
#define BADGE_H

#define BIT0_SET   0b00000001
#define BIT1_SET   0b00000010
#define BIT2_SET   0b00000100
#define BIT3_SET   0b00001000
#define BIT4_SET   0b00010000
#define BIT5_SET   0b00100000
#define BIT6_SET   0b01000000
#define BIT7_SET   0b10000000
#define BIT0_MASK  0b11111110
#define BIT1_MASK  0b11111101
#define BIT2_MASK  0b11111011
#define BIT3_MASK  0b11110111
#define BIT4_MASK  0b11101111
#define BIT5_MASK  0b11011111
#define BIT6_MASK  0b10111111
#define BIT7_MASK  0b01111111

#include "Arduino.h"

class Badge 
{
    public:
        Badge(); // Default constructor
        
        bool   Button0(long delayTime = 0, bool repeat = false);
        bool   Button1(long delayTime = 0, bool repeat = false);
        bool   Buttons(long delayTime = 0, bool repeat = false);
        bool     JoyUp(long delayTime = 0, bool repeat = false);
        bool   JoyDown(long delayTime = 0, bool repeat = false);
        bool   JoyLeft(long delayTime = 0, bool repeat = false);
        bool  JoyRight(long delayTime = 0, bool repeat = false);
        bool JoyCenter(long delayTime = 0, bool repeat = false);
        
        void SetLED01(bool onoff);
        void SetLED02(bool onoff);
        
    private:
        bool ButtonDebounce(long delayTime, long & lastTime, bool isStateLow, bool repeat);
        long lastButton0;
        long lastButton1;
        long lastButtons;
        long lastJoyUp;
        long lastJoyDown;
        long lastJoyLeft;
        long lastJoyRight;
        long lastJoyCenter;
};

#endif