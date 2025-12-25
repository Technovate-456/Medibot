const int pressurePin = A0;


void setup() {
 Serial.begin(9600); // put your setup code here, to run once:

}

void loop() {
  int sensorVal = analogRead(pressurePin);
  float voltage = sensorVal * (5.0/1023.0);
  float pressure_kPa = ((voltage - 0.2) / 4.7) * 700;
  Serial.print("Pressure: ");
  Serial.print(pressure_kPa);
  Serial.print(" kPa");
  delay(1000);
  // put your main code here, to run repeatedly:

}
