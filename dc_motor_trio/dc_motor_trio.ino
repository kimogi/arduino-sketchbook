#include <EEPROM.h>

#define EEPROM_COEF_ADDR 8 
#define EEPROM_CHASIS_ADDR 108

#define OK 0
#define ERROR 1
#define DEFAULT_SPIN_PWM 150
#define DEFAULT_SPIN_DUR 1000

//Motor A
#define PIN_A1 8
#define PIN_A2 9

//Motor B
#define PIN_B1 10
#define PIN_B2 7

//Motor С
#define PIN_C1 23
#define PIN_C2 22

#if 0
static char in_char = -1;
static char c_buff[256] = {0};                                                                                
static int c_top  = 0; 

enum MOTOR {MOTOR_A=0, MOTOR_B, MOTOR_C};
enum DIRECTION {DIR_F=0, DIR_B};
enum CMD {CMD_SET=0, CMD_GET};

typedef struct motor_coef_data_s {
	double a_f;
	double a_b;
	double b_f;
	double b_b;
	double c_f;
	double c_b;
} Coef_s;

typedef union {                                                                                               
	Coef_s fields;                                                                                       
	byte bytes[sizeof(Coef_s)];                                                                          
} Coef_t; 

enum ChasisState {
	STATE_IDLE = 0,
	STATE_SPINNING,
	STATE_PAUSE,
	STATE_ERROR,
	STATE_NUM
};

enum SpinDir {
	SPIN_RIGHT = 0,
	SPIN_LEFT
};

typedef struct chasis_s {
	int spin_dir;
	int spin_dur;
	int spin_pwm;
	ChasisState state;
} Chasis_s;

typedef union {
	Chasis_s fields;
	byte bytes[sizeof(Chasis_s)];
} Chasis_t;

static Chasis_t mChasis = {0};
static Coef_t mCoefs = {0};

void load_chasis(Chasis_t *chasis)                                                                                  
{                                                                                                        
	int i;
	for (i = 0; i < sizeof(Chasis_t); i++)                                                                      
	{                                                                                                         
		chasis->bytes[i] = EEPROM.read(EEPROM_CHASIS_ADDR + i);                                                   
	}
	Serial.println("Chasis loaded");
	Serial.println("---------------------------");
	Serial.print("spin dir : "); Serial.println(chasis->fields.spin_dir);
	Serial.print("spin dur : "); Serial.println(chasis->fields.spin_dur);
	Serial.print("spin pwm : "); Serial.println(chasis->fields.spin_pwm);
	Serial.print("state : "); Serial.println(chasis->fields.state);
	Serial.println("---------------------------");
}                                                                                                             
                                                                                                              
void save_chasis(Chasis_t *chasis)                                                                                  
{                                                                                                             
	int i;
	for (i = 0; i < sizeof(Chasis_t); i++)                                                                      
	{
		EEPROM.write(EEPROM_CHASIS_ADDR + i, chasis->bytes[i]);                                                   
	}                                                                                                         
	Serial.println("Chasis saved");                                                                         
}                                                                                                             

void reset_chasis(Chasis_t *chasis)                                                                                 
{                                                                                                             
	chasis->fields.spin_dir = SPIN_RIGHT;
	chasis->fields.spin_dur = DEFAULT_SPIN_DUR;                                                                                   
	chasis->fields.spin_pwm = DEFAULT_SPIN_PWM;
	chasis->fields.state = STATE_IDLE;                                                                                   
	Serial.println("Chasis reset");                                                                         
}     

void load_coef(Coef_t *coef)                                                                                  
{                                                                                                             
	int i;
	for (i = 0; i < sizeof(Coef_t); i++)
	{
		coef->bytes[i] = EEPROM.read(EEPROM_COEF_ADDR + i);                                            
	}
	Serial.println("Coef loaded");
	Serial.println("--------------------------------------");
	Serial.print("A foward : "); Serial.println(coef->fields.a_f);
	Serial.print("B foward : "); Serial.println(coef->fields.b_f);
	Serial.print("C foward : "); Serial.println(coef->fields.c_f);
	Serial.print("A backward : "); Serial.println(coef->fields.a_b);
	Serial.print("B backward : "); Serial.println(coef->fields.b_b);
	Serial.print("C backward : "); Serial.println(coef->fields.c_b);
	Serial.println("--------------------------------------");
}                                                                                                             
                                                                                                              
void save_coef(Coef_t *coef)                                                                                  
{                                                                                                             
	int i;
	for (i = 0; i < sizeof(Coef_t); i++)                                                                      
	{                                                                                                         
		EEPROM.write(EEPROM_COEF_ADDR + i, coef->bytes[i]);                                            
	}                                                                                                         
	Serial.println("Coef saved");                                                                             
}                                                                                                             
                                                                                                              
