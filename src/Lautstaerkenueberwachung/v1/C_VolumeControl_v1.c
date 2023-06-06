/* filename: VolumeControl.ino */
/* Der mittlere oder einzelne Fuss kommt an einen analogen Anschluss. 
 * die anderen beiden aeusseren Fuesse an GND und 5V. 
 */
/* #define PIN_A0   (14)
	static const uint8_t A0 = PIN_A0; */
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
    static int currentPotentiometer = 0;
    static int lastPotentiometer = -1;
	currentPotentiometer = analogRead(POTENTIOMETER_PIN);
	if (currentPotentiometer != lastPotentiometer) {
		static const int MIN_VOLUME = 30; //in dB
		static const int MAX_VOLUME = 110; //in dB
		static const int MAX_ANALOG_IN = 1023;
		static double decibel = 0.0; 
		static int loudness = -1;

		decibel = MIN_VOLUME + double(MAX_VOLUME - MIN_VOLUME) * currentPotentiometer / MAX_ANALOG_IN;
		loudness = int(decibel + 0.5); 

		static const int PLEASANT_THRESHOLD = 60; //in dB
		static const int HEARING_DAMAGE_THRESHOLD = 90; // in dB

		typedef enum {UNDEFINED, OK , STILL_OK , TOO_LOUD} VolumeLevel;
		static VolumeLevel lastVolumeLevel = UNDEFINED;
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

		if (currentVolumeLevel != lastVolumeLevel) {
			if (lastVolumeLevel != UNDEFINED) {
				static PinId led = 0;
				switch(lastVolumeLevel) {
					case OK: {  led = GREEN_LED_PIN; } break;
					case STILL_OK: { led = YELLOW_LED_PIN;  } break;
					default: { led = RED_LED_PIN; } break;
				}
				digitalWrite(led, LOW);
			}
			static PinId led = 0;
			switch(currentVolumeLevel) {
				case OK: {  led = GREEN_LED_PIN; } break;
				case STILL_OK: { led = YELLOW_LED_PIN;  } break;
				default: { led = RED_LED_PIN; } break;
			}
			digitalWrite(led, HIGH);
			lastVolumeLevel = currentVolumeLevel;
		}
		lastPotentiometer = currentPotentiometer;
	} 
    delay(100);
}