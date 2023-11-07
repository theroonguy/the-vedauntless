//Connecting two outputs for each motor (A & B)
int MLA = 2; //Left pin-
int MLB = 3; //pin-
int MRA = 4; //Right pin-
int MRB = 5; //pin- 
int power = 

//Setting the pin modes to output
void setupMotion(){
  pinMode(MLA, OUTPUT);
  pinMode(MLB, OUTPUT);
  pinMode(MRA, OUTPUT);
  pinMode(MRB, OUTPUT);
}

//moving forward at a set speed (0-255)
//all wheels spin backward
void goFoward(){
  digitalWrite(MLA, HIGH);
  digitalWrite(MLB, LOW);
  digitalWrite(MRA, HIGH);
  digitalWrite(MRB, LOW);
}

//moving backward at a set speed
//all wheels spin backwards
void goBack(){
  digitalWrite(MLA, LOW);
  digitalWrite(MLB, HIGH);
  digitalWrite(MRA, LOW);
  digitalWrite(MRB, HIGH);
}


//set left motors back and right motors foward
//we should add that it spins until facing a certain direction-- using aruco marker
void turnLeft(){
  digitalWrite(MLA, LOW);
  digitalWrite(MLB, HIGH);
  digitalWrite(MRA, HIGH);
  digitalWrite(MRB, LOW);
}

//set left motors foward and left motors back
//spinning right
void turnRight(){
  digitalWrite(MLA, HIGH);
  digitalWrite(MLB, LOW);
  digitalWrite(MRA, LOW);
  digitalWrite(MRB, HIGH);
}

//Set all motors to stop
void stopMotion(){
  digitalWrite(MLA, HIGH);
  digitalWrite(MLB, HIGH);
  digitalWrite(MRA, HIGH);
  digitalWrite(MRB, HIGH);
}

void MS5(){
  goFoward();
  delay(3000);
  turnRight();

}
void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // goes foward for 1 sec

}
