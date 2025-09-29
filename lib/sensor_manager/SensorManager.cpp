#include <Arduino.h>
#include <SensorManager.h>

void SensorManager::init() {
  // Setup Pins and other things here
  
}

float SensorManager::get_ec() {
  // Get the EC value
  float ec_value = random(5, 30) / 10.0f; // random from 0.5 - 3 with 1 decimal precision

  return ec_value;
}

float SensorManager::get_ph() {
  // Get the pH value
  float ph_value = random(200, 700) / 100.0f; // random from 2 - 7 with 2 decimal precision

  return ph_value;
}

uint32_t SensorManager::get_tds() {
  // Get the TDS
  float tds_value = random(300, 800); // random from 300 - 800

  return tds_value;
}

float SensorManager::get_temp() {
  // Get the temperature value
  float temp_value = random(100, 300) / 10.0f; // random from 10 - 30 with 1 decimal precision

  return temp_value;
}