const int numAx = 2; //Number of axes
const int numBtn = 3; //Number of Buttons

const int btn[numBtn] = {4, 5, 6}; //Button Pins
const int interrupt = 2; //Interrupt Pin to send Data Packet
const int axis[numAx] = {1, 0}; // Analog axis pins
const bool inv[numAx] = {false, false};

int amin[numAx];
int amax[numAx];

byte packet[numAx + (numBtn > 0)];

void setup() {
  pinMode(interrupt, INPUT);
  for(int i = 0; i < numBtn; i++) {
    pinMode(btn[i], INPUT_PULLUP);
  }
  Serial.begin(9600);
  for(int i = 0; i < numAx; i++){
    amin[i] = analogRead(axis[i]);
    amax[i] = amin[i];
  }
  attachInterrupt(digitalPinToInterrupt(interrupt), sendData, RISING);
}

void loop() {
  byte out = 0;
  
  // Write joystick data to packet.
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

  // Capture button data.
  byte btnByte = 0;
  byte j = 1;
  for(int i = 0; i < numBtn; i++) {
    if(digitalRead(btn[i]) == LOW) {
      btnByte += j; 
    }
    j *= 2;
  }

  if(numBtn > 0) {
    packet[numAx] = btnByte;
  }
  
  delay(5);
}

void sendData(){
  Serial.write(packet, numAx + (numBtn > 0));
}
