#include <SoftwareSerial.h>
#include <EEPROM.h>

#define WHITE_PIN 10
#define RED_PIN 11
#define GREEN_PIN 6
#define BLUE_PIN 9

#define CURR_TIME_BYTE 0x00
#define DAY_TIME_BYTE 0x03
#define NIGHT_TIME_BYTE 0x04

#define WHITE_LEVEL_BYTE 0x06
#define RED_LEVEL_BYTE 0x07
#define GREEN_LEVEL_BYTE 0x08
#define BLUE_LEVEL_BYTE 0x09

#define WRITE_CMD_BYTE 0x00
#define READ_CMD_BYTE 0x01
#define RESET_CMD_BYTE 0x02
#define SAVE_CMD_BYTE 0x03

#define RX_PIN 8
#define TX_PIN 7

#define EEPROM_USER_CONFIG_ADDR 8

enum LightState {
	OFF = 0,
	ON,
	OUTDATE
};

typedef union time_u {
	byte bytes[sizeof(unsigned long)];
	unsigned long long_num;
} Time;

typedef struct hrdp_data_s {
	Time day_time;
	Time night_time;
	Time curr_time;
	byte white_level;
	byte red_level;
	byte blue_level;
	byte green_level;
} HDRP_data_t;

typedef union {
	HDRP_data_t fields;
	byte bytes[sizeof(HDRP_data_t)];
} HDRP_t;

static const unsigned long DAY_TIME_DEFAULT = 3600 * 8;		// UTC + 8 hours
static const unsigned long NIGHT_TIME_DEFAULT = 0;			// UTC + 0 hours
static const byte WHITE_LEVEL_DEFAULT = 255;
static const byte RED_LEVEL_DEFAULT = 172;
static const byte GREEN_LEVEL_DEFAULT = 0;
static const byte BLUE_LEVEL_DEFAULT = 64;

static HDRP_t mHdrp;
static SoftwareSerial bleSerial(RX_PIN, TX_PIN);
static LightState light_state = OFF;

void load_hdrp(HDRP_t *hdrp)
{
	int i;
	for (i = 0; i < sizeof(HDRP_t); i++)
	{
		hdrp->bytes[i] = EEPROM.read(EEPROM_USER_CONFIG_ADDR + i);
	}
}

void save_hdrp(HDRP_t *hdrp)
{
	int i;
	for (i = 0; i < sizeof(HDRP_t); i++)
	{
		EEPROM.write(EEPROM_USER_CONFIG_ADDR + i, hdrp->bytes[i]);
	}
	Serial.println("HDRP saved");
}

void reset_hdrp(HDRP_t *hdrp)
{
	hdrp->fields.day_time.long_num = DAY_TIME_DEFAULT;
	hdrp->fields.night_time.long_num = NIGHT_TIME_DEFAULT;
	hdrp->fields.curr_time.long_num = 0;
	hdrp->fields.white_level = WHITE_LEVEL_DEFAULT;
	hdrp->fields.red_level = RED_LEVEL_DEFAULT;
	hdrp->fields.green_level = GREEN_LEVEL_DEFAULT;
	hdrp->fields.blue_level = BLUE_LEVEL_DEFAULT;
	Serial.println("HRDP reset");
}

void star_light_up(int pin, int wait_seconds)
{
	int i;
	for (i = 0; i <= 255; i++)
	{
		analogWrite(pin, i);
		delay(wait_seconds * 1000);
	}
}

void star_dim_down(int pin, int wait_seconds)
{
	int i;
	for (i = 255; i >= 0; i--)
	{
		analogWrite(pin, i);
		delay(wait_seconds * 1000);
	}
}

