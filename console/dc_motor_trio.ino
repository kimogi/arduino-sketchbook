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

#define NG -1
#define MOTOR_A_F 0
#define MOTOR_B_F 1
#define MOTOR_C_F 2
#define MOTOR_A_B 3
#define MOTOR_B_B 4
#define MOTOR_C_B 5

#define MOTOR_A_F_CMD "af"
#define MOTOR_B_F_CMD "bf"
#define MOTOR_C_F_CMD "cf"
#define MOTOR_A_B_CMD "ab"
#define MOTOR_B_B_CMD "bb"
#define MOTOR_C_B_CMD "cb"

char in_data[CONSOLE_CMD_LEN];
char in_char = -1;
int in_index = 0;
String in_str;

int do_console(void) 
{
    if (Serial.available() > 0) 
    {
        if (in_index < CONSOLE_CMD_LEN-1)
        {
            in_char = Serial.read();
            in_data[in_index] = in_char;
            in_index++;
            in_data[in_index] = '\0';
            in_str += in_char;
        }
        else
        {
            //stack overflow
        }
        
        if (in_char == 13) 
        {
            if (0 == strcmp(in_str, MOTOR_A_F_CMD))
            {
                Serial.println("a foward");
                return MOTOR_A_F;
            }
            else if (0 == strcmp((char *)in_data, MOTOR_B_F_CMD))
            {
                Serial.println("b foward");
                return MOTOR_B_F;
            }
            else if (0 == strcmp((char *)in_data, MOTOR_C_F_CMD))
            {
                Serial.println("c foward");
                return MOTOR_C_F;
            }
            else if (0 == strcmp(in_data, MOTOR_A_B_CMD))
            {
                Serial.println("a backward");
                return MOTOR_A_B;
            }
            else if (0 == strcmp(in_data, MOTOR_B_B_CMD)) 
            {
                Serial.println("b backward");
                return MOTOR_B_B;
            }
            else if (0 == strcmp(in_data, MOTOR_C_B_CMD))
            {
                Serial.println("c backward");
                return MOTOR_C_B;
            }         
            
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
    return NG;
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
    switch(do_console())
    {
        case MOTOR_A_F:
            motorAforward(140);
            delay (1000);
            motorAstop();                       
            break;

        case MOTOR_B_F:
            motorBforward(140);
            delay (1000);
            motorBstop();
            break;

        case MOTOR_C_F:
            motorCforward(140);
            delay (1000);
            motorCstop();
            break;

        case MOTOR_A_B:
            motorAbackward(140);
            delay (1000);
            motorAstop();
            break;

        case MOTOR_B_B:
            motorBbackward(140);
            delay (1000);
            motorBstop();
            break;

        case MOTOR_C_B:
            motorCbackward(140);
            delay (1000);
            motorCstop();
            break;
    }
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
