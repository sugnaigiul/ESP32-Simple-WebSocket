#include <WiFi.h>
#include <WebSocketsServer.h>

const char* ssid = "PoleDeVinci_IFT";
const char* password = "*c.r4UV@VfPn_0";

// Serveur WebSocket sur le port 81
WebSocketsServer webSocket = WebSocketsServer(81);

void setup() {
  Serial.begin(115200);
  Serial.println("Program start.");
  pinMode(LED_BUILTIN, OUTPUT);
  
  // Connexion au WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connexion au WiFi...");
  }
  Serial.println("WiFi connecté. Adresse IP : ");
  Serial.println(WiFi.localIP());
  
  // Démarrer le serveur WebSocket
  webSocket.begin();
  webSocket.onEvent(webSocketEvent);
}

void loop() {
  webSocket.loop();
}

// Fonction de gestion des événements WebSocket
void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length) {
  if (type == WStype_TEXT) {
    String message = String((char*)payload);

    if (message == "LED_ON") {
      digitalWrite(LED_BUILTIN, LOW);
      Serial.println("Led ON");
      webSocket.sendTXT(num, "LED allumée");
    } else if (message == "LED_OFF") {
      digitalWrite(LED_BUILTIN, HIGH);
      Serial.println("Led OFF");
      webSocket.sendTXT(num, "LED éteinte");
    }
  }
}
