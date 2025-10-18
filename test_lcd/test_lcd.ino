#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);  // Replace 0x27 with your actual I2C address

void setup() {
  Wire.begin(D2, D1);   // SDA, SCL for NodeMCU
  lcd.init();            // Initialize the LCD
  lcd.backlight();       // Turn on backlight

  lcd.setCursor(0, 0);
  lcd.print("Hello, World!");

  lcd.setCursor(0, 1);
  lcd.print("NodeMCU + LCD");
}

void loop() {}
