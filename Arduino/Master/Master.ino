#include <SoftwareSerial.h>

// Serial port for left hand.
// TODO - Insert correct ports
SoftwareSerial leftPort(0, 0);
// Serial port for right hand.
SoftwareSerial rightPort(0, 0);

// Array of bytes that will be sent to VJoy.
byte packet[8];
// [0] - Joystick X-Axis
// [1] - Joystick Y-Axis
// [2-6] - 5 x Flex-Axis
// [7] - Buttons

void setup() {
  // Open USB serial and wait for port to open.
  Serial.begin(9600);
  while(!Serial) {
    // Waiting for serial port to connect.
  }

  // Start software serial ports.
  leftPort.begin(9600);
  leftPort.begin(9600);
}

void loop() {
  // Get data from each slave device.
  pullLeftData();
  pullRightData();

  // Send data to VJoy
  Serial.write(packet, 8);
  
}

void pullLeftData() {
  // TODO:
  // - Signal left
  // - Pull left data and insert into byte array
}

void pullRightData() {
  // TODO:
  // - Signal right
  // - Pull right data and insert into byte array
}

