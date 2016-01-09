//Motor A
int pinA1 = 7;
int pinA2 = 8;

//Motor B
int pinB1 = 9;
int pinB2 = 10;

//Motor С
int pinC1 = 12;
int pinC2 = 13;

#define CONSOLE_PREF "micro_core$ "
#define CONSOLE_CMD_LEN 255

char in_data[CONSOLE_CMD_LEN];
char in_char = -1;
int in_index = 0;

void do_console(void) 
{
    if (Serial.available() > 0) 
    {
        if (in_index < CONSOLE_CMD_LEN-1)
        {
            in_char = Serial.read();
            in_data[in_index] = in_char;
            in_index++;
            in_data[in_index] = '\0';
        }
        else
        {
            //stack
        }
        
        if (in_char == 13) 
        {
            for (int i = 0; i < CONSOLE_CMD_LEN-1; i++) 
            {
                in_data[i] = 0;
            }
            Serial.println(in_char); Serial.print(CONSOLE_PREF);
        }
        else
        {
            Serial.print(in_char);
        }
    }
}

void setup() 
{
    Serial.begin (9600);
    Serial.print(CONSOLE_PREF);
    pinMode (pinA1, OUTPUT);
	pinMode (pinA2, OUTPUT); 
	pinMode (pinB1, OUTPUT);
	pinMode (pinB2, OUTPUT);
    pinMode (pinC1, OUTPUT);
    pinMode (pinC2, OUTPUT);
}

void loop() 
{
    do_console();
/*
        if (r_byte == 'a')
        {
            motorAforward(140);
            delay (1000);
            motorAstop();
        }
        else if (r_byte == 'b')
        {
            motorBforward(140);
            delay (1000);
            motorBstop();
        }
        else if (r_byte == 'c')
        {
            motorCforward(140);
            delay (1000);
            motorCstop();
        }
*/
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
