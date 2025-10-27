#include <Arduino.h>
#include <TDSSensor.h>

#define TDS_SensorPin 32      // Pin where TDS sensor connected
#define TDS_VREF 3.3             // analog reference voltage(Volt) of the ADC
#define TDS_SCOUNT  30           // sum of sample point

int tdsAnalogBuffer[TDS_SCOUNT];    // store the analog value in the array, read from ADC
int tdsAnalogBufferTemp[TDS_SCOUNT];
int tdsAnalogBufferIndex = 0,tdsCopyIndex = 0;
float tdsAverageVoltage = 0,tdsValue = 0,tdsTemperature = 25;
unsigned long tdsAnalogSampleTimepoint = 0;


int getMedianNum(int bArray[], int iFilterLen);


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
            tdsAnalogBuffer[tdsAnalogBufferIndex] = analogRead(TDS_SensorPin);    // Read the analog value and store into the buffer
            tdsAnalogBufferIndex++;                                               // Increase the analog buffer index
            if(tdsAnalogBufferIndex >= TDS_SCOUNT) tdsAnalogBufferIndex = 0;      // If the analog bufer index equals to or more than the number of TDS
      }
}


int getMedianNum(int bArray[], int iFilterLen) 
{
      int bTab[iFilterLen];
      for (byte i = 0; i<iFilterLen; i++)
	  bTab[i] = bArray[i];
      int i, j, bTemp;
      for (j = 0; j < iFilterLen - 1; j++) 
      {
	  for (i = 0; i < iFilterLen - j - 1; i++) 
          {
	    if (bTab[i] > bTab[i + 1]) 
            {
		bTemp = bTab[i];
	        bTab[i] = bTab[i + 1];
		bTab[i + 1] = bTemp;
	     }
	  }
      }
      if ((iFilterLen & 1) > 0)
	bTemp = bTab[(iFilterLen - 1) / 2];
      else
	bTemp = (bTab[iFilterLen / 2] + bTab[iFilterLen / 2 - 1]) / 2;
      return bTemp;
}