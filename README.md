# 🌍 IoT-Based Environmental Data Logger 🔬📊

A compact **ESP32-based IoT environmental monitoring system** designed for **laboratory research and equipment longevity studies**. The system continuously monitors and logs critical environmental parameters with local display, SD card storage, and live web-based visualization.

Developed under the **IDEA Lab (First Year)** at **RV College of Engineering (2024–25)**.

---

## 📌 Overview

Laboratories require stable environmental conditions to ensure accurate experiments and protect sensitive equipment. This project provides a **low-cost, modular, and real-time data logger** to monitor and record environmental variations automatically, reducing reliance on manual observation.

---

## 🧠 Key Features

- 🌡 Temperature & Humidity monitoring (DHT11)
- 💡 Light intensity measurement (LDR)
- 🧪 Gas concentration detection (MQ gas sensor)
- 🖥 Local display using I2C LCD
- 💾 Timestamped data logging to microSD card
- 🌐 Live web dashboard hosted on ESP32
- ⏱ NTP-based real-time clock synchronization
- 🔌 Standalone, low-power operation

---

## 🧩 System Architecture

- **ESP32** – Core controller with Wi-Fi
- **DHT11** – Temperature & humidity sensor
- **LDR** – Ambient light sensing
- **MQ Gas Sensor** – Air quality detection
- **I2C LCD** – Real-time local display
- **microSD Card** – CSV-based data logging
- **Web Server** – Live monitoring over local Wi-Fi

---

## ⚙️ Functionality

- Reads all sensors every second
- Displays live values on LCD
- Logs data to SD card with timestamps
- Serves a web page that auto-refreshes every 5 seconds
- Operates reliably during long lab sessions

---

## 🧪 Experimental Validation

- Successfully detected **NH₃ gas spikes** during a controlled chemical reaction
- Logged stable temperature, humidity, light, and gas data
- No data loss or corruption during extended runs
- Consistent performance on both LCD and web dashboard

---

## 🛠 Tech Stack

- ESP32 (Arduino framework)
- Embedded C / C++
- DHT, WiFi, WebServer, SD, NTP libraries
- I2C LCD
- CSV-based data storage

---

## 🔮 Future Enhancements

- Replace DHT11 with higher-accuracy sensors (DHT22 / BME280)
- Cloud integration (Firebase / ThingSpeak)
- Mobile dashboard & alerts
- Battery backup / UPS
- Advanced gas-specific sensors
- AI-based anomaly detection

---

## 👨‍💻 Team

- **Pramath J**
- P R Hari Hara Sai Pratham  
- Ansh Patel  
- Poojith Khanapur  

---
