 # Medibot
Arduino/ESP32-powered health assistant robot for monitoring vital signs like temperature, pulse, and blood pressure.
 (This is an educational/research prototype, not a certified medical device.)

Medibot is a low-cost, ESP32-powered health assistant prototype designed to monitor vital health signs, including body temperature, pulse rate, blood pressure (in development), and oxygen saturation, using non-invasive sensors. This project aims to improve access to basic healthcare evaluation, particularly in communities with limited access to medical services or equipment.


##  Purpose

Medibot was designed to support the healthcare community, particularly in areas with a shortage of medical staff or medical devices. With just a few components, Medibot can provide automated, hygienic, and affordable health screening, making early diagnosis and regular checkups more accessible.


##  Features

-  Non-contact temperature measurement (MLX90614) - uses thermal radiation to detect object and ambient temperature
- Pulse rate and oxygen level detection (MAX30105) - uses PPG (photoplethysmography) to compute heartbeats and SPO2
-  Blood pressure monitoring (MPX5700 + standard cuff + air pump)--in-progress
-  Arduino/ESP32-based logic processing
-  OLED/Serial Monitor display
-  buzzer/voice alert for abnormal readings

---

##  Sensors and Components

| Sensor            | Purpose                         | Notes                           |
|------------------|----------------------------------|----------------------------------|
|   MLX90614       | Non-contact temperature          | I2C communication                |
|   MAX30105       | Pulse and heart rate             | Optical sensor                   |
|    MPX5700       | Blood pressure (air pressure)    | Analog input from BP cuff        |                |
|    ESP32         | Microcontroller                  | Central control unit             |

---

##  Code Structure

Medibot/
> System initializes ESP32 and sensors (MLX90614, MAX3010x)

> The device waits for finger detection via an IR threshold

> Heart rate is measured first and stabilized using median filtering

> SpO₂ is calculated using IR/red absorption and averaged after outlier rejection

> Body temperature is measured using non-contact IR sensing

> Final readings are displayed, and system resets for the next user


## Inspiration and Vision

The need for affordable, DIY health solutions inspired this project. As part of my independent robotics and research work, I'm developing Medibot to blend **electrical engineering**, **embedded systems**, and **healthcare** innovation. It also represents my passion for **Addressing real-world challenges through the active application of technology.**.


##  Current Status

- [x] System design and architecture
- [x] Sensor logic simulation (while awaiting hardware)
- [x] Hardware integration
- [ ] Testing and field application
- [ ] Future: App connectivity, AI analysis, and speech interaction


##  About Me

I am a high school student from Saskatchewan with a strong interest in **Electrical & Computer Engineering**, **Robotics**, and **Healthcare Technology**. I founded this project independently and am currently seeking mentorship, feedback, and opportunities to scale it.



##  Contributions & Mentorship

If you are a professor, researcher, or engineer interested in collaborating, mentoring, or advising, feel free to reach out via GitHub. Medibot is an independent project I’m developing to explore embedded systems and healthcare technology.
I would be grateful for any feedback, advice, or critique you are willing to share, at any level, from high-level direction to technical details.
Any perspective that could help strengthen the project would mean a lot.




  ##  License

This project is open-source for educational and prototyping purposes. Contributions welcome!
