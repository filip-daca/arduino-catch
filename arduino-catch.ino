#include <LiquidCrystal.h>

#define DEBUG               false

#define PIN_BUZZ            8
#define PIN_BUTTON          12

#define GAME_DELAY          25

#define FIRE_COOLDOWN       5
#define FIRE_STEP_DELAY     3

#define TOP_ROW             0
#define BOTTOM_ROW          1

#define PALLET_POSITIONS    30

#define MAXIMUM_BALLS       15
#define BALL_MAX_Y          7
#define BALL_MAX_X          15
#define BALL_STEP_DELAY_MIN 5
#define BALL_STEP_DELAY_MAX 25
#define BALL_ALIVE_DELAY    40

#define MAX_ANALOG_READ 1024.0

LiquidCrystal lcd(2, 3, 4, 5, 6, 7); 

byte palletSprites[4][8] = {
  { B00000, B00000, B00000, B00000, B00000, B00000, B00001, B00011 },
  { B00000, B00000, B00000, B00000, B00000, B00000, B01010, B11111 },
  { B00000, B00000, B00000, B00000, B00000, B00000, B10000, B11000 }
};

byte ballSprites[4][8] = {
  { B01110, B00000, B00000, B00000, B00000, B00000, B00000, B00000 },
  { B00000, B01110, B01010, B01110, B00000, B00000, B00000, B00000 },
  { B00000, B00000, B00000, B01110, B01010, B01110, B00000, B00000 },
  { B00000, B00000, B00000, B00000, B00000, B00000, B01110, B01010 }
};

byte fireSprite[8] = {
  B00100, B00100, B00100, B00100, B00100, B00100, B00100, B00100
};

typedef struct {
  byte x;
  byte y;
  word ticks;
  boolean alive;
  byte fallSpeed;
} Ball;

typedef struct {
  byte x;
  byte cooldown;
  byte visible;
  byte sound;
  boolean soundHit;
} Fire;

byte score;
Fire fire;
int position;
Ball balls[MAXIMUM_BALLS];

void setup() {
  if (DEBUG) {
    Serial.begin(9600);
  }
  lcd.begin(16, 2);
  
  engineInit(lcd);
  initializeBalls();

  pinMode(PIN_BUZZ, OUTPUT);
  pinMode(PIN_BUTTON, INPUT_PULLUP);
  
  playStartSound();

  score = 0;
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
  balls[i].fallSpeed = random(BALL_STEP_DELAY_MIN, BALL_STEP_DELAY_MAX);
  balls[i].ticks = balls[i].fallSpeed;
  balls[i].y = 0;
  balls[i].x = generateFreeBallX(i);
}

byte generateFreeBallX(byte i) {
  byte newX = random(0, BALL_MAX_X);
  for (byte b = 0; b < MAXIMUM_BALLS; ++b) {
    if (b != i && balls[b].x == newX && balls[b].y < 4) {
      return generateFreeBallX(i);
    }
  }
  return newX;
}

void loop() {
  position = readPalletPosition();

  generateBalls();
  moveBalls();
  
  engineClear(lcd);

  handleFire();

  drawFire();
  drawBalls();
  drawPallet();
  engineFlush(lcd);

  playSounds();
  
  delay(GAME_DELAY);

  noTone(PIN_BUZZ);
}

byte readPalletPosition() {
  return PALLET_POSITIONS - analogRead(A0) * PALLET_POSITIONS / MAX_ANALOG_READ;
}

void drawPallet() {
  if (position % 2 == 0) {
    engineDraw(palletSprites[1], position / 2, BOTTOM_ROW);
  } else {
    engineDraw(palletSprites[0], position / 2, BOTTOM_ROW);
    engineDraw(palletSprites[2], position / 2 + 1, BOTTOM_ROW);
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
  balls[i].ticks = balls[i].fallSpeed;
  balls[i].y += 1;
    
  if (balls[i].y > BALL_MAX_Y) {
    disableBall(i);
    playMissSound();
  }
}

void drawBalls() {
  for (byte i = 0; i < MAXIMUM_BALLS; ++i) {
    if (balls[i].alive) {
      byte row = balls[i].y < 4 ? TOP_ROW : BOTTOM_ROW;
      byte ballSpriteNumber = balls[i].y % 4;

      engineDraw(ballSprites[ballSpriteNumber], balls[i].x, row);
    }
  }
}
