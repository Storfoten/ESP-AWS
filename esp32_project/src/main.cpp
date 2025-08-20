#include <Arduino.h>
#include <WiFi.h>
#include "config.h"
#include "event_system.h"
#include "wifi_manager.h"
#include "mqtt_client.h"
#include "sensor_manager.h"

// Global instances
WiFiManager wifiManager;
MQTTClient mqttClient;
SensorManager sensorManager;
EventSystem* eventSystem;

// Button handling
unsigned long lastButtonPress = 0;
bool buttonPressed = false;

// System state
bool systemInitialized = false;
unsigned long lastStatusUpdate = 0;

// Event handlers
void handleWiFiEvent(const Event& event);
void handleMQTTEvent(const Event& event);
void handleSensorEvent(const Event& event);
void handleTimerEvent(const Event& event);

void setup() {
    // Initialize serial communication
    Serial.begin(115200);
    Serial.println("\n=== ESP32 AWS IoT Project Starting ===");
    
    // Initialize event system
    eventSystem = EventSystem::getInstance();
    
    // Register event handlers
    eventSystem->registerHandler(EVENT_WIFI_CONNECTED, handleWiFiEvent);
    eventSystem->registerHandler(EVENT_WIFI_DISCONNECTED, handleWiFiEvent);
    eventSystem->registerHandler(EVENT_MQTT_CONNECTED, handleMQTTEvent);
    eventSystem->registerHandler(EVENT_MQTT_DISCONNECTED, handleMQTTEvent);
    eventSystem->registerHandler(EVENT_MQTT_MESSAGE, handleMQTTEvent);
    eventSystem->registerHandler(EVENT_ANALOG_READ, handleSensorEvent);
    eventSystem->registerHandler(EVENT_DIGITAL_READ, handleSensorEvent);
    eventSystem->registerHandler(EVENT_TIMER, handleTimerEvent);
    
    // Initialize components
    Serial.println("Initializing components...");
    
    // Initialize WiFi manager
    wifiManager.begin();
    
    // Initialize MQTT client
    mqttClient.begin();
    
    // Initialize sensor manager
    sensorManager.begin();
    
    // Start AP mode for initial WiFi configuration
    Serial.println("Starting AP mode for WiFi configuration...");
    wifiManager.startAPMode();
    
    // Configure button pin
    pinMode(BUTTON_PIN, INPUT_PULLUP);
    
    Serial.println("Setup complete! System is ready.");
    systemInitialized = true;
}

void loop() {
    // Process all events
    eventSystem->processEvents();
    
    // Process components
    wifiManager.process();
    mqttClient.process();
    sensorManager.process();
    
    // Handle button press
    handleButtonPress();
    
    // Update system status periodically
    updateSystemStatus();
    
    // Small delay to prevent watchdog issues
    delay(10);
}

void handleButtonPress() {
    static bool lastButtonState = HIGH;
    bool currentButtonState = digitalRead(BUTTON_PIN);
    
    // Button press detection with debouncing
    if (currentButtonState == LOW && lastButtonState == HIGH) {
        unsigned long currentTime = millis();
        if (currentTime - lastButtonPress > 200) { // 200ms debounce
            buttonPressed = true;
            lastButtonPress = currentTime;
            
            Serial.println("Button pressed!");
            
            // Toggle digital output
            sensorManager.toggleDigitalOutput();
            
            // Post button press event
            eventSystem->postEvent(Event(EVENT_BUTTON_PRESS));
        }
    }
    
    lastButtonState = currentButtonState;
}

void updateSystemStatus() {
    unsigned long currentTime = millis();
    
    if (currentTime - lastStatusUpdate >= STATUS_UPDATE_INTERVAL) {
        lastStatusUpdate = currentTime;
        
        // Print system status
        Serial.println("\n=== System Status ===");
        Serial.println(wifiManager.getStatusString());
        Serial.println(mqttClient.getStatusString());
        Serial.println(sensorManager.getStatusString());
        Serial.println("====================\n");
        
        // Publish status to MQTT if connected
        if (mqttClient.isConnected()) {
            mqttClient.publishStatus("running");
        }
    }
}

