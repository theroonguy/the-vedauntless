//Code adapted from https://howtomechatronics.com/tutorials/arduino/arduino-color-sensing-tutorial-tcs230-tcs3200-color-sensor/

#define S0 45
#define S1 43
#define S2 41
#define S3 39
#define sensorOut 37

int frequency = 0;
int R = 0;
int G = 0;
int B = 0;

void setup() {
  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);
  pinMode(sensorOut, INPUT);
  
  // Setting frequency-scaling to 20%
  digitalWrite(S0,HIGH);
  digitalWrite(S1,LOW);

  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(S2,LOW);
  digitalWrite(S3,LOW);
  // Reading the output frequency
  frequency = pulseIn(sensorOut, LOW);
  // Printing the value on the serial monitor
  R = frequency;
  // Uncomment the code below to see exact values!
  // Serial.print("R= ");//printing name
  // Serial.print(frequency);//printing RED color frequency
  // Serial.print("  ");
  delay(100);

  // Setting Green filtered photodiodes to be read
  digitalWrite(S2,HIGH);
  digitalWrite(S3,HIGH);
  // Reading the output frequency
  frequency = pulseIn(sensorOut, LOW);
  // Printing the value on the serial monitor
  G = frequency;
  // Uncomment the code below to see exact values!
  // Serial.print("G= ");//printing name
  // Serial.print(frequency);//printing RED color frequency
  // Serial.print("  ");
  delay(100);

  // Setting Blue filtered photodiodes to be read
  digitalWrite(S2,LOW);
  digitalWrite(S3,HIGH);
  // Reading the output frequency
  frequency = pulseIn(sensorOut, LOW);
  // Printing the value on the serial monitor
  B = frequency;
  // Uncomment the code below to see exact values!
  // Serial.print("B= ");//printing name
  // Serial.print(frequency);//printing RED color frequency
  // Serial.println("  ");
  delay(100);

  if(G-R>40 && B-R>50) {
    Serial.print("You are at the anomaly!\n");
  }
  else {
    Serial.print("You are NOT at the anomaly!\n");
  }
}