#pragma once

#include <Arduino.h>

enum APIResult {
  API_OK,
  API_Error,
  API_Unauthorized,
  API_Conflict,
  API_NotFound,
  API_NoWIFI
};

class APIManager {
public:
  /**
   * @brief Send API tto change user ID of this GAIA device
   * 
   */
  static APIResult change_user_id(String user_id);

};