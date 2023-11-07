
//Connecting two outputs for each motor (A & B)
int M1A = 30; //Left Front pin-
int M1B = 31; //pin-
int M2A = 32; //Right Front pin- 
int M2B = 33; //pin-
int M3A = 34; //Left Back pin-
int M3B = 35; //pin- 
int M4A = 36; //Right Back pin-
int M4B = 37; //pin-

int SPD = A0; //pin-

//Setting the pin modes to output
void setupMotion(){
  pinMode(M1A, OUTPUT);
  pinMode(M1B, OUTPUT);
  pinMode(M2A, OUTPUT);
  pinMode(M2B, OUTPUT);
  pinMode(M3A, OUTPUT);
  pinMode(M3B, OUTPUT);
  pinMode(M4A, OUTPUT);
  pinMode(M4B, OUTPUT);
  pinMode(SPD, OUTPUT);
}

//moving forward at a set speed (0-255)
//all wheels spin backward
void goFoward(int speed){
  digitalWrite(M1A, HIGH);
  digitalWrite(M1B, LOW);
  digitalWrite(M2A, HIGH);
  digitalWrite(M2B, LOW);
  digitalWrite(M3A, HIGH);
  digitalWrite(M3B, LOW);
  digitalWrite(M4A, HIGH);
  digitalWrite(M4B, LOW);
  analogWrite(SPD, speed);
}

//moving backward at a set speed
//all wheels spin backwards
void goBack(int speed){
  digitalWrite(M1A, LOW);
  digitalWrite(M1B, HIGH);
  digitalWrite(M2A, LOW);
  digitalWrite(M2B, HIGH);
  digitalWrite(M3A, LOW);
  digitalWrite(M3B, HIGH);
  digitalWrite(M4A, LOW);
  digitalWrite(M4B, HIGH);
  analogWrite(SPD, speed);
}

//directly left at speed 0-255
//LF & RB spin backwards, RF & LB spin forward
void strafeLeft(int speed){
  digitalWrite(M1A, LOW);
  digitalWrite(M1B, HIGH);
  digitalWrite(M2A, HIGH);
  digitalWrite(M2B, LOW);
  digitalWrite(M3A, LOW);
  digitalWrite(M3B, HIGH);
  digitalWrite(M4A, HIGH);
  digitalWrite(M4B, LOW);
  analogWrite(SPD, speed);
}

//moving directly right at speed 0-255
//FL & BR spin forward, FR & BL spin backwards
void strafeRight(int speed){
  digitalWrite(M1A, HIGH);
  digitalWrite(M1B, LOW);
  digitalWrite(M2A, LOW);
  digitalWrite(M2B, HIGH);
  digitalWrite(M3A, HIGH);
  digitalWrite(M3B, LOW);
  digitalWrite(M4A, LOW);
  digitalWrite(M4B, HIGH);
  analogWrite(SPD, speed);
}

//set left motors back and right motors foward
//we should add that it spins until facing a certain direction-- using aruco marker
void turnLeft(){
  digitalWrite(M1A, LOW);
  digitalWrite(M1B, HIGH);
  digitalWrite(M2A, HIGH);
  digitalWrite(M2B, LOW);
  digitalWrite(M3A, LOW);
  digitalWrite(M3B, HIGH);
  digitalWrite(M4A, HIGH);
  digitalWrite(M4B, LOW);
}

//set left motors foward and left motors back
//spinning right
void turnRight(){
  digitalWrite(M1A, HIGH);
  digitalWrite(M1B, LOW);
  digitalWrite(M2A, LOW);
  digitalWrite(M2B, HIGH);
  digitalWrite(M3A, HIGH);
  digitalWrite(M3B, LOW);
  digitalWrite(M4A, LOW);
  digitalWrite(M4B, HIGH);
}

//Set all motors to stop
void stopMotion(){
  digitalWrite(M1A, HIGH);
  digitalWrite(M1B, HIGH);
  digitalWrite(M2A, HIGH);
  digitalWrite(M2B, HIGH);
  digitalWrite(M3A, HIGH);
  digitalWrite(M3B, HIGH);
  digitalWrite(M4A, HIGH);
  digitalWrite(M4B, HIGH);
}


void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // goes foward for 1 sec
goFoward(255);
delay(1000);
stopMotion();
}
