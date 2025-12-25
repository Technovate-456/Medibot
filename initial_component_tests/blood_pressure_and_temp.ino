Basic Blood pressure code
// 🔧 Medibot - Simulated Blood Pressure Monitoring
// This code simulates BP readings normally taken via MPX5700 + air cuff
// Systolic range: 110–130 mmHg, Diastolic: 70–85 

void setup() {
  Serial.begin(9600);
}

void loop() {
  int systolic = random(100, 140);
  int diastolic = random(65, 90);
   Serial.print("blood pressure: ");
   Serial.print(systolic);
   Serial.print("/");
   Serial.print(diastolic);
   Serial.print(" mmHg");
   delay(3000);
}

Basic Temp sensor code
// 🔧 Medibot - Simulated Temperature Sensor
// This is mock code using random() values to simulate readings from the MLX90614
// Real sensor integration will replace this once hardware is available
// Temperature range simulated: 36.0°C to 38.0°C
void setup() {
  Serial.begin(9600);
}

void loop() {

  float tempC = random(365, 380)/ 10; 
   Serial.print("Body Temp: ");
   Serial.print(tempC);
   Serial.print(" C");
   delay(2000);
 
}
// 🔧 Medibot - Simulated Pulse & Heart Rate Sensor
// Simulated pulse range: 65–100 bpm
// This mock code simulates MAX30102 readings using random values
// Actual optical sensor data will replace this in the final ve
}

Basic pulse rate code
void setup() {
 Serial.begin(9600); 
}
void loop() {
  int heartrate = random(65, 105);

  Serial.print("Heartrate: ");
  Serial.print(heartrate);
  Serial.print(" bpm");
  delay(2000);
}





Main code (combining everything)

void setup() {
  Serial.begin(9600);
}

void loop() {
 
  float tempC = random(365, 380) / 10.0;
  int heartRate = random(65, 105);
  int systolic = random(100, 140);
  int diastolic = random(65, 90);

  Serial.println("------ Medibot Vital Check ------");
  Serial.print("Temperature: "); Serial.print(tempC); Serial.println(" °C");
  Serial.print("Pulse Rate: "); Serial.print(heartRate); Serial.println(" bpm");
  Serial.print("Blood Pressure: ");
  Serial.print(systolic); Serial.print("/");
  Serial.print(diastolic); Serial.println(" mmHg");

  Serial.println("---------------------------------");
  delay(4000);
}





