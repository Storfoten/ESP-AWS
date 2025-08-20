#!/bin/bash

# ESP32 Project Build Script
# This script provides common PlatformIO commands for development

echo "ESP32 Project Build Script"
echo "=========================="

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
        echo "  help           - Show this help message"
        echo ""
        echo "Examples:"
        echo "  $0 build"
        echo "  $0 upload"
        echo "  $0 lib-install \"WiFiManager\""
        ;;
esac