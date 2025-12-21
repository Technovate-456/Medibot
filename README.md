 # Medibot
Arduino/ESP32-powered health assistant robot for monitoring vital signs like temperature, pulse, and blood pressure.
 

Medibot is a low-cost, ESP32-powered health assistant prototype designed to monitor vital signs, including body temperature, pulse rate, blood pressure, and oxygen saturation, using non-invasive sensors. This project aims to improve access to basic healthcare diagnostics, particularly in communities with limited access to medical services or equipment.


##  Purpose

Medibot was designed to support the healthcare community, particularly in areas with a shortage of medical staff or medical devices. With just a few components, Medibot can provide automated, hygienic, and affordable health screening, making early diagnosis and regular checkups more accessible.

Medibot/
temperature_sensor/ # Temp simulation using MLX90614 setup
 pulse_sensor/ # Pulse simulation using MAX30102 setup
 blood_pressure_sensor/ # BP logic using MPX5700
 display_output/ # Serial Monitor or OLED output
 main_code/ # Combined logic (for testing + final)
 

##  Features

-  Non-contact temperature measurement (MLX90614)
- Pulse rate and oxygen level detection (MAX30102)
-  Blood pressure monitoring (MPX5700 + standard cuff + air pump)
-  Arduino/ESP32-based logic processing
-  OLED/Serial Monitor display
-  buzzer/voice alert for abnormal readings

---

##  Sensors and Components

| Sensor            | Purpose                         | Notes                           |
|-------------------|---------------------------------|----------------------------------|
|   MLX90614      | Non-contact temperature          | I2C communication                |
|   MAX30105      | Pulse and heart rate             | Optical sensor                   |
|    MPX5700      | Blood pressure (air pressure)    | Analog input from BP cuff        |
|    DS18B20      | (Optional) Contact temp sensor   | OneWire protocol                 |
|    ESP32        | Microcontroller                  | Central control unit             |

---

##  Code Structure

Medibot/
 temperature_sensor/ # Temp simulation or MLX90614 setup
 pulse_sensor/ # Pulse simulation or MAX30102 setup
blood_pressure_sensor/ # BP logic using MPX5700
 display_output/ # Serial Monitor or OLED output
 main_code/ # Combined logic (for testing + final)


## Inspiration and Vision

This project was inspired by the "need for affordable, DIY health solutions". As part of my independent robotics and research work, I'm developing Medibot to blend **electrical engineering**, **embedded systems**, and **healthcare** innovation. It also represents my passion for**Addressing real-world challenges through the active application of technology.**.


##  Current Status

- [x] System design and architecture
- [x] Sensor logic simulation (while awaiting hardware)
- [x] Hardware integration
- [ ] Testing and field application
- [ ] Future: App connectivity and speech interaction


##  About Me

I’m a high school student from Saskatchewan with a strong interest in **Electrical & Electronics Engineering**, **Robotics**, and **Healthcare Technology**. I founded this project independently and am currently seeking mentorship, feedback, and opportunities to scale it.



##  Contributions & Mentorship

If you're a professor, researcher, or engineer interested in collaborating, mentoring, or advising, feel free to reach out via GitHub. 




  




##  License

This project is open-source for educational and prototyping purposes. Contributions welcome!
