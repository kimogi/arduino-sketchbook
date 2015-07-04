#define SWING_PIN 4
#define POWER_PIN 2
#define HEAT_PIN 6

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
    pinMode(SWING_PIN, OUTPUT);
    pinMode(POWER_PIN, OUTPUT);
    pinMode(HEAT_PIN, OUTPUT);
}

void loop() {
    switch_pin(POWER_PIN);
    Serial.println("Power");
    delay(500);
    switch_pin(HEAT_PIN);
    Serial.println("Watt");
    delay(500);
    switch_pin(HEAT_PIN);
    Serial.println("Watt");
    delay(500);
    switch_pin(SWING_PIN);
    Serial.println("Swing");    
    delay(2000);
    switch_pin(POWER_PIN);
    Serial.println("Power");
    delay(5000);
}
