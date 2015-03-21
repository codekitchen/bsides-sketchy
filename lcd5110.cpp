/*
    Filename: lcd5110.cpp
    Purpose: Library for Nokia5110 LCD on the 2015 BSidesSLC Badge
    Created: March 16, 2013
    Original Author: d3c4f

    Original Credit: Nathan Seidle, Spark Fun Electronics 2011
    Original Credit URL: https://github.com/sparkfun/GraphicLCD_Nokia_5110
    (Thanks Nathan!)
*/

#include "Arduino.h"
#include "lcd5110.h"

/*
SCE      : PB4
RESET    : PB5
DC       : PB6
SDIN     : PB2
SCLK     : PB1
LED Back : Arduino Analog 11 (PWM)
*/


//This table contains the hex values that represent pixels
//for a font that is 5 pixels wide and 8 pixels high
const byte ASCII[][5] = {
    {0x00, 0x00, 0x00, 0x00, 0x00}, // 20  
    {0x00, 0x00, 0x5f, 0x00, 0x00}, // 21 !
    {0x00, 0x07, 0x00, 0x07, 0x00}, // 22 "
    {0x14, 0x7f, 0x14, 0x7f, 0x14}, // 23 #
    {0x24, 0x2a, 0x7f, 0x2a, 0x12}, // 24 $
    {0x23, 0x13, 0x08, 0x64, 0x62}, // 25 %
    {0x36, 0x49, 0x55, 0x22, 0x50}, // 26 &
    {0x00, 0x05, 0x03, 0x00, 0x00}, // 27 '
    {0x00, 0x1c, 0x22, 0x41, 0x00}, // 28 (
    {0x00, 0x41, 0x22, 0x1c, 0x00}, // 29 )
    {0x14, 0x08, 0x3e, 0x08, 0x14}, // 2a *
    {0x08, 0x08, 0x3e, 0x08, 0x08}, // 2b +
    {0x00, 0x50, 0x30, 0x00, 0x00}, // 2c ,
    {0x08, 0x08, 0x08, 0x08, 0x08}, // 2d -
    {0x00, 0x60, 0x60, 0x00, 0x00}, // 2e .
    {0x20, 0x10, 0x08, 0x04, 0x02}, // 2f /
    {0x3e, 0x51, 0x49, 0x45, 0x3e}, // 30 0
    {0x00, 0x42, 0x7f, 0x40, 0x00}, // 31 1
    {0x42, 0x61, 0x51, 0x49, 0x46}, // 32 2
    {0x21, 0x41, 0x45, 0x4b, 0x31}, // 33 3
    {0x18, 0x14, 0x12, 0x7f, 0x10}, // 34 4
    {0x27, 0x45, 0x45, 0x45, 0x39}, // 35 5
    {0x3c, 0x4a, 0x49, 0x49, 0x30}, // 36 6
    {0x01, 0x71, 0x09, 0x05, 0x03}, // 37 7
    {0x36, 0x49, 0x49, 0x49, 0x36}, // 38 8
    {0x06, 0x49, 0x49, 0x29, 0x1e}, // 39 9
    {0x00, 0x36, 0x36, 0x00, 0x00}, // 3a :
    {0x00, 0x56, 0x36, 0x00, 0x00}, // 3b ;
    {0x08, 0x14, 0x22, 0x41, 0x00}, // 3c <
    {0x14, 0x14, 0x14, 0x14, 0x14}, // 3d =
    {0x00, 0x41, 0x22, 0x14, 0x08}, // 3e >
    {0x02, 0x01, 0x51, 0x09, 0x06}, // 3f ?
    {0x32, 0x49, 0x79, 0x41, 0x3e}, // 40 @
    {0x7e, 0x11, 0x11, 0x11, 0x7e}, // 41 A
    {0x7f, 0x49, 0x49, 0x49, 0x36}, // 42 B
    {0x3e, 0x41, 0x41, 0x41, 0x22}, // 43 C
    {0x7f, 0x41, 0x41, 0x22, 0x1c}, // 44 D
    {0x7f, 0x49, 0x49, 0x49, 0x41}, // 45 E
    {0x7f, 0x09, 0x09, 0x09, 0x01}, // 46 F
    {0x3e, 0x41, 0x49, 0x49, 0x7a}, // 47 G
    {0x7f, 0x08, 0x08, 0x08, 0x7f}, // 48 H
    {0x00, 0x41, 0x7f, 0x41, 0x00}, // 49 I
    {0x20, 0x40, 0x41, 0x3f, 0x01}, // 4a J
    {0x7f, 0x08, 0x14, 0x22, 0x41}, // 4b K
    {0x7f, 0x40, 0x40, 0x40, 0x40}, // 4c L
    {0x7f, 0x02, 0x0c, 0x02, 0x7f}, // 4d M
    {0x7f, 0x04, 0x08, 0x10, 0x7f}, // 4e N
    {0x3e, 0x41, 0x41, 0x41, 0x3e}, // 4f O
    {0x7f, 0x09, 0x09, 0x09, 0x06}, // 50 P
    {0x3e, 0x41, 0x51, 0x21, 0x5e}, // 51 Q
    {0x7f, 0x09, 0x19, 0x29, 0x46}, // 52 R
    {0x46, 0x49, 0x49, 0x49, 0x31}, // 53 S
    {0x01, 0x01, 0x7f, 0x01, 0x01}, // 54 T
    {0x3f, 0x40, 0x40, 0x40, 0x3f}, // 55 U
    {0x1f, 0x20, 0x40, 0x20, 0x1f}, // 56 V
    {0x3f, 0x40, 0x38, 0x40, 0x3f}, // 57 W
    {0x63, 0x14, 0x08, 0x14, 0x63}, // 58 X
    {0x07, 0x08, 0x70, 0x08, 0x07}, // 59 Y
    {0x61, 0x51, 0x49, 0x45, 0x43}, // 5a Z
    {0x00, 0x7f, 0x41, 0x41, 0x00}, // 5b [
    {0x02, 0x04, 0x08, 0x10, 0x20}, // 5c "\"
    {0x00, 0x41, 0x41, 0x7f, 0x00}, // 5d ]
    {0x04, 0x02, 0x01, 0x02, 0x04}, // 5e ^
    {0x40, 0x40, 0x40, 0x40, 0x40}, // 5f _
    {0x00, 0x01, 0x02, 0x04, 0x00}, // 60 `
    {0x20, 0x54, 0x54, 0x54, 0x78}, // 61 a
    {0x7f, 0x48, 0x44, 0x44, 0x38}, // 62 b
    {0x38, 0x44, 0x44, 0x44, 0x20}, // 63 c
    {0x38, 0x44, 0x44, 0x48, 0x7f}, // 64 d
    {0x38, 0x54, 0x54, 0x54, 0x18}, // 65 e
    {0x08, 0x7e, 0x09, 0x01, 0x02}, // 66 f
    {0x0c, 0x52, 0x52, 0x52, 0x3e}, // 67 g
    {0x7f, 0x08, 0x04, 0x04, 0x78}, // 68 h
    {0x00, 0x44, 0x7d, 0x40, 0x00}, // 69 i
    {0x20, 0x40, 0x44, 0x3d, 0x00}, // 6a j 
    {0x7f, 0x10, 0x28, 0x44, 0x00}, // 6b k
    {0x00, 0x41, 0x7f, 0x40, 0x00}, // 6c l
    {0x7c, 0x04, 0x18, 0x04, 0x78}, // 6d m
    {0x7c, 0x08, 0x04, 0x04, 0x78}, // 6e n
    {0x38, 0x44, 0x44, 0x44, 0x38}, // 6f o
    {0x7c, 0x14, 0x14, 0x14, 0x08}, // 70 p
    {0x08, 0x14, 0x14, 0x18, 0x7c}, // 71 q
    {0x7c, 0x08, 0x04, 0x04, 0x08}, // 72 r
    {0x48, 0x54, 0x54, 0x54, 0x20}, // 73 s
    {0x04, 0x3f, 0x44, 0x40, 0x20}, // 74 t
    {0x3c, 0x40, 0x40, 0x20, 0x7c}, // 75 u
    {0x1c, 0x20, 0x40, 0x20, 0x1c}, // 76 v
    {0x3c, 0x40, 0x30, 0x40, 0x3c}, // 77 w
    {0x44, 0x28, 0x10, 0x28, 0x44}, // 78 x
    {0x0c, 0x50, 0x50, 0x50, 0x3c}, // 79 y
    {0x44, 0x64, 0x54, 0x4c, 0x44}, // 7a z
    {0x00, 0x08, 0x36, 0x41, 0x00}, // 7b {
    {0x00, 0x00, 0x7f, 0x00, 0x00}, // 7c |
    {0x00, 0x41, 0x36, 0x08, 0x00}, // 7d }
    {0x10, 0x08, 0x08, 0x10, 0x08}, // 7e ~
    {0x78, 0x46, 0x41, 0x46, 0x78}  // 7f DEL
};


