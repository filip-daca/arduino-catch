void initializePallet() {
}

void loopPallet() {
  pallet.position = readPalletPosition(); 
}

byte readPalletPosition() {
  return PALLET_POSITIONS - analogRead(A0) * PALLET_POSITIONS / MAX_ANALOG_READ;
}

void drawPallet() {
  if (pallet.position % 2 == 0) {
    engineDraw(palletSprites[1], pallet.position / 2, BOTTOM_ROW);
  } else {
    engineDraw(palletSprites[0], pallet.position / 2, BOTTOM_ROW);
    engineDraw(palletSprites[2], pallet.position / 2 + 1, BOTTOM_ROW);
  }
}
