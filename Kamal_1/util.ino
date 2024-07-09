void changeLength(long tAl, long tBl) {
  float tA = float(tAl);
  float tB = float(tBl);

  float currSpeedA = motorA.speed();
  float currSpeedB = motorB.speed();

  motorA.setSpeed(0.0);
  motorB.setSpeed(0.0);
  motorA.moveTo(tA);
  motorB.moveTo(tB);

  if (motorA.speed() < 0)
    currSpeedA = -currSpeedA;
  if (motorB.speed() < 0)
    currSpeedB = -currSpeedB;

  motorA.setSpeed(currSpeedA);
  motorB.setSpeed(currSpeedB);

  while (motorA.distanceToGo() != 0 || motorB.distanceToGo() != 0) {
    if (currentlyRunning) {
      motorA.runSpeedToPosition();
      motorB.runSpeedToPosition();
    }
  }
}

long getMaxLength() {
  if (maxLength == 0) {
    maxLength = long(getMachineA(pageWidth, pageHeight) + 0.5);
    Serial.print(F("maxLength: "));
    Serial.println(maxLength);
  }
  return maxLength;
}

float getMachineA(float cX, float cY) {
  float a = sqrt(sq(cX) + sq(cY));
  return a;
}
float getMachineB(float cX, float cY) {
  float b = sqrt(sq((pageWidth)-cX) + sq(cY));
  return b;
}

float getCartesianXFP(float aPos, float bPos) {
  float calcX = (sq((float)pageWidth) - sq((float)bPos) + sq((float)aPos)) / ((float)pageWidth * 2.0);
  return calcX;
}
float getCartesianYFP(float cX, float aPos) {
  float calcY = sqrt(sq(aPos) - sq(cX));
  return calcY;
}

long getCartesianX(float aPos, float bPos) {
  long calcX = long((pow(pageWidth, 2) - pow(bPos, 2) + pow(aPos, 2)) / (pageWidth * 2));
  return calcX;
}

long getCartesianX() {
  long calcX = getCartesianX(motorA.currentPosition(), motorB.currentPosition());
  return calcX;
}

long getCartesianY() {
  return getCartesianY(getCartesianX(), motorA.currentPosition());
}

long getCartesianY(long cX, float aPos) {
  long calcY = long(sqrt(pow(aPos, 2) - pow(cX, 2)));
  return calcY;
}