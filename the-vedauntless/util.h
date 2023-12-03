float norm(float xI, float yI, float xF, float yF) {
  return float(sqrt(pow((xF - xI), 2) + pow((yF - yI), 2)));
}