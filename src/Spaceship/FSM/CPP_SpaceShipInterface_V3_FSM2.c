#include <iostream>

// Pin Definitions
const int GREEN_LED_PIN = 3;
const int RED1_LED_PIN = 4;
const int RED2_LED_PIN = 5;
const int PUSH_BUTTON_PIN = 2;

class LED {
private:
    int pin;

public:
    LED(int pinNumber) : pin(pinNumber) {
        pinMode(pin, OUTPUT);
    }

    void on() {
        digitalWrite(pin, HIGH);
    }

    void off() {
        digitalWrite(pin, LOW);
    }
};

class Button {
private:
    int pin;

public:
    Button(int pinNumber) : pin(pinNumber) {
        pinMode(pin, INPUT);
    }

    bool isPressed() {
        return digitalRead(pin) == HIGH;
    }
};

class SpaceshipInterface {
private:
    LED greenLED;
    LED red1LED;
    LED red2LED;
    Button pushButton;

    enum State { Ok, Alarm1, Alarm2 };
    enum Event { None, AlarmOn, AlarmOff };

    State state;
    Event event;
    int lastPushButtonState;

public:
    SpaceshipInterface(int greenPin, int red1Pin, int red2Pin, int buttonPin)
        : greenLED(greenPin), red1LED(red1Pin), red2LED(red2Pin), pushButton(buttonPin) {
    }

    void setup() {
        greenLED.on();
        red1LED.off();
        red2LED.off();

        state = Ok;
        event = None;
        lastPushButtonState = 0;
    }

    void loop() {
        int currentPushButtonState = pushButton.isPressed();

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

        if (state == Ok && event == AlarmOn) {
            greenLED.off();
            state = Alarm1;
        } else if (state == Alarm1 && event == None) {
            blink(red1LED);
            state = Alarm2;
        } else if (state == Alarm2 && event == None) {
            blink(red2LED);
            state = Alarm1;
        } else if ((state == Alarm1 || state == Alarm2) && event == AlarmOff) {
            state = Ok;
            greenLED.on();
        }
    }

    void blink(LED& led) {
        static const int BLINK_DURATION_MS = 250;
        led.on();
        delay(BLINK_DURATION_MS);
        led.off();
        delay(BLINK_DURATION_MS);
    }
};

SpaceshipInterface spaceship(GREEN_LED_PIN, RED1_LED_PIN, RED2_LED_PIN, PUSH_BUTTON_PIN);

void setup() {
    spaceship.setup();
}

void loop() {
    spaceship.loop();
}
