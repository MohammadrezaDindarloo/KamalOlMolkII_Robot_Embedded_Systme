void penlift_movePen(int start, int end, int delay_ms) {
  penHeight.attach(PEN_HEIGHT_SERVO_PIN);
  if (start < end) {
    for (int i = start; i <= end; i++) {
      penHeight.write(i);
      delay(delay_ms);
    }
  } else {
    for (int i = start; i >= end; i--) {
      penHeight.write(i);
      delay(delay_ms);
    }
  }
  penHeight.detach();
}

void penlift_penUp() {
  if (isPenUp == false)
    penlift_movePen(downPosition, upPosition, penLiftSpeed);
  isPenUp = true;
}

void penlift_penDown() {
  if (isPenUp == true)
    penlift_movePen(upPosition, downPosition, penLiftSpeed);
  isPenUp = false;
}