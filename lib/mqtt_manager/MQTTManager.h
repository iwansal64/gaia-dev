#pragma once

#include <Arduino.h>
#include <vector>



class MQTTManager {
private:
  static String url;
  static uint16_t port;
  static bool is_initialized;
  static uint64_t last_time_reconnecting;
  
public:
  static bool is_connected;
  static std::vector<String> listened_topics;

  /**
   * @brief Initialize MQTT connection to the url
   * @param url a valid URL that points to MQTT broker
   * 
   */
  static void init(const char* url);


  /**
   * @brief Sending data through MQTT
   * @param topic the topic where the data should sent into
   * @param data the data that should be sent to the choosen topic
   * 
   */
  static bool send(const char* topic, const char* data);


  /**
   * @brief Perform auto-reconnect, data monitoring and more
   * 
   */
  static void loop();
};