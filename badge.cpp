/*
    Filename: badge.cpp
    Purpose: Library for BSidesSLC 2015
    Created: March 16, 2013
    Original Author: d3c4f
*/

#include "Arduino.h"
#include "badge.h"

// I/O PERIPHERALS
/*
PD4 : Left LED under the LCD
PF1 : Right LED under the LCD

PD6 : Button A
PD5 : Button B
PB0 : Joy Up
PC7 : Joy Down
PC6 : Joy Left
PE6 : Joy Right
PD7 : Joy Center
*/

// Initialize Hardware (default constructor)
Badge::Badge()
{
    // Set up GP Outputs
    DDRD |= B00010000; // Left LED under the LCD
    DDRF |= B00000010; // Right LED under the LCD
    
    // Set all GP inputs with Internal Pull-Up resistors enabled
    DDRB &= B11111110;
    DDRC &= B00111111;
    DDRD &= B00011111;
    DDRE &= B10111111;
    PORTB |= B00000001;
    PORTC |= B11000000;
    PORTD |= B11100000;
    PORTE |= B01000000;
    
    lastButton0 = 0;
    lastButton1 = 0;
    lastButtons = 0;
    lastJoyUp = 0;
    lastJoyDown = 0;
    lastJoyLeft = 0;
    lastJoyRight = 0;
    lastJoyCenter = 0;
}


// Return the state of Button 0 (Top button)
bool Badge::Button0(long delayTime, bool repeat)
{
    return ButtonDebounce(delayTime, lastButton0, (PIND & BIT6_SET), repeat);
}


// Return the state of Button 1 (Bottom button)
bool Badge::Button1(long delayTime, bool repeat)
{
    return ButtonDebounce(delayTime, lastButton1, (PIND & BIT5_SET), repeat);
}


// Return the state of Both Buttons (Top and Bottom)
bool Badge::Buttons(long delayTime, bool repeat)
{
    return ButtonDebounce(delayTime, lastButtons, ((PIND & BIT5_SET) || (PIND & BIT6_SET)) , repeat);
}


// Returns the state of Joystick Up
bool Badge::JoyUp(long delayTime, bool repeat)
{
    return ButtonDebounce(delayTime, lastJoyUp, (PINB & BIT0_SET), repeat);
}


// Returns the state of Joystick Down
bool Badge::JoyDown(long delayTime, bool repeat)
{
    return ButtonDebounce(delayTime, lastJoyDown, (PINC & BIT7_SET), repeat);
}


// Returns the state of Joystick Left
bool Badge::JoyLeft(long delayTime, bool repeat)
{
    return ButtonDebounce(delayTime, lastJoyLeft, (PINC & BIT6_SET), repeat);
}


// Returns the state of Joystick Right
bool Badge::JoyRight(long delayTime, bool repeat)
{
    return ButtonDebounce(delayTime, lastJoyRight, (PINE & BIT6_SET), repeat);
}


// Returns the state of Joystick Center Button
bool Badge::JoyCenter(long delayTime, bool repeat)
{
    return ButtonDebounce(delayTime, lastJoyCenter, (PIND & BIT7_SET), repeat);
}


// Helper function for button debouncing
// delayTime is the time before the first "fire" while !isStateLow
// if repeat is true, function will "fire" every delayTime while !isStateLow
// &lastTime is a reference to a long variable that will hold the last time / state
bool Badge::ButtonDebounce(long delayTime, long &lastTime, bool isStateLow, bool repeat)
{
    if(isStateLow)
    {
        // Button is not being pushed
        lastTime = -1;
        return false;
    }
    else 
    {
        // Button is being pushed
        if (delayTime < 1) return true;
        
        if (lastTime > 0)
        {
            if (millis() > lastTime + delayTime)
            {
                if(repeat) lastTime = millis();
                else lastTime = 0;
                return true;
            }
            else
            {
                return false;
            }
        }
        else
        {
            if(!repeat && lastTime == -1) lastTime = millis();
            if(repeat) lastTime = millis();
            return false;
        }
    }
}


// Set LED01 off or on
// PD4 : Left LED under the LCD
void Badge::SetLED01(bool onoff)
{
    if(onoff)
    {
        PORTD |= BIT4_SET;
    }
    else
    {
        PORTD &= BIT4_MASK;
    }
}


// Set LED02 off or on
// PF1 : Right LED under the LCD
void Badge::SetLED02(bool onoff)
{
    if(onoff)
    {
        PORTF |= BIT1_SET;
    }
    else
    {
        PORTF &= BIT1_MASK;
    }   
}

