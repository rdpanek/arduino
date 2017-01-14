// existUri("http://www.google.cz/", 200, "<title>Google</title>", multiLed1);

void logToElasticsearch(String uri, String expString, String httpCode, int expHTTPCode) {
  String webTester = "";
  webTester += "{\"uri\":\""+String(uri)+"\"";
  webTester += ",\"expString\":\""+String(expString)+"\"";
  webTester += ",\"httpCode\":\""+String(httpCode)+"\"";
  webTester += ",\"expHTTPCode\":";
  webTester += expHTTPCode;
  webTester += ",\"ntpDateTime\":\""+ntpDate+"T"+ntpTime+".000Z\"}";
  sendToElasticsearch(webTester);
  webTester = "";
}

void existUri(String uri, int expHTTPCode, String expString, int ledPosition) {
  if(WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    ledOn(true, ledPosition);

    http.begin(uri);
    http.setTimeout(2000);
    int httpCode = http.GET();
  
    if(httpCode > 0) {
      Serial.printf("[HTTP] GET... code: %d\n", httpCode);
  
      if (httpCode == expHTTPCode) {
        ledBlick(1,10,ledPosition);
      } else {
        beep(2,30);
        logToElasticsearch(uri, expString, String(httpCode), expHTTPCode);
        Serial.println(uri);
      }
    } else {
      beep(2,30);
      logToElasticsearch(uri, expString, http.errorToString(httpCode).c_str(), expHTTPCode);
      Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
      
    }

    http.end();
    ledOn(false, ledPosition);
  }  
}
