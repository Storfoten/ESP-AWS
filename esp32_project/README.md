# ESP32 AWS IoT Project

A comprehensive ESP32 project featuring AWS MQTT connectivity, WiFi AP mode configuration, analog/digital I/O management, and event-driven architecture.

## Key Features

- **AWS IoT MQTT Integration**: Secure MQTT communication with AWS IoT Core
- **WiFi AP Mode**: Easy WiFi configuration through captive portal
- **Event-Driven Architecture**: Modular, event-based system design
- **Analog/Digital I/O**: Read analog sensors and control digital outputs
- **Secure Communication**: SSL/TLS encryption for MQTT connections
- **Real-time Monitoring**: Continuous sensor data publishing
- **Remote Control**: MQTT-based command and control system

## Hardware Requirements

- ESP32 development board (ESP32 DevKit, NodeMCU-32S, etc.)
- USB cable for programming and power
- Analog sensor (e.g., potentiometer, light sensor, temperature sensor)
- Digital input device (e.g., button, switch, motion sensor)
- Digital output device (e.g., LED, relay, motor driver)

## Pin Connections

- **Built-in LED**: GPIO 2 (indicates system status)
- **Boot Button**: GPIO 0 (manual control and AP mode trigger)
- **Analog Input**: GPIO 36 (VP pin - analog sensor reading)
- **Digital Input**: GPIO 4 (digital sensor or switch)
- **Digital Output**: GPIO 5 (relay, LED, or other actuator)

## System Architecture

### Event-Driven Design
The project uses an event-driven architecture where:
- **Event System**: Central event management and routing
- **Event Handlers**: Modular functions that respond to specific events
- **Event Types**: WiFi, MQTT, sensor, and timer events
- **Asynchronous Processing**: Non-blocking event handling

### Component Structure
```
┌─────────────────┐    ┌─────────────────┐    ┌─────────────────┐
│   WiFi Manager  │    │   MQTT Client   │    │ Sensor Manager  │
│                 │    │                 │    │                 │
│ • AP Mode       │    │ • AWS IoT       │    │ • Analog I/O    │
│ • WiFi Config   │    │ • SSL/TLS       │    │ • Digital I/O   │
│ • Captive Portal│    │ • JSON Messages │    │ • Thresholds    │
└─────────────────┘    └─────────────────┘    └─────────────────┘
         │                       │                       │
         └───────────────────────┼───────────────────────┘
                                 │
                    ┌─────────────────┐
                    │  Event System   │
                    │                 │
                    │ • Event Queue   │
                    │ • Handlers      │
                    │ • Routing       │
                    └─────────────────┘
```

## Getting Started

### Prerequisites

1. **Install PlatformIO Core**:
   ```bash
   pip install platformio
   # or
   pipx install platformio
   ```

2. **AWS IoT Core Setup**:
   - Create an AWS IoT Core thing
   - Generate device certificates
   - Update `config.h` with your AWS endpoint and credentials

### Configuration

1. **Update AWS IoT Settings** in `config.h`:
   ```cpp
   #define AWS_IOT_ENDPOINT "your-aws-endpoint.amazonaws.com"
   #define AWS_IOT_CLIENT_ID "ESP32_Device_001"
   ```

2. **Replace Dummy Certificates** with actual AWS IoT certificates:
   - Root CA certificate
   - Device certificate
   - Private key

3. **Customize Pin Assignments** if needed:
   ```cpp
   #define ANALOG_PIN 36      // VP pin
   #define DIGITAL_INPUT_PIN 4
   #define DIGITAL_OUTPUT_PIN 5
   ```

### Building and Deployment

1. **Navigate to project**:
   ```bash
   cd esp32_project
   ```

2. **Build project**:
   ```bash
   ./build.sh build
   ```

3. **Upload to ESP32**:
   ```bash
   ./build.sh upload
   ```

4. **Monitor output**:
   ```bash
   ./build.sh monitor
   ```

## WiFi Configuration

### AP Mode Setup
1. Power on the ESP32
2. Connect to WiFi network: `ESP32_Config` (password: `12345678`)
3. Navigate to `192.168.4.1` in your browser
4. Enter your WiFi credentials
5. Device will connect to your network and attempt MQTT connection

### Manual WiFi Configuration
```cpp
// In main.cpp, you can manually set WiFi credentials
wifiManager.connectToWiFi("YourSSID", "YourPassword");
```

## MQTT Communication

### Topics
- **Sensor Data**: `esp32/device001/sensor/analog`, `esp32/device001/sensor/digital_in`
- **Device Status**: `esp32/device001/device/status`
- **Commands**: `esp32/device001/device/command`

