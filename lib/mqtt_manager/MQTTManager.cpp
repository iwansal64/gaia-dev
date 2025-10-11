#include <MQTTManager.h>
#include <WiFiClient.h>
#include <ArduinoWebsockets.h>
#include <env.h>

using namespace websockets;


//? Function helper

void onWebSocketEvent(WebsocketsEvent event, String data){
  if (event == WebsocketsEvent::ConnectionClosed) {
    Serial.println("[MQTT] Connection closed by server or lost");
    MQTTManager::is_connected = false;
  }
  else if(event == WebsocketsEvent::ConnectionOpened) {
    Serial.println("[MQTT] Connection opened by the server!");
    MQTTManager::is_connected = true;
  }
}




//? Variable Definitions

WebsocketsClient ws_client;
WiFiClient espClient;

std::vector<String> MQTTManager::listened_topics = std::vector<String>();
bool MQTTManager::is_connected = false;
bool MQTTManager::is_initialized = false;
String MQTTManager::url = "";



//? Functions Definitions

void MQTTManager::init(const char* url, void (*callback)(WebsocketsMessage msg)) {
  if(MQTTManager::is_initialized) return;
  
  MQTTManager::url = url;
  
  // Set callbacks
  ws_client.onMessage(callback);
  ws_client.onEvent(onWebSocketEvent);
  
  // Connect to the MQTT server
  if(ws_client.connect(url, 443, "/")) {
    MQTTManager::is_connected = true;
  }

  // After setting up WS client, send mqtt connection request
  MQTTManager::send_mqtt_connect();

  // Set MQTT to initialized!
  MQTTManager::is_initialized = true;
}



void MQTTManager::loop() {
  if(!ws_client.available()) {
    // Triggered once per disconnection
    if(MQTTManager::is_connected) {
      Serial.println("[MQTT] MQTT Disconnected!");
      
      if(ws_client.connectSecure(MQTTManager::url, 443, "/")) {
        Serial.println("MQTT Connected!");

        // For future updates 
        // for(String topic : MQTTManager::listened_topics) {
        //   MQTTManager::subscribe(topic.c_str());
        // }
      }
      else {
        Serial.println("There's an error when trying to connect to Web Socket");
        delay(1000);
        return;
      }

      MQTTManager::is_connected = false;
    }
  }
  else {
    // Triggered once per connection
    if(!MQTTManager::is_connected) {
      Serial.println("[MQTT] MQTT Connected!");

      MQTTManager::is_connected = true;
    }
  }

  ws_client.poll();
}



void MQTTManager::send(const char* topic, const char* data) {
  if(!MQTTManager::is_connected) return;
  
  uint8_t packet[256];
  int idx = 0;

  packet[idx++] = 0x30;  // PUBLISH command
  packet[idx++] = 0;     // Remaining length (to fill later)

  uint16_t topicLen = strlen(topic);
  packet[idx++] = topicLen >> 8;
  packet[idx++] = topicLen & 0xFF;
  memcpy(packet + idx, topic, topicLen);
  idx += topicLen;

  int msgLen = strlen(data);
  memcpy(packet + idx, data, msgLen);
  idx += msgLen;

  packet[1] = idx - 2;
  ws_client.sendBinary((const char*)packet, idx);

  Serial.print("Published: ");
  Serial.println(data);
}



void MQTTManager::send_mqtt_connect() {
  // Build a simple MQTT CONNECT packet (MQTT v3.1.1)
  const char* clientId = ENV_DEVICE_ID;

  uint8_t packet[128];
  int idx = 0;

  packet[idx++] = 0x10;  // CONNECT control packet type
  packet[idx++] = 0;     // Remaining length (to fill later)
  
  // Protocol name "MQTT"
  packet[idx++] = 0x00; packet[idx++] = 0x04;
  packet[idx++] = 'M'; packet[idx++] = 'Q'; packet[idx++] = 'T'; packet[idx++] = 'T';
  packet[idx++] = 0x04;  // Protocol level 4 = MQTT 3.1.1
  packet[idx++] = 0x02;  // Connect flags: clean session
  packet[idx++] = 0x00; packet[idx++] = 0x3C; // Keep alive = 60s

  // Client ID
  uint16_t idLen = strlen(clientId);
  packet[idx++] = idLen >> 8;
  packet[idx++] = idLen & 0xFF;
  memcpy(packet + idx, clientId, idLen);
  idx += idLen;

  // Fill remaining length
  packet[1] = idx - 2;

  ws_client.sendBinary((const char*)packet, idx);
  Serial.println("MQTT CONNECT sent!");
}
