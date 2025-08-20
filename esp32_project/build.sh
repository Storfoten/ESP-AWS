#!/bin/bash

# ESP32 AWS IoT Project Build Script
# This script provides common PlatformIO commands for development

echo "ESP32 AWS IoT Project Build Script"
echo "=================================="

case "$1" in
    "build")
        echo "Building project..."
        pio run
        ;;
    "upload")
        echo "Building and uploading to ESP32..."
        pio run --target upload
        ;;
    "monitor")
        echo "Opening serial monitor..."
        pio device monitor
        ;;
    "upload-monitor")
        echo "Building, uploading, and opening monitor..."
        pio run --target upload --target monitor
        ;;
    "clean")
        echo "Cleaning build files..."
        pio run --target clean
        ;;
    "test")
        echo "Running tests..."
        pio test
        ;;
    "lib-install")
        if [ -z "$2" ]; then
            echo "Usage: $0 lib-install <library_name>"
            exit 1
        fi
        echo "Installing library: $2"
        pio lib install "$2"
        ;;
    "lib-search")
        if [ -z "$2" ]; then
            echo "Usage: $0 lib-search <search_term>"
            exit 1
        fi
        echo "Searching for library: $2"
        pio lib search "$2"
        ;;
    "boards")
        echo "Available boards:"
        pio boards espressif32
        ;;
    "check-config")
        echo "Checking project configuration..."
        echo "1. Verify AWS IoT endpoint in config.h"
        echo "2. Check AWS IoT certificates"
        echo "3. Verify pin assignments"
        echo "4. Check WiFi AP settings"
        ;;
    "setup-aws")
        echo "AWS IoT Setup Guide:"
        echo "1. Go to AWS IoT Core console"
        echo "2. Create a new thing"
        echo "3. Generate certificates"
        echo "4. Update config.h with your credentials"
        echo "5. Attach policies to your certificates"
        echo "6. Update AWS_IOT_ENDPOINT in config.h"
        ;;
    "wifi-test")
        echo "Testing WiFi configuration..."
        echo "1. Upload code to ESP32"
        echo "2. Connect to ESP32_Config WiFi network"
        echo "3. Navigate to 192.168.4.1"
        echo "4. Enter your WiFi credentials"
        echo "5. Check serial monitor for connection status"
        ;;
    "mqtt-test")
        echo "Testing MQTT connection..."
        echo "1. Ensure WiFi is connected"
        echo "2. Check serial monitor for MQTT status"
        echo "3. Verify AWS IoT endpoint and certificates"
        echo "4. Check MQTT topics and messages"
        ;;
    "help"|*)
        echo "Available commands:"
        echo "  build          - Build the project"
        echo "  upload         - Build and upload to ESP32"
        echo "  monitor        - Open serial monitor"
        echo "  upload-monitor - Build, upload, and monitor"
        echo "  clean          - Clean build files"
        echo "  test           - Run tests"
        echo "  lib-install    - Install a library"
        echo "  lib-search     - Search for libraries"
        echo "  boards         - Show available ESP32 boards"
        echo "  check-config   - Check project configuration"
        echo "  setup-aws      - Show AWS IoT setup guide"
        echo "  wifi-test      - Test WiFi configuration"
        echo "  mqtt-test      - Test MQTT connection"
        echo "  help           - Show this help message"
        echo ""
        echo "Examples:"
        echo "  $0 build"
        echo "  $0 upload"
        echo "  $0 lib-install \"WiFiManager\""
        echo "  $0 check-config"
        echo "  $0 setup-aws"
        ;;
esac