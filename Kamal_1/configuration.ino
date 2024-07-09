#define MOTOR_B_ENABLE_PIN 32
#define MOTOR_B_STEP_PIN 16
#define MOTOR_B_DIR_PIN 17

#define MOTOR_A_ENABLE_PIN 33
#define MOTOR_A_STEP_PIN 26
#define MOTOR_A_DIR_PIN 21

AccelStepper motorA(1, MOTOR_A_STEP_PIN, MOTOR_A_DIR_PIN);
AccelStepper motorB(1, MOTOR_B_STEP_PIN, MOTOR_B_DIR_PIN);

void configuration_motorSetup() {
  pinMode(MOTOR_A_ENABLE_PIN, OUTPUT);
  digitalWrite(MOTOR_A_ENABLE_PIN, HIGH);
  pinMode(MOTOR_B_ENABLE_PIN, OUTPUT);
  digitalWrite(MOTOR_B_ENABLE_PIN, HIGH);
  motorA.setEnablePin(MOTOR_A_ENABLE_PIN);
  motorA.setPinsInverted(false, false, true);
  motorB.setEnablePin(MOTOR_B_ENABLE_PIN);
  motorB.setPinsInverted(true, false, true);
}

void configuration_setup() {
  mmPerStep = mmPerRev / motorStepsPerRev;
  stepsPerMM = motorStepsPerRev / mmPerRev;
  pageWidth = machineWidth * stepsPerMM;
  pageHeight = machineHeight * stepsPerMM;
  delay(500);
}
