byte fireSound;
byte hitSound;
byte fallSound;

void ringSound() {
  ringFireSound();
  ringHitSound();
  ringFallSound();
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
  fireSound = 5;
}

void startHitSound() {
  hitSound = 7;
}

void startFallSound() {
  fallSound = 4;
}

void ringFireSound() {
  if (fireSound > 4) {
    tone(PIN_BUZZ, 2500);
  } else if (fireSound > 3) {
    tone(PIN_BUZZ, 2300);
  } else if (fireSound > 1) {
    tone(PIN_BUZZ, 2120);
  }

  if (fireSound > 0) {
    fireSound--;
  }
}

void ringHitSound() {
  if (hitSound > 6) {
    tone(PIN_BUZZ, 2500);
  } else if (hitSound > 5) {
    tone(PIN_BUZZ, 2300);
  } else if (hitSound > 3) {
    tone(PIN_BUZZ, 1120);
  } else if (hitSound > 1) {
    tone(PIN_BUZZ, 1300);
  }

  if (hitSound > 0) {
    hitSound--;
  }
}

void ringFallSound() {
  if (fallSound > 2) {
    tone(PIN_BUZZ, 700);
  } else if (fallSound > 1) {
    tone(PIN_BUZZ, 300);
  }

  if (fallSound > 0) {
    fallSound--;
  }
}

void muteSound() {
  if (fireSound + hitSound + fallSound == 0) {
    noTone(PIN_BUZZ);
  }
}
