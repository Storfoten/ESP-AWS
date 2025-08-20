#include "sensor_manager.h"

SensorManager::SensorManager() 
    : analogPin(ANALOG_PIN), digitalInputPin(DIGITAL_INPUT_PIN), digitalOutputPin(DIGITAL_OUTPUT_PIN),
      lastAnalogValue(0), lastDigitalInputValue(false), digitalOutputState(false),
      lastAnalogRead(0), lastDigitalRead(0), lastStatusUpdate(0),
      analogThreshold(100), analogCalibrationOffset(0), analogThresholdEnabled(true) {
}

void SensorManager::begin() {
    // Configure pins
    pinMode(digitalInputPin, INPUT_PULLUP);
    pinMode(digitalOutputPin, OUTPUT);
    
    // Set initial digital output state
    digitalWrite(digitalOutputPin, LOW);
    digitalOutputState = false;
    
    // Read initial values
    lastAnalogValue = readAnalog();
    lastDigitalInputValue = readDigitalInput();
    
    Serial.println("Sensor Manager initialized");
    Serial.print("Analog Pin: ");
    Serial.println(analogPin);
    Serial.print("Digital Input Pin: ");
    Serial.println(digitalInputPin);
    Serial.print("Digital Output Pin: ");
    Serial.println(digitalOutputPin);
}

int SensorManager::readAnalog() {
    // Read analog value with calibration offset
    int rawValue = analogRead(analogPin);
    int calibratedValue = rawValue + analogCalibrationOffset;
    
    // Apply threshold if enabled
    if (analogThresholdEnabled && abs(calibratedValue - lastAnalogValue) < analogThreshold) {
        return lastAnalogValue; // Return last value if change is below threshold
    }
    
    return calibratedValue;
}

bool SensorManager::readDigitalInput() {
    return digitalRead(digitalInputPin) == LOW; // Inverted due to INPUT_PULLUP
}

void SensorManager::setDigitalOutput(bool state) {
    digitalWrite(digitalOutputPin, state);
    digitalOutputState = state;
    
    Serial.print("Digital output set to: ");
    Serial.println(state ? "HIGH" : "LOW");
}

void SensorManager::toggleDigitalOutput() {
    setDigitalOutput(!digitalOutputState);
}

void SensorManager::process() {
    unsigned long currentTime = millis();
    
    // Read analog sensor periodically
    if (currentTime - lastAnalogRead >= SENSOR_READ_INTERVAL) {
        int currentAnalogValue = readAnalog();
        
        if (currentAnalogValue != lastAnalogValue) {
            Serial.print("Analog value changed: ");
            Serial.print(lastAnalogValue);
            Serial.print(" -> ");
            Serial.println(currentAnalogValue);
            
            lastAnalogValue = currentAnalogValue;
            postSensorEvent(EVENT_ANALOG_READ, currentAnalogValue);
        }
        
        lastAnalogRead = currentTime;
    }
    
    // Read digital input periodically
    if (currentTime - lastDigitalRead >= SENSOR_READ_INTERVAL) {
        bool currentDigitalValue = readDigitalInput();
        
        if (currentDigitalValue != lastDigitalInputValue) {
            Serial.print("Digital input changed: ");
            Serial.print(lastDigitalInputValue ? "HIGH" : "LOW");
            Serial.print(" -> ");
            Serial.println(currentDigitalValue ? "HIGH" : "LOW");
            
            lastDigitalInputValue = currentDigitalValue;
            postSensorEvent(EVENT_DIGITAL_READ, 0, currentDigitalValue);
        }
        
        lastDigitalRead = currentTime;
    }
    
    // Update status periodically
    if (currentTime - lastStatusUpdate >= STATUS_UPDATE_INTERVAL) {
        lastStatusUpdate = currentTime;
        
        // Post status event
        postSensorEvent(EVENT_TIMER, lastAnalogValue, lastDigitalInputValue);
    }
}

String SensorManager::getStatusString() const {
    String status = "Sensors: ";
    status += "Analog=" + String(lastAnalogValue);
    status += " DigitalIn=" + String(lastDigitalInputValue ? "HIGH" : "LOW");
    status += " DigitalOut=" + String(digitalOutputState ? "HIGH" : "LOW");
    status += " Threshold=" + String(analogThreshold);
    status += " Calibration=" + String(analogCalibrationOffset);
    return status;
}

bool SensorManager::hasAnalogChanged(int threshold) const {
    return abs(lastAnalogValue - analogRead(analogPin)) >= threshold;
}

bool SensorManager::hasDigitalInputChanged() const {
    return lastDigitalInputValue != readDigitalInput();
}

void SensorManager::postSensorEvent(EventType type, int analogValue, bool digitalValue) {
    EventSystem::getInstance()->postEvent(
        createSensorEvent(type, analogValue, digitalValue, "sensor_manager")
    );
}