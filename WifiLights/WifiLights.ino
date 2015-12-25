#include <ESP8266WiFi.h>
#include <WebSocketClient.h>

void(* resetFunc) (void) = 0;

const char* ssid     = "Sherlock";
const char* password = "Highlander";

char path[] = "/io";
char host[] = "192.168.1.27";

bool state;

WebSocketClient webSocketClient;

// Use WiFiClient class to create TCP connections
WiFiClient client;

bool handshake() {
  digitalWrite(13, LOW);
  if (client.connect(host, 8080)) {
    Serial.println("Connected");
  } else {
    Serial.println("Connection failed.");
    return false;
  }
  
  // Handshake with the server
  webSocketClient.path = path;
  webSocketClient.host = host;
  if (webSocketClient.handshake(client)) {
    Serial.println("Handshake successful");
  } else {
    Serial.println("Handshake failed.");
    return false;
  }
  webSocketClient.sendData("start");
  return true;
}

void setup() {
  Serial.begin(115200);
  pinMode(0, OUTPUT);
  pinMode(13, OUTPUT);
  delay(100);
 
  // We start by connecting to a WiFi network
 
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
 
  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  delay(5000);

  while(!handshake()) {
    delay(5000);
  }
}
 
void loop() {
  String data;
 
  if (client.connected()) {
    
    webSocketClient.getData(data);
    if (data.length() > 0) {
      Serial.print("Received data: ");
      Serial.println(data);
      if(data == "on") {
        Serial.println("ON");
        digitalWrite(13, HIGH);
      } else if (data == "off") {
        Serial.println("OFF");
        digitalWrite(13, LOW);
      }
      else if (data == "toggle") {
        Serial.println("TOGGLE");
        state = !state;
        digitalWrite(13, state);
      }
    }
    
  } else {
    Serial.println("Client disconnected.");
    while(!handshake()) {
      delay(5000);
    }
  }
}
