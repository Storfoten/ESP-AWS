#include <Arduino.h>
#include <WiFi.h>

// WiFi credentials - uncomment and modify as needed
// const char* ssid = "your_wifi_ssid";
// const char* password = "your_wifi_password";

// Pin definitions
const int LED_PIN = 2;  // Built-in LED on most ESP32 boards
const int BUTTON_PIN = 0;  // Boot button on most ESP32 boards

// Variables
bool ledState = false;
unsigned long lastBlink = 0;
const unsigned long BLINK_INTERVAL = 1000; // 1 second

void setup() {
  // Initialize serial communication
  Serial.begin(115200);
  Serial.println("ESP32 Project Starting...");
  
  // Initialize pins
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  
  // Set initial LED state
  digitalWrite(LED_PIN, LOW);
  
  // Optional: Connect to WiFi
  // WiFi.begin(ssid, password);
  // while (WiFi.status() != WL_CONNECTED) {
  //   delay(500);
  //   Serial.print(".");
  // }
  // Serial.println("\nWiFi connected!");
  // Serial.print("IP address: ");
  // Serial.println(WiFi.localIP());
  
  Serial.println("Setup complete!");
}

void loop() {
  // Blink LED every second
  if (millis() - lastBlink >= BLINK_INTERVAL) {
    ledState = !ledState;
    digitalWrite(LED_PIN, ledState);
    lastBlink = millis();
    
    if (ledState) {
      Serial.println("LED ON");
    } else {
      Serial.println("LED OFF");
    }
  }
  
  // Check button press
  if (digitalRead(BUTTON_PIN) == LOW) {
    Serial.println("Button pressed!");
    delay(100); // Simple debounce
  }
  
  // Small delay to prevent watchdog issues
  delay(10);
}