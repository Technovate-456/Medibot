 # Medibot
Arduino-powered health assistant robot for monitoring vital signs like temperature, pulse, and blood pressure.
 

**Medibot** is a low-cost, Arduino-powered health assistant prototype designed to monitor vital signs, including **body temperature**, **pulse rate**, and **blood pressure**, using non-invasive sensors. This project aims to improve access to basic healthcare diagnostics, particularly in communities with limited access to medical professionals or equipment.

---

##  Purpose

Medibot was built to support the **healthcare community**, particularly in areas with a shortage of medical staff or devices. With just a few components, Medibot can provide automated, hygienic, and affordable health screening, making early diagnosis and regular checkups more accessible.

---Medibot/
├── temperature_sensor/ # Temp simulation or MLX90614 setup
├── pulse_sensor/ # Pulse simulation or MAX30102 setup
├── blood_pressure_sensor/ # BP logic using MPX5700
├── display_output/ # Serial Monitor or LCD output
├── main_code/ # Combined logic (for testing + final)
└── README.md # Project overview and purpose

yaml
Copy
Edit


##  Features

-  Non-contact temperature measurement (MLX90614)
- Pulse rate and oxygen level detection (MAX30102)
-  Blood pressure monitoring (MPX5700 + standard cuff + air pump)
-  Arduino-based logic processing
- Optional LCD/Serial Monitor display
-  Optional buzzer/voice alert for abnormal readings

---

##  Sensors and Components

| Sensor            | Purpose                         | Notes                           |
|-------------------|----------------------------------|----------------------------------|
| **MLX90614**      | Non-contact temperature          | I2C communication                |
| **MAX30102**      | Pulse and heart rate             | Optical sensor                   |
| **MPX5700**       | Blood pressure (air pressure)    | Analog input from BP cuff        |
| **DS18B20**       | (Optional) Contact temp sensor   | OneWire protocol                 |
| **Arduino Uno**   | Microcontroller                  | Central control unit             |

---

##  Code Structure

Medibot/
├── temperature_sensor/ # Temp simulation or MLX90614 setup
├── pulse_sensor/ # Pulse simulation or MAX30102 setup
├── blood_pressure_sensor/ # BP logic using MPX5700
├── display_output/ # Serial Monitor or LCD output
├── main_code/ # Combined logic (for testing + final)
└── README.md # Project overview and purpose

yaml
Copy
Edit

---

## Inspiration and Vision

This project was inspired by the **need for affordable, DIY health solutions**. As part of my independent robotics and research work, I'm developing Medibot to blend **electrical engineering**, **embedded systems**, and **healthcare** innovation. It also represents my passion for **real-world problem solving through hands-on technology**.

---

##  Current Status

- [x] System design and architecture
- [x] Sensor logic simulation (while awaiting hardware)
- [ ] Final hardware integration
- [ ] Testing and field application
- [ ] Future: App connectivity and speech interaction

---

##  About Me

I’m a high school student from Saskatchewan with a strong interest in **Electrical & Electronics Engineering**, **Robotics**, and **Healthcare Technology**. I founded this project independently and am currently seeking mentorship, feedback, and opportunities to scale it.

---

##  Contributions & Mentorship

If you're a professor, researcher, or engineer interested in collaborating, mentoring, or advising, feel free to reach out via GitHub. 



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
// This mock code simulates MAX30102 readings using random values
// Simulated pulse range: 65–100 bpm
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

  
Basic Blood pressure code
// 🔧 Medibot - Simulated Blood Pressure Monitoring
// This code simulates BP readings normally taken via MPX5700 + air cuff
// Systolic range: 110–130 mmHg, Diastolic: 70–85 mmHg

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



##  License

This project is open-source for educational and prototyping purposes. Contributions welcome!
