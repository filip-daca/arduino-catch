#include <LiquidCrystal.h>

#define MAX_SPRITES   8
#define SPRITE_LEN    8

byte screen[2][16][SPRITE_LEN];
byte customSprites[MAX_SPRITES][SPRITE_LEN];
boolean spriteUsed[MAX_SPRITES];
boolean fieldActive[2][16];

void engineInit(LiquidCrystal lcd) {
  engineClear(lcd);
  for (byte i = 0; i < MAX_SPRITES; ++i) {
    memset(customSprites[i], B00000, SPRITE_LEN);
  }
}

void engineClear(LiquidCrystal lcd) {
  for (byte y = 0; y < 2; ++y) {
    for (byte x = 0; x < 16; ++x) {
       memset(screen[y][x], B00000, SPRITE_LEN);
       fieldActive[y][x] = false;
    }
  }
  memset(spriteUsed, false, MAX_SPRITES);
  lcd.clear();
}

void engineDraw(byte sprite[], byte x, byte y) {
  for (byte i = 0; i < SPRITE_LEN; ++i) {
    screen[y][x][i] = (screen[y][x][i] | sprite[i]);
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

  if (DEBUG) {
    Serial.println(getUsedSpriteCount());
  }
}

byte findOrCreateSprite(LiquidCrystal lcd, byte sprite[]) {
  for (byte i = 0; i < MAX_SPRITES; ++i) {
    if (spriteUsed[i] && memcmp(sprite, customSprites[i], SPRITE_LEN) == 0) {
      return i;
    }
  }
  
  byte i = 0;
  while (spriteUsed[i] && i < 8) {
    i++;
  }

  spriteUsed[i] = true;
  memcpy(customSprites[i], sprite, SPRITE_LEN);
  lcd.createChar(i, sprite);

  return i;
}

byte getUsedSpriteCount() {
  byte count = 0;
  for (byte i = 0; i < MAX_SPRITES; ++i) {
    if (spriteUsed[i]) {
      count++;
    }
  }
  return count;
}
