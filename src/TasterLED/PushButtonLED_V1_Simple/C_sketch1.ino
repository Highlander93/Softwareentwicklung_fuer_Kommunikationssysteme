/* filename: PushButtonLED.ino */
void setup() {
    pinMode(13, OUTPUT); //LED
    pinMode(2, INPUT); //Button
}
void loop() {
    digitalWrite(13, digitalRead(2));
}
