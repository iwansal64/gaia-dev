#pragma once

#include <Arduino.h>

class PHSensor
{
private:
public:
      static float get_ph();
      static void loop();
};

