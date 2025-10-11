//? Libraries
#include <Arduino.h>
#include <MQTTManager.h>
#include <SensorManager.h>
#include <WiFiManager.h>
#include <WebConfig.h>
#include <env.h>



//? Definitions
#define DATA_TRANSFER_RATE_PER_MINUTE 30 // 1 data per 2 seconds


//? Variables
uint64_t last_data_update = 0UL;


//? Function Definitions
void callback(char* topic, byte* payload, unsigned int length);


void setup() {
  //? Setup Serial
  Serial.begin(9600);

  
  //? Setup Sensors
  SensorManager::init();
  
  
  //? Setup WiFi
  WiFiManager::initialize();

  
  //? Setup MQTT
  MQTTManager::init("gaia-odc.digital", callback);
  MQTTManager::listen("PwlDq");


  //? Setup WebConfig
  WebConfig::initialize();
}

void loop() {
  MQTTManager::loop();
  WebConfig::serve_clients();

  uint64_t current_time = millis();
  if(current_time - last_data_update > (60 / DATA_TRANSFER_RATE_PER_MINUTE) * 1000) {
    
    float ec = SensorManager::get_ec();
    float ph = SensorManager::get_ph();
    uint32_t tds = SensorManager::get_tds();
    float temp = SensorManager::get_temp();

    String data = "ec=" + String(ec) + ";" +
                  "ph=" + String(ph) + ";" +
                  "tds=" + String(tds) + ";" +
                  "tempC=" + String(temp) + "";
    

    Serial.println("Sending Data...");
    
    MQTTManager::send("PwlDq", data.c_str());

    Serial.println("Data Sent!");

    last_data_update = current_time;
  }
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