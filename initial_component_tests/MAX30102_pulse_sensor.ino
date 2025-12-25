#include <MAX30105.h>
#include <heartRate.h>
#include <spo2_algorithm.h>
MAX30105 particleSensor;

void setup() {
  Serial.begin(9600);
  if(!particleSensor.begin(Wire, I2C_SPEED_STANDARD)){
    Serial.println(" MAX30501 was not found. Please check the sensor.");
    while(1);
  }
particleSensor.setup();
particleSensor.setPulseAmplitudeRed(0x0A);
particleSensor.setPulseAmplitudeGreen(0);

  // put your setup code here, to run once:

}

void loop() {
 long irValue = particleSensor.getIR(); // put your main code here, to run repeatedly:
if(irValue > 5000) {
  Serial.print("IR=");
  Serial.print(irValue);
  Serial.print(" ,heartbeat detected");
 if (checkForBeat(irValue)) {
 float beatAvg = particleSensor.getReadPointer();
 Serial.print(beatAvg);
 Serial.println(" BPM");
 }
} else {
 Serial.println("Please place your finger on the sensor");
}
delay(100);
}
