#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiAP.h>
#include <SPI.h>
#include <SD.h>
#include <FS.h>
#include <AccelStepper.h>
#include <ESP32Servo.h>

// Pen raising servo
Servo penHeight;
const int DEFAULT_DOWN_POSITION =90;
const int DEFAULT_UP_POSITION = 60;
static int upPosition = DEFAULT_UP_POSITION;  // defaults
static int downPosition = DEFAULT_DOWN_POSITION;
static int penLiftSpeed = 9;           // ms between steps of moving motor
const byte PEN_HEIGHT_SERVO_PIN = 27;  //UNL2003 driver uses pin 9
boolean isPenUp = false;

// Machine specification defaults
const int DEFAULT_MACHINE_WIDTH = 710;
const int DEFAULT_MACHINE_HEIGHT = 950;
const int DEFAULT_MM_PER_REV = 40;
const int DEFAULT_STEPS_PER_REV = 400;
const int DEFAULT_STEP_MULTIPLIER = 1;

// working machine specification
static int motorStepsPerRev = DEFAULT_STEPS_PER_REV;
static float mmPerRev = DEFAULT_MM_PER_REV;
static byte stepMultiplier = DEFAULT_STEP_MULTIPLIER;
static int machineWidth = DEFAULT_MACHINE_WIDTH;
static int machineHeight = DEFAULT_MACHINE_HEIGHT;


static float currentMaxSpeed = 950.0;
static float currentAcceleration = 385.0;
static boolean usingAcceleration = true;

int startLengthMM = 800;

float mmPerStep = 0.0F;
float stepsPerMM = 0.0F;

long pageWidth = machineWidth * stepsPerMM;
long pageHeight = machineHeight * stepsPerMM;
long maxLength = 0;

//static char rowAxis = 'A';
const int INLENGTH = 50;
const char INTERMINATOR = 10;
const char SEMICOLON = ';';

boolean acceleration = true;

extern AccelStepper motorA;
extern AccelStepper motorB;

boolean currentlyRunning = true;

static char inCmd[10];
static char inParam1[14];
static char inParam2[14];
static char inParam3[14];
static char inParam4[14];

static byte inNoOfParams;

char lastCommand[INLENGTH + 1];
boolean commandConfirmed = false;

int rebroadcastReadyInterval = 5000;
long lastOperationTime = 0L;
long motorIdleTimeBeforePowerDown = 600000L;
boolean automaticPowerDown = true;
boolean powerIsOn = false;

long lastInteractionTime = 0L;

const static char COMMA[] = ",";
const static char CMD_END[] = ",END";
const static String CMD_SETPOSITION = "C09";
const static String CMD_PENDOWN = "C13";
const static String CMD_PENUP = "C14";
const static String CMD_CHANGELENGTHDIRECT = "C17";

const int chipSelect = 5;
boolean sdCardInit = false;

String commandFilename = "";

// set up variables using the SD utility library functions:
File root;
boolean cardPresent = false;
boolean cardInit = false;
boolean echoingStoredCommands = false;

String currentlyDrawingFilename = "";

String Name_File = "Test.txt";

const char* ssid = "Kamal-ol-Molk I ";
const char* password = "ARAS2022";
WiFiServer server(8888);
WiFiClient client;

void setup() {
  Serial.begin(57600);
  Serial.println("Kamal-Ol-Molk Robot ... ");

  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid, password);  //Connect to wifi

  IPAddress ip = WiFi.softAPIP();
  server.begin();
  Serial.println();
  Serial.println("Server started.");
  Serial.print("IP: ");
  Serial.println(ip);
  Serial.print("PORT: ");
  Serial.println(8888);

  configuration_motorSetup();
  configuration_setup();

  motorA.setMaxSpeed(currentMaxSpeed);
  motorA.setAcceleration(currentAcceleration);
  motorB.setMaxSpeed(currentMaxSpeed);
  motorB.setAcceleration(currentAcceleration);

  float startLength = ((float)startLengthMM / (float)mmPerRev) * (float)motorStepsPerRev;
  motorA.setCurrentPosition(startLength);
  motorB.setCurrentPosition(startLength);
  for (int i = 0; i < INLENGTH; i++) lastCommand[i] = 0;
  sd_initSD();

  penlift_penUp();
  delay(500);
}

void loop() {
  client = server.available();
  if (client) {
    Serial.println("Client connected");
    while (client.connected() > 0) {
      root = SD.open("/Test.txt", FILE_APPEND);
      while (client.available() > 0) {
        char c = client.read();
        switch (c) {

          case 'q':
            Serial.println("Start");
            break;

          case '+':
            Serial.println("writing data");
            break;

          case '#':
            //Serial.println("homing");
            exec_setPosition();
            break;

          case '!':
            penlift_penUp();
            //            Serial.println("pen lift");
            break;

          case '^':
            penlift_penDown();
            //            Serial.println("pen down");
            break;

          case '$':
            impl_exec_execFromStore(Name_File);
            break;

          case '-':
            for (int i = 0; i < 10; i++) {
              client.write("F\n");
            }
            break;

          default:
            Serial.write(c);
            root.print(c);
            break;
        }
      }
      root.close();
    }
    client.stop();
    Serial.println("client disconnected");
  }
}
