bool turnToMission(float totalTime, float error) {
  // assume already turned in the general direction of the mission.

  float currentDist = getDistance();
  float startTime = millis();
  float dist = 0;

  // scan left
  // while (abs(currentDist - dist) < error && (millis() - startTime) < totalTime/2)  {     // detect within time limit
  //   move(pi, 0.5, 0);
  //   dist = getDistance();
  //   if (abs(currentDist - dist) > error) {    // if catches edge
  //     Enes100.println("Found while detecting left");
  //     moveWithTime(0, 0, 0.5, 500);           // turn back to face
  //     return true;                            // and exit function
  //   } 
  // }                                           // if fails to catch edge, then it was already turned past to begin with
  // moveWithTime(0, 0.5, 0, totalTime/2);       // reset to where it started

  // startTime = millis();

  // // scan right
  // while ((millis() - startTime) < totalTime/2)  {     // detect within time limit
  //   move(0, 0.5, 0);
  //   dist = getDistance();
  //   if (abs(currentDist - dist) > error) {    // if catches edge
  //     Enes100.println("Found while detecting right");
  //     moveWithTime(pi, 0.5, 0, 500);          // turn back to face
  //     return true;                            // and exit function
  //   } 
  // }                                           // if fails to catch edge, then it was already turned past to begin with

  moveWithTime(pi, 0.5, 0, totalTime/2);          // move to the left
  while (abs(currentDist - dist) < error)  {      // until catches edge, move to the right
    move(0, 0.5, 0);
    dist = getDistance();
  }        
  Enes100.println("Found while detecting left");
  moveWithTime(0, 0.5, 0, 500);           // move to face
}