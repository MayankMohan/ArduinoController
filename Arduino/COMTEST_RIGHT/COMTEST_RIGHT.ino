const int interrupt = 2;

byte packet[5];

void setup() {
  pinMode(interrupt, INPUT);
  Serial.begin(9600);
  packet[0] = 0;
  packet[1] = 0;
  packet[2] = 0;
  packet[3] = 0;
  packet[4] = 0;

  attachInterrupt(digitalPinToInterrupt(interrupt), sendData, RISING);
}

void loop() {
  packet[0] += 1;
  packet[1] += 2;
  packet[2] += 3;
  packet[3] += 4;
  packet[4] += 5;
  delay(5);
}

void sendData(){
  Serial.write(packet, 5);
}
