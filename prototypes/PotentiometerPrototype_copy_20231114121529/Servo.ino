#include <Servo.h>

Servo myservo; //servo controlling myservo as an object

int servopot = 0; //servo potentiometer pin connected to analog pin  0
int val; //variable to read the value from analog pin

void setup(){
  myservo.attach(9); //attaches the servo on pin 9 to servo object
}

void loop() {
  //the servo motor
  val = analogRead(servopot); //reads servo pot value (value between 0 -1023)
  val = map(val, 0, 1023, 0, 180); // scale it to be in degrees (0-180)
  myservo.write(val); //sets servo position according to scaled value
  delay(15); //waits for servo to get there <- seems extremely short


}
