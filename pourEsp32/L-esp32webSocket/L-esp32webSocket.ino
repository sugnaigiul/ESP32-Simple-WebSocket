#include <WiFi.h>
#include <WebSocketsServer.h>

// Variables pour le WiFi
const char* ssid = "PoleDeVinci_IFT";
const char* password = "*c.r4UV@VfPn_0";

// Serveur WebSocket sur le port 81
WebSocketsServer webSocket = WebSocketsServer(81);

// Variables pour le potentiomètre
int potentioReading = 0;
unsigned long lastSendTime = 0;
const unsigned long sendInterval = 200; // Intervalle de 1 seconde pour envoyer les données

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

  // Lire la valeur du potentiomètre
  potentioReading = analogRead(A0);

  // Envoyer la valeur toutes les secondes
  if (millis() - lastSendTime > sendInterval) {
    String message = "Potentiometer:" + String(potentioReading);
    webSocket.broadcastTXT(message);  // Envoie à tous les clients connectés
    lastSendTime = millis();
  }
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