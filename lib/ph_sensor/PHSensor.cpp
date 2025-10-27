#include <PHSensor.h>
#include <util.h>

#define PH_SENSOR_PIN 32            // Pin where PH sensor connected
#define PH_VREF 3.3                 // Analog reference voltage(Volt) of the ADC
#define PH_SCOUNT  30               // Sum of sample point
#define PH_CALIBRATION 21.34        // Calibration of PH Sensor

float phVoltage = 0;
unsigned long phAnalogSampleTimepoint = 0UL;
int phAnalogBufferCount = 0,phCopyIndex = 0;
float phAverageVoltage = 0,phValue = 0;

void PHSensor::loop() {
      if(millis()-phAnalogSampleTimepoint > 40U) {                        // Every 40 milliseconds, read the analog value from the ADC
            phAnalogSampleTimepoint = millis();                           // Update the last sample timepoint
            phVoltage += analogRead(PH_SENSOR_PIN);                       // Read the analog value and store into the buffer
            phAnalogBufferCount++;                                        // Increase the analog buffer index
            if(phAnalogBufferCount >= PH_SCOUNT) phAnalogBufferCount = 0; // If the analog bufer index equals to or more than the number of TDS
      }
}

float PHSensor::get_ph() {
      phVoltage /= phAnalogBufferCount;                     // Get the average voltage
      phValue = 7 + ((2.5 - phVoltage) / PH_CALIBRATION);   // Calculate the voltage
}

