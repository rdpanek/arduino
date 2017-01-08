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
    jsonData += ",\"/getStateRellay\": \"Vrati stav rele\"";
    jsonData += ",\"/allowRellay\": \"Povoli pouziti rele pomoci REST-API. Pokud rele neni povoleno, je vracen http status code 202 a rele muze ovladat pouze vlastni zarizeni.\"";
    jsonData += ",\"/disableRellay\": \"Zakaze pouziti rele\"";
    jsonData += ",\""+onEndpoint+"\": \"Zapne rele\"";
    jsonData += ",\""+offEndpoint+"\": \"Vypne rele\"";
    jsonData += ",\"/allowAutoOff\": \"Povoli automaticke vypnuti rele, pokud neprijde po nastavenou dobu prikaz k zapnuti rele\"";
    jsonData += ",\"/disableAutoOff\": \"Zakaze automaticke vypnuti rele. Rele bude zapnute do doby, nez bude prikazem vypnuto nebo pokud se nepovoli automaticke vypnuti\"";
    jsonData += ",\"/getLimitAutoOff\": \"Vrati nastaveny limit pro automaticke vypnuti\"";
    jsonData += ",\"/setLimitAutoOff?value=30\": \"Nastavy limit v ms pro automaticke vypnuti\"";
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
  server.on ( "/allowRellay", []() {
    allowRellay = true;
    server.send(200, "application/json", "{\"allowRellay\": true}");
  } );
  server.on ( "/disableRellay", []() {
    allowRellay = false;
    server.send(200, "application/json", "{\"allowRellay\": false}");
  } );
  server.on ( "/allowAutoOff", []() {
    allowAutoOff = true;
    server.send(200, "application/json", "{\"allowAutoOff\": true}");
  } );
  server.on ( "/disableAutoOff", []() {
    allowAutoOff = false;
    server.send(200, "application/json", "{\"allowAutoOff\": false}");
  } );
  server.on ( "/getLimitAutoOff", []() {
    server.send(200, "application/json", "{\"dellayMS\": "+String(dellayMS)+"}");
  } );
  server.on ( "/setLimitAutoOff", []() {
    if(server.args() == 0) return server.send(500, "text/plain", "Chyba: musi se uvest hodnota v ms");
    dellayMS = server.arg(0).toInt();
    server.send(200, "application/json", "{\"dellayMS\": "+String(dellayMS)+"}");
  } );
  char* _onEndpoint = &onEndpoint[0];
  server.on ( _onEndpoint, []() {
    if (allowRellay) {
      stateRellay = true;
      lastOnMS = millis();
      server.send(200, "application/json", "{\"stateRellay\": true}");
    } else {
      server.send(202, "application/json", "{\"stateRellay\": false}");
    }
  } );
  char* _offEndpoint = &offEndpoint[0];
  server.on ( _offEndpoint, []() {
    if (allowRellay) {
      stateRellay = false;
      server.send(200, "application/json", "{\"stateRellay\": false}");
    } else {
      server.send(202, "application/json", "{\"stateRellay\": false}");
    }
  } );
  server.on ( "/getStateRellay", []() {
    String _stateRellayJson = "{\"stateRelay\": ";
    _stateRellayJson += stateRellay;
    _stateRellayJson += "}";
    server.send(200, "application/json", _stateRellayJson);
  } );
  server.begin();
}

