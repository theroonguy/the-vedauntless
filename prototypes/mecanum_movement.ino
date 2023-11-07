#include <Adafruit_MotorShield.h>

#include <AFMotor.h>

AF_DCMotor motor1 (1); //Front left wheel
AF_DCMOtor motor2 (2); //Front right wheel
AF_DCMOtor motor3 (3); //Back left wheel
AF_DCMOtor motor4 (4); //Back right wheel


//input: theta, power, and turn
void move(int theta, float power, float turn){
  sin = Math.sin(theta - Math.PI/4);
  cos = Math.cos(theta - Math.PI/4);
  max = Math.max(Math.abs(sin),Math.abs(cos));

  //how do I calculate power and turn??
  motor1 = power * cos/max + turn;
  motor2 = power * sin/max + turn;
  motor3 = power * sin/max + turn;
  motor4 = power * cos/max + turn;

  if ((power + Math.abs(turn)) > 1) {
    motor1 /= power + turn;
    motor2 /= power + turn;
    motor3 /= power + turn;
    motor4 /= power + turn;
  }
  
}

//this method uses values from a joystick, we're going to have to input the y and x values for each theta we want
//figure out if its an int or a float
x = 
y = 
turn = 

theta = Math.atan2(y,x);

void setup() {
  // put your setup code here, to run once:
  pinMode(1, OUTPUT);
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUPUT);
 
}


void loop() {
  // put your main code here, to run repeatedly:


}

