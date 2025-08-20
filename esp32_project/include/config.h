#ifndef CONFIG_H
#define CONFIG_H

// WiFi Configuration
#define WIFI_AP_SSID "ESP32_Config"
#define WIFI_AP_PASSWORD "12345678"
#define WIFI_AP_CHANNEL 1
#define WIFI_AP_MAX_CONNECTIONS 4

// AWS IoT Core Configuration
#define AWS_IOT_ENDPOINT "your-aws-endpoint.amazonaws.com"
#define AWS_IOT_PORT 8883
#define AWS_IOT_CLIENT_ID "ESP32_Device_001"
#define AWS_IOT_TOPIC_PREFIX "esp32/device001"

// AWS IoT Credentials (Dummy - Replace with actual certificates)
#define AWS_IOT_ROOT_CA "-----BEGIN CERTIFICATE-----\n" \
                        "MIIDQTCCAimgAwIBAgITBmyfz5m/jAo54vB4ikP3EjrcjQKjANBgkqhkiG9w0BAQsF\n" \
                        "ADA5MQswCQYDVQQGEwJVUzEPMA0GA1UEChMGQW1hem9uMRkwFwYDVQQDExBBbWF6\n" \
                        "b24gUm9vdCBDQSAxMB4XDTE1MDUyNjAwMDAwMFoXDTM4MDExNzAwMDAwMFowOTEL\n" \
                        "MAkGA1UEBhMCVVMxDzANBgNVBAoTBkFtYXpvbjEZMBcGA1UEAxMQQW1hem9uIFJv\n" \
                        "b3QgQ0EgMTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBALJ4gHHKeNXj\n" \
                        "ca9HgFB0fW7Y14h29Jlo91ghYPl0hAEvrAIthtOgQ3pOsqTQNroBvo3bSMgHFzZM\n" \
                        "9O6II8c+6zf1tRn4SWiw3te5djgdYZ6k/oI2peVKVuRF4fn9tBb6dNqcmzU5L/qw\n" \
                        "IFAGbHrQgLKm+a/sRxmPUDgH3KKHOVj4utWp+UhnMJbulHheb4mjUcAwhmahRWaS\n" \
                        "VOuQ6PZn+ahrmW0eD0+lSgi3cWbuJqRk/0+uC8l/1eudLcGxZUDcDuI0GIM1a0/2\n" \
                        "qDuCCmqG63J4C9POM9OEyeP9aGDCplw8D4KMI0xLmJVeGmcjG9KjlNwpM5LPI1A\n" \
                        "8kHml3BOnGOL1LpxqXW3ySJv6KfaTRskZ2zyNx6AsFCeJc9P7h2X9n9qpk0aahR\n" \
                        "lgn0MsQcAfW31rF9lnSfmR6hEeQJqC9ZR0gOyLywqIDv3KcSXb4TsGmvHAtCk9\n" \
                        "-----END CERTIFICATE-----"

#define AWS_IOT_CERT "-----BEGIN CERTIFICATE-----\n" \
                     "MIIDWTCCAkGgAwIBAgIUVp1h6I0mYLjUrW8uJ6Rq1rF7U5cwDQYJKoZIhvcNAQEL\n" \
                     "BQAwTTFLMAkGA1UEBhMCVVMxDzANBgNVBAoTBkFtYXpvbjEUMBIGA1UECxMLRW50\n" \
                     "ZXJwcmlzZXMxGzAZBgNVBAMTElRlc3QgQ2VydGlmaWNhdGUgMTAeFw0yNDAxMDEw\n" \
                     "MDAwMDBaFw0yNTAxMDEwMDAwMDBaME0xSzAJBgNVBAYTAlVTMQ8wDQYDVQQKEwZB\n" \
                     "bWF6b24xFDASBgNVBAsTC0VudGVycHJpc2VzMRswGQYDVQQDExJUZXN0IENlcnRp\n" \
                     "ZmljYXRlIDEwggEiMA0GCSqGSIb3DQEBAQUAA4IBDwAwggEKAoIBAQC7VJTUt9Us\n" \
                     "8cK2mHzx4tf/JboI6Z4Qcr7Q0/BPz2rPlHn5NB4Mof3jXzvsQfn2AlBhr9C4FjT\n" \
                     "oI4tI8AIkl+88BKjGRMDqXNcdBL6oPkKHTTw2f7XBhQ8iZMVccEtOyW48xP1Dnd\n" \
                     "Hc5UVlxJ+V+62u9VlpSo18L+A2ck/8ySB4pXls6Ne553WxkhY56RkaTtxuJvKSj\n" \
                     "WjBUFcrPJChkPGFoPqOCDDNOXDIDMW3buJfXblrOr5UreWDfF8fM2hLtBu7R1y9\n" \
                     "upZ2oGdHrG5MVfX4RnaLQNuMZBuPaY6W1AOZf9GcTnwL3CHMsMu/ftAIp1itKrB\n" \
                     "GmOvHDxMQVxr5gqCjp4r+bP6/c0ktsS+TIWxLZN1X68Zs5zSfiMfbZk5I2ahhlT\n" \
                     "-----END CERTIFICATE-----"

#define AWS_IOT_PRIVATE_KEY "-----BEGIN RSA PRIVATE KEY-----\n" \
                           "MIIEpAIBAAKCAQEAu1SU1LfVLPHCtph88eLX/yW6COmeEHK+0NPwT89qzlR5+TQe\n" \
                           "DKH941877EH59gJQYa/QuBY06COLSPACJJfvPASoxkTA6lzXHQS+qD5Ch008Nn+1\n" \
                           "wYUPImTFXHBLTstuPMT9Q53R3OVFZcSflfutrvVZaUqNfC/gNnJP/MkgeKV5bOjX\n" \
                           "ued1sZJWOekZGk7cbibyko1owVBXKzyQoZDxhaD6jggwzTlwyAzFt27iX125azq+\n" \
                           "VK3lg3xfHzNoS7Qbu0dcvbqWdqBnR6xuTFX1+EZ2i0DbjGQbj2mOltQDmX/RnE58\n" \
                           "C9whzLDLj/r7QCKdYrSqwRpjjxw8TEFca+YKgo6eK/mz+v3NJLbEvkyFsS2TdV+v\n" \
                           "GbOckn4jH22ZOSNmoYZU3wIDAQABAoIBAGZLJuao4mco4q0QtNLjSIsx9Po+6+aK\n" \
                           "M/fsHajP1C/3CljzWBXaOi4M0XHuDq76kO1/2QY1eCahCwTkG1MX1UbpXOYwWAhQ\n" \
                           "-----END RSA PRIVATE KEY-----"

// Pin Definitions
#define LED_PIN 2
#define BUTTON_PIN 0
#define ANALOG_PIN 36  // VP pin on ESP32
#define DIGITAL_INPUT_PIN 4
#define DIGITAL_OUTPUT_PIN 5

// MQTT Topics
#define MQTT_TOPIC_ANALOG "sensor/analog"
#define MQTT_TOPIC_DIGITAL_IN "sensor/digital_in"
#define MQTT_TOPIC_DIGITAL_OUT "sensor/digital_out"
#define MQTT_TOPIC_STATUS "device/status"
#define MQTT_TOPIC_COMMAND "device/command"

// Timing Configuration
#define MQTT_KEEPALIVE 60
#define MQTT_RECONNECT_DELAY 5000
#define SENSOR_READ_INTERVAL 10000
#define STATUS_UPDATE_INTERVAL 30000

// Event Types
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