#pragma once

#include <Arduino.h>
#include <WiFi.h>

class WiFiManager {
public:
  static bool isInitialized;
  static bool isConnected;

  /**
   * @brief Initialize WiFi connection
   * 
   */
  static void initialize();


  /**
   * @brief Check if connected to the WiFi or not
   * 
   */
  static bool is_connected();
};