#include <MQTTManager.h>
#include <WiFiClient.h>
#include <PsychicMqttClient.h>
#include <env.h>
#include <WiFiManager.h>
#include <ServoManager.h>


extern const uint8_t mqtt_root_ca_pem_start[] asm("_binary_src_certs_mqtt_root_ca_pem_start");
extern const uint8_t mqtt_root_ca_pem_end[]   asm("_binary_src_certs_mqtt_root_ca_pem_end");


//? Useful Definition Helper

// Time before reconnecting to the MQTT broker
#define TIME_BEFORE_RECONNECT 15000


//? Variable Definitions

PsychicMqttClient mqtt_client;
WiFiClient espClient;

std::vector<String> MQTTManager::listened_topics = std::vector<String>();
bool MQTTManager::is_connected = false;
bool MQTTManager::is_initialized = false;
String MQTTManager::url = "";
uint16_t MQTTManager::port = 443;
uint64_t MQTTManager::last_time_reconnecting = 0;



//? Functions Helper

void on_fish(const char *topic, const char *payload, int retain, int qos, bool dup) {
  Serial.printf("[MQTT] Received Topic: %s\r\n", topic);
  Serial.printf("[MQTT] Received Payload: %s\r\n", payload); 
  ServoManager::trigger();
}

void on_mqtt_connect(bool sessionPresent) {
  Serial.println("[MQTT] MQTT Connected!");
}

void on_mqtt_disconnect(bool sessionPresent) {
  Serial.println("[MQTT] MQTT Disconnected!");
}

void on_mqtt_error(esp_mqtt_error_codes_t error) {
  Serial.println("[MQTT] There's an error!");
  switch(error.error_type) {
    case MQTT_ERROR_TYPE_CONNECTION_REFUSED:
      Serial.println("[MQTT] Error: Connection Refused!");
      break;
    case MQTT_ERROR_TYPE_TCP_TRANSPORT:
      Serial.println("[MQTT] Error: TCP Transport Error!");
      break;
    default:
      break;
  }
}


//? Functions Definitions

void MQTTManager::init(const char* url) {
  // If already initialized
  if(MQTTManager::is_initialized) return;

  // Set MQTT URL
  MQTTManager::url = url;
  MQTTManager::port = port;

  // If there's no WiFi
  if(!WiFiManager::is_connected()) return;
  

  //---- Setting up MQTT client  
  // Set MQTT broker server URL
  mqtt_client.setServer(url);

  // Set CA Certificate Bundle
  mqtt_client.setCACert((const char*) mqtt_root_ca_pem_start);

  // Set Credentials
  mqtt_client.setCredentials(ENV_DEVICE_MQTT_USER, ENV_DEVICE_MQTT_PASS);
  
  // Set Buffer Size
  mqtt_client.setBufferSize(1024);

  // Subscribe to particular MQTT topic
  String topic = String(ENV_DEVICE_ID) + "/fish";
  mqtt_client.onTopic(topic.c_str(), 0, on_fish);

  // Set MQTT auto reconnect interval
  mqtt_client.setAutoReconnect(TIME_BEFORE_RECONNECT);
  
  // Set MQTT callbacks
  mqtt_client.onConnect(on_mqtt_connect);
  mqtt_client.onDisconnect(on_mqtt_disconnect);
  mqtt_client.onError(on_mqtt_error);
  
  // Connect to the MQTT broker
  mqtt_client.connect();

  Serial.println("[MQTT] MQTT initialized");
  
  // Set MQTT to initialized!
  MQTTManager::is_initialized = true;
} 



void MQTTManager::loop() {
  // If the MQTT manager isn't initialized yet, wait for WiFi to connected
  if(!MQTTManager::is_initialized && WiFiManager::is_connected()) {
    MQTTManager::init(MQTTManager::url.c_str());
  }

  bool mqtt_status = mqtt_client.connected();
  if(MQTTManager::is_connected != mqtt_status) {
    MQTTManager::is_connected = mqtt_status;
  }
}

bool MQTTManager::send(const char* topic, const char* data) {
  // Send the message
  int32_t result = mqtt_client.publish(topic, 0, true, data);
  return result != -1;
}
