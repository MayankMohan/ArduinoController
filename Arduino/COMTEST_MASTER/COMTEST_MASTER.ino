#include <SoftwareSerial.h>

const int numAnalog[2] = {2, 5};
const int numBtn[2] = {3, 0};

const int rxPorts[2] = {2, 3};
const int txPorts[2] = {6, 7};
const int interrupt[2] = {4,5};

SoftwareSerial lPort(rxPorts[0], txPorts[0]);
SoftwareSerial rPort(rxPorts[1], txPorts[1]);

SoftwareSerial ports[2] = {lPort, rPort};

const int packSize = 8;
byte packet[packSize];

void setup() {
  for(int i = 0; i < 2; i++){
    pinMode(interrupt[i], OUTPUT);
    ports[i].begin(9600);
  }

  Serial.begin(9600);

  while(!Serial.available());
}

void loop() {
  for(int i = 0; i < packSize; i++){
    packet[i] = 0;
  }

  int k = 0;
  for(int i = 0; i < 2; i++){
    ports[i].listen();
    digitalWrite(interrupt[i], HIGH);
    
    while(ports[i].available() < numAnalog[i]); //Wait for the data to arrive
    
    for(int j = 0; j < numAnalog[i]; j++){
      packet[k] = ports[i].read();
      k++;
    }
    if(numBtn[i] > 0){
      while(ports[i].available() < 1);
      packet[packSize - 1] = ports[i].read();
    }
    digitalWrite(interrupt[i], LOW);
  }

  Serial.write(packet, packSize);
  delay(50);
}
