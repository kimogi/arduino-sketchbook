#include <SoftwareSerial.h>

#define SWING_CMD "swg"
#define POWER_CMD "pwd"
#define WATT_CMD "wtt"

SoftwareSerial zSerial(2, 3);

String serialData = "";
String address = "";

void send(String address, String cmd)
{
  String msg = "+ZBD=" + address + "," + cmd;
  zSerial.println(msg);
  Serial.println(msg);
  delay(500);
}

void setup()  
{
  Serial.begin(9600);
  zSerial.begin(9600);
  Serial.println("V13");
}

void loop()
{  
  serialData = "";
  while (zSerial.available() > 0)  
  {
    char c = char(zSerial.read());
    delay(2);
    if (c == '\n' || c == '\t' || c == ' ' || c == '\r')
    {
      break;
    }
    else
    {
      serialData += c;
    }
  }

  if (serialData.length() > 0)
  {
    Serial.println(serialData);
    int indexOfZBC = serialData.indexOf("ZBC");
    delay(2000);
    if (indexOfZBC > 0)
    {      
      address = serialData.substring(indexOfZBC + 7, indexOfZBC + 11);
      delay(500);
      send(address, POWER_CMD);
      delay(1000);
      send(address, WATT_CMD);
      delay(1000);
      send(address, WATT_CMD);
      delay(1000);
      send(address, SWING_CMD);
      delay(3000);
      send(address, POWER_CMD);
    }
    Serial.println("-------------------------");
    delay(500);
  }   
}
