boolean v_interrupt = false;
char chartemp;
char chartempv;

void readFile(fs::FS &fs, const char *path) {
  Serial.printf("Reading file: %s\n", path);

  File readFile = fs.open(path);
  if (!readFile) {
    Serial.println("Failed to open file for reading");
    return;
  }
  Serial.print("Read from file: \n");
  String command = "";
  while (readFile.available()) {
    if (client.available()) {
      chartemp = (char)client.read();
      Serial.println(chartemp);
//      if (chartemp == '&') {
//        b_interrup();
//      }
      if (chartemp == '+') {
        Serial1.write('?');
      }
    }
    char ch = readFile.read();
    if (ch == INTERMINATOR || ch == SEMICOLON) {
      command.trim();
      Serial.println(command);
      command.toCharArray(lastCommand, INLENGTH + 1);
      boolean commandParsed = comms_parseCommand(lastCommand);
      if (commandParsed) {
        Serial.print(F("Executing command:"));
        Serial.println(lastCommand);
        exec_executeBasicCommand(command);
      }
      command = "";
    } else
      command += ch;
  }
  readFile.close();
}

void impl_exec_execFromStore(String inFilename) {
  Serial.println("Enter first Loop, try to open file");
  readFile(SD, "/Test.txt");
}

void impl_engageMotors() {
  motorA.enableOutputs();
  motorB.enableOutputs();
  powerIsOn = true;
  motorA.runToNewPosition(motorA.currentPosition() + 4);
  motorB.runToNewPosition(motorB.currentPosition() + 4);
  motorA.runToNewPosition(motorA.currentPosition() - 4);
  motorB.runToNewPosition(motorB.currentPosition() - 4);
}

void impl_releaseMotors() {
  motorA.disableOutputs();
  motorB.disableOutputs();
  penlift_penUp();
  powerIsOn = false;
}
