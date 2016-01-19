//Motor A
int pinA1 = 7;
int pinA2 = 8;

//Motor B
int pinB1 = 9;
int pinB2 = 10;

//Motor С
int pinC1 = 12;
int pinC2 = 13;

void setup() {
    Serial.begin (9600);
    pinMode (pinA1, OUTPUT);
	pinMode (pinA2, OUTPUT); 
	pinMode (pinB1, OUTPUT);
	pinMode (pinB2, OUTPUT);
    pinMode (pinC1, OUTPUT);
    pinMode (pinC2, OUTPUT);
}

void loop() {
  motorAforward(140);
  delay (1000);
  motorAstop();
  motorBforward(140);
  delay (1000);
  motorBstop();
  motorCforward(140);
  delay (1000);
  motorCstop();
}

void motorAforward(int pwm) {
	analogWrite (pinA1, LOW);
	analogWrite (pinA2, pwm);
}

void motorBforward(int pwm) {
	analogWrite (pinB1, LOW);
	analogWrite (pinB2, pwm);
}

void motorCforward(int pwm) {
  analogWrite (pinC1, LOW);
  analogWrite (pinC2, pwm);
}

void motorAbackward(int pwm) {
	analogWrite (pinA1, pwm);
	analogWrite (pinA2, LOW);
}

void motorBbackward(int pwm) {
	analogWrite (pinB1, pwm);
	analogWrite (pinB2, LOW);
}

void motorCbackward(int pwm) {
  analogWrite (pinC1, pwm);
  analogWrite (pinC2, LOW);
}

void motorAstop() {
	digitalWrite (pinA1, HIGH);
	digitalWrite (pinA2, HIGH);
}

void motorBstop() {
	digitalWrite (pinB1, HIGH);
	digitalWrite (pinB2, HIGH);
}

void motorCstop() {
  digitalWrite (pinC1, HIGH);
  digitalWrite (pinC2, HIGH);
}

void breakRobot (int duration) {
	motorAstop();
	motorBstop();
  motorCstop();
	delay (duration);
}
