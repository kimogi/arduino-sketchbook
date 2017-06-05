#define PUMP_PIN 5

void setup()
{
	pinMode(PUMP_PIN, OUTPUT);
}

void loop()
{
	digitalWrite(PUMP_PIN, HIGH);
 	delay(5000);
	digitalWrite(PUMP_PIN, LOW);
	delay(5000);
}

