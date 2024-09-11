# JARVIS Arduino Project

## Overview
This project is a smart home automation system called JARVIS (Just A Rather Very Intelligent System) using Arduino. It allows you to control basic home appliances like lights and fans through **voice commands** using Bluetooth, as well as monitor **soil moisture levels** and **temperature/humidity**. The fan automatically adjusts based on room temperature, and the soil moisture sensor provides visual feedback through LEDs indicating the soil condition.

## Features
- **Voice Control:** Control lights and fan using voice commands sent via Bluetooth from a smartphone app.
- **Automatic Fan Control:** Adjust fan speed automatically based on the room's temperature.
- **Soil Moisture Monitoring:** Use a soil moisture sensor with LED indicators to check soil moisture levels (Red for moist, Green for dry).

## Components Required
1. **Arduino UNO** - Main microcontroller for the project.
2. **Bluetooth Module (HC-05 or HC-06)** - To connect the smartphone with Arduino.
3. **Relay Module (2-Channel)** - To control the appliances (light and fan).
4. **DHT11 Temperature and Humidity Sensor** - For temperature and humidity monitoring.
5. **Soil Moisture Sensor** - To monitor the moisture level in the soil.
6. **2 LEDs (Green & Red)** - To indicate the soil moisture level.
7. **Jumper Wires** - For making connections.
8. **Breadboard** - For building the circuit.
9. **Smartphone with Bluetooth Serial App** - For sending voice commands.

## How It Works
1. **Voice Control:**
   - Control lights and fan using voice commands sent from your smartphone to the Arduino via Bluetooth.
   - Commands supported:
     - "TURN ON LIGHT" → Turns on the light.
     - "TURN OFF LIGHT" → Turns off the light.
     - "TURN ON FAN" → Turns on the fan.
     - "TURN OFF FAN" → Turns off the fan.

2. **Automatic Fan Control:**
   - The fan is automatically turned on if the room temperature exceeds 30°C, and it turns off if the temperature falls below that threshold.

3. **Soil Moisture Monitoring:**
   - The soil moisture sensor measures the moisture in the soil.
   - If the soil is dry, the green LED lights up. If the soil is moist, the red LED lights up.

## Circuit Diagram
Here's how to connect the components:

### **Connections:**
- **Bluetooth Module (HC-05/HC-06):**
  - VCC → 5V (Arduino)
  - GND → GND (Arduino)
  - TX → RX (Pin 0 on Arduino)
  - RX → TX (Pin 1 on Arduino)

- **Relay Module:**
  - VCC → 5V (Arduino)
  - GND → GND (Arduino)
  - IN1 (Light control) → Pin 8 (Arduino)
  - IN2 (Fan control) → Pin 9 (Arduino)
  
- **DHT11 Sensor:**
  - VCC → 5V (Arduino)
  - GND → GND (Arduino)
  - DATA → Pin 2 (Arduino)

- **Soil Moisture Sensor:**
  - VCC → 5V (Arduino)
  - GND → GND (Arduino)
  - AO (Analog Output) → A0 (Arduino)

- **LEDs for Soil Moisture:**
  - **Green LED:** 
    - Long leg (+) → Pin 13 (Arduino)
    - Short leg (-) → GND (Arduino)
  - **Red LED:**
    - Long leg (+) → Pin 12 (Arduino)
    - Short leg (-) → GND (Arduino)

### **Full Circuit Diagram**
You can find the full circuit diagram in the `circuit_diagram.png` file in this repository.

## How to Build and Set Up the Project

### **Step 1: Assemble the Circuit**
1. Follow the circuit diagram above to connect the components (Bluetooth module, relays, DHT11, LEDs, and soil moisture sensor) to the Arduino.
2. Make sure all connections are correct and tight.

