#include <SoftwareSerial.h>
#include <EEPROM.h>

#define WHITE_PIN 10
#define RED_PIN 11
#define GREEN_PIN 6
#define BLUE_PIN 9
#define PUMP_PIN 8

#define CURR_TIME_BYTE 0x00
#define DRY_TIME_BYTE 0x01
#define WATER_TIME_BYTE 0x02
#define DAY_TIME_BYTE 0x03
#define NIGHT_TIME_BYTE 0x04
#define LAST_WATER_TIME 0x05
#define WHITE_LEVEL_BYTE 0x06
#define RED_LEVEL_BYTE 0x07
#define GREEN_LEVEL_BYTE 0x08
#define BLUE_LEVEL_BYTE 0x09

#define WRITE_CMD_BYTE 0x00
#define READ_CMD_BYTE 0x01
#define RESET_CMD_BYTE 0x02
#define SAVE_CMD_BYTE 0x03

#define RX_PIN 2
#define TX_PIN 3

#define EEPROM_USER_CONFIG_ADDR 8

typedef union time_u {
	byte bytes[sizeof(unsigned long)];
	unsigned long long_num;
} Time;

struct user_config_s {
	Time dry_time;
	Time water_time;
	Time day_time;
	Time night_time;
	Time curr_time;
	Time last_water_time;
	byte white_level;
	byte red_level;
	byte blue_level;
	byte green_level;
};

typedef union {
	struct user_config_s fields;
	byte bytes[sizeof(struct user_config_s)];
} UserConfig;

static const unsigned long DRY_TIME_DEFAULT = 604800000;	// 1 week
static const unsigned long WATER_TIME_DEFAULT = 300000;		// 300 secs
static const unsigned long DAY_TIME_DEFAULT = 57600000;		// 16 hours
static const unsigned long NIGHT_TIME_DEFAULT = 28800000;	// 8 hours
static const byte WHITE_LEVEL_DEFAULT = 255;
static const byte RED_LEVEL_DEFAULT = 172;
static const byte GREEN_LEVEL_DEFAULT = 0;
static const byte BLUE_LEVEL_DEFAULT = 64;

static UserConfig user_config;
static SoftwareSerial bleSerial(RX_PIN, TX_PIN);

void load_user_config(void)
{
	int i;
	for (i=0; i< sizeof(struct user_config_s); i++)
	{
		user_config.bytes[i] = EEPROM.read(EEPROM_USER_CONFIG_ADDR + i);
	}
}

void save_data(void)
{
	int i;
	for (i=0; i< sizeof(struct user_config_s); i++)
	{
		EEPROM.write(EEPROM_USER_CONFIG_ADDR + i, user_config.bytes[i]);
	}
}

void reset_data(void)
{
	user_config.fields.dry_time.long_num = DRY_TIME_DEFAULT;
	user_config.fields.water_time.long_num = WATER_TIME_DEFAULT;
	user_config.fields.day_time.long_num = DAY_TIME_DEFAULT;
	user_config.fields.night_time.long_num = NIGHT_TIME_DEFAULT;
	user_config.fields.last_water_time.long_num = 0;
	user_config.fields.curr_time.long_num = 0;
	user_config.fields.white_level = WHITE_LEVEL_DEFAULT;
	user_config.fields.red_level = RED_LEVEL_DEFAULT;
	user_config.fields.green_level = GREEN_LEVEL_DEFAULT;
	user_config.fields.blue_level = BLUE_LEVEL_DEFAULT;
}

void setup()
{
	pinMode(WHITE_PIN, OUTPUT);
	pinMode(RED_PIN, OUTPUT);
	pinMode(GREEN_PIN, OUTPUT);
	pinMode(BLUE_PIN, OUTPUT);
	pinMode(PUMP_PIN, OUTPUT);

	load_user_config();

	if (user_config.fields.dry_time.long_num == 0)
		user_config.fields.dry_time.long_num = DRY_TIME_DEFAULT;
	if (user_config.fields.water_time.long_num == 0)
		user_config.fields.water_time.long_num = WATER_TIME_DEFAULT;
	if (user_config.fields.day_time.long_num == 0)
		user_config.fields.day_time.long_num = DAY_TIME_DEFAULT;
	if (user_config.fields.night_time.long_num == 0)
		user_config.fields.night_time.long_num = NIGHT_TIME_DEFAULT;
	if (user_config.fields.white_level == 0)
		user_config.fields.white_level = WHITE_LEVEL_DEFAULT;
	if (user_config.fields.red_level == 0)
		user_config.fields.red_level = RED_LEVEL_DEFAULT;
	if (user_config.fields.green_level == 0)
		user_config.fields.green_level = GREEN_LEVEL_DEFAULT;
	if (user_config.fields.blue_level == 0)
		user_config.fields.blue_level = BLUE_LEVEL_DEFAULT;

	bleSerial.begin(9600);
	Serial.begin(9600);
}

