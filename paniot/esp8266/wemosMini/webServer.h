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
    jsonData += ",\"/getDeviceLocation\": \"Vrati umisteni zarizeni\"";
    jsonData += ",\"/setDeviceLocation?value=zahrada\": \"Nastavy nazev umisteni zarizeni\"";
    jsonData += ",\"/getDeviceName\": \"Vrati nazev zarizeni\"";
    jsonData += ",\"/setDeviceName?value=dht22\": \"Nastavy nazev zarizeni\"";
    jsonData += ",\"/allowSendToElasticsearch\": \"Povoli odesilani namerene teploty a vlhkosti do Elasticsearch\"";
    jsonData += ",\"/disableSendToElasticsearch\": \"Zakaze odesilani dat do Elasticsearch\"";
    jsonData += ",\"/getElasticsearchUri\": \"Vrati URI pro odesilani dat do Elasticsearch\"";
    jsonData += ",\"/setElasticsearchUri?value=192.168.1.246:9200\": \"Nastavi uri pro odesilani dat do Elasticsearch\"";
    jsonData += ",\"/loadConfiguration\": \"Nahraje ulozene nastaveni\"";
    jsonData += ",\"/saveConfiguration\": \"Ulozi hodnotu promenych, ktere se daji nastavovat do konfiguracniho souboru\"";
    jsonData += ",\"/getTargetDeviceIp\": \"Vrati IP ciloveho zarizeni, kde bude zavolan endpoint v pripade aktivniho PIR senzoru\"";
    jsonData += ",\"/setTargetDeviceIp?value=192.168.1.246\": \"Nastavi IP ciloveho zarizeni\"";
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
  server.on ( "/getDeviceLocation", []() {
    server.send(200, "application/json", "{\"deviceLocation\": \""+String(deviceLocation)+"\"}");
  } );
  server.on ( "/setDeviceLocation", []() {
    if(server.args() == 0) return server.send(500, "text/plain", "Chyba: musi se uvest hodnota lower-CamelCase");
    deviceLocation = server.arg(0);
    server.send(200, "application/json", "{\"deviceLocation\": \""+String(deviceLocation)+"\"}");
  } );
  server.on ( "/getDeviceName", []() {
    server.send(200, "application/json", "{\"deviceName\": \""+String(deviceName)+"\"}");
  } );
  server.on ( "/setDeviceName", []() {
    if(server.args() == 0) return server.send(500, "text/plain", "Chyba: musi se uvest hodnota lower-CamelCase");
    deviceName = server.arg(0);
    server.send(200, "application/json", "{\"deviceName\": \""+String(deviceName)+"\"}");
  } );
  server.on ( "/allowSendToElasticsearch", []() {
    allowSendToElasticsearch = true;
    server.send(200, "application/json", "{\"allowSendToElasticsearch\": \""+String(allowSendToElasticsearch)+"\"}");
  } );
  server.on ( "/disableSendToElasticsearch", []() {
    allowSendToElasticsearch = false;
    server.send(200, "application/json", "{\"allowSendToElasticsearch\": \""+String(allowSendToElasticsearch)+"\"}");
  } );
  server.on ( "/getElasticsearchUri", []() {
    server.send(200, "application/json", "{\"elasticsearchUri\": \""+String(elasticsearchUri)+"\"}");
  } );
  server.on ( "/setElasticsearchUri", []() {
    if(server.args() == 0) return server.send(500, "text/plain", "Chyba: musi se uvest hodnota uri");
    elasticsearchUri = server.arg(0);
    server.send(200, "application/json", "{\"elasticsearchUri\": \""+String(elasticsearchUri)+"\"}");
  } );
  server.on ( "/loadConfiguration", []() {
    loadConfig();
    server.send(200, "application/json", "{\"configuration\":"+String(configuration)+"}");
  } );
  server.on ( "/saveConfiguration", []() {
    saveConfig();
    server.send(200, "application/json", "{\"configuration\":"+String(configuration)+"}");
  } );
    server.on ( "/getTargetDeviceIp", []() {
    server.send(200, "application/json", "{\"targetDeviceIp\": \""+String(targetDeviceIp)+"\"}");
  } );
  server.on ( "/setTargetDeviceIp", []() {
    if(server.args() == 0) return server.send(500, "text/plain", "Chyba: musi se uvest hodnota ip");
    targetDeviceIp = server.arg(0);
    server.send(200, "application/json", "{\"targetDeviceIp\": \""+String(targetDeviceIp)+"\"}");
  } );
  
  server.begin();
}

