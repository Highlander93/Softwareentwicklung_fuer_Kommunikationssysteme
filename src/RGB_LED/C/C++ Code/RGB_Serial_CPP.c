#include <Arduino.h>
// #include <string>
using namespace std;

// Peripherie Interface
class Peripheral {
public:
  virtual void setup() = 0; // Setup-Methode (reine virtuelle Funktion)
};

 enum CodecResult {
    OK,
    UNKNOWN_TYPE,
    WRONG_FORMAT,
    WRONG_VALUES,
    UNKNOWN_ERROR,
    INSUFFICIENT_BUFFER,
    INVALID_ARGS
  };

// RGBColorRequest Struktur
class RGBColorRequest {
  private:
    uint8_t red;
    uint8_t green;
    uint8_t blue;

  public : CodecResult decode(const string& encoding);
}

struct RGBColorRequest {
  uint8_t red;
  uint8_t green;
  uint8_t blue;
};

// RGB LED Klasse mit den Pins für rot, grün und blau als Attribute
class RGBLED : public Peripheral {
private:
  const int redPin;
  const int greenPin;
  const int bluePin;

public:
  RGBLED(int redPin, int greenPin, int bluePin) : redPin(redPin), greenPin(greenPin), bluePin(bluePin) {}

  void setup() {
    pinMode(redPin, OUTPUT);
    pinMode(greenPin, OUTPUT);
    pinMode(bluePin, OUTPUT);
  }

  void setColor(uint8_t red, uint8_t green, uint8_t blue) {
    analogWrite(redPin, red);
    analogWrite(greenPin, green);
    analogWrite(bluePin, blue);
  }
};

// Sketch Klasse
class Sketch {
private:
  RGBLED rgbLed;
  RGBColorRequest colorRequest;
  int receivedColorRequest;

public:
  Sketch(int redPin, int greenPin, int bluePin) : rgbLed(redPin, greenPin, bluePin), receivedColorRequest(0) {}

  void setup() {
    rgbLed.setup();
    const int BAUD_RATE = 9600;
    Serial.begin(BAUD_RATE);
  }

  void loop() {
    if (receivedColorRequest == 1) {
      rgbLed.setColor(colorRequest.red, colorRequest.green, colorRequest.blue);
      receivedColorRequest = 0;
      const int LOOP_DELAY_MS = 1000;
      delay(LOOP_DELAY_MS);
    }
  }

  void serialEvent() {
    const char START_BYTE = '#';
    const char STOP_BYTE = '!';
    const int maxFrame = 14; /* #R...G...B...! */
    static char encoding[maxFrame + 1] = {'\0'};

    enum ParsingState {
      idle,
      readingEncoding
    };
    static ParsingState state = idle;

    static int byteCounter = 0;
    int c = -1;
    while (receivedColorRequest != 1 && (c = Serial.read()) != -1) {
      if (state == idle) {
        if (c == START_BYTE) {
          byteCounter = 0;
          encoding[byteCounter++] = c;
          state = readingEncoding;
        }
      } else if (state == readingEncoding) {
        if (c == START_BYTE) {
          byteCounter = 0;
          encoding[byteCounter++] = c;
        } else if (c == STOP_BYTE) {
          encoding[byteCounter++] = c;
          encoding[byteCounter] = '\n';
          if (colorRequest.decode(encoding) == OK) {
            receivedColorRequest = 1;
          } else {
            state = idle;
          }
        } else {
          encoding[byteCounter++] = c;
          if (byteCounter >= maxFrame) {
            state = idle;
          }
        }
      }
    }
  }

  CodecResult RGBColorRequest :: decode(const string& encoding) {
    const int MAX_RGB = 255;
    const int MIN_RGB = 0;
    const char* const rgbColorReqTS = "#R%dG%dB%d!";
    int n = 0;
    int red = 0;
    int green = 0;
    int blue = 0;
    if (encoding.length() == 0) {
      return INVALID_ARGS;
    }
    n = sscanf(encoding.c_str(), rgbColorReqTS, &red, &green, &blue);
    if (n != 3) {
      return WRONG_FORMAT;
    } else {
      if (red < MIN_RGB || red > MAX_RGB ||
          green < MIN_RGB || green > MAX_RGB ||
          blue < MIN_RGB || blue > MAX_RGB) {
        return WRONG_VALUES;
      }
    }
    this->red = static_cast<uint8_t>(red);
    this->green = static_cast<uint8_t>(green);
    this->blue = static_cast<uint8_t>(blue);
    return OK;
  }
};

Sketch sketch(6, 5, 3);

void setup() {
  sketch.setup();
}

void loop() {
  sketch.loop();
  sketch.serialEvent();
}
//#R255G0B0!  #R255G0B0!#R0G255B0!#R0G0B255!  #R255G0B0!#R0G0B255!#R0G255B0!
