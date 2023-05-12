/* filename: SpaceshipInterface.ino */
const int GREEN_LED_PIN = 3;
const int RED1_LED_PIN = 4;
const int RED2_LED_PIN = 5;
const int PUSH_BUTTON_PIN = 2;

void setup() {
    pinMode(GREEN_LED_PIN, OUTPUT);
    pinMode(RED1_LED_PIN, OUTPUT);
    pinMode(RED2_LED_PIN, OUTPUT);
    pinMode(PUSH_BUTTON_PIN, INPUT);
	digitalWrite(RED1_LED_PIN, LOW);
	digitalWrite(RED2_LED_PIN, LOW);
    /* entry action OK */
    digitalWrite(GREEN_LED_PIN, HIGH);
}
void loop() {
    static int currentPushButtonState = 0;
    static int lastPushButtonState = 0;
	
    currentPushButtonState = digitalRead(PUSH_BUTTON_PIN);
    if (currentPushButtonState == lastPushButtonState) {
        if (currentPushButtonState == HIGH) {
            /* do activity Alarm */
            static const int BLINK_DURATION_MS = 250;
            digitalWrite(RED1_LED_PIN, HIGH);
            delay(BLINK_DURATION_MS);
            digitalWrite(RED1_LED_PIN, LOW);
            delay(BLINK_DURATION_MS);

            digitalWrite(RED2_LED_PIN, HIGH);
            delay(BLINK_DURATION_MS);
            digitalWrite(RED2_LED_PIN, LOW);
            delay(BLINK_DURATION_MS);
        }
    } else {
        if (currentPushButtonState == LOW) {
            /* entry action OK */
            digitalWrite(GREEN_LED_PIN, HIGH);
        } else {
            /* exit action OK */
            digitalWrite(GREEN_LED_PIN, LOW);
        }
		lastPushButtonState = currentPushButtonState;
    }
}