// Constructor
LCD5110::LCD5110()
{
    Init();
    
    SetBacklight(0x80); // Set initial backlight level
}


// Initialize the display.
// Sets the LCD to a known state, Sets up the command list, Contrast, etc.
void LCD5110::Init()
{
    // Set display pins as outputs
    DDRB |= B11110110; // Set B7, B6, B5, B4, B2, B1 as outputs

    // Reset the LCD by toggling the reset pin
    PORTB &= B11011111; // Pull B5 Low
    PORTB |= B00100000; // Pull B5 High

    Write(LCD_COMMAND, 0x21); //Tell LCD that extended commands follow
    Write(LCD_COMMAND, 0xB3); //Set LCD Vop (Contrast): Try 0xB1(good @ 3.3V) or 0xBF if your display is too dark
    Write(LCD_COMMAND, 0x04); //Set Temp coefficient
    Write(LCD_COMMAND, 0x13); //LCD bias mode 1:48: Try 0x13 or 0x14

    Write(LCD_COMMAND, 0x20); //We must send 0x20 before modifying the display control mode
    Write(LCD_COMMAND, 0x0C); //Set display control, normal mode. 0x0D for inverse

    Clear();
}


// Change the backlight level
void LCD5110::SetBacklight(uint8_t level)
{
    // Could maybe switch this to direct port manipulation later...
    analogWrite(11, level);
}


