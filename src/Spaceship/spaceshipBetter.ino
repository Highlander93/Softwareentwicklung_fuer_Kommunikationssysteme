// Peripherie Interface
class Peripheral {
public:
  virtual void setup() = 0; // Setup-Methode (reine virtuelle Funktion)
};

// LED Klasse mit pin als Attribut
class LED : public Peripheral {
private:
  int pin;

public:
  LED(int pin) : pin(pin) {}

  void setup() {
    pinMode(pin, OUTPUT);
  }
  
  void turnOn() {
    digitalWrite(pin, HIGH);
  }

  void turnOff() {
    digitalWrite(pin, LOW);
  }
};

// Schalter Klasse
class Switch : public Peripheral {
private:
  int pin;

public:
  Switch(int pin) : pin(pin) {}

  void setup() {
    pinMode(pin, INPUT);
  }

  bool isPressed() {
    return digitalRead(pin) == HIGH;
  }
};

// Sketch Klasse
class Sketch {
private:
  LED ledRight;
  LED ledMiddle;
  LED ledLeft;
  Switch button;

public:
  Sketch() : ledRight(3), ledMiddle(4), ledLeft(5), button(2) {}

  void setup() {
    ledRight.setup();
    ledMiddle.setup();
    ledLeft.setup();
    button.setup();
  }

  void loop() {
    if (!button.isPressed()) {
      ledLeft.turnOff();
      ledRight.turnOn();
    } else {
      ledRight.turnOff();
      ledLeft.turnOff();
      ledMiddle.turnOn();
      delay(250);
      ledLeft.turnOn();
      ledMiddle.turnOff();
      delay(250);
    }
  }
};

Sketch sketch;

void setup() {
  sketch.setup();
}

void loop() {
  sketch.loop();
}
