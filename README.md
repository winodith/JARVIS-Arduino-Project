# JARVIS Arduino Project with Voice and Wi-Fi Control

## Overview
The JARVIS (Just A Rather Very Intelligent System) project is a smart home automation system using Arduino and the ESP8266/ESP32 Wi-Fi module. This project enables you to control home appliances like lights and fans via voice commands using Bluetooth and remotely via a web interface over Wi-Fi. It also includes automatic fan control based on room temperature and soil moisture monitoring with visual indicators.

## Features
- **Voice Control via Bluetooth:** Control appliances with voice commands from a smartphone.
- **Wi-Fi Control via Web Interface:** Control appliances and monitor sensor data using a web browser.
- **Automatic Fan Control:** Adjust fan speed based on room temperature.
- **Soil Moisture Monitoring:** Check soil moisture levels with LED indicators (Red for moist, Green for dry).

## Components Required
1. **Arduino UNO** - Main microcontroller for the project.
2. **ESP8266/ESP32** - Wi-Fi module for network connectivity.
3. **Bluetooth Module (HC-05/HC-06)** - For Bluetooth communication.
4. **Relay Module (2-Channel)** - To control appliances like lights and fans.
5. **DHT11 Temperature and Humidity Sensor** - To measure temperature and humidity.
6. **Soil Moisture Sensor** - To measure soil moisture levels.
7. **2 LEDs (Green & Red)** - To indicate soil moisture status.
8. **Jumper Wires** - For connections.
9. **Breadboard** - For assembling the circuit.
10. **Smartphone with Bluetooth Serial App** - For sending Bluetooth commands.
11. **Web Browser** - For accessing the web interface.

## Circuit Diagram
Refer to `circuit_diagram.png` in this repository for detailed wiring instructions. The circuit includes connections for the Arduino, ESP8266/ESP32, Bluetooth module, relay module, sensors, and LEDs.

## How to Build and Set Up the Project

### **Step 1: Assemble the Circuit**
1. **ESP8266/ESP32 Connections:**
   - VCC → 3.3V (or 5V depending on your module)
   - GND → GND
   - TX → RX (Pin 2 on Arduino, use a voltage divider for level shifting)
   - RX → TX (Pin 3 on Arduino)

2. **Relay Module:**
   - VCC → 5V (Arduino)
   - GND → GND (Arduino)
   - IN1 → Pin 8 (Arduino) [For Light]
   - IN2 → Pin 9 (Arduino) [For Fan]

3. **DHT11 Sensor:**
   - VCC → 5V (Arduino)
   - GND → GND (Arduino)
   - DATA → Pin 2 (Arduino)

4. **Soil Moisture Sensor:**
   - VCC → 5V (Arduino)
   - GND → GND (Arduino)
   - AO → A0 (Arduino)

5. **LEDs for Soil Moisture:**
   - **Green LED:** Long leg (+) → Pin 13 (Arduino), Short leg (-) → GND (Arduino)
   - **Red LED:** Long leg (+) → Pin 12 (Arduino), Short leg (-) → GND (Arduino)

