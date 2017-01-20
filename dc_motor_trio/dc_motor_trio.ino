//Motor A
int pinA1 = 8;
int pinA2 = 9;

//Motor B
int pinB1 = 10;
int pinB2 = 11;

//Motor С
int pinC1 = 12;
int pinC2 = 13;

char in_char = -1;

char do_console(void) 
{
    if (Serial.available() > 0) 
    {
        in_char = Serial.read();
        if (in_char == 13)
        {
            Serial.println(in_char);
        }
        else
        {
            Serial.print(in_char);
        }
        return in_char;
    }
    return 0;
}

void setup() 
{
    Serial.begin (9600);
    pinMode (pinA1, OUTPUT);
	pinMode (pinA2, OUTPUT); 
	pinMode (pinB1, OUTPUT);
	pinMode (pinB2, OUTPUT);
    pinMode (pinC1, OUTPUT);
    pinMode (pinC2, OUTPUT);
}

void loop() 
{
#if 0
    switch(do_console())
    {
        case 'a':
            motorAforward(140);
            delay (1000);
            motorAstop();                       
            break;
        case 'b':
            motorBforward(140);
            delay (1000);
            motorBstop();
            break;
        case 'c':
            motorCforward(140);
            delay (1000);
            motorCstop();
            break;
    }
#else
    motorAforward(40);
    motorBforward(40);
    motorCforward(40);
    delay (1000);
    motorAstop();
    motorBstop();
    motorCstop();

    delay(1000);

    motorAbackward(40);
    motorBbackward(40);
    motorCbackward(40);    
    delay (1000);
    motorAstop();
    motorBstop();
    motorCstop();
#endif
}

void motorAforward(int pwm) 
{
    analogWrite (pinA1, LOW);
    analogWrite (pinA2, pwm);
}

void motorBforward(int pwm) 
{
	analogWrite (pinB1, LOW);
	analogWrite (pinB2, pwm);
}

void motorCforward(int pwm) 
{
    analogWrite (pinC1, LOW);
    analogWrite (pinC2, pwm);
}

void motorAbackward(int pwm) 
{
	analogWrite (pinA1, pwm);
	analogWrite (pinA2, LOW);
}

void motorBbackward(int pwm) 
{
	analogWrite (pinB1, pwm);
	analogWrite (pinB2, LOW);
}

void motorCbackward(int pwm) 
{
    analogWrite (pinC1, pwm);
    analogWrite (pinC2, LOW);
}

void motorAstop() 
{
	digitalWrite (pinA1, HIGH);
	digitalWrite (pinA2, HIGH);
}

void motorBstop() 
{
	digitalWrite (pinB1, HIGH);
	digitalWrite (pinB2, HIGH);
}

void motorCstop() 
{
    digitalWrite (pinC1, HIGH);
    digitalWrite (pinC2, HIGH);
}

void breakRobot (int duration) 
{
	motorAstop();
	motorBstop();
    motorCstop();
	delay (duration);
}
