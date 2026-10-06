#include <Arduino.h>
#include <Wire.h>
#include <LIDARLite.h>

// Explicitly define ESP32-S3 I2C pins
#define I2C_SDA 21
#define I2C_SCL 20

LIDARLite myLidar;
int loopCounter = 0;

void setup() {
    Serial.begin(9600);
    while(!Serial) {
        delay(10); // Wait for native USB serial on ESP32-S3
    }
    Serial.println("Initializing system...");

    // 1. Initialize I2C bus configuration for ESP32-S3
    bool i2c_ok = Wire.begin(I2C_SDA, I2C_SCL, 400000U); // 400 kHz fast I2C
    if (!i2c_ok) {
        Serial.println("Error: Failed to initialize I2C bus.");
        while(1);
    }

    /* 
     2. Initialize the Garmin LiDAR
        Configuration options:
        0 : Default configuration, balanced performance
        1 : Short range, high speed
        2 : Default range, higher speed
        3 : Maximum range
    */
    myLidar.begin(0, true); 
    Serial.println("LIDAR-Lite v3 Initialized successfully!");
}

void loop() {
    int distance = 0;

    /*
      The sensor needs periodic bias corrections to adjust for ambient factors.
      - Every 100 readings, pass 'true' to clear bias and force a reset measurement.
      - For normal fast readings, pass 'false' to use the pre-existing calibration.
    */
    if (loopCounter % 100 == 0) {
        distance = myLidar.distance(true);  // With bias correction (takes slightly longer)
    } else {
        distance = myLidar.distance(false); // Without bias correction (fastest)
    }

    // Print measurements out to the Serial Monitor
    Serial.print("Distance: ");
    Serial.print(distance);
    Serial.println(" cm");

    loopCounter++;
    delay(20); // Small delay between scans (max sampling rate around 100Hz)
}
