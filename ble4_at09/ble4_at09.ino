#include <SoftwareSerial.h>

SoftwareSerial mySerial(2, 3);

void setup() 
{
	Serial.begin(9600);
	mySerial.begin(9600);

	while(!Serial); //if it is an Arduino Micro
}

void loop() 
{
	if(mySerial.available())
		Serial.println(mySerial.read(), HEX);
    
	if(Serial.available())
		mySerial.write(Serial.read());
}
