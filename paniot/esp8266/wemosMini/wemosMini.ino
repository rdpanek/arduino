const int ledPin = D8;

void setup() {
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);


  Serial.println("-- start --");
}

void loop() {
  digitalWrite(ledPin, HIGH);
  delay(1000);
  digitalWrite(ledPin, LOW);
  delay(1000);
} 

