// AS3935 Lightning Detector Example Sketch
// This example demonstrates basic lightning detection

#include "AS3935.h"

// Create AS3935 instance
AS3935 lightning;

// Interrupt pin (change based on your microcontroller)
const int INTERRUPT_PIN = 2;

// Lightning detection flag
volatile bool lightning_detected = false;

void setup() {
    Serial.begin(115200);
    delay(1000);
    
    Serial.println("AS3935 Lightning Detector - Starting...");
    
    // Initialize the sensor
    if (!lightning.begin()) {
        Serial.println("Failed to initialize AS3935!");
        while (1);
    }
    
    Serial.println("AS3935 initialized successfully");
    
    // Configure sensor
    lightning.setIndoorOutdoor(true);      // Set indoor mode
    lightning.setNoiseLevel(2);            // Set noise level
    lightning.setWatchdogThreshold(10);    // Set watchdog threshold
    lightning.setAFEGain(20);              // Set AFE gain
    
    // Setup interrupt pin
    pinMode(INTERRUPT_PIN, INPUT);
    attachInterrupt(digitalPinToInterrupt(INTERRUPT_PIN), 
                    lightningInterrupt, RISING);
    
    Serial.println("Configuration complete. Listening for lightning...");
}

void loop() {
    if (lightning_detected) {
        lightning_detected = false;
        
        uint8_t int_type = lightning.getInterruptType();
        
        if (int_type == AS3935_INT_LIGHTNING) {
            uint8_t distance = lightning.getDistance();
            uint32_t energy = lightning.getEnergyLevel();
            
            Serial.println("⚡ LIGHTNING DETECTED!");
            Serial.print("Distance: ");
            Serial.print(distance);
            Serial.println(" km");
            Serial.print("Energy Level: ");
            Serial.println(energy);
            Serial.println();
        }
        else if (int_type == AS3935_INT_NOISE) {
            Serial.println("⚠ Noise detected");
        }
        else if (int_type == AS3935_INT_DISTURB) {
            Serial.println("⚠ Disturbance detected");
        }
    }
    
    delay(100);
}

// Interrupt service routine
void lightningInterrupt() {
    lightning_detected = true;
}
