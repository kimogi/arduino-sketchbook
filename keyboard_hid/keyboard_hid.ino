#include <SoftwareSerial.h>
#include <stdio.h>

/* Arduino USB HID Keyboard Demo
 * Random Key/Random Delay
 */

#define SSERIAL_RX 2
#define SSERIAL_TX 3

char hex_smbls[16] = {0};
char *pos;
uint8_t hid_buff[8] = {0};
SoftwareSerial mySerial(SSERIAL_RX, SSERIAL_TX);

void setup() 
{
    Serial.begin(9600);
    mySerial.begin(9600);
}

void loop() 
{
    if (mySerial.available() == 16) 
    {
        for (int i=0; i<16; i++)
        {
            hex_smbls[i] = mySerial.read();
        }

        pos = hex_smbls;

        for (int i=0; i < 8; i++) 
        {
            sscanf(pos + 2*i, "%02x", &hid_buff[i]);
        }

        Serial.write(hid_buff, 8);
        releaseKey();
    }
}

void releaseKey(void)
{
    hid_buff[0] = 0x00;
    hid_buff[2] = 0x00;
    Serial.write(hid_buff, 8);
}
