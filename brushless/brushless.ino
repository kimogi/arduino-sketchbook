#define ADJ_STEP 1
#define ZERO_PWM 130


class ESC {
private:
	byte _pwmValue;
	byte _gpio;
	byte _id;
public:
	ESC(byte id, byte gpio);
	void speedUp();
	void slowDown();
	void stop();
	byte getPWMValue();
	byte getId();
};

ESC::ESC(byte id, byte gpio) {
	_id = id;
	_gpio = gpio;
	pinMode(_gpio, OUTPUT);
	_pwmValue = ZERO_PWM;
	analogWrite(_gpio, _pwmValue);
}

void ESC::speedUp() {
	int value = _pwmValue + ADJ_STEP;
	if (value > 255) {
		value = 255;
	}
	_pwmValue = value;
	analogWrite(_gpio, _pwmValue);
}

void ESC::slowDown() {
	int value = _pwmValue - ADJ_STEP;
	if (value < 0) {
		value = 0;	
	}
	_pwmValue = value;
	analogWrite(_gpio, _pwmValue);
}

byte ESC::getId() {
	return _id;
}

byte ESC::getPWMValue() {
	return _pwmValue;
}

void ESC::stop() {
	_pwmValue = ZERO_PWM;
	analogWrite(_gpio, _pwmValue);	
}

ESC *esc_l;
ESC *esc_r;

void setup (void) {
	Serial.begin(9600);
	Serial.println("Initializing ...");
        esc_l = new ESC(0, 9);
        esc_r = new ESC(0, 10);
}

void handleCmd () {
	char c;
	if (Serial.available()) {
		c = Serial.read();
		Serial.print("READ : "); Serial.println(c);
		if (c == 'l') {
			esc_l->slowDown();
			Serial.print("PWM : "); Serial.println(esc_l->getPWMValue());
		} else if (c == 'L') {
			esc_l->speedUp();
			Serial.print("PWM : "); Serial.println(esc_l->getPWMValue());
		}  if (c == 'r') {
			esc_r->slowDown();
			Serial.print("PWM : "); Serial.println(esc_r->getPWMValue());
		} else if (c == 'R') {
			esc_r->speedUp();
			Serial.print("PWM : "); Serial.println(esc_r->getPWMValue());
		} else if (c == 's' || c == 'S') {
			esc_l->stop();
			esc_r->stop();
                        Serial.print("PWM:  "); Serial.println(esc_l->getPWMValue());
                        Serial.print("PWM:  "); Serial.println(esc_r->getPWMValue());
		} 
	}
}

void loop (void) {
	handleCmd();
}
