#include <WebConfig.h>
#include <ESPmDNS.h>
#include <LocalStorage.h>

bool WebConfig::isInitialized = false;
WebServer *WebConfig::server = nullptr;

void WebConfig::initialize() {
  if(WebConfig::isInitialized) return;

  // Create web server instance
  WebConfig::server = new WebServer(80);

  // Initialize SPIFFS
  if (!SPIFFS.begin(true)) {
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }
  
  // Setup server mDNS
  MDNS.begin("gaia-dev");
  
  // Setup server endpoints
  WebConfig::server->on("/", ([](){ WebConfig::serve_web(0); }));
  WebConfig::server->on("/configure/wifi", ([](){ WebConfig::serve_web(1); }));
  WebConfig::server->on("/configure/style.css", ([](){ WebConfig::serve_web(2); }));

  // Start the server
  WebConfig::server->begin();
  Serial.println("[WebConfig] Server started");
  
  WebConfig::isInitialized = true;
}

void WebConfig::serve_web(uint8_t endpoint_code) {
  // Check if there's WiFi configuration
  if(endpoint_code == 1) {
    // If there's SSID and password as the arguments
    if(WebConfig::server->hasArg("ssid") && WebConfig::server->hasArg("pass")) {
      // Get the SSID and password value
      String ssid = WebConfig::server->arg("ssid");
      String pass = WebConfig::server->arg("pass");

      Serial.println();
      Serial.println("[WebConfig] +----- NEW CONFIG ----->");
      Serial.println("[WebConfig] |--> SSID:       " + ssid);
      Serial.println("[WebConfig] |--> Password:   " + pass);
      Serial.println("[WebConfig] +---------------------->");
      Serial.println();

      // Save the wifi creds
      LocalStorage::save_wifi_creds(ssid, pass);
    }
  }
  
  // Create a variable to hold the file data
  File file;
  String type = "text/html";
  
  // Change the file accordingly
  switch (endpoint_code)
  {
    case 0:
      file = SPIFFS.open("/index.html", "r");
      break;

    case 1:
      file = SPIFFS.open("/configure-wifi.html", "r");
      break;

    case 2:
      file = SPIFFS.open("/style.css", "r");
      type = "text/css";
      break;
    
    default:
      return;
  }

  // If there's no file
  if (!file) {
    WebConfig::server->send(404, "text/plain", "File not found");
    return;
  }

  // Stream the html
  WebConfig::server->streamFile(file, type);

  // Close the file after finish
  file.close();
}

void WebConfig::serve_clients() {
  WebConfig::server->handleClient();
}