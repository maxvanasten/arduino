#include <HardwareSerial.h>
#define ACCEL_TX 9
#define ACCEL_RX 8
HardwareSerial accel(1);

void setup() {
  Serial.begin(115200);
  accel.begin(9600, SERIAL_8N1, ACCEL_TX, ACCEL_RX);
}

void loop() {
  if(accel.available()) {
    String s = accel.readStringUntil('\n');
    Serial.println("SC7A20H: " + s);
  }
}
