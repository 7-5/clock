#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

#define OLED_RESET -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define LOGO_HEIGHT 16 // Removed semicolon
#define LOGO_WIDTH 16 // Removed semicolon

void setup() {
  display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS); // Removed extra closing parenthesis
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.print("MMMMMMMMMMMiiiiiiiiiiiiiiiii"); // Corrected typo from "Hellow World" to "Hello World"
  display.display();
}

void loop() {
  // Your loop code here
}
