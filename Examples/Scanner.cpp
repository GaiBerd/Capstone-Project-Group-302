#include <Arduino.h>
#include <Wire.h>

// Default ESP32 I2C pins: SDA = GPIO21, SCL = GPIO22
// You can change them if needed
#define SDA_PIN 21
#define SCL_PIN 20

void setup() {
    Serial.begin(9600);
    while (!Serial) {
        delay(10); // Wait for Serial to initialize
    }

    Serial.println("\nESP32 I2C Scanner");
    Wire.begin(SDA_PIN, SCL_PIN); // Initialize I2C with custom pins
}

void loop() {
    byte error, address;
    int deviceCount = 0;

    Serial.println("Scanning I2C bus...");

    for (address = 1; address < 127; address++) {
        Wire.beginTransmission(address);
        error = Wire.endTransmission();

        if (error == 0) {
            Serial.printf("I2C device found at address 0x%02X\n", address);
            deviceCount++;
        } else if (error == 4) {
            Serial.printf("Unknown error at address 0x%02X\n", address);
        }
    }

    if (deviceCount == 0) {
        Serial.println("No I2C devices found.\n");
    } else {
        Serial.printf("Scan complete. %d device(s) found.\n\n", deviceCount);
    }

    delay(3000); // Wait before next scan
}
