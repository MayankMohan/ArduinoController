
const int interruptPin = 2;

int interruptsSoFar = 0;
int packetLength = 5;

void setup() {
  pinMode(interruptPin, INPUT);
  attachInterrupt(digitalPinToInterrupt(interruptPin), onInterrupt, RISING);
  
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:

}

void onInterrupt() {

  interruptsSoFar++;

  for(int i = 0; i < packetLength; i++) {
    Serial.write(interruptsSoFar);
  }
}

