const int ldrPin = 0;
int ldrValue = 0;

void setup() {
  Serial.begin(115200);


  Serial.println("-- start --");
}

void loop() {
  ldrValue = analogRead(ldrPin);
  
  Serial.print("LDR: ");
  Serial.println(ldrValue);
} 

