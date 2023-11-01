//Motor A
int in1 = 2;
int in2 = 3;
//Motor B
int in3 = 4;
int in4 = 5;

//EN pins - Must be connected by PMW pins on Arduino
//Motor A
int enA = 9;
//Motor B
int enB = 10;

//Electromagnet pin
int sig = 13;

void setup() {
	pinMode(enA, OUTPUT);
	pinMode(enB, OUTPUT);
	pinMode(in1, OUTPUT);
	pinMode(in2, OUTPUT);
	pinMode(in3, OUTPUT);
	pinMode(in4, OUTPUT);

  // electromagnet pin
  pinMode(sig, OUTPUT);
	
  //Turn motors off and electromagnet on to start
	digitalWrite(in1, LOW);
	digitalWrite(in2, LOW);
	digitalWrite(in3, LOW);
	digitalWrite(in4, LOW);
  digitalWrite(sig, HIGH);
}

void loop() {
  //ENA/B pins, adjusted to correct for drift
	analogWrite(enA, 252);
	analogWrite(enB, 255);

  // Rotate clockwise 90 degrees
	digitalWrite(in1, HIGH);
	digitalWrite(in2, LOW);
	digitalWrite(in3, LOW);
	digitalWrite(in4, HIGH);
	delay(1875);
  
  // Pause
	digitalWrite(in1, LOW);
	digitalWrite(in2, LOW);
	digitalWrite(in3, LOW);
	digitalWrite(in4, LOW);
  delay(500);

  //Rotate counterclockwise 90 degrees
  digitalWrite(in1, LOW);
	digitalWrite(in2, HIGH);
	digitalWrite(in3, HIGH);
	digitalWrite(in4, LOW);
  delay(1875);

  //Pause
  digitalWrite(in1, LOW);
	digitalWrite(in2, LOW);
	digitalWrite(in3, LOW);
	digitalWrite(in4, LOW);
  delay(500);

  //Rotate counterclockwise 90 degrees
  digitalWrite(in1, LOW);
	digitalWrite(in2, HIGH);
	digitalWrite(in3, HIGH);
	digitalWrite(in4, LOW);
  delay(1875);

  //Stop
  digitalWrite(in1, LOW);
	digitalWrite(in2, LOW);
	digitalWrite(in3, LOW);
	digitalWrite(in4, LOW);
  digitalWrite(sig, LOW);
  delay(100000);
}