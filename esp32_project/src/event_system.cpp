#include "event_system.h"

EventSystem* EventSystem::instance = nullptr;

EventSystem* EventSystem::getInstance() {
    if (instance == nullptr) {
        instance = new EventSystem();
    }
    return instance;
}

void EventSystem::registerHandler(EventType type, EventHandler handler) {
    if (type >= 0 && type <= EVENT_TIMER) {
        handlers[type].push_back(handler);
    }
}

void EventSystem::unregisterHandler(EventType type, EventHandler handler) {
    if (type >= 0 && type <= EVENT_TIMER) {
        auto& handlerList = handlers[type];
        handlerList.erase(
            std::remove(handlerList.begin(), handlerList.end(), handler),
            handlerList.end()
        );
    }
}

void EventSystem::postEvent(const Event& event) {
    eventQueue.push_back(event);
}

void EventSystem::processEvents() {
    unsigned long currentTime = millis();
    
    // Process all events in queue
    while (!eventQueue.empty()) {
        Event event = eventQueue.front();
        eventQueue.erase(eventQueue.begin());
        
        // Call all registered handlers for this event type
        if (event.type >= 0 && event.type <= EVENT_TIMER) {
            for (auto& handler : handlers[event.type]) {
                handler(event);
            }
        }
    }
    
    lastProcessTime = currentTime;
}

void EventSystem::clearEvents() {
    eventQueue.clear();
}

// Helper functions for creating events
Event createWiFiEvent(EventType type, const String& ssid, const String& ip, int rssi) {
    WiFiEventData* data = new WiFiEventData{ssid, ip, rssi};
    return Event(type, data, sizeof(WiFiEventData));
}

Event createMQTTEvent(EventType type, const String& topic, const String& payload) {
    MQTTPublishData* data = new MQTTPublishData{topic, payload, false};
    return Event(type, data, sizeof(MQTTPublishData));
}

Event createSensorEvent(EventType type, int analogValue, bool digitalValue, const String& sensorType) {
    SensorData* data = new SensorData{analogValue, digitalValue, sensorType};
    return Event(type, data, sizeof(SensorData));
}