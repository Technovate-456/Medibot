#include <Wire.h>
#include "MAX30105.h"
#include "heartRate.h"

MAX30105 particleSensor;

long lastBeat = 0; // Time at which the last beat occurred
float beatsPerMinute;
int beatAvg;

void setup() {
  Serial.begin(9600);

  if (!particleSensor.begin(Wire, I2C_SPEED_STANDARD)) {
    Serial.println("MAX30105 was not found. Please check the sensor.");
    while (1);
  }

  particleSensor.setup(); // Configure sensor with default settings
  particleSensor.setPulseAmplitudeRed(0x0A);   // Turn Red LED to low power
  particleSensor.setPulseAmplitudeGreen(0);    // Turn off Green LED
}

void loop() {
  long irValue = particleSensor.getIR();

  if (irValue > 5000) {
    if (checkForBeat(irValue)) {
      // Calculate BPM
      long delta = millis() - lastBeat;
      lastBeat = millis();

      beatsPerMinute = 60 / (delta / 1000.0);

      // Filter out unreasonable values
      if (beatsPerMinute < 255 && beatsPerMinute > 20) {
        beatAvg = (beatAvg * 0.9) + (beatsPerMinute * 0.1);
        Serial.print("IR=");
        Serial.print(irValue);
        Serial.print(" BPM=");
        Serial.print(beatsPerMinute);
        Serial.print(" Avg BPM=");
        Serial.println(beatAvg);
      }
    }
  } else {
    Serial.println("Please place your finger on the sensor");
  }

  delay(100);
}
