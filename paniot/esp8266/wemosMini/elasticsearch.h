String elasticsearchUri = "ip:9200";
String jsonData;
bool allowSendToElasticsearch = true;

void sendToElasticsearch(String jsonData) {
  Serial.println(jsonData);
  if (allowSendToElasticsearch) {
    if(WiFi.status() == WL_CONNECTED) {
      HTTPClient http;
      http.begin("http://"+elasticsearchUri+"/paniot-webtester-"+ntpDate+"/paniot-webtester-"+ntpDate);
      http.addHeader("Content-Type","application/json; charset=UTF-8");
      int httpCode = http.POST(jsonData);
      Serial.println(httpCode);
      Serial.println(http.getString());
      if (httpCode > 300) {
        ledBlick(3,100,multiLed1);
      } else {
        ledBlick(1,10,multiLed1);
      }
      jsonData = "";
      http.end();
    } else {
      ledBlick(2,100,multiLed1);
    }
  }
  jsonData = "";
}
