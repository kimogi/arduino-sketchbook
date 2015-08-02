#include <SoftwareSerial.h>

SoftwareSerial zSerial(2, 3); // RX, TX

void setup() {
  Serial.begin(9600);
  zSerial.begin(9600);
}

void loop() {
  Serial.println("hello");
  zSerial.println("hello");
  delay(1000);
}
