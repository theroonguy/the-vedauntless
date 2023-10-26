include <

AF_DCMotor motor1(1); //Motor 1 connected to pin1
AF_DCMotor motor2(2); //Motor 2 connected to pin2
AF_DCMotor motor3(3); //Motor 3 connected to pin3
AF_DCMotor motor4(4); //Motor 4 connected to pin4

void setup() {
  // put your setup code here, to run once:
  pinMode


  int moverobot(int speedX, int speedY, int rotation){
    //Calculate motor speeds for mecanum drive
  int motor1speed = speedX + speedY + rotation; //Left front wheel
  int motor2speed = speedX + speedY - rotation; //Right front wheel
  int motor3speed = speedX + speedY + rotation; //Left back wheel
  int motor4speed = speedX + speedY - rotation; //Right back wheel
    return (motor1speed, motor2speed, motor3speed, motor4speed)
  }

}


void loop() {
  // put your main code here, to run repeatedly:

s(255, 0 , 0);
delay (10000);

}

int moverobot(int speedX, int speedY, int rotation){
//Calculate motor speeds for mecanum drive
int motor1speed = speedX + speedY + rotation; //Left front wheel
int motor2speed = speedX + speedY - rotation; //Right front wheel
int motor3speed = speedX + speedY + rotation; //Left back wheel
int motor4speed = speedX + speedY - rotation; //Right back wheel


}