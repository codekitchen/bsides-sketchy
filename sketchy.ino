#include <EEPROM.h>
#include "lcd5110.h"
#include "badge.h"

uint8_t image[DISPLAY_BYTES];
LCD5110 display;
Badge badge;

uint8_t x = 5, y = 5;

void setup() {
  for (int i = 0; i < DISPLAY_BYTES; ++i) {
    image[i] = EEPROM.read(i);
  }
}

void save(int byte) {
  EEPROM.write(byte, image[byte]);
}

void clearImage() {
  for (int i = 0; i < DISPLAY_BYTES; ++i) {
    image[i] = 0;
    EEPROM.write(i, 0);
  }
}

void loop() {
  if (badge.JoyUp(50, true) && y > 0) {
    y = y-1;
  }
  if (badge.JoyLeft(50, true) && x > 0) {
    x = x-1;
  }
  if (badge.JoyDown(50, true) && y < 47) {
    y = y+1;
  }
  if (badge.JoyRight(50, true) && x < 83) {
    x = x+1;
  }
  
  bool b1 = badge.Button0(15, true),
       b2 = badge.Button1(15, true),
       j0 = badge.JoyCenter(15, true);
  
  if (b1 && b2 && j0) {
    clearImage();
  } else if (b1) {
    image[(84 * (y / 8)) + x] &= ~(1 << (y % 8));
    save((84 * (y / 8)) + x);
    badge.SetLED02(true);
  } else if (b2) {
    image[(84 * (y / 8)) + x] |= (1 << (y % 8));
    save((84 * (y / 8)) + x);
    badge.SetLED01(true);
  } else {
    badge.SetLED01(false);
    badge.SetLED02(false);
  }
  
  display.DisplayBitmap(image, false);
  if (x > 0)
    display.TogglePixel(x-1, y);
  display.TogglePixel(x+1, y);
  if (y > 0)
    display.TogglePixel(x, y-1);
  display.TogglePixel(x, y+1);
  display.Redraw();
  delay(50);
}
