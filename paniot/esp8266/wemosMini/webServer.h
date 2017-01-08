bool apiRelayState = false;

//format bytes
String formatBytes(size_t bytes){
  if (bytes < 1024){
    return String(bytes)+"B";
  } else if(bytes < (1024 * 1024)){
    return String(bytes/1024.0)+"KB";
  } else if(bytes < (1024 * 1024 * 1024)){
    return String(bytes/1024.0/1024.0)+"MB";
  } else {
    return String(bytes/1024.0/1024.0/1024.0)+"GB";
  }
}

String getContentType(String filename){
  if(server.hasArg("download")) return "application/octet-stream";
  else if(filename.endsWith(".htm")) return "text/html";
  else if(filename.endsWith(".html")) return "text/html";
  else if(filename.endsWith(".css")) return "text/css";
  else if(filename.endsWith(".js")) return "application/javascript";
  else if(filename.endsWith(".png")) return "image/png";
  else if(filename.endsWith(".xml")) return "text/xml";
  else if(filename.endsWith(".zip")) return "application/x-zip";
  else if(filename.endsWith(".gz")) return "application/x-gzip";
  return "text/plain";
}

String addESPInfo() {
  String jsonData = "";
  jsonData += "{\"freeHeap\": \""+String(ESP.getFreeHeap())+"\"";
  jsonData += ",\"resetReason\": \""+String(ESP.getResetReason())+"\"";
  jsonData += ",\"deviceName\": \""+deviceName+"\"";
  jsonData += ",\"deviceLocation\": \""+deviceLocation+"\"";
  jsonData += ",\"rssi\":\""+String(WiFi.RSSI())+"\"";
  jsonData += ",\"ip\":\""+String(WiFi.localIP())+"\"";
  jsonData += "}";
  return jsonData;
}

void webServerInit() {
  server.onNotFound([](){
     server.send(404, "text/plain", "FileNotFound");
  });
  server.on ( "/help", []() {
    String jsonData = "";
    jsonData += "{\"/help\": \"Dostupne endpointy\"";
    jsonData += ",\"/restart\": \"Restartuje zarizeni\"";
    jsonData += ",\"/factoryRestart\": \"Restartuje zarizeni do tovarniho nastaveni\"";
    jsonData += ",\"/getESPInfo\": \"Zakladni informace o zarizeni\"";
    jsonData += ",\"/getTemperature\": \"Vrati namerenou teplotu\"";
    jsonData += ",\"/getHumidity\": \"Povoli namerenou vlhkost\"";
    jsonData += ",\"/getLimitMeasure\": \"Vrati nastaveny interval mereni\"";
    jsonData += ",\"/setLimitMeasure?value=30\": \"Nastavy interval mereni\"";
    jsonData += ",\"/getDeviceLocation\": \"Vrati umisteni zarizeni\"";
    jsonData += ",\"/setDeviceLocation?value=zahrada\": \"Nastavy nazev umisteni zarizeni\"";
    jsonData += "}";
    server.send(200, "application/json", jsonData);
  } );
    server.on ( "/restart", []() {
    server.send ( 200, "text/plain", "Zarizeni bude restartovano." );
    ESP.reset();
    delay(1000);
  } );
  server.on ( "/factoryRestart", []() {
    server.send ( 200, "text/plain", "Bude nastaveno tovarni nastaveni. Vyhledejte wifi pojmenovanou " + deviceName );
    WiFi.disconnect(true);
    ESP.reset();
    delay(1000);
  } );
  server.on ( "/getESPInfo", []() {
    server.send(200, "application/json", addESPInfo());
  } );
  server.on ( "/getTemperature", []() {
    server.send(200, "application/json", "{\"temperature\": "+String(temperature)+"}");
  } );
  server.on ( "/getHumidity", []() {
    server.send(200, "application/json", "{\"humidity\": "+String(humidity)+"}");
  } );
  server.on ( "/getLimitMeasure", []() {
    server.send(200, "application/json", "{\"dellayTemperatureMS\": "+String(dellayTemperatureMS)+"}");
  } );
  server.on ( "/setLimitMeasure", []() {
    if(server.args() == 0) return server.send(500, "text/plain", "Chyba: musi se uvest hodnota v ms");
    dellayTemperatureMS = server.arg(0).toInt();
    server.send(200, "application/json", "{\"dellayTemperatureMS\": "+String(dellayTemperatureMS)+"}");
  } );
  server.on ( "/getDeviceLocation", []() {
    server.send(200, "application/json", "{\"deviceLocation\": \""+String(deviceLocation)+"\"}");
  } );
  server.on ( "/setDeviceLocation", []() {
    if(server.args() == 0) return server.send(500, "text/plain", "Chyba: musi se uvest hodnota lower-CamelCase");
    deviceLocation = server.arg(0);
    server.send(200, "application/json", "{\"deviceLocation\": \""+String(deviceLocation)+"\"}");
  } );
  
  server.begin();
}

