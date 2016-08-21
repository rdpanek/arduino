String jsonData;

void sendToElastic() {

  jsonData += "{\"umisteni\": \"pracovna\"";
  jsonData += ",\"teplota\": \"";
  char tmp[5]; 
  dtostrf(getTeplota(), 1, 2, tmp);
  jsonData += tmp;
  jsonData += "\"";
  jsonData += ",\"chip\": \""+getDSOChip()+"\"";
  jsonData += ",\"freeHeap\": "+String(ESP.getFreeHeap());
  jsonData += ",\"resetReason\": \""+ESP.getResetReason()+"\"";
  jsonData += ",\"flashChipSpeed\": "+String(ESP.getFlashChipSpeed());
  jsonData += ",\"vcc\":"+String(ESP.getVcc());
  jsonData += ",\"dateTime\":\""+String(getTime())+"\"";
  jsonData += ",\"rssi\":\""+String(WiFi.RSSI())+"\"";
  jsonData += "}";

  Serial.println(jsonData);
  // wait for WiFi connection
  if((WiFiMulti.run() == WL_CONNECTED)) {
    HTTPClient http;
    http.begin("http://146.148.102.113:9200/wifi-teplomer/wifi-teplomer/"); //HTTP
    http.addHeader("Content-Type","application/json; charset=UTF-8");
    int httpCode = http.POST(jsonData);
    if (httpCode < 300) {
      digitalWrite(LED, HIGH);
      Serial.println(http.getString());
      delay(100);
      digitalWrite(LED, LOW);
    } else { 
      digitalWrite(LED, HIGH);
      delay(100);
      digitalWrite(LED, LOW);
      delay(100);
      digitalWrite(LED, HIGH);
      delay(100);
      digitalWrite(LED, LOW);
    }
    Serial.println(http.getString());
    http.end();
  }
  jsonData = "";
  
}
