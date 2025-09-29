#pragma once

#include <Arduino.h>
#include <PubSubClient.h>
#include <vector>

class MQTTManager {
public:
  static PubSubClient client;
  static std::vector<String> listened_topics;

  /**
   * @brief Initialize MQTT connection to the url
   * @param url a valid URL that points to MQTT broker
   * 
   */
  static void init(const char* url, void (*callback)(char*, byte*, unsigned int));


  /**
   * @brief Sending data through MQTT
   * @param topic the topic where the data should sent into
   * @param data the data that should be sent to the choosen topic
   * 
   * @return Whether the send attempt success or not
   */
  static bool send(const char* topic, const char* data);


  /**
   * @brief Perform auto-reconnect, data monitoring and more
   * 
   */
  static void loop();


  /**
   * @brief Add topic to the list of listened topics
   * 
   */
  static void listen(const char* topic);
};