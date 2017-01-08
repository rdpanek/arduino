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

void webServerInit() {
  server.onNotFound([](){
     server.send(404, "text/plain", "FileNotFound");
  });
    server.on ( "/restart", []() {
    server.send ( 200, "text/plain", "Zarizeni bude restartovano." );
    ESP.reset();
    delay(1000);
  } );
  server.on ( "/factoryrestart", []() {
    server.send ( 200, "text/plain", "Bude nastaveno tovarni nastaveni. Vyhledejte wifi pojmenovanou " + deviceName );
    WiFi.disconnect(true);
    ESP.reset();
    delay(1000);
  } );
  server.on ( "/rellayOn", []() {
    apiRelayState = true;
    server.send(200, "application/json", "{\"switchRellayStatus\": true}");
  } );
  server.on ( "/rellayOff", []() {
    apiRelayState = false;
    server.send(200, "application/json", "{\"switchRellayStatus\": true}");
  } );
  server.begin();
}

