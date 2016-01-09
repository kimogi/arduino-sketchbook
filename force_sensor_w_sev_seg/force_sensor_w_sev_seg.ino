byte seven_seg_digits[10][7] = { { 1,1,1,1,1,1,0 },  // = 0
                                  { 0,1,1,0,0,0,0 },  // = 1
                                  { 1,1,0,1,1,0,1 },  // = 2
                                  { 1,1,1,1,0,0,1 },  // = 3
                                  { 0,1,1,0,0,1,1 },  // = 4
                                  { 1,0,1,1,0,1,1 },  // = 5
                                  { 1,0,1,1,1,1,1 },  // = 6
                                  { 1,1,1,0,0,0,0 },  // = 7
                                  { 1,1,1,1,1,1,1 },  // = 8
                                  { 1,1,1,0,0,1,1 }   // = 9
                                };
int sensorPin = A0;
int sensorValue = 0;
int points = 0;

void setup() {
  pinMode(sensorPin, INPUT);
  Serial.begin(9600);

  pinMode(2, OUTPUT);   
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  writeDot(0);  // start with the "dot" off
}

void writeDot(byte dot) {
  digitalWrite(9, dot);
}
    
void sevenSegWrite(byte digit) {
  byte pin = 2;
  for (byte segCount = 0; segCount < 7; ++segCount) {
    digitalWrite(pin, seven_seg_digits[digit][segCount]);
    ++pin;
  }
}

void loop() {
  delay(1000);
  sevenSegWrite(points); 
  Serial.print(sensorValue);
  Serial.print("\n");

  sensorValue = analogRead(sensorPin);
  if (sensorValue > 500)
  {
    points = (points + 1) % 10;    
  }
  else
  {
    delay(10);
  } 
}
