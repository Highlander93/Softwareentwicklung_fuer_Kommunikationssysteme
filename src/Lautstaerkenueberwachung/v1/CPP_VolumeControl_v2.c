
enum PinId { POTENTIOMETER_PIN = A0, GREEN_LED_PIN = 4, YELLOW_LED_PIN = 5, RED_LED_PIN = 6 };

class LED {
private:
    PinId pin;

public:
    LED(PinId pinNumber) : pin(pinNumber) {
        pinMode(pin, OUTPUT);
    }

    void on() {
        digitalWrite(pin, HIGH);
    }

    void off() {
        digitalWrite(pin, LOW);
    }
};

class VolumeControl {
private:
    int currentPotentiometer;
    int lastPotentiometer;
    const int MIN_VOLUME;
    const int MAX_VOLUME;
    const int MAX_ANALOG_IN;
    double decibel;
    int loudness;

    const int PLEASANT_THRESHOLD;
    const int HEARING_DAMAGE_THRESHOLD;

    enum VolumeLevel { UNDEFINED, OK, STILL_OK, TOO_LOUD };
    VolumeLevel lastVolumeLevel;
    VolumeLevel currentVolumeLevel;

    LED greenLED;
    LED yellowLED;
    LED redLED;

public:
    VolumeControl()
        : currentPotentiometer(0), lastPotentiometer(-1), MIN_VOLUME(30), MAX_VOLUME(110), MAX_ANALOG_IN(1023),
          PLEASANT_THRESHOLD(60), HEARING_DAMAGE_THRESHOLD(90), lastVolumeLevel(UNDEFINED),
          currentVolumeLevel(UNDEFINED), greenLED(GREEN_LED_PIN), yellowLED(YELLOW_LED_PIN), redLED(RED_LED_PIN) {}

    void setup()
    {
        pinMode(POTENTIOMETER_PIN, INPUT);

        digitalWrite(GREEN_LED_PIN, LOW);
        digitalWrite(YELLOW_LED_PIN, LOW);
        digitalWrite(RED_LED_PIN, LOW);

        greenLED.off();
        yellowLED.off();
        redLED.off();
    }

    void loop()
    {
        currentPotentiometer = analogRead(POTENTIOMETER_PIN);

        if (currentPotentiometer != lastPotentiometer)
        {
            decibel = MIN_VOLUME + double(MAX_VOLUME - MIN_VOLUME) * currentPotentiometer / MAX_ANALOG_IN;
            loudness = int(decibel + 0.5);

            currentVolumeLevel = TOO_LOUD;
            if (loudness < PLEASANT_THRESHOLD)
            {
                currentVolumeLevel = OK;
            }
            else if (loudness < HEARING_DAMAGE_THRESHOLD)
            {
                currentVolumeLevel = STILL_OK;
            }

            if (currentVolumeLevel != lastVolumeLevel)
            {
                if (lastVolumeLevel != UNDEFINED)
                {
                    LED led = getLEDFromVolumeLevel(lastVolumeLevel);
                    led.off();
                }

                LED led = getLEDFromVolumeLevel(currentVolumeLevel);
                led.on();

                lastVolumeLevel = currentVolumeLevel;
            }

            lastPotentiometer = currentPotentiometer;
        }

        delay(100);
    }

    LED getLEDFromVolumeLevel(VolumeLevel level)
    {
        switch (level)
        {
            case OK:
                return greenLED;
            case STILL_OK:
                return yellowLED;
            default:
                return redLED;
        }
    }
};

VolumeControl volumeControl;

void setup()
{
    volumeControl.setup();
}

void loop()
{
    volumeControl.loop();
}