### **Step 2: Upload the Arduino Code**
1. Download the [Arduino IDE](https://www.arduino.cc/en/software) if you don’t have it installed yet.
2. Clone this GitHub repository or download the code in the `jarvis_arduino.ino` file.
3. Connect your Arduino to your computer via USB.
4. Open the `jarvis_arduino.ino` file in the Arduino IDE.
5. Select the correct **Board** (Arduino UNO) and **Port** from the "Tools" menu.
6. Click the upload button to upload the code to your Arduino.

### **Step 3: Setup the Bluetooth Serial App**
1. Download a Bluetooth serial app on your smartphone. Some common apps:
   - **Android**: [Serial Bluetooth Terminal](https://play.google.com/store/apps/details?id=de.kai_morich.serial_bluetooth_terminal)
   - **iOS**: [Bluetooth Terminal](https://apps.apple.com/us/app/bluetooth-terminal/id1217001123)
2. Pair your smartphone with the Bluetooth module (HC-05/HC-06). Default pairing password is usually `1234` or `0000`.
3. Open the Bluetooth serial app and connect to your Bluetooth module.
4. Once connected, use the app to send text commands such as "TURN ON LIGHT" or "TURN OFF FAN".

### **Step 4: Voice Control Setup**

#### **For Android Users:**
1. **Install Tasker and AutoBluetooth Apps:**
   - **Tasker:** [Tasker](https://play.google.com/store/apps/details?id=net.dinglisch.android.taskerm)
   - **AutoBluetooth:** [AutoBluetooth](https://play.google.com/store/apps/details?id=com.joaomgcd.autobluetooth)
   
2. **Configure Tasker for Voice Commands:**
   - Open Tasker and create a new profile for voice commands.
   - Choose the “Voice” option and set up the command phrases (e.g., "Turn on the light", "Turn off the fan").
   - Create tasks in Tasker that send Bluetooth text commands when voice commands are recognized.

3. **Configure AutoBluetooth:**
   - Open AutoBluetooth and configure it to automatically connect to your Bluetooth module when in range.

#### **For iOS Users:**
1. **Set Up Siri Shortcuts:**
   - Open the Shortcuts app on your iPhone.
   - Create new shortcuts for each command, such as turning on the light or fan.
   - Use the "Send Bluetooth Command" action to send text commands to the Arduino via Bluetooth.

2. **Use Siri to Trigger Commands:**
   - Once shortcuts are set up, you can activate them using Siri with your chosen voice commands.

### **Step 5: Test the Project**
1. Test the voice commands using your configured voice assistant.
   - For Android, speak the commands you've set up in Tasker.
   - For iOS, use Siri to activate your shortcuts.
2. Verify that the corresponding actions (e.g., light turns on/off, fan adjusts) occur as expected.
3. Check the soil moisture sensor by inserting it into the soil and observing the LEDs.
   - If the soil is dry, the green LED will light up. If the soil is moist, the red LED will light up.
4. Monitor the temperature and humidity in the serial monitor and ensure the fan automatically adjusts based on the temperature.

### **Step 6: Modifications (Optional)**
- Modify the voice commands or sensor thresholds in the Arduino code as needed.
- Add more relays to control additional appliances.
- Use a different temperature sensor (e.g., DHT22) for more accuracy.

## Code Explanation

```cpp
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

void setup() {
  Serial.begin(9600); // For Bluetooth communication
  dht.begin();
  
  pinMode(relayLight, OUTPUT);
  pinMode(relayFan, OUTPUT);
  pinMode(greenLED, OUTPUT);
  pinMode(redLED, OUTPUT);
}

void loop() {
  // Bluetooth command handling
  if (Serial.available() > 0) {
    String command = Serial.readString();
    command.trim();
    
    if (command == "TURN ON LIGHT") {
      digitalWrite(relayLight, HIGH); // Turn on light
      Serial.println("Light ON");
    } else if (command == "TURN OFF LIGHT") {
      digitalWrite(relayLight, LOW); // Turn off light
      Serial.println("Light OFF");
    } else if (command == "TURN ON FAN") {
      digitalWrite(relayFan, HIGH); // Turn on fan
      Serial.println("Fan ON");
    } else if (command == "TURN OFF FAN") {
      digitalWrite(relayFan, LOW); // Turn off fan
      Serial.println("Fan OFF");
    }
  }
  
  // Read and display temperature and humidity
  float temp = dht.readTemperature();
  float humidity = dht.readHumidity();
  
  if (isnan(temp) || isnan(humidity)) {
    Serial.println("Failed to read from DHT sensor!");
  } else {
    Serial.print("Temperature: ");
    Serial.print(temp);
    Serial.print(" °C, Humidity: ");
    Serial.print(humidity);
    Serial.println(" %");
    
    // Automatically control fan based on temperature
    if (temp > 30) {
      digitalWrite(relayFan, HIGH);
    } else {
      digitalWrite(relayFan, LOW);
    }
  }

  // Read soil moisture sensor value
  int soilMoisture = analogRead(soilMoisturePin);
  
  if (soilMoisture < 300) {
    digitalWrite(greenLED, HIGH);
    digitalWrite(redLED, LOW);
    Serial.println("Soil is dry.");
  } else if (soilMoisture >= 300) {
    digitalWrite(greenLED, LOW);
    digitalWrite(redLED, HIGH);
    Serial.println("Soil is moist.");
  }
  
  delay(2000);  // Wait for 2 seconds before next reading
}
