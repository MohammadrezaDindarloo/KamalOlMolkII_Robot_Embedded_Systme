// Define pin connections & motor's steps per revolution
const int dirPin = 26;
const int stepPin = 25;
const int stepsPerRevolution = 500;

void setup()
{
	// Declare pins as Outputs
	pinMode(stepPin, OUTPUT);
	pinMode(dirPin, OUTPUT);
}
void loop()
{
	// Set motor direction clockwise
	digitalWrite(dirPin, HIGH);
int mohammad=2000;
	// Spin motor slowly
	for(int x = 0; x < stepsPerRevolution; x++)
	{
		digitalWrite(stepPin, HIGH);
		delayMicroseconds(mohammad);
		digitalWrite(stepPin, LOW);
		delayMicroseconds(mohammad);
	}
	delay(1000); // Wait a second
	
	// Set motor direction counterclockwise
	digitalWrite(dirPin, LOW);

	// Spin motor quickly
	for(int x = 0; x < stepsPerRevolution; x++)
	{
		digitalWrite(stepPin, HIGH);
		delayMicroseconds(mohammad);
		digitalWrite(stepPin, LOW);
		delayMicroseconds(mohammad);
	}
	delay(1000); // Wait a second
}
