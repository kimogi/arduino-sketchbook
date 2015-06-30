const int AIB = 5;  // (pwm) pin 5 connected to pin A-IB
const int BIA = 10; // (pwm) pin 10 connected to pin B-IA 
 
byte speed = 100;  // change this (0-255) to control the speed of the motors
 
void setup() {
  pinMode(AIB, OUTPUT);
  pinMode(BIA, OUTPUT);
}

void loop() {
  forward();
  delay(1000);
}
 
void forward()
{
  analogWrite(AIB, 0);
  analogWrite(BIA, speed);
}

