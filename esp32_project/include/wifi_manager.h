#ifndef WIFI_MANAGER_H
#define WIFI_MANAGER_H

#include <WiFi.h>
#include <WiFiManager.h>
#include <DNSServer.h>
#include <WebServer.h>
#include "config.h"
#include "event_system.h"

class WiFiManager {
private:
    WiFiManager wifiManager;
    DNSServer dnsServer;
    WebServer webServer;
    
    bool apModeActive;
    bool wifiConnected;
    String currentSSID;
    String currentIP;
    int currentRSSI;
    
    // AP mode configuration
    void setupAPMode();
    void handleAPMode();
    void stopAPMode();
    
    // Web server handlers
    void handleRoot();
    void handleWiFiConfig();
    void handleConnect();
    void handleStatus();
    
    // Event posting
    void postWiFiEvent(EventType type);
    
public:
    WiFiManager();
    
    // Initialize WiFi manager
    void begin();
    
    // Start AP mode for configuration
    void startAPMode();
    
    // Connect to WiFi
    bool connectToWiFi(const String& ssid, const String& password);
    
    // Disconnect from WiFi
    void disconnect();
    
    // Check if WiFi is connected
    bool isConnected() const { return wifiConnected; }
    
    // Get current WiFi info
    String getSSID() const { return currentSSID; }
    String getIP() const { return currentIP; }
    int getRSSI() const { return currentRSSI; }
    
    // Process WiFi manager
    void process();
    
    // Get WiFi status string
    String getStatusString() const;
};

#endif