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
    Potentiometer(int pin) : pin(pin), currentValue(0), lastValue(-1) {
        pinMode(pin, INPUT);
    }

    int readValue() {
        currentValue = analogRead(pin);
        return currentValue;
    }

    bool hasChanged() {
        if (currentValue != lastValue) {
            lastValue = currentValue;
            return true;
        }
        return false;
    }
};

enum class VolumeLevel { UNDEFINED, OK, STILL_OK, TOO_LOUD };

const int MIN_VOLUME = 30; //in dB
const int MAX_VOLUME = 110; //in dB
const int MAX_ANALOG_IN = 1023;
const int PLEASANT_THRESHOLD = 60; //in dB
const int HEARING_DAMAGE_THRESHOLD = 90; // in dB

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
        potentiometer.readValue();
        if (potentiometer.hasChanged()) {
            double decibel = MIN_VOLUME + double(MAX_VOLUME - MIN_VOLUME) * potentiometer.readValue() / MAX_ANALOG_IN;
            int loudness = static_cast<int>(decibel + 0.5);

            if (loudness < PLEASANT_THRESHOLD) {
                currentVolumeLevel = VolumeLevel::OK;
            } else if (loudness < HEARING_DAMAGE_THRESHOLD) {
                currentVolumeLevel = VolumeLevel::STILL_OK;
            } else {
                currentVolumeLevel = VolumeLevel::TOO_LOUD;
            }

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
