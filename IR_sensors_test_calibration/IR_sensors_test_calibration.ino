const uint8_t IR1 = 0; // IR sensor 1 is attached to pin A0
//const uint8_t IR2 = 1; // IR sensor 2 is attached to pin A1

void setup() {

  // we will not send data to the IR sensors, only recieve
  pinMode(IR1, INPUT);
  //pinMode(IR2, INPUT);

  // open connection to serial moniter
  long baudRate = 9600;
  Serial.begin(baudRate);

}

void loop() {

  float reading = analogRead(IR1);
  Serial.println(reading);

  delay(40);

}