void reset_coef(Coef_t *coef)                                                                                 
{                                                                                                             
	coef->fields.a_f = 1.0;
	coef->fields.a_b = 1.0;
	coef->fields.b_f = 1.0;
	coef->fields.b_b = 1.0;
	coef->fields.c_f = 1.0;
	coef->fields.c_b = 1.0;
	Serial.println("Coef reset");                                                                             
}           

void coef_set(Coef_s *coef, MOTOR motor, DIRECTION dir, double value)
{
	if (dir == DIR_F)
	{
		if (motor == MOTOR_A)
			coef->a_f = value;
		else if (motor == MOTOR_B)
			coef->b_f = value;
		else
			coef->c_f = value;
	}
	else
	{
		if (motor == MOTOR_A)
			coef->a_b = value;                                                                          
		else if (motor == MOTOR_B)                                                                            
			coef->b_b = value;                                                                          
		else                                                                                                  
			coef->c_b = value;   
	}
}

void coef_get(Coef_s *coef, MOTOR motor, DIRECTION dir, double *value)                                                 
{                                                                                                             
	if (dir == DIR_F)                                                                                         
	{                                                                                                         
		if (motor == MOTOR_A)                                                                                 
			*value = coef->a_f;                                                                          
		else if (motor == MOTOR_B)                                                                            
			*value = coef->b_f;                                                                          
		else
			*value = coef->c_f;
	}
	else                                                                                                      
	{                                                                                                         
		if (motor == MOTOR_A)                                                                                 
			*value = coef->a_b;
		else if (motor == MOTOR_B)                                                                            
			*value = coef->b_b;
		else
			*value = coef->c_b;
	}
}  

void c_clear(void)
{
	memset(c_buff, 0x0, 256);
	c_top = 0;   
}

void setup() 
{
	Serial.begin (9600);
	pinMode (PIN_A1, OUTPUT);
	pinMode (PIN_A2, OUTPUT); 
	pinMode (PIN_B1, OUTPUT);
	pinMode (PIN_B2, OUTPUT);
	pinMode (PIN_C1, OUTPUT);
	pinMode (PIN_C2, OUTPUT);

	mChasis.fields.spin_dur = DEFAULT_SPIN_DUR;                                                                      
	mChasis.fields.spin_pwm = DEFAULT_SPIN_PWM;                                                                      
	mChasis.fields.state = STATE_IDLE;        
                                                                                      
	load_coef(&mCoefs);
	load_chasis(&mChasis);
	c_clear();
}

void help(void)
{
	//TODO redesign
	Serial.println("\tset|get|save|reset [a|b|c] [f|b] [value (0..255)]");
	Serial.println("\tspin right|left");
}

int transition_to_spin(Chasis_s *chasis, Coef_s *coef)
{
	if (chasis->state != STATE_IDLE)
		return ERROR;

	if (chasis->spin_dir == SPIN_RIGHT)
	{
		spinClockwise(chasis->spin_pwm, coef);
	}
	else if (chasis->spin_dir == SPIN_LEFT)
	{
		spinAntiClockwise(chasis->spin_pwm, coef);
	}
	else
	{
		chasis->state = STATE_ERROR;
		return ERROR;
	}

	chasis->state = STATE_SPINNING;
	return OK;
}

void c_process(void)
{
	//TODO redesign
	char *delim = " ";
	char *cmd_tok, *motor_tok, *dir_tok, *value_tok, *spin_dir_tok;
	MOTOR motor;
	DIRECTION dir;
	CMD cmd;
	double value;

	cmd_tok = strtok(c_buff, delim);
	if (cmd_tok == NULL)
		goto exit;

	if (strcmp(cmd_tok, "save") == 0)
	{
		save_coef(&mCoefs);
		goto exit;
	}
	else if (strcmp(cmd_tok, "reset") == 0)
	{
		reset_coef(&mCoefs);
		goto exit;
	}
	else if (strcmp(cmd_tok, "spin") == 0)
	{
		spin_dir_tok = strtok(NULL, delim);
		if (strcmp(spin_dir_tok, "right") == 0)
			mChasis.fields.spin_dir = SPIN_RIGHT;
		else if (strcmp(spin_dir_tok, "left") == 0)
			mChasis.fields.spin_dir = SPIN_LEFT;

		if (OK != transition_to_spin(&(mChasis.fields), &(mCoefs.fields)))
			goto error;
		else
			goto exit;
	}

	motor_tok = strtok(NULL, delim);
	if (motor_tok == NULL)
		goto help;

	if (strcmp(motor_tok, "a") == 0)                                                                          
	{                                                                                                         
		motor = MOTOR_A;                                                                                      
	}                                                                                                         
	else if (strcmp(motor_tok, "b") == 0)                                                                     
	{                                                                                                         
		motor = MOTOR_B;
	}                                                                                                         
	else if (strcmp(motor_tok, "c") == 0)                                                                     
	{                                                                                                         
		motor = MOTOR_C;                                                                                      
	}
	else
		goto help; 

	dir_tok = strtok(NULL, delim);
	if (dir_tok == NULL)
		goto help;

	if (strcmp(dir_tok, "f") == 0)                                                                            
	{
		dir = DIR_F;                                                                                          
	}                                                                                                         
	else if (strcmp(dir_tok, "b") == 0)                                                                       
	{                                                                                                         
		dir = DIR_B;                                                                                          
	}                                                                                                         
	else
		goto help;

	if (strcmp(cmd_tok, "set") == 0)                                                                          
	{                                                                                                         
		value_tok = strtok(NULL, delim);                                                                        
		if (value_tok == NULL)
			goto help;
                                                                                                              
		value = (double)(atoi(value_tok)) / 255.0;
		coef_set(&(mCoefs.fields), motor, dir, value);
		goto exit;
	}                                                                                                         
	else if (strcmp(cmd_tok, "get") == 0)                                                                     
	{
		coef_get(&(mCoefs.fields), motor, dir, &value);                                                                         
		Serial.print("value : "); Serial.println(value);
		goto exit; 
	}                                                                                                
	else
		goto help;

help:
	help();
error:
	Serial.println("Error");
exit:
	c_clear();
}

