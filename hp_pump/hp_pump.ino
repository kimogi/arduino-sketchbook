#include <SoftwareSerial.h>
#include <EEPROM.h>

#define PUMP_PIN 8

#define CURR_TIME_BYTE 0x00
#define DRY_TIME_BYTE 0x01
#define WATER_TIME_BYTE 0x02
#define LAST_WATER_TIME 0x05

#define WRITE_CMD_BYTE 0x00
#define READ_CMD_BYTE 0x01
#define RESET_CMD_BYTE 0x02
#define SAVE_CMD_BYTE 0x03

#define RX_PIN 2
#define TX_PIN 3

#define EEPROM_HDRP_ADDR 8

typedef union time_u {
	byte bytes[sizeof(unsigned long)];
	unsigned long long_num;
} Time;

typedef struct hrdp_data_s {
	Time dry_time;
	Time water_time;
	Time curr_time;
	Time last_water_time;
} HDRP_data_t;

typedef union {
	HDRP_data_t fields;
	byte bytes[sizeof(HDRP_data_t)];
} HDRP_t;

static const unsigned long DRY_TIME_DEFAULT = 260000;	// 3 days
static const unsigned long WATER_TIME_DEFAULT = 15;

static HDRP_t mHdrp;
static SoftwareSerial bleSerial(RX_PIN, TX_PIN);

void load_hdrp(HDRP_t *hdrp)
{
	int i;
	for (i = 0; i < sizeof(HDRP_t); i++)
	{
		hdrp->bytes[i] = EEPROM.read(EEPROM_HDRP_ADDR + i);
	}
	Serial.println("Settings loaded:");
}

void save_hdrp(HDRP_t *hdrp)
{
	int i;
	for (i = 0; i < sizeof(HDRP_t); i++)
	{
		EEPROM.write(EEPROM_HDRP_ADDR + i, hdrp->bytes[i]);
	}
	Serial.println("Settings saved");
}

void save_last_water_time(void)
{
	HDRP_t hdrp = {0};
	load_hdrp(&hdrp);
	hdrp.fields.last_water_time = mHdrp.fields.last_water_time;
	save_hdrp(&hdrp);
	Serial.print("Last water time saved: "); 
	Serial.println(mHdrp.fields.last_water_time.long_num);
}

void reset_hdrp(HDRP_t *hdrp)
{
	hdrp->fields.dry_time.long_num = DRY_TIME_DEFAULT;
	hdrp->fields.water_time.long_num = WATER_TIME_DEFAULT;
	hdrp->fields.last_water_time.long_num = 0;
	hdrp->fields.curr_time.long_num = 0;
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
	int i;
	Time time_data;
	switch (type)
	{
		case CURR_TIME_BYTE:
		{
			time_data = mHdrp.fields.curr_time;
			break;
		}
		case DRY_TIME_BYTE:
		{
			time_data = mHdrp.fields.dry_time;
			break;
		}
		case WATER_TIME_BYTE:
		{
			time_data = mHdrp.fields.water_time;
			break;
		}
		case LAST_WATER_TIME:
		{
			time_data = mHdrp.fields.last_water_time;
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

void receive_data(byte type, byte len)
{
	int i = 0;

	while (bleSerial.available() < len);
		delay(100);

	Serial.print("Received : ");
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
		case DRY_TIME_BYTE:
		{
			mHdrp.fields.dry_time = time_data;
			break;
		}
		case WATER_TIME_BYTE:
		{
			mHdrp.fields.water_time = time_data;
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

void watchBle(void)
{
	byte user_type, user_cmd, user_len;

	Serial.print("curr_time: "); Serial.println(mHdrp.fields.curr_time.long_num);
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

void print_hdrp(HDRP_t *hdrp)
{
	Serial.println("------------------------------------------------------------");
	Serial.print("water_time: "); Serial.println(hdrp->fields.water_time.long_num);                           
	Serial.print("dry_time: "); Serial.println(hdrp->fields.dry_time.long_num);                               
	Serial.print("last_water_time: "); Serial.println(hdrp->fields.last_water_time.long_num);                 
	Serial.print("curr_time: "); Serial.println(hdrp->fields.curr_time.long_num);                             
	Serial.println("------------------------------------------------------------"); 
}

void watch_water(void)
{
	if (mHdrp.fields.curr_time.long_num > 0)
	{
		if (mHdrp.fields.curr_time.long_num - mHdrp.fields.last_water_time.long_num 
				>= mHdrp.fields.dry_time.long_num)
		{
			Serial.println("Watering ...");
			pump_on();                                                                                                
			delay(mHdrp.fields.water_time.long_num * 1000);                                                           
			pump_off();                                                                                               
			mHdrp.fields.last_water_time.long_num = mHdrp.fields.curr_time.long_num;
			save_last_water_time();
		}
	}
}

void setup()                                                                                                  
{                                                                                                             
	pinMode(PUMP_PIN, OUTPUT);                                                                                
                                                                                                              
	load_hdrp(&mHdrp);                                                                                        
                                                                                                              
	if (mHdrp.fields.dry_time.long_num == 0)                                                                  
		mHdrp.fields.dry_time.long_num = DRY_TIME_DEFAULT;                                                    
	if (mHdrp.fields.water_time.long_num == 0)                                                                
		mHdrp.fields.water_time.long_num = WATER_TIME_DEFAULT;                                                

	bleSerial.begin(9600);                                                                                    
	Serial.begin(9600);  

	print_hdrp(&mHdrp);                                                                                     
}              

void loop()
{
//	watchBle();
//	watch_water();
//  delay(1000);
  if (bleSerial.available()) {
    Serial.print("Received : ");
    Serial.print(bleSerial.read(), HEX);
    Serial.println();
  }
	delay(100);
}
