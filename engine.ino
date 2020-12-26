#include <LiquidCrystal.h>

byte screen[2][16][8];
byte customSprites[8][8];
boolean spriteUsed[8];
boolean fieldActive[2][16];

void engineInit(LiquidCrystal lcd) {
  engineClear(lcd);
  for (byte i = 0; i < 8; ++i) {
    memset(customSprites[i], B00000, 8);
  }
}

void engineDraw(byte sprite[], byte x, byte y) {
  for (byte i = 0; i < 8; ++i) {
    screen[x][y][i] = (screen[x][y][i] | sprite[i]);
  }
  fieldActive[y][x] = true;
}

void engineFlush(LiquidCrystal lcd) {
  for (byte y = 0; y < 2; ++y) {
    for (byte x = 0; x < 16; ++x) {
      if (fieldActive[y][x]) {
        byte spriteNumber = findOrCreateSprite(lcd, screen[y][x]);
        lcd.setCursor(x, y);
        lcd.write(spriteNumber);
      }
    }
  }
}

void engineClear(LiquidCrystal lcd) {
  for (byte y = 0; y < 2; ++y) {
    for (byte x = 0; x < 16; ++x) {
       memset(screen[y][x], B00000, 8);
       fieldActive[y][x] = false;
    }
  }
  memset(spriteUsed, false, 8);
  lcd.clear();
}

byte findOrCreateSprite(LiquidCrystal lcd, byte sprite[]) {
  for (byte i = 0; i < 8; ++i) {
    if (spriteUsed[i] && memcmp(sprite, customSprites[i], 8) == 0) {
      return i;
    }
  }
  
  byte i = 0;
  while (spriteUsed[i] && i < 8) {
    i++;
  }

  spriteUsed[i] = true;
  memcpy(customSprites[i], sprite, 8);
  lcd.createChar(i, sprite);

  return i;
}