void c_push(char c)
{
	c_buff[c_top] = c;
	c_top = (c_top + 1) % 256;
}

void loop() 
{
	if (Serial.available() > 0)                                                                               
	{                                                                                                         
		in_char = Serial.read();                                                                              
		if (in_char != 13)
		{
			c_push(in_char);
			Serial.write(in_char);
		}
		else
		{
			c_process();
			Serial.write("\r\na:>");
		}
	}
}

void motorAforward(int pwm) 
{
	analogWrite (PIN_A1, LOW);
	analogWrite (PIN_A2, pwm);
}

void motorBforward(int pwm) 
{
	analogWrite (PIN_B1, LOW);
	analogWrite (PIN_B2, pwm);
}

void motorCforward(int pwm) 
{
	analogWrite (PIN_C1, LOW);
	analogWrite (PIN_C2, pwm);
}

void motorAbackward(int pwm) 
{
	analogWrite (PIN_A1, pwm);
	analogWrite (PIN_A2, LOW);
}

void motorBbackward(int pwm) 
{
	analogWrite (PIN_B1, pwm);
	analogWrite (PIN_B2, LOW);
}

void motorCbackward(int pwm) 
{
	analogWrite (PIN_C1, pwm);
	analogWrite (PIN_C2, LOW);
}

void motorAstop() 
{
	digitalWrite (PIN_A1, HIGH);
	digitalWrite (PIN_A2, HIGH);
}

void motorBstop() 
{
	digitalWrite (PIN_B1, HIGH);
	digitalWrite (PIN_B2, HIGH);
}

void motorCstop() 
{
	digitalWrite (PIN_C1, HIGH);
	digitalWrite (PIN_C2, HIGH);
}

void spinClockwise(int pwm, Coef_s *coef)
{
	motorAforward(pwm * coef->a_f);
	motorBforward(pwm * coef->b_f);                                                          
	motorCforward(pwm * coef->c_f);       
}

void spinAntiClockwise(int pwm, Coef_s *coef)                                                                     
{                                                                                                             
	motorAbackward(pwm * coef->a_b);                                                                           
	motorBbackward(pwm * coef->b_b);                                                                           
	motorCbackward(pwm * coef->c_b);                                                                           
}  

void breakRobot(void) 
{
	motorAstop();
	motorBstop();
	motorCstop();
}
#else

void setup()
{
	pinMode(PIN_A1, OUTPUT);
	pinMode(PIN_A2, OUTPUT);
	pinMode(PIN_B1, OUTPUT);
	pinMode(PIN_B2, OUTPUT);
	pinMode(PIN_C1, OUTPUT);
	pinMode(PIN_C2, OUTPUT);

	TCCR1A = _BV(COM1A1) | _BV(COM1B1) | _BV(WGM11) | _BV(WGM10);                                             
	TCCR1B = _BV(CS10); // | _BV(WGM12) | _BV(WGM13) | _BV(CS10)
                                                                                                              
	TCCR0A = _BV(COM0A1) | _BV(COM0B1) | _BV(WGM01) | _BV(WGM00);                                              
	TCCR0B = _BV(CS00); // | _BV(WGM02);                                                                          
                                                                                                              
	TCCR2A = _BV(COM2A1) | _BV(COM2B1) | _BV(WGM21) | _BV(WGM20);                                              
	TCCR2B = _BV(CS20); // | _BV(WGM22);   
}

#if 1
void loop()
{
	analogWrite(PIN_A1, 127);
	analogWrite(PIN_A2, 127);
	analogWrite(PIN_B1, 127);
	analogWrite(PIN_B2, 127);
	analogWrite(PIN_C1, 127);
	analogWrite(PIN_C2, 127);
	delay(20000);
}
#else
void loop()
{
	OCR1A=400;
	OCR1B=50; 
	OCR0A=200;
	OCR0B=400;
	OCR2A=200;                                                                                                
	OCR2B=400;  
}
#endif
#endif
