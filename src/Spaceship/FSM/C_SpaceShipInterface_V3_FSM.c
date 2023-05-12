/* filename: SpaceshipInterface.ino */
const int GREEN_LED_PIN = 3;
const int RED1_LED_PIN = 4;
const int RED2_LED_PIN = 5;
const int PUSH_BUTTON_PIN = 2;
void blink(int pin);

void setup() {
	pinMode(GREEN_LED_PIN, OUTPUT);
	pinMode(RED1_LED_PIN, OUTPUT);
	pinMode(RED2_LED_PIN, OUTPUT);
	pinMode(PUSH_BUTTON_PIN, INPUT);
	digitalWrite(RED1_LED_PIN, LOW);
	digitalWrite(RED2_LED_PIN, LOW);
	digitalWrite(GREEN_LED_PIN, HIGH); /* entry action OK */
}
void loop() {
	typedef enum { Ok, Alarm1, Alarm2 } State;
	typedef enum { None, AlarmOn, AlarmOff } Event;
	static State state = Ok;
	static Event event = None;

    /* determine event */
	static int currentPushButtonState = 0;
	static int lastPushButtonState = 0;
	currentPushButtonState = digitalRead(PUSH_BUTTON_PIN);
    if (currentPushButtonState == lastPushButtonState) {
        event = None;
    } else {
        if (currentPushButtonState == LOW) {
            event = AlarmOff;
        } else {
            event = AlarmOn;
        }
		lastPushButtonState = currentPushButtonState;
    }

    //possibly react to event or execute do activity of state
    if (state == Ok && event == AlarmOn)  {
        digitalWrite(GREEN_LED_PIN, LOW); /* exit action OK */
        state = Alarm1;
    } else if (state == Alarm1 && event == None) {
        blink(RED1_LED_PIN); /* exit action Alarm 1 */
        state = Alarm2;
    } else if (state == Alarm2 && event == None) {
        blink(RED2_LED_PIN); /* exit action Alarm 2 */
        state = Alarm1;
    } else if ( (state == Alarm1 || state == Alarm2) 
										&& event == AlarmOff) {
        state = Ok;
        digitalWrite(GREEN_LED_PIN, HIGH); /* entry action OK */
    }
}
void blink(int pin) {
    static const int BLINK_DURATION_MS = 250;
    digitalWrite(pin, HIGH);
    delay(BLINK_DURATION_MS);
    digitalWrite(pin, LOW);
    delay(BLINK_DURATION_MS);
}
