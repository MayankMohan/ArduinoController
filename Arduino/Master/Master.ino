const int BTN[] = {8,9}; // Button pins
const int interrupt = 2; //Interrupt Pin
const int numAx = 2;
const int axis[numAx] = {0,1}; // Analog axis pins
const bool inv[numAx] = {false, false};

int amin[numAx];
int amax[numAx];

byte packet[numAx];

void setup() {
  // put your setup code here, to run once:
  pinMode(BTN[0], INPUT_PULLUP);
  pinMode(BTN[1], INPUT_PULLUP);
  pinMode(interrupt, INPUT);
  Serial.begin(9600);
  for(int i = 0; i < numAx; i++){
    amin[i] = analogRead(axis[i]);
    amax[i] = amin[i];
  }
  attachInterrupt(digitalPinToInterrupt(interrupt), sendData, RISING);
}

void loop() {
  // put your main code here, to run repeatedly:
  byte out = 0;
  //Output analog Byte
  for(int i = 0; i < numAx; i++){
    int an = analogRead(axis[i]);
    if(an < amin[i]){
      amin[i] = an;
    }
    if(an > amax[i]){
      amax[i] = an;
    }
    out = map(an, amin[i], amax[i], 0 + inv[i] * 100, 100 - inv[i] * 100);
    packet[i] = out;
  }

  //Output buttons Byte
  /*
  out = (digitalRead(BTN[0]) == LOW) * 2 + (digitalRead(BTN[1]) == LOW);
  Serial.write(out);
  */
  delay(5);
}

void sendData(){
  for(int i = 0; i < numAx; i++){
    Serial.write(packet[i]);
  }
}
