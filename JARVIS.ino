#include <ESP8266WiFi.h>    // Include the ESP8266 library
#include <DHT.h>            // Include the DHT sensor library

#define DHTPIN 2            // Pin where the DHT sensor is connected
#define DHTTYPE DHT11       // Define the type of DHT sensor
DHT dht(DHTPIN, DHTTYPE);   // Create an instance of the DHT sensor

// Define relay pins for appliances
const int relayLight = 8;   // Relay for light control
const int relayFan = 9;     // Relay for fan control

// Define soil moisture sensor and LED pins
const int soilMoisturePin = A0; // Pin for the soil moisture sensor
const int greenLED = 13;  // LED for dry soil indication
const int redLED = 12;    // LED for moist soil indication

// Wi-Fi credentials
const char* ssid = "your_SSID";       // Replace with your Wi-Fi SSID
const char* password = "your_PASSWORD"; // Replace with your Wi-Fi password

// Create an instance of the Wi-Fi server
WiFiServer server(80); // Server will run on port 80

void setup() {
  Serial.begin(9600); // Start serial communication for debugging and Bluetooth
  dht.begin();        // Initialize the DHT sensor
  
  pinMode(relayLight, OUTPUT);  // Set relay pins as output
  pinMode(relayFan, OUTPUT);
  pinMode(greenLED, OUTPUT);    // Set LED pins as output
  pinMode(redLED, OUTPUT);

  // Connect to Wi-Fi network
  WiFi.begin(ssid, password);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Connected to Wi-Fi");

  // Start the server
  server.begin();
}

void loop() {
  // Handle Wi-Fi client requests
  WiFiClient client = server.available();
  if (client) {
    String request = client.readStringUntil('\r');
    client.flush();

    if (request.indexOf("/LIGHT_ON") != -1) {
      digitalWrite(relayLight, HIGH); // Turn on light
      Serial.println("Light ON");
    } else if (request.indexOf("/LIGHT_OFF") != -1) {
      digitalWrite(relayLight, LOW);  // Turn off light
      Serial.println("Light OFF");
    } else if (request.indexOf("/FAN_ON") != -1) {
      digitalWrite(relayFan, HIGH);   // Turn on fan
      Serial.println("Fan ON");
    } else if (request.indexOf("/FAN_OFF") != -1) {
      digitalWrite(relayFan, LOW);    // Turn off fan
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
      digitalWrite(relayLight, HIGH); // Turn on light
      Serial.println("Light ON");
    } else if (command == "TURN OFF LIGHT") {
      digitalWrite(relayLight, LOW);  // Turn off light
      Serial.println("Light OFF");
    } else if (command == "TURN ON FAN") {
      digitalWrite(relayFan, HIGH);   // Turn on fan
      Serial.println("Fan ON");
    } else if (command == "TURN OFF FAN") {
      digitalWrite(relayFan, LOW);    // Turn off fan
      Serial.println("Fan OFF");
    }
  }

  // Read temperature and humidity from DHT sensor
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
      digitalWrite(relayFan, HIGH); // Turn on fan if temperature > 30°C
    } else {
      digitalWrite(relayFan, LOW);  // Turn off fan if temperature <= 30°C
    }
  }

  // Read soil moisture sensor value
  int soilMoisture = analogRead(soilMoisturePin);

  if (soilMoisture < 300) { // Dry soil threshold
    digitalWrite(greenLED, HIGH); // Turn on green LED for dry soil
    digitalWrite(redLED, LOW);    // Turn off red LED
    Serial.println("Soil is dry.");
  } else { // Moist soil
    digitalWrite(greenLED, LOW);  // Turn off green LED
    digitalWrite(redLED, HIGH);   // Turn on red LED for moist soil
    Serial.println("Soil is moist.");
  }

  delay(2000); // Wait for 2 seconds before the next reading
}
