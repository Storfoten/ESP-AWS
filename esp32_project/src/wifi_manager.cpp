#include "wifi_manager.h"

WiFiManager::WiFiManager() 
    : apModeActive(false), wifiConnected(false), currentSSID(""), currentIP(""), currentRSSI(0) {
}

void WiFiManager::begin() {
    // Configure WiFiManager
    wifiManager.setDebugOutput(true);
    wifiManager.setConfigPortalTimeout(180); // 3 minutes timeout
    
    // Set custom AP credentials
    wifiManager.setAPCallback([this](WiFiManager* myWiFiManager) {
        Serial.println("Entered AP mode");
        apModeActive = true;
        postWiFiEvent(EVENT_WIFI_DISCONNECTED);
    });
    
    wifiManager.setSaveConfigCallback([this]() {
        Serial.println("WiFi credentials saved");
        apModeActive = false;
    });
    
    // Set custom HTML for configuration portal
    wifiManager.setCustomHeadElement("<style>body{font-family:Arial;margin:20px;} .form-group{margin:15px 0;} input[type='text'],input[type='password']{width:100%;padding:8px;margin:5px 0;border:1px solid #ddd;border-radius:4px;} button{background-color:#4CAF50;color:white;padding:10px 15px;border:none;border-radius:4px;cursor:pointer;} button:hover{background-color:#45a049;}</style>");
}

void WiFiManager::startAPMode() {
    if (!apModeActive) {
        Serial.println("Starting AP mode for WiFi configuration");
        setupAPMode();
    }
}

void WiFiManager::setupAPMode() {
    // Configure AP
    WiFi.mode(WIFI_AP);
    WiFi.softAP(WIFI_AP_SSID, WIFI_AP_PASSWORD, WIFI_AP_CHANNEL, false, WIFI_AP_MAX_CONNECTIONS);
    
    // Start DNS server for captive portal
    dnsServer.start(53, "*", WiFi.softAPIP());
    
    // Start web server
    webServer.on("/", std::bind(&WiFiManager::handleRoot, this));
    webServer.on("/config", std::bind(&WiFiManager::handleWiFiConfig, this));
    webServer.on("/connect", std::bind(&WiFiManager::handleConnect, this));
    webServer.on("/status", std::bind(&WiFiManager::handleStatus, this));
    
    webServer.begin();
    apModeActive = true;
    
    Serial.print("AP Mode Active - SSID: ");
    Serial.print(WIFI_AP_SSID);
    Serial.print(" Password: ");
    Serial.println(WIFI_AP_PASSWORD);
    Serial.print("AP IP: ");
    Serial.println(WiFi.softAPIP());
}

void WiFiManager::handleAPMode() {
    dnsServer.processNextRequest();
    webServer.handleClient();
}

void WiFiManager::stopAPMode() {
    if (apModeActive) {
        webServer.stop();
        dnsServer.stop();
        WiFi.softAPdisconnect(true);
        apModeActive = false;
        Serial.println("AP mode stopped");
    }
}

void WiFiManager::handleRoot() {
    String html = "<html><head><title>ESP32 WiFi Config</title></head><body>";
    html += "<h1>ESP32 WiFi Configuration</h1>";
    html += "<p>Current Status: ";
    html += wifiConnected ? "Connected to " + currentSSID : "Not Connected";
    html += "</p>";
    html += "<p><a href='/config'>Configure WiFi</a></p>";
    html += "<p><a href='/status'>Status</a></p>";
    html += "</body></html>";
    
    webServer.send(200, "text/html", html);
}

void WiFiManager::handleWiFiConfig() {
    String html = "<html><head><title>WiFi Configuration</title></head><body>";
    html += "<h1>WiFi Configuration</h1>";
    html += "<form method='POST' action='/connect'>";
    html += "<div class='form-group'>";
    html += "<label>SSID:</label><br>";
    html += "<input type='text' name='ssid' required>";
    html += "</div>";
    html += "<div class='form-group'>";
    html += "<label>Password:</label><br>";
    html += "<input type='password' name='password' required>";
    html += "</div>";
    html += "<button type='submit'>Connect</button>";
    html += "</form>";
    html += "<p><a href='/'>Back to Home</a></p>";
    html += "</body></html>";
    
    webServer.send(200, "text/html", html);
}

