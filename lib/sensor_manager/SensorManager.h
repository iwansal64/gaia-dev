#pragma once

#include <Arduino.h>

class SensorManager {
private:
public:
  /**
   * @brief Initialize all sensors
   * 
   */
  static void init();
  
  /**
   * @brief Get current pH value of the water
   * 
   */
  static float get_ph();

  /**
   * @brief Get current temperature value of the water
   * 
   */
  static float get_temp();

  /**
   * @brief Get the Total Dissolved Solids value of the water 
   * 
   */
  static uint32_t get_tds();

  /**
   * @brief Get the current Electrical Conductivity value of the water
   * 
   */
  static float get_ec();

  /**
   * @brief Runs loop program
   * 
   */
  static void loop();
};