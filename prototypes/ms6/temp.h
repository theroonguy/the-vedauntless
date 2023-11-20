float calibrateNormal() {
  float yInitial = Enes100.getY(); //Get initial location
  float xInitial = Enes100.getX();
  void moveWithTime(pi/2, 1, 0, 2000); //Move forward for two seconds
  float xFinal = Enes100.getY(); //Get final location
  float yFfnal = Enes100.getX();
  metersPerSecond = sqrt((xFinal-xInitial)^2+(yFinal-yInitial)^2)/2 //Calculate distance traveled
  void moveWithTime(pi/2, -1, 0, 2000); //Return to initial position
  return metersPerSecond;
}

float calibrateStrafe() {
  float yInitial = Enes100.getY(); //Get initial location
  float xInitial = Enes100.getX();
  void moveWithTime(0, 1, 0, 2000); //Move to the side for two seconds
  float xFinal = Enes100.getY(); //Get final location
  float yFfnal = Enes100.getX();
  metersPerSecond = sqrt((xFinal-xInitial)^2+(yFinal-yInitial)^2)/2 //Calculate distance traveled
  void moveWithTime(0, -1, 0, 2000); //Return to initial position
  return metersPerSecond;
}

float calibrateRotate() {
  float thetaInitial = convertVisionTo2pi(Enes100.getTheta()); //Get initial angle
  void moveWithTime(0, 0, 1, 2000); //Rotate for two seconds
  float thetaFinal = convertVisionTo2pi(Enes100.getTheta()); //Get final angle
  radiansPerSecond = abs(thetaFinal-thetaInitial)/2 //Calculate radians per second
  void moveWithTime(0, 0, -1, 2000); //Return to initial position
  return radiansPerSecond;
}