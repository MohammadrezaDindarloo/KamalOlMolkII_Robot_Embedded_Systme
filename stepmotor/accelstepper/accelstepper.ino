// Include the AccelStepper Library
#include <AccelStepper.h>

// Define pin connections
const int dirPin = 21;
const int stepPin = 26;

// Define motor interface type
// #define motorInterfaceType 1

// Creates an instance
AccelStepper myStepper(AccelStepper::DRIVER, stepPin, dirPin);

void setup() {


  // set the maximum speed, acceleration factor,
  // initial speed and the target position
  myStepper.setMaxSpeed(1000);
  myStepper.setAcceleration(400);
  myStepper.setSpeed(1000);
  myStepper.moveTo(1000);
}

void loop() {
  pinMode(32, OUTPUT); // set the pin as output
  digitalWrite(33, LOW);
  pinMode(33, OUTPUT); // set the pin as output
  digitalWrite(32, LOW);

  // Change direction once the motor reaches target position
  if (myStepper.distanceToGo() == 0)
    myStepper.moveTo(-myStepper.currentPosition());

  // Move the motor one step
  myStepper.run();
}
