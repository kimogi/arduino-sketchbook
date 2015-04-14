#include <Servo.h>
 
Servo esc_l;
Servo esc_r;

void slow_down(Servo esc) {
	esc.write(esc.read() - 1);
} 

void speed_up(Servo esc) {
        esc.write(esc.read() + 1);
}

void stop(Servo esc) {
        esc.write(0);
}

void setup()
{
	Serial.begin(9600);
        Serial.println("Initializing ...");
	esc_l.attach(9);
        stop(esc_l);
	speed_up(esc_l);
        esc_r.attach(10);
        stop(esc_r);
        speed_up(esc_l);
}
 
void handleCmd () {
	char c;
	if (Serial.available()) {
		c = Serial.read();
		Serial.print("READ : "); Serial.println(c);
		if (c == 'l') {
			slow_down(esc_l);
			Serial.print("Throttle: "); Serial.println(esc_l.read());
		} else if (c == 'L') {
			speed_up(esc_l);
			Serial.print("Throttle: "); Serial.println(esc_l.read());
		}  if (c == 'r') {
			slow_down(esc_r);
			Serial.print("Throttle: "); Serial.println(esc_r.read());
		} else if (c == 'R') {
			speed_up(esc_r);
			Serial.print("Throttle: "); Serial.println(esc_r.read());
		} else if (c == 's' || c == 'S') {
			stop(esc_l);
			stop(esc_r);
                        Serial.print("Throttle: "); Serial.println(esc_l.read());
                        Serial.print("Throttle: "); Serial.println(esc_r.read());
		} 
	}
}

void loop (void) {
	handleCmd();
}
