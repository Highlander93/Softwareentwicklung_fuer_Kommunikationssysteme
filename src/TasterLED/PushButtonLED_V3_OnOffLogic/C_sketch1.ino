/* filename: PushButtonLEDOnOff.ino */
const int LED_PIN = LED_BUILTIN;
const int PUSH_BUTTON_PIN = 2;

void setup() {
	pinMode(LED_PIN, OUTPUT);
	pinMode(PUSH_BUTTON_PIN, INPUT);
	digitalWrite(LED_PIN, LOW);
}
void loop() {
	static int currentPushButtonState = LOW;
	static int lastPushButtonState = LOW;

	currentPushButtonState = digitalRead(PUSH_BUTTON_PIN);
	if (currentPushButtonState != lastPushButtonState) {
		if (currentPushButtonState == HIGH) {
			/* from LOW to HIGH, push button pressed */
			static int ledOnOff = LOW;
			/* ledOnOff ^= 1; */
			ledOnOff = (ledOnOff == LOW) ? HIGH : LOW; 
			digitalWrite(LED_PIN, ledOnOff);
		}
		lastPushButtonState = currentPushButtonState;
	}
	delay(100);
}
