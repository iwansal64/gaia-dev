#include <Arduino.h>
#include <MQTTManager.h>
#include <WiFi.h>
#include <env.h>

void callback(char* topic, byte* payload, unsigned int length);


void setup() {
  //? Setup Serial
  Serial.begin(9600);

  //? Setup WiFi
  WiFi.begin(ENV_SSID, ENV_PASS);
  Serial.println("Connecting");
  while(!WiFi.isConnected()) {
    Serial.print(" .");
    delay(1000);
  }
  Serial.print("\nConnected!\n");

  //? Setup MQTT
  MQTTManager::init("192.168.137.1", callback);
  MQTTManager::listen("PwlDq");
}

void loop() {
  MQTTManager::loop();
}


void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] <");
  for (unsigned int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println(">");
}