// Set a pixel of the (84 x 48) display buffer
void LCD5110::SetPixel(uint16_t x, uint16_t y)
{
    if (x >= 84 || y >= 48) return; // Bounds check
    displayBuffer[(84 * (y / 8)) + x] |= (1 << (y % 8));
}


// Clear a pixel of the (84 x 48) display buffer
void LCD5110::ClearPixel(uint16_t x, uint16_t y)
{
    if (x >= 84 || y >= 48) return; // Bounds check
    displayBuffer[(84 * (y / 8)) + x] &= ~(1 << (y % 8));
}


// Toggle a pixel of the (84 x 48) display buffer
void LCD5110::TogglePixel(uint16_t x, uint16_t y)
{
    if (x >= 84 || y >= 48) return; // Bounds check
    displayBuffer[(84 * (y / 8)) + x] ^= (1 << (y % 8));
}


// Write a byte array to the display buffer and show it
void LCD5110::DisplayBitmap(const uint8_t bitmap_array[], bool redraw)
{
    for (uint16_t curByte = 0 ; curByte < 504 ; curByte++)
    {
        //Write(LCD_DATA, bitmap_array[index]);
        displayBuffer[curByte] = bitmap_array[curByte];
    }
    
    if (redraw)
      Redraw();
}


// Write a byte array stored in program (flash) memory to the display buffer and show it
void LCD5110::DisplayBitmap_PROGMEM(const uint8_t bitmap_array[])
{
    for (uint16_t curByte = 0 ; curByte < 504 ; curByte++)
    {
        //Write(LCD_DATA, bitmap_array[index]);
        displayBuffer[curByte] = pgm_read_byte_near(bitmap_array + curByte);
    }
    
    Redraw();
}


// Write a character to the display
// Pads each character with 1px on either side for readability
void LCD5110::WriteCharacter(const char character, uint16_t x, uint16_t y)
{
    for (uint8_t colByte = 0 ; colByte < 5 ; colByte++)
    {
        //0x20 is the ASCII character for Space (' '). The font table starts with this character
        if(ASCII[character - 0x20][colByte] & BIT0_SET) SetPixel(x + colByte, y);
        else ClearPixel(x + colByte, y);

        if(ASCII[character - 0x20][colByte] & BIT1_SET) SetPixel(x + colByte, y + 1);
        else ClearPixel(x + colByte, y + 1);

        if(ASCII[character - 0x20][colByte] & BIT2_SET) SetPixel(x + colByte, y + 2);
        else ClearPixel(x + colByte, y + 2);

        if(ASCII[character - 0x20][colByte] & BIT3_SET) SetPixel(x + colByte, y + 3);
        else ClearPixel(x + colByte, y + 3);

        if(ASCII[character - 0x20][colByte] & BIT4_SET) SetPixel(x + colByte, y + 4);
        else ClearPixel(x + colByte, y + 4);

        if(ASCII[character - 0x20][colByte] & BIT5_SET) SetPixel(x + colByte, y + 5);
        else ClearPixel(x + colByte, y + 5);

        if(ASCII[character - 0x20][colByte] & BIT6_SET) SetPixel(x + colByte, y + 6);
        else ClearPixel(x + colByte, y + 6);

        if(ASCII[character - 0x20][colByte] & BIT7_SET) SetPixel(x + colByte, y + 7);
        else ClearPixel(x + colByte, y + 7);
        
        //Write(LCD_DATA, ASCII[character - 0x20][colByte]);
    }
}


