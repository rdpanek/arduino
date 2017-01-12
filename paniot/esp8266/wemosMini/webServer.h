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
  server.on ( "/allowRellay", []() {
    allowRellay = true;
    server.send(200, "application/json", "{\"allowRellay\": true}");
  } );
  server.on ( "/disableRellay", []() {
    allowRellay = false;
    server.send(200, "application/json", "{\"allowRellay\": false}");
  } );
  server.on ( "/allowAutoOff", []() {
    rellayAllowAutoOff = true;
    server.send(200, "application/json", "{\"allowAutoOff\": true}");
  } );
  server.on ( "/disableAutoOff", []() {
    rellayAllowAutoOff = false;
    server.send(200, "application/json", "{\"allowAutoOff\": false}");
  } );
  server.on ( "/getLimitAutoOff", []() {
    server.send(200, "application/json", "{\"dellayMS\": "+String(rellayDellayMS)+"}");
  } );
  server.on ( "/setLimitAutoOff", []() {
    if(server.args() == 0) return server.send(500, "text/plain", "Chyba: musi se uvest hodnota v ms");
    rellayDellayMS = server.arg(0).toInt();
    server.send(200, "application/json", "{\"dellayMS\": "+String(rellayDellayMS)+"}");
  } );
  char* _onEndpoint = &onEndpoint[0];
  server.on ( _onEndpoint, []() {
    if (allowRellay) {
      stateRellay = true;
      rellayLastOnMS = millis();
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

