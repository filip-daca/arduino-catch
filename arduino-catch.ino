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

#define HEAL_DELAY          20

#define MAXIMUM_BALLS       15
#define BALL_MAX_Y          7
#define BALL_MAX_X          15
#define BALL_STEP_DELAY_MIN 5
#define BALL_STEP_DELAY_MAX 25
#define BALL_ALIVE_DELAY    40
#define BALL_HIT_DAMAGE     200

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
  byte position;
} Pallet;

typedef struct {
  byte x;
  byte y;
  word ticks;
  boolean alive;
  byte stepDelay;
} Ball;

typedef struct {
  byte x;
  byte cooldown;
  byte visible;
  byte sound;
  boolean soundHit;
} Fire;

Fire fire;
Ball balls[MAXIMUM_BALLS];
Pallet pallet;

byte score;
byte damageTaken;
byte healDelay;

void setup() {
  initializeHardware();
  
  engineInit(lcd);
  
  initializeBalls();
  initializeGame();
}

void initializeHardware() {
  if (DEBUG) {
    Serial.begin(9600);
  }
  lcd.begin(16, 2);
  pinMode(PIN_BUZZ, OUTPUT);
  pinMode(PIN_BUTTON, INPUT_PULLUP);
}

void initializeGame() {
  score = 0;
  damageTaken = 0;
  playStartSound();
}

void loopGame() {
  healDelay--;
  if (healDelay <= 0) {
    healDelay = HEAL_DELAY;
    if (damageTaken > 0) {
      damageTaken--;
    }
  }
}

void loop() {
  loopPallet();
  loopBalls();
  loopFire();

  engineClear(lcd);
  drawFire();
  drawBalls();
  drawPallet();
  engineFlush(lcd);

  ringSound();
  
  delay(GAME_DELAY);

  muteSound();
}
