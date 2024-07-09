void exec_executeBasicCommand(String &com) {
  if (com.startsWith(CMD_CHANGELENGTHDIRECT))
    exec_changeLengthDirect();
  else if (com.startsWith(CMD_SETPOSITION))
    exec_setPosition();
  else if (com.startsWith(CMD_PENDOWN))
    penlift_penDown();
  else if (com.startsWith(CMD_PENUP))
    penlift_penUp();
  else
    Serial.println("No !!!");
}

void exec_setPosition() {
  long targetA = atol(inParam1);
  long targetB = atol(inParam2);
  motorA.setCurrentPosition(targetA);
  motorB.setCurrentPosition(targetB);

  impl_engageMotors();
}

void exec_changeLengthDirect() {
  float endA = (float)atof(inParam1);
  float endB = (float)atof(inParam2);
  int maxSegmentLength = atoi(inParam3);

  float startA = motorA.currentPosition();
  float startB = motorB.currentPosition();

  if (endA < 20 || endB < 20 || endA > getMaxLength() || endB > getMaxLength()) {
    Serial.println(F("This point."));
  } else {
    exec_drawBetweenPoints(startA, startB, endA, endB, maxSegmentLength);
  }
}

void exec_drawBetweenPoints(float p1a, float p1b, float p2a, float p2b, int maxSegmentLength) {
  // ok, we're going to plot some dots between p1 and p2.  Using maths. I know! Brave new world etc.

  // First, convert these values to cartesian coordinates
  // We're going to figure out how many segments the line
  // needs chopping into.
  float c1x = getCartesianXFP(p1a, p1b);
  float c1y = getCartesianYFP(c1x, p1a);

  float c2x = getCartesianXFP(p2a, p2b);
  float c2y = getCartesianYFP(c2x, p2a);

  // test to see if it's on the page
  // AND ALSO TO see if the current position is on the page.
  // Remember, the native system can easily specify points that can't exist,
  // particularly up at the top.
  if (c2x > 20
      && c2x < pageWidth - 20
      && c2y > 20
      && c2y < pageHeight - 20
      && c1x > 20
      && c1x < pageWidth - 20
      && c1y > 20
      && c1y < pageHeight - 20) {
    float deltaX = c2x - c1x;  // distance each must move (signed)
    float deltaY = c2y - c1y;

    int linesegs = 1;
    if (abs(deltaX) > abs(deltaY)) {
      // slope <=1 case
      while ((abs(deltaX) / linesegs) > maxSegmentLength) {
        linesegs++;
      }
    } else {
      // slope >1 case
      while ((abs(deltaY) / linesegs) > maxSegmentLength) {
        linesegs++;
      }
    }

    // reduce delta to one line segments' worth.
    deltaX = deltaX / linesegs;
    deltaY = deltaY / linesegs;

    // render the line in N shorter segments
    long runSpeed = 0;

    usingAcceleration = false;
    while (linesegs > 0) {
      //      Serial.print("Line segment: " );
      //      Serial.println(linesegs);
      // compute next new location
      c1x = c1x + deltaX;
      c1y = c1y + deltaY;

      // convert back to machine space
      float pA = getMachineA(c1x, c1y);
      float pB = getMachineB(c1x, c1y);

      // do the move
      runSpeed = desiredSpeed(linesegs, runSpeed, currentAcceleration * 4);

      Serial.print("Setting speed:");
      Serial.println(runSpeed);

      motorA.setSpeed(runSpeed);
      motorB.setSpeed(runSpeed);
      changeLength(pA, pB);

      // one line less to do!
      linesegs--;
    }
  } else {
    Serial.println(F("Line is not on the page. Skipping it."));
  }
}

float desiredSpeed(long distanceTo, float currentSpeed, float acceleration) {
  float requiredSpeed;

  if (distanceTo == 0)
    return 0.0f;  // We're there

  // sqrSpeed is the signed square of currentSpeed.
  float sqrSpeed = sq(currentSpeed);
  if (currentSpeed < 0.0)
    sqrSpeed = -sqrSpeed;

  float twoa = 2.0f * acceleration;  // 2ag

  // if v^^2/2as is the the left of target, we will arrive at 0 speed too far -ve, need to accelerate clockwise
  if ((sqrSpeed / twoa) < distanceTo) {
    // Accelerate clockwise
    // Need to accelerate in clockwise direction
    if (currentSpeed == 0.0f)
      requiredSpeed = sqrt(twoa);
    else
      requiredSpeed = currentSpeed + fabs(acceleration / currentSpeed);

    if (requiredSpeed > currentMaxSpeed)
      requiredSpeed = currentMaxSpeed;
  } else {
    // Decelerate clockwise, accelerate anticlockwise
    // Need to accelerate in clockwise direction
    if (currentSpeed == 0.0f)
      requiredSpeed = -sqrt(twoa);
    else
      requiredSpeed = currentSpeed - fabs(acceleration / currentSpeed);
    if (requiredSpeed < -currentMaxSpeed)
      requiredSpeed = -currentMaxSpeed;
  }

  //Serial.println(requiredSpeed);
  return requiredSpeed;
}