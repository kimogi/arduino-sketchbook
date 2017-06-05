#define PUMP_PIN 10

void setup()
{
  pinMode(PUMP_PIN, OUTPUT);
}

void loop()
{
  unsigned long startMillis = millis();
 
  digitalWrite(PUMP_PIN, HIGH);
  while (millis() - startMillis < 230000); // 230 seconds
  
  startMillis = millis();
  digitalWrite(PUMP_PIN, LOW);
  while (millis() - startMillis < 604800000); // one week  
}

