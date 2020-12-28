void initializeGameplay() {
  initializeBalls();
  
  score = 0;
  damageTaken = 0;
  playStartSound();

  mode = MODE_GAME;
}

void loopGameplay() {
  loopHealth();
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

void loopHealth() {
  if (damageTaken > 0) {
    damageTaken -= HEAL_STEP;
  }

  analogWrite(PIN_LED, damageTaken);
  if (DEBUG) {
    Serial.println("DMG: " + String(damageTaken));
  }
}
