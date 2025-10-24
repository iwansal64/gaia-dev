#include <ServoManager.h>
#include <ESP32Servo.h>

// Useful Definitions
#define SERVO_PIN 14
#define SERVO_OPENING_TIME 2000 // in miliseconds


Servo servo;

bool ServoManager::isInitialized = false;
bool ServoManager::isOpen = false;
uint64_t ServoManager::lastTimeOpeningLid = 0UL;

void ServoManager::init() {
      // If already initialized
      if(ServoManager::isInitialized) return;

      // Allow allocation of all timers
      ESP32PWM::allocateTimer(0);
	ESP32PWM::allocateTimer(1);
	ESP32PWM::allocateTimer(2);
	ESP32PWM::allocateTimer(3);
      
      // Setting up servo instance
      servo.attach(SERVO_PIN);

      // Set initialized to true
      ServoManager::isInitialized = true;
}

void ServoManager::trigger() {
      if(!ServoManager::isInitialized) {
            Serial.println("[ServoManager] The servo has not initialized yet.");
            return;
      }
      
      // Opening up the servo
      servo.write(20);

      // Set the last time opening lid
      ServoManager::lastTimeOpeningLid = millis();
      ServoManager::isOpen = true;
}

void ServoManager::loop() {
      
      // If the current state is open
      if(ServoManager::isOpen) {
            // Get current milliseconds
            uint64_t currentTime = millis();

            // If the last time open is more than the servo opening time (already 2 seconds from the last time for example)
            if(currentTime - ServoManager::lastTimeOpeningLid > SERVO_OPENING_TIME) {
                  // Move the servo back to the initial position
                  servo.write(0);
      
                  // Reset the isOpen variable to false
                  ServoManager::isOpen = false;
            }
      }
}