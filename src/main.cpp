//? Libraries
#include <Arduino.h>
#include <MQTTManager.h>
#include <SensorManager.h>
#include <WiFiManager.h>
#include <WebConfig.h>
#include <ServoManager.h>
#include <env.h>



//? Definitions
#define DATA_TRANSFER_RATE_PER_MINUTE 1 // 1 data per 2 seconds


//? Variables
uint64_t last_data_update = 0UL;


void setup() {
  //? Setup Serial
  Serial.begin(9600);

  
  //? Setup Sensors
  SensorManager::init();
  
  
  //? Setup WiFi
  WiFiManager::initialize();

  
  //? Setup MQTT
  MQTTManager::init("wss://mqttws.gaia-odc.digital:443");


  //? Setup WebConfig
  WebConfig::initialize();


  //? Setup Servo
  ServoManager::init();
}

void loop() {
  MQTTManager::loop();
  WebConfig::serve_clients();
  WiFiManager::loop();
  ServoManager::loop();

  uint64_t current_time = millis();
  if(WiFiManager::is_connected() && MQTTManager::is_connected && current_time - last_data_update > (60 / DATA_TRANSFER_RATE_PER_MINUTE) * 1000) {    
    float ec = SensorManager::get_ec();
    float ph = SensorManager::get_ph();
    uint32_t tds = SensorManager::get_tds();
    float temp = SensorManager::get_temp();

    Serial.println("Sending Data...");
    
    bool result = MQTTManager::send("PwlDq/ec", String(ec).c_str());
    result = MQTTManager::send("PwlDq/ph", String(ph).c_str()) && result;
    result = MQTTManager::send("PwlDq/tds", String(tds).c_str()) && result;
    result = MQTTManager::send("PwlDq/tempC", String(temp).c_str()) && result;

    if(result) {
      Serial.println("Data Successfully Sent! :)");
    }
    else {
      Serial.println("Data Failed Sent! :(");
    }

    last_data_update = current_time;
  }
}

