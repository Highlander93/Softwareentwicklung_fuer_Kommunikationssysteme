/* filename: BlinkWithSoftwareTimer.ino */

typedef struct {
	unsigned long timeout;
} SoftwareTimer;

int startTimer(SoftwareTimer* t, int d);
int isTimeoutEvent(SoftwareTimer* t);
int restart(SoftwareTimer* t);
int stopTimer(SoftwareTimer* t);

const int LED_GREEN_PIN = 4;
const int LED_YELLOW_PIN = 5;
const int GREEN_BLINK_DURATION_MS = 500;
const int YELLOW_BLINK_DURATION_MS = 750;

SoftwareTimer greenTimer;
SoftwareTimer yellowTimer;

void setup() {
	pinMode(LED_GREEN_PIN , OUTPUT);
	pinMode(LED_YELLOW_PIN , OUTPUT);
	startTimer(&greenTimer, GREEN_BLINK_DURATION_MS);
	startTimer(&yellowTimer, YELLOW_BLINK_DURATION_MS);
}

void loop() {
	if (isTimeoutEvent(&greenTimer) == 1) {
		static int ledOnOff = 1;
		digitalWrite(LED_GREEN_PIN, ledOnOff);
		ledOnOff = !ledOnOff;
		startTimer(&greenTimer, GREEN_BLINK_DURATION_MS);
	}
	if (isTimeoutEvent(&yellowTimer) == 1) {
		static int ledOnOff = 1;
		digitalWrite(LED_YELLOW_PIN, ledOnOff);
		ledOnOff = !ledOnOff;
		startTimer(&yellowTimer, YELLOW_BLINK_DURATION_MS);
	}
}

int startTimer(SoftwareTimer* t, int d) {
	if (t == 0 || d <= 0) { return 0; }
	t->timeout =  millis() + d;
	return 1;
}

int isTimeoutEvent(SoftwareTimer* t) {
	if (t == 0 || t->timeout == -1) { return 0; }
	if (t->timeout <= millis()) {
		 t->timeout = -1;
		 return 1;
	}
	return 0;
}

int stopTimer(SoftwareTimer* t) {
	if (t == 0) { return 0; }
	t->timeout = -1;
	return 1;
}


