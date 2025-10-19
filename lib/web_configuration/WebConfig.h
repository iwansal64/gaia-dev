#pragma once

#include <Arduino.h>
#include <WebServer.h>
#include <SPIFFS.h>

class WebConfig {
public:
  static WebServer* server;
  static String last_message;
  static bool isInitialized;
  
  /**
   * @brief Initialize the web service first
   * 
   */
  static void initialize();

  /**
   * @brief Handle the web serving
   * @param endpoint_code is the code that corresponds to an endpoint (ex: 0 = /, 1 = /configure/wifi)
   * 
   */
  static void serve_web(uint8_t endpoint_code);

  /**
   * @brief Serve clients
   * 
   * @attention Should called in the loop to make configuration works
   * 
   */
  static void serve_clients();
};
