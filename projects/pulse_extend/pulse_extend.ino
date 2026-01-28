int signal_pin = 0;
int button_pin = 6;

void setup() {
  pinMode(signal_pin, OUTPUT);
  pinMode(button_pin, INPUT_PULLUP);
  Serial.begin(9600);
  delay(2000);
  Serial.print("Serial monitor started\n");
}

void loop() {
  if (digitalRead(button_pin) == LOW) {
    Serial.print("Button pressed\n");
    digitalWrite(signal_pin, HIGH);
    delay(250);
    digitalWrite(signal_pin, LOW);
  }
}
