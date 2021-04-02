#ifndef OLEDDISPLAY
#define OLEDDISPLAY

#include "SparkFun_VL53L1X.h"
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32

void InitOled(Adafruit_SSD1306 Oled){
  Oled.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS);
  Oled.display();
}

void SendString(Adafruit_SSD1306 Oled, String s){
  Oled.clearDisplay();
  Oled.setTextSize(1);             // Normal 1:1 pixel scale
  Oled.setTextColor(SSD1306_WHITE);        // Draw white text
  Oled.setCursor(0,0);             // Start at top-left corner
  Oled.println(s);
  Oled.display();
}

#endif
