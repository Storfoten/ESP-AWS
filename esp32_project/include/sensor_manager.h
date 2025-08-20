#ifndef SENSOR_MANAGER_H
#define SENSOR_MANAGER_H

#include <Arduino.h>
#include "config.h"
#include "event_system.h"

class SensorManager {
private:
    // Pin configurations
    int analogPin;
    int digitalInputPin;
    int digitalOutputPin;
    
    // Sensor states
    int lastAnalogValue;
    bool lastDigitalInputValue;
    bool digitalOutputState;
    
    // Timing
    unsigned long lastAnalogRead;
    unsigned long lastDigitalRead;
    unsigned long lastStatusUpdate;
    
    // Thresholds and calibration
    int analogThreshold;
    int analogCalibrationOffset;
    bool analogThresholdEnabled;
    
    // Event posting
    void postSensorEvent(EventType type, int analogValue = 0, bool digitalValue = false);
    
public:
    SensorManager();
    
    // Initialize sensors
    void begin();
    
    // Read analog sensor
    int readAnalog();
    
    // Read digital input
    bool readDigitalInput();
    
    // Set digital output
    void setDigitalOutput(bool state);
    
    // Toggle digital output
    void toggleDigitalOutput();
    
    // Get sensor values
    int getAnalogValue() const { return lastAnalogValue; }
    bool getDigitalInputValue() const { return lastDigitalInputValue; }
    bool getDigitalOutputState() const { return digitalOutputState; }
    
    // Process sensors (call in main loop)
    void process();
    
    // Set analog threshold
    void setAnalogThreshold(int threshold) { analogThreshold = threshold; }
    
    // Enable/disable analog threshold
    void setAnalogThresholdEnabled(bool enabled) { analogThresholdEnabled = enabled; }
    
    // Set analog calibration offset
    void setAnalogCalibrationOffset(int offset) { analogCalibrationOffset = offset; }
    
    // Get sensor status string
    String getStatusString() const;
    
    // Check if analog value changed significantly
    bool hasAnalogChanged(int threshold = 10) const;
    
    // Check if digital input changed
    bool hasDigitalInputChanged() const;
};

#endif