byte introDelay;
byte palletDelay;

void initializeIntro() {
  introDelay = 20;
  palletDelay = 3;

  initializePallet();
  initializeBalls();
  
  mode = MODE_INTRO;
}

void loopIntro() {
  palletDelay--;
  if (palletDelay == 0) {
    palletDelay = 3;
    pallet.position = (pallet.position + 1) % PALLET_POSITIONS;
  }

  loopBalls();

  engineClear(lcd);
  drawBalls();
  drawPallet();
  engineFlush(lcd);

  lcd.setCursor(3, TOP_ROW);
  lcd.print("STRZELANKO");
  
  checkStart();

  if (introDelay > 0) {
    introDelay--;
  }
  
  delay(GAME_DELAY);
}


void checkStart() {
  if (digitalRead(PIN_BUTTON) == LOW && introDelay == 0) {
    initializeGameplay();
  }
}
