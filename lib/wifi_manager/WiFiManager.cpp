#include <WiFiManager.h>
#include <LocalStorage.h>
#include <env.h>

bool WiFiManager::isInitialized = false;
bool WiFiManager::isConnected = false;


void WiFiManager::initialize() {
  // Set the WiFi mode
  WiFi.mode(WIFI_AP_STA);
  

  // Start WiFi AP
  WiFi.softAP(ENV_DEVICE_NAME); // Without password (automatically connect)
  
  
  // Get the SSID and password
  String ssid = "";
  String password = "";

  LocalStorage::get_wifi_creds(ssid, password);
  
  if(ssid != "") {
    // Begin connecting to the WiFi
    WiFi.begin(ssid, password);
  }


  // Set to initialized
  WiFiManager::isInitialized = true;
}

bool WiFiManager::is_connected() {
  WiFiManager::isConnected = WiFi.isConnected();
  return WiFiManager::isConnected;
}