#include <SoftwareSerial.h>
SoftwareSerial zSerial(2, 3); // RX, TX
#define SWING_PIN 6
#define POWER_PIN 12
#define WATT_PIN 7

#define SWING_CMD "swg"
#define POWER_CMD "pwr"
#define WATT_CMD "wtt"

String serialData = "";
String cmd = "";

void switch_pin(int pin)
{
    digitalWrite(pin, LOW);
    delay(300);
    digitalWrite(pin, HIGH);
    delay(300);
    digitalWrite(pin, LOW);
}

void setup() {
    Serial.begin(9600);
    zSerial.begin(9600);
    pinMode(SWING_PIN, OUTPUT);
    pinMode(POWER_PIN, OUTPUT);
    pinMode(WATT_PIN, OUTPUT);
}

void loop() { 
  serialData = "";
  while (zSerial.available() > 0)
  {
    serialData += char(zSerial.read());
    delay(2);
  }
  
  if (serialData.length() > 0)
  {
    Serial.println(serialData);
    int indexOfZBD = serialData.indexOf("ZBD");
    cmd = serialData.substring(indexOfZBD + 9, indexOfZBD + 12);
    if (cmd.equals(POWER_CMD))
    {
      Serial.println("switch power");
      switch_pin(POWER_PIN);
    }
    else if (cmd.equals(SWING_CMD))
    {
      Serial.println("switch swing");
      switch_pin(SWING_PIN);
    }
    else if (cmd.equals(WATT_CMD))
    {
      Serial.println("switch heat");
      switch_pin(WATT_PIN);
    }
    else
    {
      Serial.println("unknown command: " + cmd);
    }
  }
  delay(1000);
}
