String elasticsearchUri = "http://192.168.1.246:9200";
String jsonData;

void sendToElasticsearch(String jsonData) {
  Serial.println(jsonData);
  if(WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(elasticsearchUri+"/paniot-wifi-20"+getDate("ISO8601Date")+"/paniot-wifi-20"+getDate("ISO8601Date"));
    http.addHeader("Content-Type","application/json; charset=UTF-8");
    int httpCode = http.POST(jsonData);
    Serial.println(httpCode);
    Serial.println(http.getString());
    if (httpCode > 300) {
      ledBlick(3,100);
    } else {
      ledBlick(1,10);  
    }
    jsonData = "";
    http.end();
  } else {
    ledBlick(2,100);
  }
}

String addESPInfo(String jsonData) {
  jsonData += ",\"freeHeap\": \""+String(ESP.getFreeHeap())+"\"";
  jsonData += ",\"reset\": \""+ESP.getResetReason()+"\"";
  jsonData += ",\"rssi\":\""+String(WiFi.RSSI())+"\"";
  jsonData += ",\"ISO8601DateTime\":\""+getDate("ISO8601DateTime")+"\"";
  jsonData += "}";
  return jsonData;
}


void ds18b20ToElastic() {
  if (devicePlace.length() == 0) { 
    devicePlace = "pojmenujSeznor";
  }
  dallasDS.requestTemperatures();
  jsonData += "{\"place\": \""+devicePlace+"\"";
  jsonData += ",\"senzor\": \"ds18b20\"";
  jsonData += ",\"valInt\":\""+String(dallasDS.getTempCByIndex(0)).toInt()+"\"";
  jsonData = addESPInfo(jsonData);
  sendToElasticsearch(jsonData);
  jsonData = "";
}
