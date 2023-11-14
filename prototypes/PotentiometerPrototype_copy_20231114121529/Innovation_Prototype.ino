#include <servo.h>


//read value from iput pin-- conected to output pin 
analogValue = analogRead (A0); //connected to pin A0

Servo myservo; //servo controlling myservo as an object

int servopot = 1; //servo potentiometer pin connected to analog pin  1
int val; //variable to read the value from analog pin

void setupPotentiometer(){
  //initialize serial communication at 9600 bits per second
  serial.begin(9600);
  myservo.attach(9); //attaches the servo on pin 9 to servo object
}

void loop() {
  //the servo motor
  val = analogRead(servopot); //reads servo pot value (value between 0 -1023)
  val = map(val, 0, 1023, 0, 180); // scale it to be in degrees (0-180)
  myservo.write(val); //sets servo position according to scaled value
  delay(15); //waits for servo to get there <- seems extremely short

  
  //separate potentiometer code
  //read the input of analog pin 0
  int sensorValue = analogRead(A0);
  //print out the value you read in the Serial Monitor:
  serial.println(sensorValue);
  delay(500); //delay in between reads for stability
}