### Message Format
```json
{
  "device_id": "ESP32_Device_001",
  "sensor_type": "analog",
  "value": 2048,
  "timestamp": 1234567890
}
```

### Commands
```json
{
  "action": "set_digital_output",
  "value": true
}
```

Available actions:
- `set_digital_output`: Set digital output state
- `toggle_digital_output`: Toggle digital output
- `set_analog_threshold`: Set analog reading threshold
- `get_status`: Request device status

## Event System

### Event Types
- `EVENT_WIFI_CONNECTED`: WiFi connection established
- `EVENT_WIFI_DISCONNECTED`: WiFi connection lost
- `EVENT_MQTT_CONNECTED`: MQTT connection established
- `EVENT_MQTT_DISCONNECTED`: MQTT connection lost
- `EVENT_MQTT_MESSAGE`: MQTT message received
- `EVENT_ANALOG_READ`: Analog sensor reading
- `EVENT_DIGITAL_READ`: Digital input change
- `EVENT_BUTTON_PRESS`: Button pressed
- `EVENT_TIMER`: Periodic timer event

### Adding Custom Events
```cpp
// Register event handler
eventSystem->registerHandler(EVENT_CUSTOM, handleCustomEvent);

// Post custom event
eventSystem->postEvent(Event(EVENT_CUSTOM, data, dataSize));
```

## Sensor Management

### Analog Input
- **Reading**: Continuous monitoring with configurable threshold
- **Calibration**: Offset adjustment for sensor calibration
- **Threshold**: Configurable change detection to reduce noise

### Digital I/O
- **Input**: Pull-up configuration with change detection
- **Output**: Direct control with state tracking
- **Debouncing**: Built-in button debouncing

### Configuration
```cpp
// Set analog threshold
sensorManager.setAnalogThreshold(100);

// Enable/disable threshold
sensorManager.setAnalogThresholdEnabled(true);

// Set calibration offset
sensorManager.setAnalogCalibrationOffset(-50);
```

## Security Features

### SSL/TLS Configuration
- **Root CA**: AWS IoT root certificate
- **Client Certificate**: Device-specific certificate
- **Private Key**: Device private key
- **Port**: 8883 (MQTT over SSL)

### Production Security
1. **Remove `setInsecure()`** in production
2. **Store certificates securely** (SPIFFS, secure element)
3. **Implement certificate rotation**
4. **Add authentication mechanisms**

## Troubleshooting

### Common Issues

1. **WiFi Connection Fails**:
   - Check SSID and password
   - Verify network availability
   - Check signal strength

2. **MQTT Connection Fails**:
   - Verify AWS IoT endpoint
   - Check certificate validity
   - Ensure proper SSL configuration

3. **Sensor Readings Unstable**:
   - Adjust analog threshold
   - Check wiring and connections
   - Calibrate sensor offset

4. **AP Mode Not Working**:
   - Check if WiFi is already connected
   - Verify AP credentials in config
   - Check available memory

### Debug Output
Enable debug output by setting in `platformio.ini`:
```ini
build_flags = -DCORE_DEBUG_LEVEL=5
```

### Serial Monitor
Use the serial monitor to view:
- System initialization
- WiFi connection status
- MQTT connection status
- Sensor readings
- Event processing

## Customization

### Adding New Sensors
1. **Create sensor class** extending `SensorManager`
2. **Register event handlers** for sensor events
3. **Update MQTT publishing** for new data types

### Adding New MQTT Topics
1. **Define topics** in `config.h`
2. **Subscribe to topics** in MQTT client
3. **Handle messages** in event handlers

### Extending Event System
1. **Add new event types** to `EventType` enum
2. **Create event data structures**
3. **Implement event handlers**

## Performance Optimization

### Memory Management
- **Event Queue Size**: Limit event queue to prevent memory overflow
- **JSON Buffer Size**: Optimize JSON document sizes
- **String Handling**: Use references to avoid string copying

### Power Management
- **Deep Sleep**: Implement deep sleep for battery operation
- **WiFi Power**: Optimize WiFi power consumption
- **Sensor Sampling**: Adjust sensor reading intervals

## Contributing

1. Fork the repository
2. Create a feature branch
3. Implement your changes
4. Add tests if applicable
5. Submit a pull request

## License

This project is open source and available under the MIT License.

## Support

For issues and questions:
1. Check the troubleshooting section
2. Review the code comments
3. Open an issue on GitHub
4. Check PlatformIO and ESP32 documentation