#include <LiquidCrystal.h>

#define GAME_DELAY 25

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
#define MAXIMUM_BALLS 12
#define BALL_MAX_Y 8
#define BALL_MAX_X 15
#define BALL_SPEED_DELAY 10
#define BALL_ALIVE_DELAY 50

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

typedef struct {
  byte x;
  byte y;
  byte ticks;
  boolean alive;
} Ball;

int position;
Ball balls[MAXIMUM_BALLS];

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

  initializeBalls();
}

void loop() {
  position = PALLET_POSITIONS - analogRead(A0) * PALLET_POSITIONS / 1024.0;

  generateBalls();
  moveBalls();

  clearAllRows();
  
  drawBalls();
  drawPallet();
  
  delay(GAME_DELAY);
}

void clearAllRows() {
  clearTopRow();
  clearBottomRow();
}

void clearTopRow() {
  lcd.setCursor(0, TOP_ROW);
  lcd.print("                ");
}

void clearBottomRow() {
  lcd.setCursor(0, BOTTOM_ROW);
  lcd.print("                ");
}

void disableBall(byte i) {
  balls[i].alive = false;
  balls[i].ticks = random(50, 256);
}

void enableBall(byte i) {
  balls[i].alive = true;
  balls[i].y = 0;
  balls[i].x = random(0, BALL_MAX_X);
  balls[i].ticks = BALL_SPEED_DELAY;
}

void initializeBalls() {
  for (byte i = 0; i < MAXIMUM_BALLS; ++i) {
    disableBall(i);
  }
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
  
  if (balls[i].ticks == 0) {
    balls[i].ticks = BALL_SPEED_DELAY;
    balls[i].y += 1;
    
    if (balls[i].y > BALL_MAX_Y) {
      disableBall(i);
    }
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

      if (balls[i].y == 0 || balls[i].y == 4) {
        lcd.write(byte(BALL_SPRITE_0));
      } else if (balls[i].y == 1 || balls[i].y == 5) {
        lcd.write(byte(BALL_SPRITE_1));
      } else if (balls[i].y == 2 || balls[i].y == 6) {
        lcd.write(byte(BALL_SPRITE_2));
      } else if (balls[i].y == 3 || balls[i].y == 7) {
        lcd.write(byte(BALL_SPRITE_3));
      }
    }
  }
}