void star_light_up(int pin, int wait_millis)
{
	int i=0;
	for (i=0; i<=255; i++)
	{
		analogWrite(pin, i);
		delay(wait_millis);
	}
}

void star_dim_down(int pin, int wait_millis)
{
	int i=0;
	for (i=255; i>=0; i--)
	{
		analogWrite(pin, i);
		delay(wait_millis);
	}
}

void pump_on(void)
{
	digitalWrite(PUMP_PIN, HIGH);
}

void pump_off(void)
{
	digitalWrite(PUMP_PIN, LOW);
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
				time_data = user_config.fields.curr_time;
				break;
			}
			case DRY_TIME_BYTE:
			{
				time_data = user_config.fields.dry_time;
				break;
			}
			case WATER_TIME_BYTE:
			{
				time_data = user_config.fields.water_time;
				break;
			}
			case DAY_TIME_BYTE:
			{
				time_data = user_config.fields.day_time;
				break;
			}
			case NIGHT_TIME_BYTE:
			{
				time_data = user_config.fields.night_time;
				break;
			}
			case LAST_WATER_TIME:
			{
				time_data = user_config.fields.last_water_time;
				break;
			}
			default:
			{
				Serial.print("Error. Unknown data type ");
				Serial.println(type, HEX);
				break;
			}
		}

		for (i = 0; i < len; i++)
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
				level_data = user_config.fields.white_level;
				break;
			}
			case RED_LEVEL_BYTE:
			{
				level_data = user_config.fields.red_level;
				break;
			}
			case BLUE_LEVEL_BYTE:
			{
				level_data = user_config.fields.blue_level;
				break;
			}
			case GREEN_LEVEL_BYTE:
			{
				level_data = user_config.fields.green_level;
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
		for (i = 0; i < len; i++)
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
				user_config.fields.curr_time = time_data;
				break;
			}
			case DRY_TIME_BYTE:
			{
				user_config.fields.dry_time = time_data;
				break;
			}
			case WATER_TIME_BYTE:
			{
				user_config.fields.water_time = time_data;
				break;
			}
			case DAY_TIME_BYTE:
			{
				user_config.fields.day_time = time_data;
				break;
			}
			case NIGHT_TIME_BYTE:
			{
				user_config.fields.night_time = time_data;
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
				user_config.fields.white_level = level_data;
				break;
			}
			case RED_LEVEL_BYTE:
			{
				user_config.fields.red_level = level_data;
				break;
			}
			case BLUE_LEVEL_BYTE:
			{
				user_config.fields.blue_level = level_data;
				break;
			}
			case GREEN_LEVEL_BYTE:
			{
				user_config.fields.green_level = level_data;
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

void wait(unsigned long wait_millis)
{
	unsigned long start_millis = user_config.fields.curr_time.long_num;
	byte user_type, user_cmd, user_len;

	while (user_config.fields.curr_time.long_num - start_millis < wait_millis)
	{
		if (bleSerial.available()  >= 3)
		{
			user_cmd = bleSerial.read();
			user_type = bleSerial.read();
			user_len = bleSerial.read();

			switch (user_cmd)
			{
				case RESET_CMD_BYTE:
				{
					reset_data();
					break;
				}
				case SAVE_CMD_BYTE:
				{
					save_data();
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
}

void loop()
{
	analogWrite(WHITE_PIN, user_config.fields.white_level);
	analogWrite(BLUE_PIN, user_config.fields.blue_level);
	analogWrite(RED_PIN, user_config.fields.red_level);
	analogWrite(GREEN_PIN, user_config.fields.green_level);

	wait(user_config.fields.day_time.long_num);

	analogWrite(WHITE_PIN, 0);
	analogWrite(BLUE_PIN, 0);
	analogWrite(RED_PIN, 0);
	analogWrite(GREEN_PIN, 0);

	wait(user_config.fields.night_time.long_num);

	if (user_config.fields.curr_time.long_num - user_config.fields.last_water_time.long_num 
		>= user_config.fields.dry_time.long_num)
	{
		pump_on();
		wait(user_config.fields.water_time.long_num);
		pump_off();
		user_config.fields.last_water_time.long_num = user_config.fields.curr_time.long_num;
	}
}
