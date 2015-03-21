/*
    Filename: lcd5110.h
    Purpose: Library for Nokia5110 LCD on the 2015 BSidesSLC Badge
    Created: March 16, 2013
    Author: d3c4f

    Original Credit: Nathan Seidle, Spark Fun Electronics 2011
    Original Credit URL: https://github.com/sparkfun/GraphicLCD_Nokia_5110
    (Thanks Nathan!)
*/

#ifndef LCD5110_H
#define LCD5110_H

#define LCD_COMMAND 0 
#define LCD_DATA    1
#define LCD_X       84
#define LCD_Y       48

#define DISPLAY_BYTES 504

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

class LCD5110  
{
    public:
        // Constructor
        LCD5110();

        void DisplayBitmap(const uint8_t bitmap_array[], bool redraw = true); // Write a byte-array coded bitmap to the display. Must be full 504 bytes for now.
        void DisplayBitmap_PROGMEM(const uint8_t bitmap_array[]); // Write a byte-array coded bitmap from PROGRAM MEMORY to the display. Must be full 504 bytes for now.
        void SetPixel(uint16_t x, uint16_t y); // Set a pixel of the (84 x 48) display buffer
        void ClearPixel(uint16_t x, uint16_t y); // Clear a pixel of the (84 x 48) display buffer        
        void TogglePixel(uint16_t x, uint16_t y); // Toggle a pixel of the (84 x 48) display buffer        
        void WriteCharacter(const char character, uint16_t x, uint16_t y);
        void WriteString(const char *characters, uint16_t x, uint16_t y);
        void Clear();
        void Redraw();
        
        // Initialize the display.
        // Sets the LCD to a known state, Sets up the command list, Contrast, etc.
        void Init();
        
        // Change the contrast: try 0xB1 to 0xB3
        //void SetContrast(uint8_t cont);
        
        // Change the display bias mode: try 0x13 or 0x14
        //void SetBias(uint8_t bias);
        
        // Change the backlight level
        void SetBacklight(uint8_t level);
    
private:
        // Display buffer
        uint8_t displayBuffer[DISPLAY_BYTES]; //Display is 84px x 48px 

        void GotoXY(uint8_t x, uint8_t y);
        void Write(uint8_t data_or_command, uint8_t data);
        void WriteLSBF(uint8_t data);
};

#endif
