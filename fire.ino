void handleFire() {
  if (fire.cooldown > 0) {
    fire.cooldown--;
  }
  
  if (digitalRead(PIN_BUTTON) == LOW && fire.cooldown == 0) {
    fire.cooldown = FIRE_COOLDOWN;
    shoot();
  }
}

void shoot() {
  if (pallet.position % 2 == 0) {
    startFireSound();
    
    fire.x = pallet.position / 2;
    fire.visible = FIRE_STEP_DELAY;

    for (byte i = 0; i < MAXIMUM_BALLS; ++i) {
      if (balls[i].x == fire.x) {
        score++;
        disableBall(i);
        startHitSound();
      }
    }
  } else {
    playMissSound();
  }
}

void drawFire() {
  if (fire.visible > 0) {
    engineDraw(fireSprite, fire.x, BOTTOM_ROW);
    engineDraw(fireSprite, fire.x, TOP_ROW);
    fire.visible--;
  }
}
