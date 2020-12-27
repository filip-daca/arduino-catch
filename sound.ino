void playSounds() {
  playFireSound();
}

void playStartSound() {
  tone(PIN_BUZZ, 1000);
  delay(100);
  noTone(PIN_BUZZ);
}

void playCatchSound() {
  tone(PIN_BUZZ, 1500);
}

void playMissSound() {
  tone(PIN_BUZZ, 500);
}

void startFireSound() {
  fire.sound = 6;
}

void startHitSound() {
  fire.soundHit = true;
}

void playFireSound() {
  if (fire.sound > 4) {
    tone(PIN_BUZZ, 2500);
  } else if (fire.sound > 2) {
    if (fire.soundHit) {
      tone(PIN_BUZZ, 400);  
    } else {
      tone(PIN_BUZZ, 2000);
    }
  } else if (fire.sound > 1) {
    noTone(PIN_BUZZ);
    fire.soundHit = false;
  }

  if (fire.sound > 0) {
    fire.sound--; 
  }
}
