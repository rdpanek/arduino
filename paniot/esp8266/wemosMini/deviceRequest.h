void sendGetRequest(String endpoint) {
  if(WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(rellayUri+endpoint);
    http.addHeader("Content-Type","text/plain; charset=UTF-8");
    int httpCode = http.GET();
    Serial.println(httpCode);
    Serial.println(http.getString());
    if (httpCode > 300) {
      ledBlick(3,100);
    } else {
      ledBlick(1,10);  
    }
    http.end();
  } else {
    ledBlick(2,100);
  }
}
