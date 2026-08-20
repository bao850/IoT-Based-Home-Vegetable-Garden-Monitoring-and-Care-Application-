#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  Wire.begin();      // SDA = PB9, SCL = PB8 (bắt buộc với code mới)
  Wire.setClock(100000);

  lcd.init();
  lcd.backlight();

  lcd.setCursor(0, 0);
  lcd.print("STM32 OK");

  lcd.setCursor(0, 1);
  lcd.print("I2C PB9 PB8 WORK");
}

void loop() {}
