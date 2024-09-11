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
  if (Serial.available() > 0) {
    String command = Serial.readString();
    command.trim();
    
    if (command == "TURN ON LIGHT") {
      digitalWrite(relayLight, HIGH);
      Serial.println("Light ON");
    } else if (command == "TURN OFF LIGHT") {
      digitalWrite(relayLight, LOW);
      Serial.println("Light OFF");
    } else if (command == "TURN ON FAN") {
      digitalWrite(relayFan, HIGH);
      Serial.println("Fan ON");
    } else if (command == "TURN OFF FAN") {
      digitalWrite(relayFan, LOW);
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
  
  delay(2000);
}
