
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

const int LED_GREEN_PIN = 4;
const int LED_YELLOW_PIN = 5;
const int GREEN_BLINK_DURATION_MS = 500;
const int YELLOW_BLINK_DURATION_MS = 750;

SoftwareTimer greenTimer;
SoftwareTimer yellowTimer;

void setup() {
    pinMode(LED_GREEN_PIN, OUTPUT);
    pinMode(LED_YELLOW_PIN, OUTPUT);
    greenTimer.start(GREEN_BLINK_DURATION_MS);
    yellowTimer.start(YELLOW_BLINK_DURATION_MS);
}

void loop() {
    if (greenTimer.isTimeoutEvent() == 1) {
        static int ledOnOff = 1;
        digitalWrite(LED_GREEN_PIN, ledOnOff);
        ledOnOff = !ledOnOff;
        greenTimer.start(GREEN_BLINK_DURATION_MS);
    }
    if (yellowTimer.isTimeoutEvent() == 1) {
        static int ledOnOff = 1;
        digitalWrite(LED_YELLOW_PIN, ledOnOff);
        ledOnOff = !ledOnOff;
        yellowTimer.start(YELLOW_BLINK_DURATION_MS);
    }
}
