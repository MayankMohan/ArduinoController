const int numAx = 2; //Number of axes
const int numBtn = 3; //Number of Buttons

const int btn[numBtn] = {7, 8, 9}; //Button Pins
const int interrupt = 2; //Interrupt Pin to send Data Packet
const int reCal = 3; //Interrupt Pin for recalibration
const int axis[numAx] = {0, 1}; // Analog axis pins
const bool inv[numAx] = {false, false};

int amin[numAx];
int amax[numAx];

byte packet[numAx + (numBtn > 0)];

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
  Serial.write(packet, numAx + (numBtn > 0));
}

void calibrate(){
  for(int i = 0; i < numAx; i++){
    amin[i] = analogRead(axis[i]);
    amax[i] = amin[i];
  }
}
