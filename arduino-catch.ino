#include <LiquidCrystal.h>

#define TOP_ROW 0
#define BOTTOM_ROW 1

#define PALLET_SPRITE 0
#define PALLET_SPRITE_LEFT 1
#define PALLET_SPRITE_RIGHT 2
#define BALL_SPRITE_0 3
#define BALL_SPRITE_1 4
#define BALL_SPRITE_2 5
#define BALL_SPRITE_3 6

#define PALLET_POSITIONS 30

LiquidCrystal lcd(2, 3, 4, 5, 6, 7); 

byte palletSprite[] = {
  B00000, B00000, B00000, B00000, B00000, B00000, B00000, B11111
};

byte palletSpriteLeft[] = {
  B00000, B00000, B00000, B00000, B00000, B00000, B00000, B00011
};

byte palletSpriteRight[] = {
  B00000, B00000, B00000, B00000, B00000, B00000, B00000, B11000
};

byte ballSprite0[] = {
  B11000, B11000, B00000, B00000, B00000, B00000, B00000, B00000
};

byte ballSprite1[] = {
  B00000, B00000, B11000, B11000, B00000, B00000, B00000, B00000
};

byte ballSprite2[] = {
  B00000, B00000, B00000, B00000, B11000, B11000, B00000, B00000
};

byte ballSprite3[] = {
  B00000, B00000, B00000, B00000, B00000, B00000, B11000, B11000
};

int position;

struct ball {
  byte x;
  byte y;
  int ticks;
};

void setup() {
  lcd.begin(16, 2);

  lcd.createChar(PALLET_SPRITE, palletSprite);
  lcd.createChar(PALLET_SPRITE_LEFT, palletSpriteLeft);
  lcd.createChar(PALLET_SPRITE_RIGHT, palletSpriteRight);
  lcd.createChar(BALL_SPRITE_0, ballSprite0);
  lcd.createChar(BALL_SPRITE_1, ballSprite1);
  lcd.createChar(BALL_SPRITE_2, ballSprite2);
  lcd.createChar(BALL_SPRITE_3, ballSprite3);
  
  lcd.clear();
  lcd.setCursor(0, TOP_ROW); 
  lcd.print("Hej :)");
}

void loop() {
  position = PALLET_POSITIONS - analogRead(A0) * PALLET_POSITIONS / 1024.0;

  moveBalls();
  clearBottomRow();
  drawBalls();
  drawPallet();
  
  delay(25);
}

void clearBottomRow() {
  lcd.setCursor(0, BOTTOM_ROW);
  lcd.print("                ");
}

void drawPallet() {
  if (position % 2 == 0) {  
    lcd.setCursor(position / 2, BOTTOM_ROW);
    lcd.write(byte(PALLET_SPRITE));
  } else {
    lcd.setCursor(position / 2, BOTTOM_ROW);
    lcd.write(byte(PALLET_SPRITE_LEFT));
    lcd.setCursor(position / 2 + 1, BOTTOM_ROW);
    lcd.write(byte(PALLET_SPRITE_RIGHT));
  }
}

void moveBalls() {
  
}

void drawBalls() {
  
}
