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
