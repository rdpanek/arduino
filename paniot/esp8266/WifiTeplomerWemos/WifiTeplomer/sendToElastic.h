
void send(String jsonData) {
  if(WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin("http://"+elasticsearchUri);
    http.addHeader("Content-Type","application/json; charset=UTF-8");
    int httpCode = http.POST(jsonData);
    if (httpCode > 300) { 
      displayMessage("Elastic", "chyba", 300);
      blick(3,30);
    }
    jsonData = "";
    //DBG_OUTPUT_PORT.println(jsonData);
    http.end();
  }
}

String addESPInfo(String jsonData) {
  jsonData += ",\"freeHeap\": "+String(ESP.getFreeHeap());
  jsonData += ",\"resetReason\": \""+ESP.getResetReason()+"\"";
  jsonData += ",\"rssi\":\""+String(rssi)+"\"";
  jsonData += ",\"ip\":\""+String(localIP)+"\"";
  jsonData += ",\"macAddress\":\""+String(macAddress)+"\"";
  jsonData += "}";
  return jsonData;
}

void sendTemperatureToElasticsearch() {
  if (nazevZarizeni.length() == 0) { 
    nazevZarizeni = "Nepojmenovane cidlo PanIoT";
  }
  jsonData += "{\"nazevZarizeni\": \""+String(nazevZarizeni)+"\"";
  jsonData += ",\"senzor\": \"teplomer\"";
  jsonData += ",\"hodnota\": \"";
  jsonData += String(teplota);
  jsonData += "\"";
  jsonData = addESPInfo(jsonData);
  
  send(jsonData);
  jsonData = "";
}

void sendLdrToElasticsearch() {
  if (nazevZarizeni.length() == 0) { 
    nazevZarizeni = "Nepojmenovane cidlo PanIoT";
  }
  jsonData = "{\"nazevZarizeni\": \""+String(nazevZarizeni)+"\"";
  jsonData += ",\"senzor\": \"ldr\"";
  jsonData += ",\"hodnota\": \"";
  jsonData += String(ldr);
  jsonData += "\"";
  jsonData = addESPInfo(jsonData);

  send(jsonData);
  jsonData = "";
}
