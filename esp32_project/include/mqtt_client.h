#ifndef MQTT_CLIENT_H
#define MQTT_CLIENT_H

#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include "config.h"
#include "event_system.h"

class MQTTClient {
private:
    WiFiClientSecure wifiClient;
    PubSubClient mqttClient;
    
    bool connected;
    unsigned long lastReconnectAttempt;
    unsigned long lastKeepAlive;
    
    // AWS IoT specific
    String clientId;
    String endpoint;
    int port;
    
    // SSL/TLS configuration
    void setupSSL();
    bool loadCertificates();
    
    // MQTT callbacks
    static void mqttCallback(char* topic, byte* payload, unsigned int length);
    void handleMessage(char* topic, byte* payload, unsigned int length);
    
    // Connection management
    bool connectToAWS();
    void disconnectFromAWS();
    
    // Event posting
    void postMQTTEvent(EventType type, const String& topic = "", const String& payload = "");
    
public:
    MQTTClient();
    
    // Initialize MQTT client
    void begin();
    
    // Connect to AWS IoT
    bool connect();
    
    // Disconnect from AWS IoT
    void disconnect();
    
    // Check if connected
    bool isConnected() const { return connected; }
    
    // Publish message
    bool publish(const String& topic, const String& payload, bool retained = false);
    
    // Subscribe to topic
    bool subscribe(const String& topic);
    
    // Unsubscribe from topic
    bool unsubscribe(const String& topic);
    
    // Process MQTT client
    void process();
    
    // Publish sensor data
    bool publishAnalogData(int value);
    bool publishDigitalData(bool value, const String& sensorType);
    bool publishStatus(const String& status);
    
    // Get connection status string
    String getStatusString() const;
    
    // Set client ID
    void setClientId(const String& id) { clientId = id; }
    
    // Set endpoint
    void setEndpoint(const String& ep) { endpoint = ep; }
    
    // Set port
    void setPort(int p) { port = p; }
};

#endif