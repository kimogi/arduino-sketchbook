#include "SevSeg.h"

SevSeg sevseg;
int sensorPin = A0;
int sensorValue = 0;
int i=0;

void setup() {
  pinMode(sensorPin, INPUT);
  Serial.begin(9600);

   byte numDigits = 1;
   byte digitPins[] = {3,8};
   byte segmentPins[] = {1, 2, 4, 5, 6, 7, 9, 10};
   sevseg.begin(COMMON_ANODE, numDigits, digitPins, segmentPins);
   sevseg.setBrightness(100);
}

void loop() {
 // sensorValue = analogRead(sensorPin);
 // if (sensorValue > 1000)
  {
    //Serial.print(sensorValue);
    //Serial.print("\n");

    Serial.print(i);
    Serial.print("\n");
    sevseg.setNumber(i, 0);
    i = (i + 1) % 10;
    sevseg.refreshDisplay();
   for (int j = 0; j < 100; j++)
   {
       sevseg.refreshDisplay();
   }
  }
//  else
//  {
 //   delay(10);
 // }
}
