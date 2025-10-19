#pragma once

#include <Arduino.h>
#include <WiFi.h>

class WiFiManager {
public:
  static String ssid;
  static String pass;
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

  /**
   * @brief Reconnect WiFi with new WiFi credentials
   * 
   */
  static void reconnect(String ssid, String pass);

  /**
   * @brief Run the function on the loop
   * 
   */
  static void loop();
};