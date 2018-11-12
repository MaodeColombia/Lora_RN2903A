//MMO: https://support.sodaq.com/sodaq-one/button/

void setup() {
  // Configure the button as an input and enable the internal pull-up resistor
  pinMode(BUTTON, INPUT_PULLUP);
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  // Read the button value into a variable
  int sensorVal = digitalRead(BUTTON);

  // Turn on the LED when the Button is pushed
  if (sensorVal == HIGH) {
    digitalWrite(LED_BUILTIN, LOW);
  } else {
    digitalWrite(LED_BUILTIN, HIGH);
  }
}
