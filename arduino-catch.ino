#include <LiquidCrystal.h>
#define TOP_ROW 0
#define BOTTOM_ROW 1

LiquidCrystal lcd(2, 3, 4, 5, 6, 7); 

void setup() {
  lcd.begin(16, 2);
  lcd.clear();
  lcd.setCursor(0, TOP_ROW); 
  lcd.print("Hej :)");
}

double readValue;
int position;


void loop() {
  position = 15 - analogRead(A0) * 15.0/1024.0;

  lcd.setCursor(0, BOTTOM_ROW);
  lcd.print("                ");

  lcd.setCursor(position, BOTTOM_ROW);
  lcd.print("_");
  delay(50);
}
