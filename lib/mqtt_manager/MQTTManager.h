#pragma once

#include <Arduino.h>
#include <vector>
#include <ArduinoWebsockets.h>

using namespace websockets;


class MQTTManager {
private:
  static String url;
  static bool is_initialized;
  
public:
  static bool is_connected;
  static std::vector<String> listened_topics;

  /**
   * @brief Initialize MQTT connection to the url
   * @param url a valid URL that points to MQTT broker
   * 
   */
  static void init(const char* url, void (*callback)(WebsocketsMessage msg));


  /**
   * @brief Sending data through MQTT
   * @param topic the topic where the data should sent into
   * @param data the data that should be sent to the choosen topic
   * 
   */
  static void send(const char* topic, const char* data);


  /**
   * @brief Perform auto-reconnect, data monitoring and more
   * 
   */
  static void loop();

  
  /**
   * @brief send connect request to the MQTT broker
   * 
   */
  static void send_mqtt_connect();
};