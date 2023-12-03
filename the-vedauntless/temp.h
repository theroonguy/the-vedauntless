bool turnToMission(float totalTime, float error) {
  // assume already turned in the general direction of the mission.

  float currentDist = getDistance();
  float startTime = millis();
  float dist = 0;

  // scan left
  while ((millis() - startTime) < totalTime/2)  {     // detect within time limit
    move(0, 0, -0.5);
    dist = getDistance();
    if (abs(currentDist - dist) > error) {    // if catches edge
      Enes100.println("Found while detecting left");
      moveWithTime(0, 0, 0.5, 500);           // turn back to face
      return true;                            // and exit function
    } 
  }                                           // if fails to catch edge, then it was already turned past to begin with
  moveWithTime(0, 0, 0.5, totalTime/2);       // reset to where it started

  // scan right
  while ((millis() - startTime) < totalTime/2)  {     // detect within time limit
    move(0, 0, 0.5);
    dist = getDistance();
    if (abs(currentDist - dist) > error) {    // if catches edge
      Enes100.println("Found while detecting right");
      moveWithTime(0, 0, -0.5, 500);          // turn back to face
      return true;                            // and exit function
    } 
  }                                           // if fails to catch edge, then it was already turned past to begin with
}