void send_data(byte type, byte len)
{
	Serial.print("Sending : ");
	if (type < WHITE_LEVEL_BYTE)
	{
		int i;
		Time time_data;
		switch (type)
		{
			case CURR_TIME_BYTE:
			{
				time_data = mHdrp.fields.curr_time;
				break;
			}
			case DAY_TIME_BYTE:
			{
				time_data = mHdrp.fields.day_time;
				break;
			}
			case NIGHT_TIME_BYTE:
			{
				time_data = mHdrp.fields.night_time;
				break;
			}
			default:
			{
				Serial.print("Error. Unknown data type ");
				Serial.println(type, HEX);
				break;
			}
		}

		for (i = len-1; i >= 0; i--)
		{
			bleSerial.write(time_data.bytes[i]);
			Serial.print(time_data.bytes[i], HEX);
			Serial.print(" ");
		}
		Serial.println();
	}
	else
	{
		byte level_data;
		switch(type)
		{
			case WHITE_LEVEL_BYTE:
			{
				level_data = mHdrp.fields.white_level;
				break;
			}
			case RED_LEVEL_BYTE:
			{
				level_data = mHdrp.fields.red_level;
				break;
			}
			case BLUE_LEVEL_BYTE:
			{
				level_data = mHdrp.fields.blue_level;
				break;
			}
			case GREEN_LEVEL_BYTE:
			{
				level_data = mHdrp.fields.green_level;
				break;
			}
			default:
			{
				Serial.print("Error. Unknown data type ");
				Serial.println(type, HEX);
				break;
			}
		}
		bleSerial.write(level_data);
		Serial.println(level_data, HEX);
	}
}

void receive_data(byte type, byte len)
{
	int i = 0;

	while (bleSerial.available() < len);
		delay(100);

	Serial.print("Received : ");
	if (type < WHITE_LEVEL_BYTE)
	{
		Time time_data;
		for (i = len-1 ; i >= 0; i--)
		{
			time_data.bytes[i] = bleSerial.read();
			Serial.print(time_data.bytes[i], HEX);
			Serial.print(" ");
		}
		Serial.println();

		switch (type)
		{
			case CURR_TIME_BYTE:
			{
				mHdrp.fields.curr_time = time_data;
				break;
			}
			case DAY_TIME_BYTE:
			{
				mHdrp.fields.day_time = time_data;
				break;
			}
			case NIGHT_TIME_BYTE:
			{
				mHdrp.fields.night_time = time_data;
				break;
			}
			default:
			{
				Serial.print("Error. Unknown data type ");
				Serial.println(type, HEX);
				break;
			}
		}
	}
	else
	{
		byte level_data = bleSerial.read();
		Serial.println(level_data, HEX);
		switch (type)
		{
			case WHITE_LEVEL_BYTE:
			{
				mHdrp.fields.white_level = level_data;
				light_state = OUTDATE;
				break;
			}
			case RED_LEVEL_BYTE:
			{
				mHdrp.fields.red_level = level_data;
				light_state = OUTDATE;
				break;
			}
			case BLUE_LEVEL_BYTE:
			{
				mHdrp.fields.blue_level = level_data;
				light_state = OUTDATE;
				break;
			}
			case GREEN_LEVEL_BYTE:
			{
				mHdrp.fields.green_level = level_data;
				light_state = OUTDATE;
				break;
			}
			default:
			{
				Serial.print("Error. Unknown data type ");
				Serial.println(type, HEX);
				break;
			}
		}
	}
}

void watchBle(void)
{
	byte user_type, user_cmd, user_len;

	if (bleSerial.available()  >= 3)
	{
		user_cmd = bleSerial.read();
		user_type = bleSerial.read();
		user_len = bleSerial.read();

		switch (user_cmd)
		{
			case RESET_CMD_BYTE:
			{
				reset_hdrp(&mHdrp);
				break;
			}
			case SAVE_CMD_BYTE:
			{
				save_hdrp(&mHdrp);
				break;
			}
			case WRITE_CMD_BYTE:
			{
				receive_data(user_type, user_len);
				break;
			}
			case READ_CMD_BYTE:
			{
				send_data(user_type, user_len);
				break;
			}
			default:
			{
				Serial.print("Error. Unknown user command ");
				Serial.println(user_cmd, HEX);
				break;
			}
		}
	}
}

