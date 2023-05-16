/* ORIGINAL:
void setup() {
    pinMode(13, OUTPUT); //LED
    pinMode(2, INPUT); //Button
}
void loop() {
    digitalWrite(13, digitalRead(2));
}
*/

//###################################   NEU    ########################:
//Konstanten
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
  private: int pin;

  public: Button(int pin) : pin(pin) {}
    
    //Button muss setup() aus Peripheral implementieren
    void setup() {
        pinMode(pin, INPUT);
    }

    int read() {
      return digitalRead(pin);
    }
};

// Sketch Klasse
class OnOffSimple {
private:
  LED ledOnBoard;
  Button button;

public: OnOffSimple() : ledOnBoard(LED_PIN), button(PUSH_BUTTON_PIN) {}

    void setup() {
        ledOnBoard.setup();
        button.setup();
    }

    void loop() {
        if(button.read()) {
          ledOnBoard.on();
        }
        ledOnBoard.off();
    }
};

OnOffSimple onOffSimpleSketch;

void setup() {
  onOffSimpleSketch.setup();
}

void loop() {
  onOffSimpleSketch.loop();
}
