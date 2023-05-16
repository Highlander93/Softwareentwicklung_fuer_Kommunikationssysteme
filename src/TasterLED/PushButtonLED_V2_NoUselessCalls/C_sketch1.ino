/* filename: PushButtonLED.ino */
const int LED_PIN = LED_BUILTIN;
const int PUSH_BUTTON_PIN = 2;

void setup() {
	pinMode(LED_PIN, OUTPUT);
	pinMode(PUSH_BUTTON_PIN, INPUT);
	digitalWrite(LED_PIN, LOW);
}
void loop() {
	static int currentPushButtonState = 0;
	static int lastPushButtonState = 0;

	currentPushButtonState = digitalRead(PUSH_BUTTON_PIN);
	if (currentPushButtonState != lastPushButtonState) {
		digitalWrite(LED_PIN, currentPushButtonState);
		lastPushButtonState = currentPushButtonState;
	}
}