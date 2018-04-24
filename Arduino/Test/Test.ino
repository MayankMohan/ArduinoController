//Adjustable parameters (e.g. pin numbers)
const int numAx = 2; //Number of axes
const int numBtn = 3; //Number of buttons
const int BTN[numBtn] = {8, 9, 10}; // Button pins
const int axis[numAx] = {0,1}; // Analog axis pins
const bool inv[numAx] = {false, false}; // Whether or not to invert the values of axis data


//End of Parameters
int amin[numAx];
int amax[numAx];

byte packet[numAx + 1];

void setup() {
  // put your setup code here, to run once:
  pinMode(LED_BUILTIN, OUTPUT);
  for(int i = 0; i < numBtn; i++){
    pinMode(BTN[i], INPUT_PULLUP);
  }
  Serial.begin(9600);
  for(int i = 0; i < numAx; i++){
    amin[i] = analogRead(axis[i]);
    amax[i] = amin[i];
  }
  digitalWrite(LED_BUILTIN, LOW);
  while(!Serial.available()); //Wait until we confirm an established connection from PC (PC sends a signal)
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
  digitalWrite(LED_BUILTIN, HIGH);
  out = 0;
  int j = 1;
  for(int i = 0; i < numBtn; i++){
    out += (digitalRead(BTN[i]) == LOW) * j;
    j *= 2;
  }
  packet[numAx] = out;

  //Send the data over serial
  Serial.write(packet, (numAx + 1));
  /*
  for(int i = 0; i <= numAx; i++){
    Serial.print(packet[i]);
    Serial.print("   ");
  }
  */
  
  delay(5);
}
