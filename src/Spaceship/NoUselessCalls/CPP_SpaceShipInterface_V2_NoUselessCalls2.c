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

    void blink(int duration) {
        on();
        delay(duration);
        off();
        delay(duration);
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

public:
    SpaceshipInterface(int greenPin, int red1Pin, int red2Pin, int buttonPin)
        : greenLED(greenPin), red1LED(red1Pin), red2LED(red2Pin), pushButton(buttonPin) {
    }

    void setup() {
        greenLED.on();
        red1LED.off();
        red2LED.off();
    }

    void loop() {
        static int currentPushButtonState = 0;
        static int lastPushButtonState = 0;

        currentPushButtonState = pushButton.isPressed();

        if (currentPushButtonState == lastPushButtonState) {
            if (currentPushButtonState == HIGH) {
                int blinkDuration = 250;
                red1LED.blink(blinkDuration);
                red2LED.blink(blinkDuration);
            }
        } else {
            if (currentPushButtonState == LOW) {
                greenLED.on();
            } else {
                greenLED.off();
            }
            lastPushButtonState = currentPushButtonState;
        }
    }
};

SpaceshipInterface spaceship(GREEN_LED_PIN, RED1_LED_PIN, RED2_LED_PIN, PUSH_BUTTON_PIN);

void setup() {
    spaceship.setup();
}

void loop() {
    spaceship.loop();
}
