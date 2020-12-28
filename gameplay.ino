void initializeGameplay() {
  initializePallet();
  initializeBalls();
  
  score = 0;
  damageTaken = 0;
  playStartSound();

  mode = MODE_GAME;
}

void loopGameplay() {
  loopHealth();
  loopPallet();
  loopFire();
  loopBalls();

  engineClear(lcd);
  drawPallet();
  drawFire();
  drawBalls();
  engineFlush(lcd);

  ringSound();
  
  delay(GAME_DELAY);

  muteSound();
}

void loopHealth() {
  if (damageTaken > 0) {
    damageTaken -= HEAL_STEP;
  }

  analogWrite(PIN_LED, damageTaken);
  if (DEBUG) {
    Serial.println("DMG: " + String(damageTaken));
  }
}

void gameOver() {
  initializeScoreboard();
}