### **Step 2: Upload the Arduino Code**
1. **Install Arduino IDE:** Download and install the [Arduino IDE](https://www.arduino.cc/en/software).
2. **Download Code:** Clone this repository or download the `jarvis_arduino.ino` file.
3. **Connect Arduino and ESP Module:** Use a USB cable to connect your Arduino and ESP module to your computer.
4. **Open Code in IDE:**
   - Open the `jarvis_arduino.ino` file in the Arduino IDE.
5. **Select Board and Port:**
   - In the Arduino IDE, go to "Tools" and select the correct **Board** (Arduino UNO) and **Port**.
   - For the ESP module, select the appropriate board and port (e.g., ESP8266 or ESP32).
6. **Upload Code:**
   - Click the upload button to compile and upload the code to your Arduino and ESP module.

### **Step 3: Configure Wi-Fi and Web Interface**
1. **Update Wi-Fi Credentials:**
   - Open the `jarvis_arduino.ino` file and update the `ssid` and `password` variables with your Wi-Fi network details.
2. **Restart ESP Module:**
   - Ensure the ESP module connects to your Wi-Fi network and starts the web server. The IP address assigned to the ESP module will be displayed in the Serial Monitor.

### **Step 4: Setup Voice Control**
1. **For Android Users:**
   - **Using Google Assistant and IFTTT:**
     1. Create an IFTTT account and set up applets that use Google Assistant as a trigger.
     2. Configure actions to send HTTP requests to the ESP module’s IP address with commands like `/LIGHT_ON` and `/FAN_OFF`.
   - **Using Tasker:**
     1. Install Tasker and AutoBluetooth apps.
     2. Create Tasker profiles to send HTTP requests when specific voice commands are recognized.

2. **For iOS Users:**
   - **Using Siri Shortcuts:**
     1. Open the Shortcuts app on your iPhone.
     2. Create shortcuts that send HTTP requests to the ESP module with commands like `/LIGHT_ON` and `/FAN_OFF`.
     3. Use Siri to trigger these shortcuts.

### **Step 5: Testing and Validation**
1. **Wi-Fi Control:**
   - Access the web interface by navigating to the ESP module’s IP address in your web browser.
   - Test the control of appliances and monitor sensor data.
2. **Voice Control:**
   - Use your configured voice assistant (Google Assistant, Siri) to send commands and verify that the appliances respond correctly.
3. **Sensor Monitoring:**
   - Check the soil moisture levels and ensure the LEDs indicate the correct status (Green for dry, Red for moist).
   - Verify that the fan automatically adjusts based on the temperature readings.

### **Step 6: Modifications (Optional)**
- Enhance the web interface with additional features and controls.
- Integrate more sensors or actuators for expanded functionality.
- Improve security with authentication for the web interface.

## Code Explanation

### **Arduino Code:**

```cpp
#include <ESP8266WiFi.h>
#include <DHT.h>

#define DHTPIN 2     // Pin where the DHT sensor is connected
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

// Bluetooth and appliance pins
const int relayLight = 8;
const int relayFan = 9;

// Soil Moisture Sensor pins
const int soilMoisturePin = A0;
const int greenLED = 13;
const int redLED = 12;

// Wi-Fi credentials
const char* ssid = "your_SSID";
const char* password = "your_PASSWORD";

// Create an instance of the WiFi server
WiFiServer server(80);

void setup() {
  Serial.begin(9600); // For Bluetooth communication
  dht.begin();
  
  pinMode(relayLight, OUTPUT);
  pinMode(relayFan, OUTPUT);
  pinMode(greenLED, OUTPUT);
  pinMode(redLED, OUTPUT);

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("Connected to Wi-Fi");

  // Start the server
  server.begin();
}

void loop() {
  // Handle Wi-Fi client
  WiFiClient client = server.available();
  if (client) {
    String request = client.readStringUntil('\r');
    client.flush();

    if (request.indexOf("/LIGHT_ON") != -1) {
      digitalWrite(relayLight, HIGH);
      Serial.println("Light ON");
    } else if (request.indexOf("/LIGHT_OFF") != -1) {
      digitalWrite(relayLight, LOW);
      Serial.println("Light OFF");
    } else if (request.indexOf("/FAN_ON") != -1) {
      digitalWrite(relayFan, HIGH);
      Serial.println("Fan ON");
    } else if (request.indexOf("/FAN_OFF") != -1) {
      digitalWrite(relayFan, LOW);
      Serial.println("Fan OFF");
    }

    // Send response to client
    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: text/html");
    client.println();
    client.println("<!DOCTYPE HTML>");
    client.println("<html>");
    client.println("<h1>Control Page</h1>");
    client.println("<p><a href=\"/LIGHT_ON\">Turn Light ON</a></p>");
    client.println("<p><a href=\"/LIGHT_OFF\">Turn Light OFF</a></p>");
    client.println("<p><a href=\"/FAN_ON\">Turn Fan ON</a></p>");
    client.println("<p><a href=\"/FAN_OFF\">Turn Fan OFF</a></p>");
    client.println("</html>");
    client.stop();
  }

  // Handle Bluetooth commands
  if (Serial.available() > 0) {
    String command = Serial.readString();
    command.trim();

    if (command == "TURN ON LIGHT") {
      digitalWrite(relayLight, HIGH);
      Serial.println("Light ON");
    } else if (command == "TURN OFF LIGHT")
