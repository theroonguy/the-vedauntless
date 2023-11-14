
//read value from iput pin-- conected to output pin 
analogValue = analogRead (A0); //connected to pin A0


void setupPotentiometer(){
  //initialize serial communication at 9600 bits per second
  serial.begin(9600);
}

void loop() {
  //separate potentiometer code
  //read the input of analog pin 0
  int sensorValue = analogRead(A0);
  //print out the value you read in the Serial Monitor:
  serial.println(sensorValue);
  delay(500); //delay in between reads for stability

}
