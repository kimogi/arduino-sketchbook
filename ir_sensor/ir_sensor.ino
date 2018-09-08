#define PD_0 A0
#define PD_1 A1
#define PD_2 A2
#define PD_3 A3
#define PD_4 A6

int value[5] = {0};
int i = 0;

void setup() {
	Serial.begin(9600);
}

void loop() {
  for (i=0; i<5; i++)
    value[i] = 0;
      
	value[0] = analogRead(PD_0);
	value[1] = analogRead(PD_1);
	value[2] = analogRead(PD_2);
	value[3] = analogRead(PD_3);
	value[4] = analogRead(PD_4);

 // for(i=0; i<5; i++)
 // {
 //   Serial.print(value[i]); Serial.print(" ");
 // }
 // Serial.println();
 Serial.println(value[0]);
	delay(500);
}
