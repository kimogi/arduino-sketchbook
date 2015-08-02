#include <SoftwareSerial.h>

SoftwareSerial zSerial(2, 3);

String serialData = "";
String address = "";
String serialCmd = "";

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
  Serial.println("V14");
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
    if (indexOfZBC > 0)
    {      
      address = serialData.substring(indexOfZBC + 7, indexOfZBC + 11);
    }
  }

  while(Serial.available() > 0)
  {
    serialCmd += char(Serial.read());
    delay(2);
  }

  if (address.length() > 0)
  {
    send(address, serialCmd);
    delay(1000);
    serialCmd = "";
  }
  Serial.println("-------------------------");
  delay(500);
}
