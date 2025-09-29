#include <MQTTManager.h>
#include <PubSubClient.h>
#include <WiFiClient.h>


WiFiClient espClient;

PubSubClient MQTTManager::client = PubSubClient(espClient);
std::vector<String> MQTTManager::listened_topics = std::vector<String>();


void MQTTManager::init(const char* url, void (*callback)(char*, byte*, unsigned int)) {
  MQTTManager::client.setServer(url, 1883);
  MQTTManager::client.setCallback(callback);
}



void MQTTManager::loop() {
  if(!MQTTManager::client.connected()) {
    if(MQTTManager::client.connect("PwlDq", "iwan", "Iwaniwan123")) {
      Serial.println("MQTT Connected!");

      for(String topic : MQTTManager::listened_topics) {
        MQTTManager::client.subscribe(topic.c_str());
      }
    }
    else {
      Serial.println("There's an error when trying to connect to Web Socket");
      Serial.printf("Error Code: %d\n", MQTTManager::client.state());
      delay(1000);
      return;
    }
  }

  MQTTManager::client.loop();
}



bool MQTTManager::send(const char* topic, const char* data) {
  return MQTTManager::client.publish(topic, data);
}


void MQTTManager::listen(const char* topic) {
  MQTTManager::listened_topics.push_back(String(topic));
  
  if(MQTTManager::client.connected()) {
    MQTTManager::client.subscribe(topic);
  }
}