// Writes an array of characters to a single line.
void LCD5110::WriteString(const char *characters, uint16_t x, uint16_t y)
{
    uint8_t curChar = 0;
    while (*characters)
    {
        WriteCharacter(*characters++, x + (curChar * 6), y);
        curChar ++;
    }
}


// Zero out the entire display buffer and redraw
void LCD5110::Clear()
{
    for (uint16_t curByte = 0 ; curByte < 504 ; curByte++)
    {
        displayBuffer[curByte] = 0x00;
    }
    Redraw();
}


// Change the contrast: try 0xB1 to 0xB3
//void SetContrast(uint8_t cont);


// Change the display bias mode: try 0x13 or 0x14
//void SetBias(uint8_t bias);


// Move to X,Y position
void LCD5110::GotoXY(uint8_t x, uint8_t y)
{
        Write(LCD_COMMAND, 0x80 | x);  // Column.
        Write(LCD_COMMAND, 0x40 | y);  // Row.  ?
}


// There are two memory banks in the LCD, data/RAM and commands. This 
// function sets the DC pin high or low depending, and then sends
// the data byte
void LCD5110::Write(byte data_or_command, byte data)
{
    // Set if we are sending data or a command : 0 for command, 1 for data
    PORTB = data_or_command == 1 ? (PORTB | BIT6_SET) : (PORTB & BIT6_MASK);
    
    // Send the data
    PORTB &= BIT4_MASK; // Pull SCE Low (B5)
    
    // Clock in each bit
    PORTB = (data & BIT7_SET) ? (PORTB | BIT2_SET) : (PORTB & BIT2_MASK);
    PORTB |= BIT1_SET; PORTB &= BIT1_MASK; // Pull SCLK (B1) High and then Low    
    
    PORTB = (data & BIT6_SET) ? (PORTB | BIT2_SET) : (PORTB & BIT2_MASK);
    PORTB |= BIT1_SET; PORTB &= BIT1_MASK; // Pull SCLK (B1) High and then Low    

    PORTB = (data & BIT5_SET) ? (PORTB | BIT2_SET) : (PORTB & BIT2_MASK);
    PORTB |= BIT1_SET; PORTB &= BIT1_MASK; // Pull SCLK (B1) High and then Low    

    PORTB = (data & BIT4_SET) ? (PORTB | BIT2_SET) : (PORTB & BIT2_MASK);
    PORTB |= BIT1_SET; PORTB &= BIT1_MASK; // Pull SCLK (B1) High and then Low    

    PORTB = (data & BIT3_SET) ? (PORTB | BIT2_SET) : (PORTB & BIT2_MASK);
    PORTB |= BIT1_SET; PORTB &= BIT1_MASK; // Pull SCLK (B1) High and then Low    

    PORTB = (data & BIT2_SET) ? (PORTB | BIT2_SET) : (PORTB & BIT2_MASK);
    PORTB |= BIT1_SET; PORTB &= BIT1_MASK; // Pull SCLK (B1) High and then Low    

    PORTB = (data & BIT1_SET) ? (PORTB | BIT2_SET) : (PORTB & BIT2_MASK);
    PORTB |= BIT1_SET; PORTB &= BIT1_MASK; // Pull SCLK (B1) High and then Low    

    PORTB = (data & BIT0_SET) ? (PORTB | BIT2_SET) : (PORTB & BIT2_MASK);
    PORTB |= BIT1_SET; PORTB &= BIT1_MASK; // Pull SCLK (B1) High and then Low    
    
    PORTB |= BIT4_SET; // Pull SCE High (B4)
}


