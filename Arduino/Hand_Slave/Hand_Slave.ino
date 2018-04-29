#include <LiquidCrystal.h>

const int interrupt = 2; //Interrupt Pin to send Data Packet
const int reCal = 4; //Interrupt Pin for recalibration
const int numAx = 5;
const int axis[numAx] = {0, 1, 2, 3, 4}; // Analog axis pins
const bool inv[numAx] = {false, false, false, false, false};
// LCD constants.
const int rs = 13, en = 12, d4 = 11, d5 = 10, d6 = 9, d7 = 8;

LiquidCrystal lcd(rs, en, d4, d5, d6, d7);


int amin[numAx];
int amax[numAx];

byte packet[numAx];

unsigned long baseTime = 0;
int screenRefreshMillis = 250;

void setup() {
  pinMode(interrupt, INPUT);
  pinMode(reCal, INPUT_PULLUP);
  
  lcd.begin(16, 2);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Hello");
  
  Serial.begin(9600);
  for(int i = 0; i < numAx; i++){
    packet[i] = 0;
    amin[i] = analogRead(axis[i]);
    amax[i] = amin[i];
  }
  attachInterrupt(digitalPinToInterrupt(interrupt), sendData, RISING);
  attachInterrupt(digitalPinToInterrupt(reCal),calibrate, FALLING);

  baseTime = millis();
  
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

  // If enough time has elapsed, refresh LCD screen.
  if(millis() - baseTime > screenRefreshMillis) {
    //printFlexValues();
    baseTime = millis();
  }
  
  delay(5);
}

void sendData(){
    Serial.write(packet, numAx);
}

void calibrate(){
  for(int i = 0; i < numAx; i++){
    amin[i] = analogRead(axis[i]);
    amax[i] = amin[i];
  }
}

void printFlexValues() {
  // Clear screen.
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("HI :)");
/*
  // Print flex values.
  for(int i = 0; i < numAx; i++) {
    int r = i / 4;
    int c = (i % 4) * 4;
    lcd.setCursor(r, c);
    lcd.print(packet[i]);
  }
  */
}
