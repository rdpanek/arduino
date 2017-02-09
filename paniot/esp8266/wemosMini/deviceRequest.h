String targetDeviceIp = "0.0.0.0";

void sendGetRequest(String endpoint) {
  if(WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin("http://"+targetDeviceIp+endpoint);
    http.addHeader("Content-Type","text/plain; charset=UTF-8");
    int httpCode = http.GET();
    Serial.print("httpCode: ");
    Serial.println(httpCode);
    Serial.print("httpString: ");
    Serial.println(http.getString());
    if (httpCode > 300 || httpCode < 200) {
    } else {
    }
    http.end();
  } else {
  }
}
