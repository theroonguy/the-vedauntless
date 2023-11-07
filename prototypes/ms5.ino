//output A
int in1 = 2;
int in2 = 3;
//output B
int in3 = 4;
int in4 = 5;

//en pins - Must be connected by PMW pins on Arduino
//output A
int ena = 9;
//output B
int enb = 10;

//electromagnet pin
int sig = 13;

//calibration
int msPerMeter = 2000; //ms per meter of linear movement
int msPer90Degrees = 1875; //ms per 90 degrees of rotation

void setup() {
  //motor driver pins
	pinMode(ena, OUTPUT);
	pinMode(enb, OUTPUT);
	pinMode(in1, OUTPUT);
	pinMode(in2, OUTPUT);
	pinMode(in3, OUTPUT);
	pinMode(in4, OUTPUT);

  // electromagnet pin
  pinMode(sig, OUTPUT);
	
  //turn motors off and electromagnet on to start
	digitalWrite(in1, LOW);
	digitalWrite(in2, LOW);
	digitalWrite(in3, LOW);
	digitalWrite(in4, LOW);
  digitalWrite(sig, HIGH);
}


void loop() {
  //ena/b pins, adjusted to correct for drift
	analogWrite(ena, 252);
	analogWrite(enb, 255);

  //driver forward 3 meters
  goForward(3); //meters

  // rotate one direction, then the other
  rotateClockwise(90); //degrees
  rotateCounterclockwise(90); //degrees
  rotateCounterclockwise(90); //degrees

  //stop and turn off electromagnet
  endRun();
  return;
}

void rotateClockwise(int angle) {
  // takes input angle and figures out time to reach it
  int rotateTime = int(angle/90*msPer90Degrees);
  digitalWrite(in1, HIGH);
	digitalWrite(in2, LOW);
	digitalWrite(in3, LOW);
	digitalWrite(in4, HIGH);
  delay(rotateTime);
  //pause after movement
  delay(500);
}

void rotateCounterclockwise(int angle) {
  // takes input angle and figures out time to reach it
  int rotateTime = int(angle/90*msPer90Degrees);
  digitalWrite(in1, LOW);
	digitalWrite(in2, HIGH);
	digitalWrite(in3, HIGH);
	digitalWrite(in4, LOW);
  delay(rotateTime);
  //pause after movement
  delay(500);
}

void goForward(int distance) {
  //takes input distance and figures out movement time
  int movementTime = int(distance*msPerMeter);
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
  delay(movementTime);

  //pause for half a second after movement
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
  delay(500);
}

void goBackward(int distance) {
  //takes input distance and figures out movement time
  int movementTime = int(distance*msPerMeter);
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
  delay(movementTime);

    //pause for half a second after movement
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
  delay(500);
}

void endRun() {
  //turns off motors and electromagnet
  digitalWrite(in1, LOW);
	digitalWrite(in2, LOW);
	digitalWrite(in3, LOW);
	digitalWrite(in4, LOW);
  digitalWrite(sig, LOW);
}
