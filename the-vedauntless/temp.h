bool turnToMission(float totalTime, float error) {
  // assume already turned in the general direction of the mission.

  float startTime = millis();

  moveWithTime(pi, 0.5, 0, totalTime);          // move to the left
  while (getDistance() > error) { move(0, 0.5, 0); }
  moveWithTime(0, 0.5, 0, 200);
}