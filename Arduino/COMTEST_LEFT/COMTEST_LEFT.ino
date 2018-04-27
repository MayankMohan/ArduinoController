const int interrupt = 2;

byte packet[3];

int count = 0;

void setup() {
  pinMode(interrupt, INPUT);
  Serial.begin(9600);
  packet[0] = 0;
  packet[1] = 0;
  packet[2] = 0;

  attachInterrupt(digitalPinToInterrupt(interrupt), sendData, RISING);
}

void loop() {
  packet[0] += 6;
  packet[1] += 7;
  count++;
  if(count > 50){
    count = 0;
    packet[2]++;
    if(packet[2] > 7){
      packet[2] = 0;
    }
  }
  delay(5);
}

void sendData(){
  Serial.write(packet, 3);
}
