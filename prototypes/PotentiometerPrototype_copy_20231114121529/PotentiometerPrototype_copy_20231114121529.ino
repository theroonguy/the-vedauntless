
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  //read the input
  int analogValue = analogRead(A0);
  Serial.println(analogValue);
  delay(1000);
}
