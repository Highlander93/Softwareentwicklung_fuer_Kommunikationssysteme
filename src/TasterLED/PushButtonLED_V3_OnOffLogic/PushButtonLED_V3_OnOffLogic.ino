/* ORIGINAL:
const int LED_PIN = LED_BUILTIN;
const int PUSH_BUTTON_PIN = 2;

void setup() {
	pinMode(LED_PIN, OUTPUT);
	pinMode(PUSH_BUTTON_PIN, INPUT);
	digitalWrite(LED_PIN, LOW);
}
void loop() {
	static int currentPushButtonState = LOW;
	static int lastPushButtonState = LOW;

	currentPushButtonState = digitalRead(PUSH_BUTTON_PIN);
	if (currentPushButtonState != lastPushButtonState) {
		if (currentPushButtonState == HIGH) {
			// from LOW to HIGH, push button pressed
			static int ledOnOff = LOW;
			// ledOnOff ^= 1;
			ledOnOff = (ledOnOff == LOW) ? HIGH : LOW; 
			digitalWrite(LED_PIN, ledOnOff);
		}
		lastPushButtonState = currentPushButtonState;
	}
	delay(100);
}
*/

const int LED_PIN = LED_BUILTIN;
const int PUSH_BUTTON_PIN = 2;

// Peripherie Interface
class Peripheral {
public:
  virtual void setup() = 0; // Setup-Methode (reine virtuelle Funktion)
};

// LED Klasse
class LED : public Peripheral {
private: int pin;
private: boolean state;

public: LED(int pin) : pin(pin) {}

  void setup() {
    pinMode(pin, OUTPUT);
    // Beim konstruieren der LED, wird der LED-Status geprüft
    setState(digitalRead(pin));
  }
  
  void setState (boolean value) {
    state = value;
  }

  boolean getState (){
    return state;
  }

  void change() {
    if (state == LOW) {
      on();
      state = HIGH;
    } else if (state == HIGH) {
      off();
      state = LOW;
    }
  }

  void on() {
    digitalWrite(pin, HIGH);
  }

  void off() {
    digitalWrite(pin, LOW);
  }
};

// Button Klasse
class Button : public Peripheral {
private:
  int pin;
  boolean lastState;

public:
  Button(int pin) : pin(pin) {}

  void setup() {
    pinMode(pin, INPUT);
    lastState = LOW;
  }

  boolean doAction() {
    boolean currentState = digitalRead(pin);
    boolean pressed = false;

    if (currentState == HIGH && lastState == LOW) {
      // Taster wurde gedrückt, HIGH muss auf LOW folgen
      pressed = true;
    }

    lastState = currentState;
    return pressed;
  }

};

// Sketch Klasse
class OnOffNoUselessCalls {
private:
  LED ledOnBoard;
  Button button;

public: OnOffNoUselessCalls() : ledOnBoard(LED_PIN), button(PUSH_BUTTON_PIN) {}

    void setup() {
        ledOnBoard.setup();
        button.setup();
    }

    // mit einfacher Logik
    void loop() {
        if (button.doAction()) {
          ledOnBoard.change();
        }
    }
};

OnOffNoUselessCalls onOffNoUselessCallsSketch;

void setup() {
  onOffNoUselessCallsSketch.setup();
}

void loop() {
  onOffNoUselessCallsSketch.loop();
}
