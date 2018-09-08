#include <EEPROM.h>

void setup()
{
	pinMode(10, INPUT);
	Serial.begin(9600);
}

void loop()
{
	Serial.println(digitalRead(10));
	delay(100);
}
