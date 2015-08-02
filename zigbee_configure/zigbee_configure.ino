#include <SoftwareSerial.h>
SoftwareSerial mySerial(2, 3); // RX, TX
String myStringSoftCom="";
String myStringHardCom="";
void setup()
{
// Open serial communications and wait for port to open:
Serial.begin(9600);
// set the data rate for the SoftwareSerial port
  mySerial.begin(9600);
}

void loop() // run over and over
{
  while (Serial.available() > 0)
  {
    myStringHardCom += char(Serial.read());
    delay(2);
  }
  if(myStringHardCom!="")
    mySerial.println(myStringHardCom);
    myStringHardCom="";
  while (mySerial.available() > 0)
  {
    myStringSoftCom += char(mySerial.read());
    delay (2);
  }
  if (myStringSoftCom.length() > 0)
  {
    Serial.print(myStringSoftCom);
    myStringSoftCom="";
  }
  delay(300);
}
