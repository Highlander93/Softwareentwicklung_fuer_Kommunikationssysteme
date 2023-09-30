typedef enum { RED_LED_PIN = 6, GREEN_LED_PIN = 5, BLUE_LED_PIN = 3 } RGBLedPinId;

typedef struct {
	uint8_t red;
	uint8_t green;
	uint8_t blue;
} RGBColorRequest;

static RGBColorRequest req;
static int receivedRGBColorRequest = 0;

void setup() {
	const int BAUT_RATE = 9600;
	Serial.begin(BAUT_RATE);
	pinMode(RED_LED_PIN, OUTPUT);
	pinMode(GREEN_LED_PIN, OUTPUT);
	pinMode(BLUE_LED_PIN, OUTPUT);
}

void loop() {
	if (receivedRGBColorRequest == 1) {
		analogWrite(RED_LED_PIN, req.red);
		analogWrite(GREEN_LED_PIN, req.green);
		analogWrite(BLUE_LED_PIN, req.blue);
		receivedRGBColorRequest = 0;
		static const int LOOP_DELAY_MS = 1000;
		delay(LOOP_DELAY_MS);
	}
}

typedef enum {OK, UNKNOWN_TYPE,
				WRONG_FORMAT, WRONG_VALUES,
				UNKNOWN_ERROR, INSUFFICIENT_BUFFER,
				INVALID_ARGS} CodecResult;

CodecResult decodeRGBColorRequest(const char* encoding,
										RGBColorRequest* req);

void serialEvent() {
	static const char START_BYTE = '#';
	static const char STOP_BYTE = '!';
	enum { maxFrame = 14 };  /* #R...G...B...! */
	static char encoding[maxFrame + 1] = { '\0' };

	typedef enum { idle, readingEncoding } ParsingState;
	static ParsingState state = idle;

	static int byteCounter = 0;
	int c = -1;
	while (  receivedRGBColorRequest != 1 && (c = Serial.read()) != -1) {
		/* Serial.write(c); */
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
				if (decodeRGBColorRequest(encoding, &req) == OK) {
					receivedRGBColorRequest = 1;
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

CodecResult decodeRGBColorRequest(const char* encoding,
                                  RGBColorRequest* req) {
	const int MAX_RGB = 255;
	const int MIN_RGB = 0;
	const char* const rgbColorReqTS = "#R%dG%dB%d!";
    int n = 0;
    int red = 0;
    int green = 0;
    int blue = 0;
    if ((encoding == 0) || (req == 0)) { return INVALID_ARGS; }
    n = sscanf(encoding, rgbColorReqTS, &red, &green, &blue);
    if (n != 3) {
        return WRONG_FORMAT;
    } else {
        if (red < MIN_RGB || red > MAX_RGB
                || green < MIN_RGB || green > MAX_RGB
                || blue < MIN_RGB || blue > MAX_RGB) {
            return WRONG_VALUES;
        }
    }
    req->red = (uint8_t)red;
    req->green = (uint8_t)green;
    req->blue = (uint8_t)blue;
    return OK;
}

//#R255G0B0!  #R255G0B0!#R0G255B0!#R0G0B255!  #R255G0B0!#R0G0B255!#R0G255B0!