// Event Handlers
void handleWiFiEvent(const Event& event) {
    switch (event.type) {
        case EVENT_WIFI_CONNECTED:
            Serial.println("WiFi Connected Event");
            if (event.data) {
                WiFiEventData* data = static_cast<WiFiEventData*>(event.data);
                Serial.print("SSID: ");
                Serial.println(data->ssid);
                Serial.print("IP: ");
                Serial.println(data->ip);
                Serial.print("RSSI: ");
                Serial.println(data->rssi);
            }
            
            // Attempt MQTT connection
            Serial.println("Attempting MQTT connection...");
            mqttClient.connect();
            break;
            
        case EVENT_WIFI_DISCONNECTED:
            Serial.println("WiFi Disconnected Event");
            // Disconnect MQTT if WiFi is lost
            if (mqttClient.isConnected()) {
                mqttClient.disconnect();
            }
            break;
    }
}

void handleMQTTEvent(const Event& event) {
    switch (event.type) {
        case EVENT_MQTT_CONNECTED:
            Serial.println("MQTT Connected Event");
            // Subscribe to topics
            mqttClient.subscribe(MQTT_TOPIC_COMMAND);
            break;
            
        case EVENT_MQTT_DISCONNECTED:
            Serial.println("MQTT Disconnected Event");
            break;
            
        case EVENT_MQTT_MESSAGE:
            Serial.println("MQTT Message Event");
            if (event.data) {
                MQTTPublishData* data = static_cast<MQTTPublishData*>(event.data);
                Serial.print("Topic: ");
                Serial.println(data->topic);
                Serial.print("Payload: ");
                Serial.println(data->payload);
                
                // Handle commands
                if (data->topic == MQTT_TOPIC_COMMAND) {
                    handleCommand(data->payload);
                }
            }
            break;
    }
}

void handleSensorEvent(const Event& event) {
    switch (event.type) {
        case EVENT_ANALOG_READ:
            Serial.println("Analog Read Event");
            if (event.data) {
                SensorData* data = static_cast<SensorData*>(event.data);
                Serial.print("Analog Value: ");
                Serial.println(data->analogValue);
                
                // Publish to MQTT if connected
                if (mqttClient.isConnected()) {
                    mqttClient.publishAnalogData(data->analogValue);
                }
            }
            break;
            
        case EVENT_DIGITAL_READ:
            Serial.println("Digital Read Event");
            if (event.data) {
                SensorData* data = static_cast<SensorData*>(event.data);
                Serial.print("Digital Value: ");
                Serial.println(data->digitalValue ? "HIGH" : "LOW");
                
                // Publish to MQTT if connected
                if (mqttClient.isConnected()) {
                    mqttClient.publishDigitalData(data->digitalValue, "digital_input");
                }
            }
            break;
    }
}

void handleTimerEvent(const Event& event) {
    // Periodic status update
    if (event.data) {
        SensorData* data = static_cast<SensorData*>(event.data);
        
        // Publish sensor data to MQTT if connected
        if (mqttClient.isConnected()) {
            mqttClient.publishAnalogData(data->analogValue);
            mqttClient.publishDigitalData(data->digitalValue, "digital_input");
        }
    }
}

void handleCommand(const String& command) {
    Serial.print("Executing command: ");
    Serial.println(command);
    
    // Parse JSON command
    DynamicJsonDocument doc(256);
    DeserializationError error = deserializeJson(doc, command);
    
    if (error) {
        Serial.print("JSON parsing failed: ");
        Serial.println(error.c_str());
        return;
    }
    
    // Handle different command types
    if (doc.containsKey("action")) {
        String action = doc["action"];
        
        if (action == "set_digital_output") {
            if (doc.containsKey("value")) {
                bool value = doc["value"];
                sensorManager.setDigitalOutput(value);
                Serial.print("Digital output set to: ");
                Serial.println(value ? "HIGH" : "LOW");
            }
        }
        else if (action == "toggle_digital_output") {
            sensorManager.toggleDigitalOutput();
            Serial.println("Digital output toggled");
        }
        else if (action == "set_analog_threshold") {
            if (doc.containsKey("threshold")) {
                int threshold = doc["threshold"];
                sensorManager.setAnalogThreshold(threshold);
                Serial.print("Analog threshold set to: ");
                Serial.println(threshold);
            }
        }
        else if (action == "get_status") {
            // Status will be published in the next status update
            Serial.println("Status request received");
        }
        else {
            Serial.print("Unknown action: ");
            Serial.println(action);
        }
    }
}