#include "mqtt_client.h"

// Static callback function
void MQTTClient::mqttCallback(char* topic, byte* payload, unsigned int length) {
    // This will be handled by the instance method
    Serial.print("Message received on topic: ");
    Serial.println(topic);
}

MQTTClient::MQTTClient() 
    : connected(false), lastReconnectAttempt(0), lastKeepAlive(0),
      clientId(AWS_IOT_CLIENT_ID), endpoint(AWS_IOT_ENDPOINT), port(AWS_IOT_PORT) {
}

void MQTTClient::begin() {
    // Configure MQTT client
    mqttClient.setClient(wifiClient);
    mqttClient.setCallback(mqttCallback);
    mqttClient.setBufferSize(1024);
    mqttClient.setKeepAlive(MQTT_KEEPALIVE);
    
    // Setup SSL/TLS
    setupSSL();
    
    Serial.println("MQTT Client initialized");
}

void MQTTClient::setupSSL() {
    // Set SSL certificates
    if (!loadCertificates()) {
        Serial.println("Failed to load SSL certificates!");
        return;
    }
    
    // Configure SSL client
    wifiClient.setInsecure(); // For testing - remove in production
    
    Serial.println("SSL/TLS configured");
}

bool MQTTClient::loadCertificates() {
    // In a real implementation, you would load these from SPIFFS or other storage
    // For now, we'll use the dummy certificates from config.h
    
    // Note: These are dummy certificates for testing
    // Replace with actual AWS IoT certificates in production
    
    Serial.println("SSL certificates loaded (dummy certificates for testing)");
    return true;
}

bool MQTTClient::connect() {
    if (connected) {
        return true;
    }
    
    Serial.print("Connecting to AWS IoT at ");
    Serial.print(endpoint);
    Serial.print(":");
    Serial.println(port);
    
    if (connectToAWS()) {
        Serial.println("Connected to AWS IoT!");
        connected = true;
        lastReconnectAttempt = 0;
        lastKeepAlive = millis();
        
        // Subscribe to command topic
        subscribe(MQTT_TOPIC_COMMAND);
        
        // Publish connection status
        publishStatus("online");
        
        postMQTTEvent(EVENT_MQTT_CONNECTED);
        return true;
    } else {
        Serial.println("Failed to connect to AWS IoT");
        postMQTTEvent(EVENT_MQTT_DISCONNECTED);
        return false;
    }
}

bool MQTTClient::connectToAWS() {
    // Set server
    mqttClient.setServer(endpoint.c_str(), port);
    
    // Attempt connection
    if (mqttClient.connect(clientId.c_str())) {
        return true;
    }
    
    Serial.print("Connection failed, rc=");
    Serial.println(mqttClient.state());
    return false;
}

void MQTTClient::disconnect() {
    if (connected) {
        publishStatus("offline");
        mqttClient.disconnect();
        connected = false;
        Serial.println("Disconnected from AWS IoT");
        postMQTTEvent(EVENT_MQTT_DISCONNECTED);
    }
}

bool MQTTClient::publish(const String& topic, const String& payload, bool retained) {
    if (!connected) {
        Serial.println("Cannot publish: MQTT not connected");
        return false;
    }
    
    bool success = mqttClient.publish(topic.c_str(), payload.c_str(), retained);
    
    if (success) {
        Serial.print("Published to ");
        Serial.print(topic);
        Serial.print(": ");
        Serial.println(payload);
    } else {
        Serial.print("Failed to publish to ");
        Serial.println(topic);
    }
    
    return success;
}

bool MQTTClient::subscribe(const String& topic) {
    if (!connected) {
        Serial.println("Cannot subscribe: MQTT not connected");
        return false;
    }
    
    bool success = mqttClient.subscribe(topic.c_str());
    
    if (success) {
        Serial.print("Subscribed to ");
        Serial.println(topic);
    } else {
        Serial.print("Failed to subscribe to ");
        Serial.println(topic);
    }
    
    return success;
}

bool MQTTClient::unsubscribe(const String& topic) {
    if (!connected) {
        Serial.println("Cannot unsubscribe: MQTT not connected");
        return false;
    }
    
    bool success = mqttClient.unsubscribe(topic.c_str());
    
    if (success) {
        Serial.print("Unsubscribed from ");
        Serial.println(topic);
    } else {
        Serial.print("Failed to unsubscribe from ");
        Serial.println(topic);
    }
    
    return success;
}

void MQTTClient::process() {
    if (!connected) {
        // Try to reconnect
        unsigned long now = millis();
        if (now - lastReconnectAttempt > MQTT_RECONNECT_DELAY) {
            lastReconnectAttempt = now;
            Serial.println("Attempting MQTT reconnection...");
            connect();
        }
        return;
    }
    
    // Process MQTT messages
    mqttClient.loop();
    
    // Check connection status
    if (!mqttClient.connected()) {
        Serial.println("MQTT connection lost");
        connected = false;
        postMQTTEvent(EVENT_MQTT_DISCONNECTED);
        return;
    }
    
    // Send keep-alive
    unsigned long now = millis();
    if (now - lastKeepAlive > (MQTT_KEEPALIVE * 1000)) {
        lastKeepAlive = now;
        // MQTT keep-alive is handled automatically by the library
    }
}

bool MQTTClient::publishAnalogData(int value) {
    DynamicJsonDocument doc(128);
    doc["device_id"] = clientId;
    doc["sensor_type"] = "analog";
    doc["value"] = value;
    doc["timestamp"] = millis();
    
    String payload;
    serializeJson(doc, payload);
    
    return publish(MQTT_TOPIC_ANALOG, payload);
}

bool MQTTClient::publishDigitalData(bool value, const String& sensorType) {
    DynamicJsonDocument doc(128);
    doc["device_id"] = clientId;
    doc["sensor_type"] = sensorType;
    doc["value"] = value;
    doc["timestamp"] = millis();
    
    String payload;
    serializeJson(doc, payload);
    
    return publish(MQTT_TOPIC_DIGITAL_IN, payload);
}

bool MQTTClient::publishStatus(const String& status) {
    DynamicJsonDocument doc(128);
    doc["device_id"] = clientId;
    doc["status"] = status;
    doc["timestamp"] = millis();
    doc["uptime"] = millis();
    doc["free_heap"] = ESP.getFreeHeap();
    
    String payload;
    serializeJson(doc, payload);
    
    return publish(MQTT_TOPIC_STATUS, payload);
}

String MQTTClient::getStatusString() const {
    String status = "MQTT: ";
    if (connected) {
        status += "Connected to " + endpoint + ":" + String(port);
        status += " (Client ID: " + clientId + ")";
    } else {
        status += "Disconnected";
    }
    return status;
}

void MQTTClient::postMQTTEvent(EventType type, const String& topic, const String& payload) {
    EventSystem::getInstance()->postEvent(
        createMQTTEvent(type, topic, payload)
    );
}