// Peripherie Interface
class Peripheral {
public:
  virtual void setup() = 0; // Setup-Methode (reine virtuelle Funktion)
};

// LED Klasse mit pin als Attribut
class LED : public Peripheral {
private:
  int pin;

//pin wird im Konstruktor übergeben und ins Attribut geschrieben
public:
  LED(int pin) : pin(pin) {}

  //implementiert setup() der Peripheral-Klasse, bei Aufruf auf Objekt ist pin gesetzt
  //damit wird gearbeitet
  void setup() {
    pinMode(pin, OUTPUT);
  }
  
  // arbeiten mit pin-Attribut
  void turnOn() {
    digitalWrite(pin, HIGH);
  }

  // arbeiten mit pin-Attribut  
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

// Hauptprogramm
LED ledRight(3); // LED an Pin 3
LED ledMiddle(4);
LED ledLeft(5);
Switch button(2); // Schalter an Pin 2

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
