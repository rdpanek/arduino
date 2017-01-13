int i = 0;

void setup() {
  Serial.begin(115200);

  Serial.println("-- start --");
}

void loop() {
  Serial.println(i++);
}

