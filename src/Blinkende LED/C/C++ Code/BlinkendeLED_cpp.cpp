// C-Code so small, c++ is exact the same like the C-code.
/* filename: InterruptLedOnOff.ino */
static const int LED_PIN = LED_BUILTIN;
static volatile int buttonPressedEvent = 0;
void isrPushButton();

void setup() {
	const int PUSH_BUTTON_PIN = 2;
	pinMode(PUSH_BUTTON_PIN, INPUT);
	attachInterrupt(digitalPinToInterrupt(PUSH_BUTTON_PIN),
										isrPushButton, RISING);
	pinMode(LED_PIN, OUTPUT);
	digitalWrite(LED_PIN, LOW);
}
void loop() {
	if (buttonPressedEvent == 1) {
		static int ledOnOff = LOW;
		ledOnOff = (ledOnOff == LOW) ? HIGH : LOW;
		digitalWrite(LED_PIN, ledOnOff);
		buttonPressedEvent = 0;
	}
}

/* Das Prellen sollte in der ISR-Prozedur und nicht in der loop-Prozedur erkannt werden. */

void isrPushButton() {
	static const int BOUNCE_DURATION_MS = 250; /* ms */
	static unsigned long lastPushButtonPressedTime = 0;

	unsigned long now = millis();
	if (now - lastPushButtonPressedTime > BOUNCE_DURATION_MS) {
		buttonPressedEvent = 1;
		lastPushButtonPressedTime = now;
	} /* else { ignore ISR call, it was a bounce } */
}
