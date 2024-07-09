// multiple stepper with acceleration and servo
// Include Libraries

#include <AccelStepper.h>
#include <elapsedMillis.h>

//#include <ESP32Servo.h>
// Motor Connections (constant current, step/direction bipolar motor driver)


//stepper1
const int dirPin_m1 = 17;
const int stepPin_m1 = 16;
//stepper2
const int dirPin_m2 = 21;
const int stepPin_m2 = 26;

// Creates an instance -- stepper motor
AccelStepper Stepper1(AccelStepper::DRIVER, stepPin_m1, dirPin_m1); 
AccelStepper Stepper2(AccelStepper::DRIVER, stepPin_m2, dirPin_m2); 
//Creates an instance -- servo motor


// monitoring 
elapsedMillis printTime;

void setup() {
 
  
  Serial.begin(115200);
  //stepper motor
  // set the maximum speed, acceleration factor,
  // and the target position
  Stepper1.setMaxSpeed(1000);
  Stepper1.setAcceleration(400);
  Stepper1.setSpeed(1000);
  Stepper1.moveTo(1000);

  Stepper2.setMaxSpeed(1000);
  Stepper2.setAcceleration(400);
  Stepper2.setSpeed(1000); 
  Stepper2.moveTo(1000);
}

void loop() {
  // Stepper
  pinMode(32, OUTPUT); // set the pin as output
  digitalWrite(33, LOW);
  pinMode(33, OUTPUT); // set the pin as output
  digitalWrite(32, LOW);
  
  if (Stepper1.distanceToGo() == 0)
    Stepper1.moveTo(-Stepper1.currentPosition());
   if (Stepper2.distanceToGo() == 0)
    Stepper2.moveTo(-Stepper2.currentPosition());
    
  Stepper1.run();
  Stepper2.run();
  }
  
