#include <Arduino.h>
#include <util.h>
#include <TDSSensor.h>

#define TDS_SENSOR_PIN 32           // Pin where TDS sensor connected
#define TDS_VREF 3.3                // analog reference voltage(Volt) of the ADC
#define TDS_SCOUNT  30              // sum of sample point

int tdsAnalogBuffer[TDS_SCOUNT];
int tdsAnalogBufferTemp[TDS_SCOUNT];
float tdsAverageVoltage = 0,tdsValue = 0,tdsTemperature = 25;
int tdsAnalogBufferIndex = 0,tdsCopyIndex = 0;
unsigned long tdsAnalogSampleTimepoint = 0;




float TDSSensor::get_tds() {
      for(tdsCopyIndex=0;tdsCopyIndex<TDS_SCOUNT;tdsCopyIndex++)                                                                                                        // Iterate for each TDS sensor reading
        tdsAnalogBufferTemp[tdsCopyIndex]=tdsAnalogBuffer[tdsCopyIndex];                                                                                                // 
      tdsAverageVoltage = getMedianNum(tdsAnalogBufferTemp,TDS_SCOUNT) * (float)TDS_VREF / 1024.0;                                                                      // Read the analog value more stable by the median filtering algorithm, and convert to voltage value
      float compensationCoefficient=1.0+0.02*(tdsTemperature-25.0);                                                                                                     // Temperature compensation formula: fFinalResult(25^C) = fFinalResult(current)/(1.0+0.02*(fTP-25.0));
      float compensationVolatge=tdsAverageVoltage/compensationCoefficient;                                                                                              // Temperature compensation
      tdsValue=(133.42*compensationVolatge*compensationVolatge*compensationVolatge - 255.86*compensationVolatge*compensationVolatge + 857.39*compensationVolatge)*0.5;  // Convert voltage value to tds value
      return tdsValue;                                                                                                                                                  // Return the TDS value
}


void TDSSensor::loop() {
      if(millis()-tdsAnalogSampleTimepoint > 40U) {                               // Every 40 milliseconds, read the analog value from the ADC
            tdsAnalogSampleTimepoint = millis();                                  // Update the last sample timepoint
            tdsAnalogBuffer[tdsAnalogBufferIndex] = analogRead(TDS_SENSOR_PIN);    // Read the analog value and store into the buffer
            tdsAnalogBufferIndex++;                                               // Increase the analog buffer index
            if(tdsAnalogBufferIndex >= TDS_SCOUNT) tdsAnalogBufferIndex = 0;      // If the analog bufer index equals to or more than the number of TDS
      }
}


