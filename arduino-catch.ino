#include <LiquidCrystal.h>

#define GAME_DELAY          25

#define TOP_ROW             0
#define BOTTOM_ROW          1

#define BALL_SPRITE_0       0
#define BALL_SPRITE_1       1
#define BALL_SPRITE_2       2
#define BALL_SPRITE_3       3
#define PALLET_SPRITE       4
#define PALLET_SPRITE_LEFT  5
#define PALLET_SPRITE_RIGHT 6
#define MIXED_SPRITE        4

#define PALLET_POSITIONS    30

#define MAXIMUM_BALLS       12
#define BALL_MAX_Y          7
#define BALL_MAX_X          15
#define BALL_STEP_DELAY     20
#define BALL_ALIVE_DELAY    50

#define MAX_ANALOG_READ 1024.0

LiquidCrystal lcd(2, 3, 4, 5, 6, 7); 

byte palletSprite[8] = {
  B00000, B00000, B00000, B00000, B00000, B00000, B00000, B11111
};

byte palletSpriteLeft[8] = {
  B00000, B00000, B00000, B00000, B00000, B00000, B00000, B00011
};

byte palletSpriteRight[8] = {
  B00000, B00000, B00000, B00000, B00000, B00000, B00000, B11000
};

byte ballSprites[4][8] = {
  { B11000, B11000, B00000, B00000, B00000, B00000, B00000, B00000 },
  { B00000, B00000, B11000, B11000, B00000, B00000, B00000, B00000 },
  { B00000, B00000, B00000, B00000, B11000, B11000, B00000, B00000 },
  { B00000, B00000, B00000, B00000, B00000, B00000, B11000, B11000 }
};

typedef struct {
  byte x;
  byte y;
  word ticks;
  boolean alive;
} Ball;

int position;
Ball balls[MAXIMUM_BALLS];
boolean isPalletSpriteMixed;
byte mixedSpriteY;

void setup() {
  lcd.begin(16, 2);

  lcd.createChar(PALLET_SPRITE, palletSprite);
  lcd.createChar(PALLET_SPRITE_LEFT, palletSpriteLeft);
  lcd.createChar(PALLET_SPRITE_RIGHT, palletSpriteRight);
  
  lcd.createChar(BALL_SPRITE_0, ballSprites[0]);
  lcd.createChar(BALL_SPRITE_1, ballSprites[1]);
  lcd.createChar(BALL_SPRITE_2, ballSprites[2]);
  lcd.createChar(BALL_SPRITE_3, ballSprites[3]);
  
  lcd.clear();

  isPalletSpriteMixed = false;
  mixedSpriteY = 0;
  
  initializeBalls();
}

void initializeBalls() {
  for (byte i = 0; i < MAXIMUM_BALLS; ++i) {
    disableBall(i);
  }
}

void disableBall(byte i) {
  balls[i].alive = false;
  balls[i].ticks = random(150, 550);
}

void enableBall(byte i) {
  balls[i].alive = true;
  balls[i].ticks = BALL_STEP_DELAY;
  balls[i].y = 0;
  balls[i].x = random(0, BALL_MAX_X);
}

void loop() {
  position = readPalletPosition();

  generateBalls();
  moveBalls();
  
  lcd.clear();

  drawBalls();
  drawPallet();
  
  delay(GAME_DELAY);
}

byte readPalletPosition() {
  return PALLET_POSITIONS - analogRead(A0) * PALLET_POSITIONS / MAX_ANALOG_READ;
}

void clearTopRow() {
  lcd.setCursor(0, TOP_ROW);
  lcd.print("                ");
}

void clearBottomRow() {
  lcd.setCursor(0, BOTTOM_ROW);
  lcd.print("                ");
}

void drawPallet() {
  if (palletSharesFieldWithAnyBall()) {
    return;
  }

  if (isPalletSpriteMixed) {
    lcd.createChar(PALLET_SPRITE, palletSprite);
    isPalletSpriteMixed = false;
  }
  
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

void generateBalls() {
  for (byte i = 0; i < MAXIMUM_BALLS; ++i) {
    if (!balls[i].alive) {
      generateBall(i);
    }
  }
}

void generateBall(byte i) {
  balls[i].ticks -= 1;

  if (balls[i].ticks <= 0) {
    enableBall(i);
  }
}

void moveBalls() {
  for (byte i = 0; i < MAXIMUM_BALLS; ++i) {
    if (balls[i].alive) {
      moveBall(i);
    }
  }
}

void moveBall(byte i) {
  balls[i].ticks -= 1;
  
  if (balls[i].ticks <= 0) {
    stepBall(i);
  }
}

void stepBall(byte i) {
  balls[i].ticks = BALL_STEP_DELAY;
  balls[i].y += 1;
    
  if (balls[i].y > BALL_MAX_Y) {
    disableBall(i);
  }
}

void drawBalls() {
  for (byte i = 0; i < MAXIMUM_BALLS; ++i) {
    if (balls[i].alive) {
      
      if (balls[i].y < 4) {
        lcd.setCursor(balls[i].x, TOP_ROW);
      } else {
        lcd.setCursor(balls[i].x, BOTTOM_ROW);
      }

      byte ballSpriteNumber = balls[i].y % 4;
     
      if (palletSharesFieldWithBall(i)) {
        if (!isPalletSpriteMixed) {
          isPalletSpriteMixed = true;
        }
        updateMixedSpriteIfNeeded(ballSpriteNumber, balls[i].y);
        lcd.write(MIXED_SPRITE);
      } else {
        lcd.write(ballSpriteNumber);
      }
    }
  }
}

boolean palletSharesFieldWithAnyBall() {
  for (byte i = 0; i < MAXIMUM_BALLS; ++i) {
    if (balls[i].alive) {
      if (palletSharesFieldWithBall(i)) {
        return true;
      }
    }
  }
  return false;
}

boolean palletSharesFieldWithBall(byte i) {
  return (balls[i].y >= 4 && balls[i].x == position / 2);
}

void updateMixedSpriteIfNeeded(byte ballSpriteNumber, byte ballY) {
  if (mixedSpriteY != ballY) {
    createBallAndPalletMixedSprite(ballSpriteNumber);
    mixedSpriteY = ballY;
  }
}

void createBallAndPalletMixedSprite(byte ballSpriteNumber) {
  byte mixedSprite[8];
  
  for (byte y = 0; y < 8; ++y) {
    mixedSprite[y] = (palletSprite[y] | ballSprites[ballSpriteNumber][y]);
  }
  lcd.createChar(MIXED_SPRITE, mixedSprite);
}