void WiFiManager::handleConnect() {
    if (webServer.hasArg("ssid") && webServer.hasArg("password")) {
        String ssid = webServer.arg("ssid");
        String password = webServer.arg("password");
        
        if (connectToWiFi(ssid, password)) {
            webServer.send(200, "text/html", "<h1>Connection Successful!</h1><p>Redirecting...</p><script>setTimeout(function(){window.location.href='/';},2000);</script>");
        } else {
            webServer.send(400, "text/html", "<h1>Connection Failed</h1><p>Please check your credentials and try again.</p><a href='/config'>Try Again</a>");
        }
    } else {
        webServer.send(400, "text/html", "<h1>Missing Parameters</h1><p>SSID and password are required.</p><a href='/config'>Go Back</a>");
    }
}

void WiFiManager::handleStatus() {
    String status = "{\"wifi_connected\":" + String(wifiConnected ? "true" : "false");
    status += ",\"ssid\":\"" + currentSSID + "\"";
    status += ",\"ip\":\"" + currentIP + "\"";
    status += ",\"rssi\":" + String(currentRSSI);
    status += ",\"ap_mode\":" + String(apModeActive ? "true" : "false");
    status += "}";
    
    webServer.send(200, "application/json", status);
}

bool WiFiManager::connectToWiFi(const String& ssid, const String& password) {
    Serial.print("Attempting to connect to WiFi: ");
    Serial.println(ssid);
    
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid.c_str(), password.c_str());
    
    int attempts = 0;
    while (WiFi.status() != WL_CONNECTED && attempts < 20) {
        delay(500);
        Serial.print(".");
        attempts++;
    }
    
    if (WiFi.status() == WL_CONNECTED) {
        currentSSID = ssid;
        currentIP = WiFi.localIP().toString();
        currentRSSI = WiFi.RSSI();
        wifiConnected = true;
        
        Serial.println();
        Serial.println("WiFi connected successfully!");
        Serial.print("IP Address: ");
        Serial.println(currentIP);
        Serial.print("RSSI: ");
        Serial.println(currentRSSI);
        
        // Stop AP mode if it was active
        if (apModeActive) {
            stopAPMode();
        }
        
        postWiFiEvent(EVENT_WIFI_CONNECTED);
        return true;
    } else {
        Serial.println();
        Serial.println("WiFi connection failed!");
        wifiConnected = false;
        postWiFiEvent(EVENT_WIFI_DISCONNECTED);
        return false;
    }
}

void WiFiManager::disconnect() {
    WiFi.disconnect();
    wifiConnected = false;
    currentSSID = "";
    currentIP = "";
    currentRSSI = 0;
    
    Serial.println("WiFi disconnected");
    postWiFiEvent(EVENT_WIFI_DISCONNECTED);
}

void WiFiManager::process() {
    if (apModeActive) {
        handleAPMode();
    }
    
    // Check WiFi status
    if (wifiConnected && WiFi.status() != WL_CONNECTED) {
        wifiConnected = false;
        currentSSID = "";
        currentIP = "";
        currentRSSI = 0;
        
        Serial.println("WiFi connection lost");
        postWiFiEvent(EVENT_WIFI_DISCONNECTED);
    }
}

String WiFiManager::getStatusString() const {
    String status = "WiFi: ";
    if (wifiConnected) {
        status += "Connected to " + currentSSID + " (" + currentIP + ") RSSI: " + String(currentRSSI);
    } else if (apModeActive) {
        status += "AP Mode Active (" + String(WIFI_AP_SSID) + ")";
    } else {
        status += "Disconnected";
    }
    return status;
}

void WiFiManager::postWiFiEvent(EventType type) {
    EventSystem::getInstance()->postEvent(
        createWiFiEvent(type, currentSSID, currentIP, currentRSSI)
    );
}