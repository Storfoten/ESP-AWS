#ifndef EVENT_SYSTEM_H
#define EVENT_SYSTEM_H

#include <Arduino.h>
#include <functional>
#include <vector>
#include "config.h"

// Event structure
struct Event {
    EventType type;
    unsigned long timestamp;
    void* data;
    size_t dataSize;
    
    Event(EventType t, void* d = nullptr, size_t size = 0) 
        : type(t), timestamp(millis()), data(d), dataSize(size) {}
};

// Event handler function type
typedef std::function<void(const Event&)> EventHandler;

// Event system class
class EventSystem {
private:
    static EventSystem* instance;
    std::vector<EventHandler> handlers[EVENT_TIMER + 1];
    std::vector<Event> eventQueue;
    unsigned long lastProcessTime;
    
    EventSystem() : lastProcessTime(0) {}
    
public:
    static EventSystem* getInstance();
    
    // Register event handler
    void registerHandler(EventType type, EventHandler handler);
    
    // Unregister event handler
    void unregisterHandler(EventType type, EventHandler handler);
    
    // Post event to queue
    void postEvent(const Event& event);
    
    // Process all events in queue
    void processEvents();
    
    // Clear all events
    void clearEvents();
    
    // Get event count in queue
    size_t getEventCount() const { return eventQueue.size(); }
};

// Event data structures
struct WiFiEventData {
    String ssid;
    String ip;
    int rssi;
};

struct MQTTPublishData {
    String topic;
    String payload;
    bool retained;
};

struct SensorData {
    int analogValue;
    bool digitalValue;
    String sensorType;
};

// Helper functions for creating events
Event createWiFiEvent(EventType type, const String& ssid = "", const String& ip = "", int rssi = 0);
Event createMQTTEvent(EventType type, const String& topic = "", const String& payload = "");
Event createSensorEvent(EventType type, int analogValue = 0, bool digitalValue = false, const String& sensorType = "");

#endif