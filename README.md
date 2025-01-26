# IoT-Based Flood Monitoring and Early Warning System

## Project Overview
This project is a real-time flood monitoring system with early warning capabilities developed for the **EN3251 Internet of Things** module. The system integrates IoT devices such as ultrasonic sensors, DHT22 sensors, and NodeMCU to monitor environmental parameters and provide timely alerts during flood risks.

## Key Features
- **Real-Time Monitoring**: Collects water level, temperature, and humidity data.
- **Cloud Storage & Visualization**: Utilizes ThingSpeak and Node-RED for data analysis and visualization.
- **Alerts & Notifications**: Sends SMS alerts via Twilio when flood thresholds are exceeded.
- **Power Management**: Operates uninterrupted during power outages using a rechargeable battery.

## System Components
### End-Devices
- **Ultrasonic Sensors**: Measure water levels using sound waves.
- **DHT22 Sensor**: Captures temperature and humidity data.
- **ESP8266 (NodeMCU)**: Collects and transmits data to the cloud using MQTT.
- **Rechargeable Battery**: Provides backup power during outages.

### Management Interface
- **Node-RED Dashboard**: Displays real-time sensor data on a web-based interface.
- **ThingSpeak Cloud**: Stores and processes sensor data, triggers alerts based on conditions.
- **Twilio Integration**: Sends SMS notifications when critical water levels are detected.

## Functional Workflow
1. **Data Collection**:
   - Ultrasonic sensors measure water levels.
   - DHT22 sensor records temperature and humidity.
2. **Data Transmission**:
   - NodeMCU publishes sensor data to the MQTT broker.
   - Data is sent to ThingSpeak for storage and visualization.
3. **Data Processing**:
   - Node-RED processes incoming data for real-time monitoring.
   - Sends SMS alerts if thresholds are breached.
4. **Power Management**:
   - Automatically switches to battery power during outages.

## System Architecture
The system architecture connects end-devices, a microcontroller (ESP8266), a cloud platform (ThingSpeak), and a user interface (Node-RED Dashboard). The architecture also includes an SMS notification system for immediate flood alerts.

## Getting Started
### Prerequisites
- **Hardware**:
  - 2 HC-SR04 Ultrasonic Sensors
  - 1 DHT22 Temperature and Humidity Sensor
  - 1 NodeMCU (ESP8266)
  - Rechargeable battery and power supply
  - Jumper wires and breadboard

- **Software**:
  - Arduino IDE
  - Node-RED
  - ThingSpeak account
  - Twilio account

### Installation
1. Clone this repository:
   ```bash
   git clone https://github.com/dmpcd/IoT-Based-Flood-Monitoring-System.git
   ```
2. Open the Arduino IDE and upload the code to the NodeMCU.
3. Set up Node-RED flows for data visualization and alerts.
4. Configure ThingSpeak and Twilio accounts for cloud storage and notifications.

### Usage
1. Power on the system and connect to Wi-Fi.
2. Monitor real-time data on the Node-RED Dashboard.
3. Receive SMS alerts during flood risks.

## Project Demonstration
![System Diagram]("D:\GitHub\IoT-Based-Flood-Monitoring-System\IoT-Based-Flood-Monitoring-System\Pictures\Final Product at the Demonstration.jpg")

## Team Members
- De Zoysa A.K.N. - 210108C
- Dimagi D.H.P. - 210131N
- Dissanayaka D.M.P.C. - 210140P

## Acknowledgments
- [IoT Design Pro](https://iotdesignpro.com)
- [Twilio](https://www.twilio.com)
- [ThingSpeak](https://thingspeak.com)
