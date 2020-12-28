byte scoreDelay;

void initializeScoreboard() {
  scoreDelay = 50;
  mode = MODE_SCORE;
}

void loopScoreboard() {
  loopBalls();

  engineClear(lcd);
  drawBalls();
  engineFlush(lcd);

  drawScore();
  checkForRestart();

  ringSound();
  delay(GAME_DELAY);
  muteSound();
}

void drawScore() {
  if (scoreDelay > 0) {
    scoreDelay--;
  } else {
    lcd.setCursor(3, TOP_ROW);
    lcd.print("GAME OVER");
    lcd.setCursor(8, BOTTOM_ROW);
    lcd.print(String(score));
  }
}

void checkForRestart() {
  if (digitalRead(PIN_BUTTON) == LOW && scoreDelay == 0) {
    initializeIntro();
  }
}
