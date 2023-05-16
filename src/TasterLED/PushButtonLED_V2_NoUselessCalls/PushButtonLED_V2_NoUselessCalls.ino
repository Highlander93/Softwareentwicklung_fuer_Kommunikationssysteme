/* ORIGINAL:
const int LED_PIN = LED_BUILTIN;
const int PUSH_BUTTON_PIN = 2;

void setup() {
	pinMode(LED_PIN, OUTPUT);
	pinMode(PUSH_BUTTON_PIN, INPUT);
	digitalWrite(LED_PIN, LOW);
}
void loop() {
	static int currentPushButtonState = 0;
	static int lastPushButtonState = 0;

	currentPushButtonState = digitalRead(PUSH_BUTTON_PIN);
	if (currentPushButtonState != lastPushButtonState) {
		digitalWrite(LED_PIN, currentPushButtonState);
		lastPushButtonState = currentPushButtonState;
	}
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

public: LED(int pin) : pin(pin) {}

  void setup() {
    pinMode(pin, OUTPUT);
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

public:
  Button(int pin) : pin(pin) {}

  // Button muss setup() aus Peripheral implementieren
  void setup() {
    pinMode(pin, INPUT);
  }

  bool isPressed() {
    return digitalRead(pin) == HIGH;
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

    // ohne unnötige Aufrufe
  void loop() {
    if (button.isPressed()) {
      ledOnBoard.on();
    } else {
      ledOnBoard.off();
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
