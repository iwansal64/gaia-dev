//? Libraries
#include <Arduino.h>            // Built-in library for using essentials basic microcontroller code (Serial, pinMode, etc..)
#include <MQTTManager.h>        // Custom library for managing ESP32 to be MQTT client
#include <SensorManager.h>      // Custom library for managing sensors
#include <WiFiManager.h>        // Custom library for managing the way ESP32 handle WiFi connection
#include <WebConfig.h>          // Custom library for managing the way ESP32 handle configuration from the user through local website
#include <ServoManager.h>       // Custom library for managing servo motor as actuator
#include <env.h>                // Custom library for managing secrets (must created manually if you've just cloned the repository in this path -> lib/env/env.h)



//? Definitions
#define DATA_TRANSFER_RATE_PER_MINUTE 1 // example: 30. Means 1 data per 2 seconds or 30 data per minute


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
  WiFiManager::loop();
  ServoManager::loop();
  WebConfig::serve_clients();

  uint64_t current_time = millis();
  if(WiFiManager::is_connected && MQTTManager::is_connected && current_time - last_data_update > (60 / DATA_TRANSFER_RATE_PER_MINUTE) * 1000) {    
    // Get all of the data
    float ec = SensorManager::get_ec();
    float ph = SensorManager::get_ph();
    uint32_t tds = SensorManager::get_tds();
    float temp = SensorManager::get_temp();

    // Sending the data through MQTT connection
    Serial.println("Sending Data...");
    
    bool result = MQTTManager::send("PwlDq/ec", String(ec).c_str());
    result = MQTTManager::send("PwlDq/ph", String(ph).c_str()) && result;
    result = MQTTManager::send("PwlDq/tds", String(tds).c_str()) && result;
    result = MQTTManager::send("PwlDq/tempC", String(temp).c_str()) && result;

    // Check the result
    if(result) {
      Serial.println("Data Successfully Sent! :)");
    }
    else {
      Serial.println("Data Failed Sent! :(");
    }

    // Update the last time updating data
    last_data_update = current_time;
  }
}

