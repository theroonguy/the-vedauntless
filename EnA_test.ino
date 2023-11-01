// Motor A connections
int enA = 9;
int in1 = 2;
int in2 = 3;
// Motor B connections
int enB = 10;
int in3 = 4;
int in4 = 5;

void setup() {
	// Set all the motor control pins to outputs
	pinMode(enA, OUTPUT);
	pinMode(enB, OUTPUT);
	pinMode(in1, OUTPUT);
	pinMode(in2, OUTPUT);
	pinMode(in3, OUTPUT);
	pinMode(in4, OUTPUT);
	
	// Turn off motors - Initial state
	digitalWrite(in1, LOW);
	digitalWrite(in2, LOW);
	digitalWrite(in3, LOW);
	digitalWrite(in4, LOW);
}

void loop() {
  goStraight();
  delay(500);
  // Set motors to maximum speed
	// For PWM maximum possible values are 0 to 255
	analogWrite(enA, 252);
	analogWrite(enB, 255);

	// Turn on motor A & B
	digitalWrite(in1, HIGH);
	digitalWrite(in2, LOW);
	digitalWrite(in3, HIGH);
	digitalWrite(in4, LOW);
	delay(2000);
}

void goStraight() {
    // Set motors to maximum speed
	// For PWM maximum possible values are 0 to 255
	analogWrite(enA, 252);
	analogWrite(enB, 255);

	// Turn on motor A & B
	digitalWrite(in1, HIGH);
	digitalWrite(in2, LOW);
	digitalWrite(in3, HIGH);
	digitalWrite(in4, LOW);
	delay(2000);
}