/*
 *  This sketch demonstrates how to set up a simple HTTP-like server.
 *  The server will set a GPIO pin depending on the request
 *    http://server_ip/gpio/0 will set the GPIO2 low,
 *    http://server_ip/gpio/1 will set the GPIO2 high
 *  server_ip is the IP address of the ESP8266 module, will be 
 *  printed to Serial when the module is connected.
 */

#include <ESP8266WiFi.h>
#include <RCSwitch.h>

RCSwitch mySwitch = RCSwitch();

const char* ssid = "panek6";
const char* password = "panekSit";

// Create an instance of the server
// specify the port to listen on as an argument
WiFiServer server(80);

void setup() {
  Serial.begin(115200);
  delay(10);
  mySwitch.enableTransmit(5);

  // prepare GPIO2
  pinMode(4, OUTPUT);
  digitalWrite(4, 0);
  
  // Connect to WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    digitalWrite(4, HIGH);
    delay(500);
    Serial.print(".");
    digitalWrite(4, LOW);
  }
  Serial.println("");
  Serial.println("WiFi connected");
  
  // Start the server
  server.begin();
  Serial.println("Server started");

  // Print the IP address
  Serial.println(WiFi.localIP());
}

void loop() {
  // Check if a client has connected
  WiFiClient client = server.available();
  if (!client) {
    return;
  }
  
  // Wait until the client sends some data
  Serial.println("new client");
  while(!client.available()){
    delay(1);
  }
  
  // Read the first line of the request
  String req = client.readStringUntil('\r');
  Serial.println(req);
  client.flush();
  
  // Match the request
  int val;
  if (req.indexOf("/gpio/0") != -1) {
    val = 0;
    mySwitch.send(11548892, 24);
    delay(1000);
  } else if (req.indexOf("/gpio/1") != -1) {
    val = 1;
    mySwitch.send(11548893, 24);
    delay(1000);
  } else {
    Serial.println("invalid request");
    client.stop();
    return;
  }

  // Set GPIO2 according to the request
  digitalWrite(4, val);
  
  client.flush();

  // Prepare the response
  String s = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE HTML>\r\n<html>";
  //s += (val)?"high":"low";
  s += "<head><title>Wifi switcher</title><link href=\"https://maxcdn.bootstrapcdn.com/bootstrap/3.3.7/css/bootstrap.min.css\" rel=\"stylesheet\" integrity=\"sha384-BVYiiSIFeK1dGmJRAkycuHAHRg32OmUcww7on3RYdg4Va+PmSTsz/K68vbdEjh4u\" crossorigin=\"anonymous\">";
  s += "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">";
  s += "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1, maximum-scale=1, user-scalable=no\">";
  s += "</head>";
  s += "<body>";
  s += "<div class=\"jumbotron\">";
  s += "<h2>Wifi switcher</h2>";
  s += "<div class=\"panel panel-default\"><div class=\"panel-heading\"> Napajeni ";
  if (val == HIGH) {
    s += "<span class=\"label label-success\">Zapnuto</span>";  
  } else {
    s += "<span class=\"label label-danger\">Vypnuto</span>";
  }
  s += "</div><div class=\"panel-body\">";
  s += "<p><a href=\"/gpio/1\"><button type=\"button\" class=\"btn btn-success btn-lg btn-block\"";
  if (val == HIGH) { s += "disabled=\"disabled\""; } 
  s += ">Zapnout</button></a> <br/> <a href=\"/gpio/0\"><button type=\"button\" class=\"btn btn-danger btn-lg btn-block\"";
  if (val != HIGH) { s += "disabled=\"disabled\""; } 
  s += ">Vypnout</button></a></p>";
  s += "</div></div></div>";
  s += "</body></html>\n";

  // Send the response to the client
  client.print(s);
  delay(1);
  Serial.println("Client disonnected");

  // The client will actually be disconnected 
  // when the function returns and 'client' object is detroyed
}

