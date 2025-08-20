// Example Configuration File
// Copy this file to config.h and modify with your actual values

#ifndef CONFIG_EXAMPLE_H
#define CONFIG_EXAMPLE_H

// WiFi Configuration
#define WIFI_AP_SSID "ESP32_Config"
#define WIFI_AP_PASSWORD "12345678"
#define WIFI_AP_CHANNEL 1
#define WIFI_AP_MAX_CONNECTIONS 4

// AWS IoT Core Configuration - UPDATE THESE VALUES
#define AWS_IOT_ENDPOINT "your-aws-endpoint.amazonaws.com"
#define AWS_IOT_PORT 8883
#define AWS_IOT_CLIENT_ID "ESP32_Device_001"
#define AWS_IOT_TOPIC_PREFIX "esp32/device001"

// AWS IoT Credentials - REPLACE WITH ACTUAL CERTIFICATES
// You can get these from AWS IoT Core console
#define AWS_IOT_ROOT_CA "-----BEGIN CERTIFICATE-----\n" \
                        "PASTE YOUR ROOT CA CERTIFICATE HERE\n" \
                        "-----END CERTIFICATE-----"

#define AWS_IOT_CERT "-----BEGIN CERTIFICATE-----\n" \
                     "PASTE YOUR DEVICE CERTIFICATE HERE\n" \
                     "-----END CERTIFICATE-----"

#define AWS_IOT_PRIVATE_KEY "-----BEGIN RSA PRIVATE KEY-----\n" \
                           "PASTE YOUR PRIVATE KEY HERE\n" \
                           "-----END RSA PRIVATE KEY-----"

// Pin Definitions - MODIFY IF NEEDED
#define LED_PIN 2
#define BUTTON_PIN 0
#define ANALOG_PIN 36  // VP pin on ESP32
#define DIGITAL_INPUT_PIN 4
#define DIGITAL_OUTPUT_PIN 5

// MQTT Topics - CUSTOMIZE IF NEEDED
#define MQTT_TOPIC_ANALOG "sensor/analog"
#define MQTT_TOPIC_DIGITAL_IN "sensor/digital_in"
#define MQTT_TOPIC_DIGITAL_OUT "sensor/digital_out"
#define MQTT_TOPIC_STATUS "device/status"
#define MQTT_TOPIC_COMMAND "device/command"

// Timing Configuration - ADJUST BASED ON YOUR NEEDS
#define MQTT_KEEPALIVE 60
#define MQTT_RECONNECT_DELAY 5000
#define SENSOR_READ_INTERVAL 10000  // 10 seconds
#define STATUS_UPDATE_INTERVAL 30000 // 30 seconds

// Event Types (Don't modify these)
enum EventType {
    EVENT_WIFI_CONNECTED,
    EVENT_WIFI_DISCONNECTED,
    EVENT_MQTT_CONNECTED,
    EVENT_MQTT_DISCONNECTED,
    EVENT_MQTT_MESSAGE,
    EVENT_ANALOG_READ,
    EVENT_DIGITAL_READ,
    EVENT_BUTTON_PRESS,
    EVENT_TIMER
};

#endif