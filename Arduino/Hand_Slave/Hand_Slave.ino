const int interrupt = 2; //Interrupt Pin to send Data Packet
const int reCal = 3; //Interrupt Pin for recalibration
const int numAx = 5;
const int axis[numAx] = {0, 1, 2, 3, 4}; // Analog axis pins
const bool inv[numAx] = {false, false, false, false, false};

int amin[numAx];
int amax[numAx];

byte packet[numAx];

void setup() {
  // put your setup code here, to run once:
  pinMode(interrupt, INPUT);
  Serial.begin(9600);
  for(int i = 0; i < numAx; i++){
    amin[i] = analogRead(axis[i]);
    amax[i] = amin[i];
  }
  attachInterrupt(digitalPinToInterrupt(interrupt), sendData, RISING);
  attachInterrupt(digitalPinToInterrupt(reCal),calibrate, RISING);
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
  delay(5);
}

void sendData(){
  for(int i = 0; i < numAx; i++){
    Serial.write(packet[i]);
  }
}

void calibrate(){
  for(int i = 0; i < numAx; i++){
    amin[i] = analogRead(axis[i]);
    amax[i] = amin[i];
  }
}
