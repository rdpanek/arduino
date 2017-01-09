String elasticsearchUri = "192.168.1.246:9200";
String jsonData;
bool allowSendToElasticsearch = false;

void sendToElasticsearch(String jsonData) {
  if (allowSendToElasticsearch) {
    if(WiFi.status() == WL_CONNECTED) {
      HTTPClient http;
      http.begin("http://"+elasticsearchUri+"/paniot-"+ntpDate+"/paniot-"+ntpDate);
      http.addHeader("Content-Type","application/json; charset=UTF-8");
      int httpCode = http.POST(jsonData);
      Serial.println(httpCode);
      Serial.println(http.getString());
      if (httpCode > 300) {
        //ledBlick(3,100);
      } else {
        //ledBlick(1,10);  
      }
      jsonData = "";
      http.end();
    } else {
      //ledBlick(2,100);
    }
  }
}
