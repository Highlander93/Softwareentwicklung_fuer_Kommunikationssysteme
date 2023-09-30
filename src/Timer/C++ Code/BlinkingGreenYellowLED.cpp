class SoftwareTimer {
public:
    SoftwareTimer() : timeout(0) {}

    int start(int d) {
        if (d <= 0) {
            return 0;
        }
        timeout = millis() + static_cast<unsigned long>(d);
        return 1;
    }

    int isTimeoutEvent() {
        if (timeout == 0) {
            return 0;
        }
        if (timeout <= millis()) {
            timeout = 0;
            return 1;
        }
        return 0;
    }

    int stop() {
        timeout = 0;
        return 1;
    }

private:
    unsigned long timeout;
};

class LEDController {
public:
    LEDController(int pin, int blinkDuration) : ledPin(pin), blinkDuration(blinkDuration) {
        pinMode(ledPin, OUTPUT);
        timer.start(blinkDuration);
    }

    void update() {
        if (timer.isTimeoutEvent() == 1) {
            static int ledOnOff = 1;
            digitalWrite(ledPin, ledOnOff);
            ledOnOff = !ledOnOff;
            timer.start(blinkDuration);
        }
    }

private:
    int ledPin;
    int blinkDuration;
    SoftwareTimer timer;
};

const int LED_GREEN_PIN = 4;
const int LED_YELLOW_PIN = 5;
const int GREEN_BLINK_DURATION_MS = 500;
const int YELLOW_BLINK_DURATION_MS = 750;

LEDController greenLED(LED_GREEN_PIN, GREEN_BLINK_DURATION_MS);
LEDController yellowLED(LED_YELLOW_PIN, YELLOW_BLINK_DURATION_MS);

void setup() {
    // Nothing specific to set up here
}

void loop() {
    greenLED.update();
    yellowLED.update();
}
