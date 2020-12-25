#include <LiquidCrystal.h>

#define TOP_ROW 0
#define BOTTOM_ROW 1

#define PALLET_SPRITE 0

LiquidCrystal lcd(2, 3, 4, 5, 6, 7); 

byte palletSprite[] = {
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B11111
};

double readValue;
int position;

void setup() {
  lcd.begin(16, 2);

  lcd.createChar(PALLET_SPRITE, palletSprite);
  
  lcd.clear();
  lcd.setCursor(0, TOP_ROW); 
  lcd.print("Hej :)");
}

void loop() {
  position = 15 - analogRead(A0) * 15.0/1024.0;

  clearBottomRow();
  drawPallet();
  
  delay(25);
}

void clearBottomRow() {
  lcd.setCursor(0, BOTTOM_ROW);
  lcd.print("                ");
}

void drawPallet() {
  lcd.setCursor(position, BOTTOM_ROW);
  lcd.write(byte(PALLET_SPRITE));
}
