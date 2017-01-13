const int ledPin = D8;
const int ldrPin = 0;
int ldrValue = 0;

void setup() {
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);


  Serial.println("-- start --");
}

void loop() {
  ldrValue = analogRead(ldrPin);

  if (ldrValue < 500) {
    digitalWrite(ledPin, HIGH);
  } else {
    digitalWrite(ledPin, LOW);
  }
}
