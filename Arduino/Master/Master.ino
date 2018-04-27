#include <SoftwareSerial.h>

// Number of joystick axes.
const int numJoyAxes = 2;
// Number of hand axes (like a graph, not a hatchet).
const int numHandAxes = 5;
// Number of bytes required for all buttons on joystick.
const int numButtonBytes = 1;
// Total number of bytes.
const int totalBytes = numJoyAxes + numHandAxes + numButtonBytes;

// Pins used to send interrupt signal to slaves.
int interruptControllerPin = 4;
int interruptHandPin = 5;

// Serial port for controller.
SoftwareSerial controllerPort(2, 1);
// Serial port for glove.
SoftwareSerial handPort(3, 1);

// Array of bytes that will be sent to VJoy.
// Bytes order for data: [joystick axes][hand axes][button bytes].
byte packet[totalBytes];

void setup() {
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
  // Get data from each slave device.
  pullControllerData();
  pullHandData();

  // Send data to VJoy
  Serial.write(packet, totalBytes);
}

void pullControllerData() {
  // Signal controller to send data. (1 millisecond delay should be more than enough time to detect interrupt)
  digitalWrite(interruptControllerPin, HIGH);
  delay(1);
  digitalWrite(interruptControllerPin, LOW);
  
  // Listen to controller data.
  controllerPort.listen();
  // Put controller data into packet.
  for(int i = 0; controllerPort.available() > 0; i++) {
    byte inByte = controllerPort.read();
    if(i < numJoyAxes) {
      packet[i] = inByte;
    } else if(i < totalBytes) {
      packet[(numJoyAxes + numHandAxes) + (i - numJoyAxes)] = inByte;
    }
  }
}

void pullHandData() {
  // Signal hand to send data. (1 millisecond delay should be more than enough time to detect interrupt)
  digitalWrite(interruptControllerPin, HIGH);
  delay(1);
  digitalWrite(interruptControllerPin, LOW);
  
  // Listen to hand data.
  handPort.listen();
  // Put hand data into packet.
  for(int i = numJoyAxes; handPort.available() > 0; i++) {
    byte inByte = handPort.read();
    if(i < numHandAxes + numJoyAxes) {
      packet[i] = inByte;
    }
  }
}

