#include <SoftwareSerial.h>

// Number of joystick axes.
const int numJoyAxes = 2;
// Number of hand axes (like a graph, not a hatchet).
const int numHandAxes = 5;
// Number of bytes required for all buttons on joystick.
const int numButtonBytes = 1;
// Total number of bytes.
const int totalBytes = numJoyAxes + numHandAxes + numButtonBytes;

// TODO - test code
const int led1 = 11;
const int led2 = 13;

// Pins used to send interrupt signal to slaves.
int interruptControllerPin = 4;
int interruptHandPin = 5;

// Serial port for controller.
SoftwareSerial controllerPort(2, 7);
// Serial port for glove.
SoftwareSerial handPort(3, 6);

// Array of bytes that will be sent to VJoy.
// Bytes order for data: [joystick axes][hand axes][button bytes].
byte packet[totalBytes];

void setup() {

  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);

  digitalWrite(led1, LOW);
  digitalWrite(led2, LOW);

  pinMode(interruptControllerPin, OUTPUT);
  pinMode(interruptHandPin, OUTPUT);
  
  // Open USB serial and wait for port to open.
  Serial.begin(9600);
  
  while(!Serial.available()) {
    // Waiting for serial port to connect.
  }

  // Start software serial ports.
  controllerPort.begin(9600);
  handPort.begin(9600);
}

void loop() {
  // Initialize packet to 0.
  for(int i = 0; i < totalBytes; i++) {
    packet[i] = 0;
  }
  
  // Get data from each slave device.
  pullControllerData();
  pullHandData();

  // Send data to VJoy
  // Serial.write(packet, totalBytes);
  Serial.print("[");
  for(int i = 0; i < 8; i++) {
    Serial.print(packet[i]);
    Serial.print("  ");
  }
  Serial.println("]");

  //delay(100);
}

void pullControllerData() {
  // Listen to controller data.
  controllerPort.listen();
  
  // Signal controller to send data. (1 millisecond delay should be more than enough time to detect interrupt)
  digitalWrite(interruptControllerPin, HIGH);
  delay(1);
  digitalWrite(interruptControllerPin, LOW);
  
  // Put controller data into packet.
  Serial.print("Controller: (");
  for(int i = 0; controllerPort.available() > 0 && i < (numJoyAxes + numButtonBytes) ; i++) {
    int inInt = controllerPort.read();
    if(inInt == -1) {
      digitalWrite(led1, HIGH);
    }
    byte inByte = (byte)inInt;
    Serial.print("  ");
    Serial.print(inByte);
    Serial.print("  ");
    if(i < numJoyAxes) {
      packet[i] = inByte;
    } else {
      packet[(numJoyAxes + numHandAxes) + (i - numJoyAxes)] = inByte;
    }
  }
  Serial.println(")");
  
}

void pullHandData() {
  // Listen to hand data.
  handPort.listen();
  
  // Signal hand to send data. (1 millisecond delay should be more than enough time to detect interrupt)
  digitalWrite(interruptHandPin, HIGH);
  delay(1);
  digitalWrite(interruptHandPin, LOW);

  Serial.print("Hand: (");
  // Put hand data into packet.
  for(int i = numJoyAxes; handPort.available() > 0 && i < (numHandAxes + numJoyAxes); i++) {
     int inInt = handPort.read();
    if(inInt == -1) {
      digitalWrite(led2, HIGH);
    }
    byte inByte = inInt;
    Serial.print("  ");
    Serial.print(inByte);
    Serial.print("  ");
    packet[i] = inByte;
  }

  Serial.println(")");
}

