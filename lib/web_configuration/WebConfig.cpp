#include <WebConfig.h>
#include <WiFi.h>
#include <ESPmDNS.h>

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
  Serial.println("Server started");
  
  WebConfig::isInitialized = true;
}

void WebConfig::serve_web(uint8_t endpoint_code) {
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