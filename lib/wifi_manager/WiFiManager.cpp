#include <WiFiManager.h>
#include <LocalStorage.h>
#include <env.h>



//? Variable Definitions

bool WiFiManager::isInitialized = false;
bool WiFiManager::isConnected = false;
String WiFiManager::ssid = "";
String WiFiManager::pass = "";



//? Function Definitions

void WiFiManager::initialize() {
  if(WiFiManager::isInitialized) return;
  
  
  // Set Host Name
  WiFi.setHostname(ENV_DEVICE_NAME);
  
  // Set the WiFi mode
  WiFi.mode(WIFI_AP_STA);
  delay(100);
  
  // Get the SSID and password
  LocalStorage::load_wifi_creds(WiFiManager::ssid, WiFiManager::pass);
  
  if(WiFiManager::ssid != "") {
    // Begin connecting to the WiFi
    Serial.printf("[WiFi] Connecting to [ %s ]\n", WiFiManager::ssid.c_str());
    WiFi.begin(WiFiManager::ssid, WiFiManager::pass);
  }


  // Setup WiFi AP
  WiFi.softAPConfig(IPAddress(192,168,4,1), IPAddress(192,168,4,1), IPAddress(255,255,255,0));

  // Start WiFi AP
  WiFi.softAP(ENV_DEVICE_NAME, emptyString, 6); // Without password (automatically connect)
  

  // Set to initialized
  WiFiManager::isInitialized = true;
}

bool WiFiManager::is_connected() {
  bool connected = WiFi.isConnected();
  if(!WiFiManager::isConnected && connected) {
    Serial.println("[WiFi] Conencted!");
  }
  else if(WiFiManager::isConnected && !connected) {
    Serial.println("[WiFi] Disconnected!");
  }

  WiFiManager::isConnected = connected;
  
  return WiFiManager::isConnected;
}

void WiFiManager::reconnect(String ssid, String pass) {
  WiFi.disconnect();
  WiFi.begin(ssid, pass);
}

void WiFiManager::loop() {
  WiFiManager::is_connected();
}