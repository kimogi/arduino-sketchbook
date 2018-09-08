#include <SoftwareSerial.h>

#define RX_PIN 11
#define TX_PIN 13

#define PWR_CMD 0xC0
#define HEAT_CMD 0xC1
#define SWING_CMD 0xC2

#define SWING_PIN 6
#define POWER_PIN 12
#define WATT_PIN 7

//#define DEBUG 1

static SoftwareSerial bleSerial(RX_PIN, TX_PIN);

void switch_pin(int pin)
{
    digitalWrite(pin, LOW);
    delay(300);
    digitalWrite(pin, HIGH);
    delay(300);
    digitalWrite(pin, LOW);
}

void setup()
{
#ifdef DEBUG
  Serial.begin(9600);
#endif
	bleSerial.begin(9600);
	pinMode(SWING_PIN, OUTPUT);
	pinMode(POWER_PIN, OUTPUT);
	pinMode(WATT_PIN, OUTPUT);

	while(!Serial);
}

void loop()
{
	byte cmd;
	if (bleSerial.available() > 0)
	{
		cmd = bleSerial.read();
		switch (cmd)
		{
			case PWR_CMD:
			{
				switch_pin(POWER_PIN);
#ifdef DEBUG
				Serial.println("power");
#endif
				break;
			}
			case HEAT_CMD:
			{
				switch_pin(WATT_PIN);
#ifdef DEBUG
				Serial.println("watt");
#endif
				break;
			}
			case SWING_CMD:
			{
				switch_pin(SWING_PIN);
#ifdef DEBUG
				Serial.println("swing");
#endif
				break;
			}
			default:
				break;
		}
	}
}