void lights_on(void)
{
	switch (light_state)
	{
		case ON:
			Serial.println("Light are up to date");
			break;
		case OFF:
		case OUTDATE:
			analogWrite(WHITE_PIN, mHdrp.fields.white_level);                                                         
			analogWrite(BLUE_PIN, mHdrp.fields.blue_level);
			analogWrite(RED_PIN, mHdrp.fields.red_level);                                                             
			analogWrite(GREEN_PIN, mHdrp.fields.green_level);
			light_state = ON;
			break;
		default:
			Serial.println("Lights error");
			break;
	}
}

void lights_off(void)
{
	switch (light_state)
	{
		case OFF:
			Serial.println("Lights already off");
			break;
		case ON:
		case OUTDATE:
			analogWrite(WHITE_PIN, 0);                                                                                
			analogWrite(BLUE_PIN, 0);                                                                                 
			analogWrite(RED_PIN, 0);                                                                                  
			analogWrite(GREEN_PIN, 0); 
			light_state = OFF;
			break;
		default:
			Serial.println("Lights error");
			break;
	}
}

void print_hdrp(HDRP_t *hdrp)
{
	Serial.println("------------------------------------------------------------");
	Serial.print("white_level: "); Serial.println(hdrp->fields.white_level);                                  
	Serial.print("blue_level: "); Serial.println(hdrp->fields.blue_level);                                    
	Serial.print("red_level: "); Serial.println(hdrp->fields.red_level);                                      
	Serial.print("green_level: "); Serial.println(hdrp->fields.green_level);
	Serial.print("day_time: "); Serial.println(hdrp->fields.day_time.long_num);                               
	Serial.print("night_time: "); Serial.println(hdrp->fields.night_time.long_num);                                                                   
	Serial.print("curr_time: "); Serial.println(hdrp->fields.curr_time.long_num);                             
	Serial.println("------------------------------------------------------------"); 
}

unsigned long get_sod(unsigned long epoche)                                                                      
{
	return epoche % ((unsigned long)24 * (unsigned long)3600);
}

void watch_lights(void)
{
	if (mHdrp.fields.curr_time.long_num > 0)
	{
		//UTC time
		unsigned long sod = get_sod(mHdrp.fields.curr_time.long_num);
		Serial.print("Sod : "); Serial.println(sod);
		if (mHdrp.fields.night_time.long_num <= sod && sod <= mHdrp.fields.day_time.long_num)
			lights_off();
		else
			lights_on();
	}
}

void setup()                                                                                                  
{                                                                                                             
	pinMode(WHITE_PIN, OUTPUT);
	pinMode(RED_PIN, OUTPUT);                                                                                 
	pinMode(GREEN_PIN, OUTPUT);                                                                               
	pinMode(BLUE_PIN, OUTPUT);                                                                                
                                                                                                              
	load_hdrp(&mHdrp);

	if (mHdrp.fields.day_time.long_num == 0)
		mHdrp.fields.day_time.long_num = DAY_TIME_DEFAULT;
	if (mHdrp.fields.night_time.long_num == 0)
		mHdrp.fields.night_time.long_num = NIGHT_TIME_DEFAULT;
	if (mHdrp.fields.white_level == 0)
		mHdrp.fields.white_level = WHITE_LEVEL_DEFAULT;
	if (mHdrp.fields.red_level == 0)
		mHdrp.fields.red_level = RED_LEVEL_DEFAULT;
	if (mHdrp.fields.green_level == 0)
		mHdrp.fields.green_level = GREEN_LEVEL_DEFAULT;
	if (mHdrp.fields.blue_level == 0)
		mHdrp.fields.blue_level = BLUE_LEVEL_DEFAULT;

	bleSerial.begin(9600);                                                                                    
	Serial.begin(9600);  

	print_hdrp(&mHdrp);                                                                                     
}              

void loop()
{
	watchBle();
	watch_lights();
	delay(1000);
}
