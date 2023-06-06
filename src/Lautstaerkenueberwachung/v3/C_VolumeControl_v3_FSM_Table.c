/* filename: VolumeControl.ino */
typedef enum {POTENTIOMETER = A0, LED_GREEN = 4, LED_YELLOW = 5, LED_RED = 6 } PinId;

void setup() {
    pinMode(POTENTIOMETER, INPUT);
    pinMode(LED_GREEN, OUTPUT);
    pinMode(LED_YELLOW, OUTPUT);
    pinMode(LED_RED, OUTPUT);

    digitalWrite(LED_GREEN, LOW);
    digitalWrite(LED_YELLOW, LOW);
    digitalWrite(LED_RED, LOW);
}

void loop() {
    static int potentiometer = 0;
    static int lastPotentiometer = -1;
	potentiometer = analogRead(POTENTIOMETER);
	if (potentiometer != lastPotentiometer) {
		static const int MIN_VOLUME = 30; //in dB
		static const int MAX_VOLUME = 110; //in dB
		static const int MAX_ANALOG_IN = 1023;
		static double decibel = 0.0;
		static int loudness = -1;

		decibel = MIN_VOLUME + double(MAX_VOLUME - MIN_VOLUME) * potentiometer / MAX_ANALOG_IN;
		loudness = int(decibel + 0.5);

		static const int PLEASANT_THRESHOLD = 60; //in dB
		static const int HEARING_DAMAGE_THRESHOLD = 90; // in dB

		typedef enum {UNDEFINED, OK , STILL_OK , TOO_LOUD} VolumeLevel;
		static VolumeLevel volumeLevel = UNDEFINED;

		if (volumeLevel == UNDEFINED) {
			if (loudness < PLEASANT_THRESHOLD) {
				volumeLevel = OK;
				digitalWrite(LED_GREEN, HIGH);
			} else if (loudness < HEARING_DAMAGE_THRESHOLD) {
				volumeLevel = STILL_OK;
				digitalWrite(LED_YELLOW, HIGH);
			} else {
				volumeLevel = TOO_LOUD;
				digitalWrite(LED_RED, HIGH);
			}
		} else if (volumeLevel == OK && loudness >= PLEASANT_THRESHOLD && loudness < HEARING_DAMAGE_THRESHOLD) {
			/* exit action OK */
			digitalWrite(LED_GREEN, LOW);
			volumeLevel = STILL_OK;
			/* entry action STILL_OK */
			digitalWrite(LED_YELLOW, HIGH);
		} else if (volumeLevel == OK && loudness >= HEARING_DAMAGE_THRESHOLD) {
			digitalWrite(LED_GREEN, LOW);
			volumeLevel = TOO_LOUD;
			digitalWrite(LED_RED, HIGH);
		} else if (volumeLevel == STILL_OK && loudness < PLEASANT_THRESHOLD) {
			digitalWrite(LED_YELLOW, LOW);
			volumeLevel = OK;
			digitalWrite(LED_GREEN, HIGH);
		} else if (volumeLevel == STILL_OK && loudness >= HEARING_DAMAGE_THRESHOLD) {
			digitalWrite(LED_YELLOW, LOW);
			volumeLevel = TOO_LOUD;
			digitalWrite(LED_RED, HIGH);
		} else if (volumeLevel == TOO_LOUD && loudness < PLEASANT_THRESHOLD) {
			digitalWrite(LED_RED, LOW);
			volumeLevel = OK;
			digitalWrite(LED_GREEN, HIGH);
		} else if (volumeLevel == TOO_LOUD && loudness >= PLEASANT_THRESHOLD && loudness < HEARING_DAMAGE_THRESHOLD) {
			digitalWrite(LED_RED, LOW);
			volumeLevel = STILL_OK;
			digitalWrite(LED_YELLOW, HIGH);
		}
		lastPotentiometer = potentiometer;
	}
    delay(100);
}