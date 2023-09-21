enum class VolumeLevel { OK, STILL_OK, TOO_LOUD, UNDEFINED };

class LED {
private:
    int pin;

public:
    LED(int pin) : pin(pin) {
        pinMode(pin, OUTPUT);
        digitalWrite(pin, LOW);
    }

    void turnOn() {
        digitalWrite(pin, HIGH);
    }

    void turnOff() {
        digitalWrite(pin, LOW);
    }
};

class Potentiometer {
private:
    int pin;
    int currentValue;
    int lastValue;

public:
    Potentiometer(int pin) : pin(pin) {
        pinMode(pin, INPUT);
    }

    int readValue() {
        return analogRead(pin);
    }
    bool hasChanged() {
		if (currentValue != lastValue) {
			lastValue = currentValue;
			return true;
		}
		return false;
	}
};

class VolumeControl {
private:
    Potentiometer potentiometer;
    LED greenLED;
    LED yellowLED;
    LED redLED;
    VolumeLevel lastVolumeLevel;
    VolumeLevel currentVolumeLevel;

public:
    VolumeControl(int potPin, int greenPin, int yellowPin, int redPin)
        : potentiometer(potPin),
          greenLED(greenPin),
          yellowLED(yellowPin),
          redLED(redPin),
          lastVolumeLevel(VolumeLevel::UNDEFINED),
          currentVolumeLevel(VolumeLevel::UNDEFINED) {}

    void setup() {
        // Nothing to do here
    }

    void loop() {
        int currentPotentiometer = potentiometer.readValue();
        if (potentiometer.hasChanged()) {
            int loudness = potentiometer2loudness(currentPotentiometer);
            currentVolumeLevel = loudness2VolumeLevel(loudness);

            if (currentVolumeLevel != lastVolumeLevel) {
                if (lastVolumeLevel != VolumeLevel::UNDEFINED) {
                    turnOffLED(lastVolumeLevel);
                }
                turnOnLED(currentVolumeLevel);
                lastVolumeLevel = currentVolumeLevel;
            }
        }

        delay(100);
    }

private:
    int potentiometer2loudness(int potentiometer) {
        static const int MIN_VOLUME = 30; //in dB
        static const int MAX_VOLUME = 110; //in dB
        static const int MAX_ANALOG_IN = 1023;
        static double decibel = 0.0;
        static int loudness = -1;

        decibel = MIN_VOLUME + static_cast<double>(MAX_VOLUME - MIN_VOLUME) * potentiometer / MAX_ANALOG_IN;
        loudness = static_cast<int>(decibel + 0.5);
        return loudness;
    }

    VolumeLevel loudness2VolumeLevel(int loudness) {
        static const int PLEASANT_THRESHOLD = 60; //in dB
        static const int HEARING_DAMAGE_THRESHOLD = 90; // in dB
        static VolumeLevel currentVolumeLevel = VolumeLevel::UNDEFINED;

        currentVolumeLevel = VolumeLevel::TOO_LOUD;
        if (loudness < PLEASANT_THRESHOLD) {
            currentVolumeLevel = VolumeLevel::OK;
        } else if (loudness < HEARING_DAMAGE_THRESHOLD) {
            currentVolumeLevel = VolumeLevel::STILL_OK;
        }
        return currentVolumeLevel;
    }

    void turnOnLED(VolumeLevel level) {
        switch (level) {
            case VolumeLevel::OK:
                greenLED.turnOn();
                break;
            case VolumeLevel::STILL_OK:
                yellowLED.turnOn();
                break;
            case VolumeLevel::TOO_LOUD:
                redLED.turnOn();
                break;
            default:
                break;
        }
    }

    void turnOffLED(VolumeLevel level) {
        switch (level) {
            case VolumeLevel::OK:
                greenLED.turnOff();
                break;
            case VolumeLevel::STILL_OK:
                yellowLED.turnOff();
                break;
            case VolumeLevel::TOO_LOUD:
                redLED.turnOff();
                break;
            default:
                break;
        }
    }
};

// Pin definitions
const int POTENTIOMETER_PIN = A0;
const int GREEN_LED_PIN = 4;
const int YELLOW_LED_PIN = 5;
const int RED_LED_PIN = 6;

VolumeControl volumeControl(POTENTIOMETER_PIN, GREEN_LED_PIN, YELLOW_LED_PIN, RED_LED_PIN);

void setup() {
    volumeControl.setup();
}

void loop() {
    volumeControl.loop();
}
