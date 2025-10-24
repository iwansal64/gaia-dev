#pragma once

// Dependencies
#include <Arduino.h>

class ServoManager
{
private:
public:
      static bool isInitialized;
      static uint64_t lastTimeOpeningLid;
      static bool isOpen;

      /**
       * @brief Initialize servo
       *
       */
      static void init();

      /**
       * @brief Trigger opening servo and then automatically closing it
       *
       */
      static void trigger();

      /**
       * @brief Used to make this works asynchronously
       *
       */
      static void loop();
};
