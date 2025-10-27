#pragma once

class TDSSensor
{
private:
public:
      /**
       * @brief Get TDS value
       * 
       */
      static float get_tds();

      /**
       * @brief Run the TDS sensor
       * 
       */
      static void loop();
};