// Same as Write, but in LSBF-order for screen-buffer processing moistly ;)
void LCD5110::WriteLSBF(byte data)
{
    // We will only send data this way
    PORTB |= BIT6_SET;
    
    PORTB &= BIT4_MASK; // Pull SCE Low (B5)
    
    // Clock in each bit
    PORTB = (data & BIT0_SET) ? (PORTB | BIT2_SET) : (PORTB & BIT2_MASK);
    PORTB |= BIT1_SET; PORTB &= BIT1_MASK; // Pull SCLK (B1) High and then Low    

    PORTB = (data & BIT1_SET) ? (PORTB | BIT2_SET) : (PORTB & BIT2_MASK);
    PORTB |= BIT1_SET; PORTB &= BIT1_MASK; // Pull SCLK (B1) High and then Low    

    PORTB = (data & BIT2_SET) ? (PORTB | BIT2_SET) : (PORTB & BIT2_MASK);
    PORTB |= BIT1_SET; PORTB &= BIT1_MASK; // Pull SCLK (B1) High and then Low    

    PORTB = (data & BIT3_SET) ? (PORTB | BIT2_SET) : (PORTB & BIT2_MASK);
    PORTB |= BIT1_SET; PORTB &= BIT1_MASK; // Pull SCLK (B1) High and then Low    

    PORTB = (data & BIT4_SET) ? (PORTB | BIT2_SET) : (PORTB & BIT2_MASK);
    PORTB |= BIT1_SET; PORTB &= BIT1_MASK; // Pull SCLK (B1) High and then Low    

    PORTB = (data & BIT5_SET) ? (PORTB | BIT2_SET) : (PORTB & BIT2_MASK);
    PORTB |= BIT1_SET; PORTB &= BIT1_MASK; // Pull SCLK (B1) High and then Low    

    PORTB = (data & BIT6_SET) ? (PORTB | BIT2_SET) : (PORTB & BIT2_MASK);
    PORTB |= BIT1_SET; PORTB &= BIT1_MASK; // Pull SCLK (B1) High and then Low    

    PORTB = (data & BIT7_SET) ? (PORTB | BIT2_SET) : (PORTB & BIT2_MASK);
    PORTB |= BIT1_SET; PORTB &= BIT1_MASK; // Pull SCLK (B1) High and then Low    
    
    PORTB |= BIT4_SET; // Pull SCE High (B4)
}


// Pushes the Display Buffer out to the Display
void LCD5110::Redraw()
{
    Write(LCD_COMMAND, 0x80); Write(LCD_COMMAND, 0x40); // Reset position to "0,0"
    PORTB |= BIT6_SET; // We're sending date (not commands)
    PORTB &= BIT4_MASK; // Pull SCE Low (B5) (enable)
    
    // Draw the screen buffer
    for(int curByte = 503; curByte > 0; curByte --)
    {
        // Clock in each bit
        PORTB = (displayBuffer[curByte] & BIT0_SET) ? (PORTB | BIT2_SET) : (PORTB & BIT2_MASK);
        PORTB |= BIT1_SET; PORTB &= BIT1_MASK; // Pull SCLK (B1) High and then Low    

        PORTB = (displayBuffer[curByte] & BIT1_SET) ? (PORTB | BIT2_SET) : (PORTB & BIT2_MASK);
        PORTB |= BIT1_SET; PORTB &= BIT1_MASK; // Pull SCLK (B1) High and then Low    

        PORTB = (displayBuffer[curByte] & BIT2_SET) ? (PORTB | BIT2_SET) : (PORTB & BIT2_MASK);
        PORTB |= BIT1_SET; PORTB &= BIT1_MASK; // Pull SCLK (B1) High and then Low    

        PORTB = (displayBuffer[curByte] & BIT3_SET) ? (PORTB | BIT2_SET) : (PORTB & BIT2_MASK);
        PORTB |= BIT1_SET; PORTB &= BIT1_MASK; // Pull SCLK (B1) High and then Low    

        PORTB = (displayBuffer[curByte] & BIT4_SET) ? (PORTB | BIT2_SET) : (PORTB & BIT2_MASK);
        PORTB |= BIT1_SET; PORTB &= BIT1_MASK; // Pull SCLK (B1) High and then Low    

        PORTB = (displayBuffer[curByte] & BIT5_SET) ? (PORTB | BIT2_SET) : (PORTB & BIT2_MASK);
        PORTB |= BIT1_SET; PORTB &= BIT1_MASK; // Pull SCLK (B1) High and then Low    

        PORTB = (displayBuffer[curByte] & BIT6_SET) ? (PORTB | BIT2_SET) : (PORTB & BIT2_MASK);
        PORTB |= BIT1_SET; PORTB &= BIT1_MASK; // Pull SCLK (B1) High and then Low    

        PORTB = (displayBuffer[curByte] & BIT7_SET) ? (PORTB | BIT2_SET) : (PORTB & BIT2_MASK);
        PORTB |= BIT1_SET; PORTB &= BIT1_MASK; // Pull SCLK (B1) High and then Low    
    }
    PORTB |= BIT4_SET; // Pull SCE High (B4) (done)
}

