#pragma once

#include <Arduino.h>

class LocalStorage {
public:
  static bool isOpened;

  /**
   * @brief Open Local Storage
   * 
   */
  static void open();

  /**
   * @brief Close Local Storage
   * 
   */
  static void close();

  /**
   * @brief Grab foo- oh I mean... Grab pizza! Oh- I mean data!
   * @param key is a key that holds the value you want to get!
   * @return value that holds information from the key value pair
   * 
   */
  static String grab(const char *key);

  /**
   * @brief Put data to the fridg- Oh I mean storage!
   * @param key is a key that the value will be labeled
   * @param value is the value that will contained in the key
   * 
   */
  static void put(const char *key, String value); 

  /**
   * @brief Getting WiFi credentials from local storage
   * @param ssid_container is a variable that will contains the WiFi SSID
   * @param pass_container is a variable that will contains the WiFi password
   * 
   */
  static void load_wifi_creds(String &ssid_container, String &pass_container);

  /**
   * @brief Set WiFi credentials to the local storage
   * @param new_ssid is a variable that contains the value of new WiFi SSID
   * @param new_pass is a variable that contains the value of new WiFi password
   * 
   */
  static void save_wifi_creds(String new_ssid, String new_pass);
};