/* filename: VolumeControl.ino */
/* Schnittstelle unabhängig vom Arduino API */
typedef enum {OK , STILL_OK , TOO_LOUD, UNDEFINED} VolumeLevel;
int potentiometer2loudness(int potentiometer);
VolumeLevel loudness2VolumeLevel(int loudness);

typedef enum {POTENTIOMETER_PIN = A0, GREEN_LED_PIN = 4, YELLOW_LED_PIN = 5, RED_LED_PIN = 6 } PinId;

void setup() {
    pinMode(POTENTIOMETER_PIN, INPUT);
    pinMode(GREEN_LED_PIN, OUTPUT);
    pinMode(YELLOW_LED_PIN, OUTPUT);
    pinMode(RED_LED_PIN, OUTPUT);

    digitalWrite(GREEN_LED_PIN, LOW);
    digitalWrite(YELLOW_LED_PIN, LOW);
    digitalWrite(RED_LED_PIN, LOW);
}

void loop() {
	static const PinId ledList[] = {
						GREEN_LED_PIN /* VolumeLevel OK */,
						YELLOW_LED_PIN /* VolumeLevel STILL_OK */,
						RED_LED_PIN /* VolumeLevel TOO_LOUD */};
	static int currentPotentiometer = 0;
	static int lastPotentiometer = -1;
	currentPotentiometer = analogRead(POTENTIOMETER_PIN);
	if (currentPotentiometer != lastPotentiometer) {
		static int loudness = -1;
		static VolumeLevel currentVolumeLevel = UNDEFINED;
		static VolumeLevel lastVolumeLevel = UNDEFINED;

		loudness = potentiometer2loudness(currentPotentiometer);
		currentVolumeLevel = loudness2VolumeLevel(loudness);
		if (currentVolumeLevel != lastVolumeLevel) {
			if (lastVolumeLevel != UNDEFINED) {
				digitalWrite(ledList[lastVolumeLevel], LOW);
			}
			digitalWrite(ledList[currentVolumeLevel], HIGH);
			lastVolumeLevel = currentVolumeLevel;
		}
		lastPotentiometer = currentPotentiometer;
	}
	delay(100);
}

int potentiometer2loudness(int potentiometer)
{
	static const int MIN_VOLUME = 30; //in dB
	static const int MAX_VOLUME = 110; //in dB
	static const int MAX_ANALOG_IN = 1023;
	static double decibel = 0.0;
	static int loudness = -1;

	decibel = MIN_VOLUME + (double)(MAX_VOLUME - MIN_VOLUME) * potentiometer / MAX_ANALOG_IN;
	loudness = (int)(decibel + 0.5);
	return loudness;
}

VolumeLevel loudness2VolumeLevel(int loudness)
{
	static const int PLEASANT_THRESHOLD = 60; //in dB
	static const int HEARING_DAMAGE_THRESHOLD = 90; // in dB
	static VolumeLevel currentVolumeLevel = UNDEFINED;

	currentVolumeLevel = TOO_LOUD;
	if (loudness < PLEASANT_THRESHOLD) {
		currentVolumeLevel = OK;
	}
	else if (loudness < HEARING_DAMAGE_THRESHOLD) {
		currentVolumeLevel = STILL_OK;
	} /* else {
		currentVolumeLevel = TOO_LOUD;
	} */
	return currentVolumeLevel;
}
