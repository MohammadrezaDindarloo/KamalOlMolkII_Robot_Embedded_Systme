void sd_initSD() {
  currentlyDrawingFilename = "";
  cardPresent = false;
  cardInit = false;
  commandFilename = "";
  sd_simpleInit();
}

void sd_simpleInit() {
  pinMode(chipSelect, OUTPUT);  // necessary for SD card reading to work
  // see if the card is present and can be initialized:
  int initValue = 0;
  initValue = SD.begin(chipSelect);
  if (initValue == 0) Serial.println("Card failed, or not present");
  else {
    Serial.println("Successfully beginned.");
    cardPresent = true;
  }

  if (cardPresent) {
    Serial.println("card initialized.");
    root = SD.open("/", FILE_READ);
    cardInit = true;
    Serial.println("done!");
  }

  if (SD.remove("/Test.txt")) {
    Serial.println("File deleted");
  } else {
    Serial.println("Delete failed");
  }
}