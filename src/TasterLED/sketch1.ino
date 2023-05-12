const int LED_PIN = LED_BUILTIN;
const int PUSH_BUTTON_PIN = 2;

class Button {
  private:
    int pin;

  public:
    Button(int pin) : pin(pin) {
      pinMode(pin, INPUT);
    }

    int read() {
      return digitalRead(pin);
    }
};

class LED {
  private:
    int pin;

  public:
    LED(int pin) : pin(pin) {
      pinMode(pin, OUTPUT);
      off();
    }

    void on() {
      digitalWrite(pin, HIGH);
    }

    void off() {
      digitalWrite(pin, LOW);
    }
};

Button pushButton(PUSH_BUTTON_PIN);
LED led(LED_PIN);

void setup() {
  Serial.begin(9600);

  while (!Serial) {
    ;  // Warten, bis serielle Verbindung hergestellt ist
  }

  Serial.println("Programm gestartet!");
}

void loop() {
  static int currentPushButtonState = 0;
  static int lastPushButtonState = 0;

  currentPushButtonState = pushButton.read();

  if (currentPushButtonState != lastPushButtonState) {
    if (currentPushButtonState == HIGH) {
      led.on();
      Serial.println("Button gedrückt!");
    } else {
      led.off();
      Serial.println("Button losgelassen!");
    }

    lastPushButtonState = currentPushButtonState;
  }
}
