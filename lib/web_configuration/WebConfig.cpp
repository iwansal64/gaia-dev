#include <WebConfig.h>
#include <ESPmDNS.h>
#include <LocalStorage.h>
#include <APIManager.h>
#include <WiFiManager.h>
#include <HTTPUpdateServer.h>


bool WebConfig::isInitialized = false;
WebServer *WebConfig::server = nullptr;
String WebConfig::last_message = "";

WebServer sync_server(8080);
HTTPUpdateServer http_ota_updater;
uint64_t last_ota_progress_update = 0UL;

void WebConfig::initialize() {
  if(WebConfig::isInitialized) return;

  // Create web server instance
  WebConfig::server = new WebServer(80);

  // Initialize SPIFFS
  if (!SPIFFS.begin(true)) {
    Serial.println("[SPIFFS] An Error has occurred while mounting SPIFFS");
    return;
  }
  
  // Setup server mDNS
  MDNS.begin("gaia-dev");
  
  // Setup OTA server
  http_ota_updater.setup(&sync_server);
  Serial.print("[OTA] Async OTA started!\n");

  sync_server.begin();
  Serial.print("[OTA] Web servers started!\n");
  
  // Setup server endpoints
  WebConfig::server->on("/", ([](){ WebConfig::serve_web(0); }));
  WebConfig::server->on("/configure/style.css", ([](){ WebConfig::serve_web(1); }));
  WebConfig::server->on("/configure/wifi", ([](){ WebConfig::serve_web(2); }));
  WebConfig::server->on("/configure/security", ([](){ WebConfig::serve_web(3); }));
  WebConfig::server->on("/status-notifier.js", ([](){ WebConfig::serve_web(4); }));
  WebConfig::server->on("/status", ([](){ WebConfig::serve_web(5); }));

  // Start the server
  WebConfig::server->begin();
  Serial.println("[WebConfig] Server started");
  
  WebConfig::isInitialized = true;
}

void WebConfig::serve_web(uint8_t endpoint_code) {
  Serial.printf("[WebConfig] Endpoint Code: %d\n", endpoint_code);
  
  // If the endpoint is status notifier
  if(endpoint_code == 5) {
    WebConfig::server->send(200, "text/plain", WebConfig::last_message);
    WebConfig::last_message = "";
  }
  
  
  // Check if there's WiFi configuration
  if(endpoint_code == 2) {
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

      // Reconnect to the WiFi
      WiFiManager::reconnect(ssid, pass);

      // Set message
      WebConfig::last_message = "Success!";
    }
  }
  else if(endpoint_code == 3) {
    // If there's SSID and password as the arguments
    if(WebConfig::server->hasArg("user_id")) {
      // Get the SSID and password value
      String user_id = WebConfig::server->arg("user_id");

      Serial.println();
      Serial.println("[WebConfig] +----- NEW CONFIG ----->");
      Serial.println("[WebConfig] |--> USER ID:       " + user_id);
      Serial.println("[WebConfig] +---------------------->");
      Serial.println();

      // Save the wifi creds
      APIResult result = APIManager::change_user_id(user_id);
      switch (result)
      {
        case API_OK:
          Serial.println("[WebConfig] User has been successfully added!");
          WebConfig::last_message = "Success!";
          break;
        
        case API_Unauthorized:
          Serial.println("[WebConfig] Unauthorized!");
          WebConfig::last_message = "Unauthorized! Please, contact developer for more information";
          break;
          
        case API_Error:
          Serial.println("[WebConfig] There's an unknown error!");
          WebConfig::last_message = "There's an unknown error!";
          break;
          
        case API_NoWIFI:
          Serial.println("[WebConfig] WiFi is not connected!");
          WebConfig::last_message = "WiFi is not connected";
          break;

        case API_NotFound:
          Serial.println("[WebConfig] User ID is not found!");
          WebConfig::last_message = "User ID not found!";
          break;

        default:
          break;
      }
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
      file = SPIFFS.open("/style.css", "r");
      type = "text/css";
      break;

    case 2:
      file = SPIFFS.open("/configure-wifi.html", "r");
      break;

    case 3:
      file = SPIFFS.open("/configure-security.html", "r");
      break;
      
    case 4:
      file = SPIFFS.open("/status-notifier.js", "r");
      type = "text/javascript";
      break;
    
    default:
      WebConfig::server->send(404, "text/plain", "Endpoint not set");
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