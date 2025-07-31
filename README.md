# -IoT-Based-ESP32-Weather-Station
🌦️ IoT-Based ESP32 Weather Station
This project implements a low-cost, open-source IoT weather station using two ESP32 microcontrollers and a range of environmental sensors. Designed as part of an academic project at the University of Vocational Technology, the system collects real-time data on temperature, humidity, atmospheric pressure, rainfall, and soil moisture/water level.

📌 Features
🌐 Real-time monitoring via Wi-Fi (UDP communication)

🌦️ Sensors: DHT22, BMP280, Rain Sensor, Water Level Sensor

💾 Optional integration with web dashboards or cloud platforms (Flask, ThingSpeak, etc.)

⚡ Power via USB (supports future solar/battery options)

📊 Data visualization using Python, Flask, Chart.js, or Plotly

🌍 Dual ESP32 nodes for wider environmental coverage

🧩 Components Used
ESP32 Dev Boards (2x)

DHT22 (Temp & Humidity)

BMP280 (Pressure)

Rain sensor

Water/Soil Moisture sensor

⚙️ How It Works
Sensor Node (ESP32-A) reads environmental data every 3 seconds

Data is sent via UDP to a receiver (ESP32 or web server)

Receiver node logs, displays, or forwards data to dashboards

Optional data storage: local CSV, SQLite, or cloud DBs

⚠️ Challenges & Solutions
Sensor instability → Resolved with better wiring and error handling

Wi-Fi disconnections → Retry logic and watchdog timers added

Noisy analog signals → Averaging and smoothing techniques used

UDP packet loss → Planned future upgrade to MQTT protocol

🚀 Future Enhancements
MQTT for reliable messaging

Cloud integration (Firebase, AWS, etc.)

Battery/solar power support

More sensors: UV, wind, air quality

Mobile app with real-time alerts

Rugged, weatherproof enclosure
