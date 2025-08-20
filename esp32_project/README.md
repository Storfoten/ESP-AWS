# ESP32 Project

A basic ESP32 project created with PlatformIO.

## Features

- LED blinking with configurable interval
- Button input handling with debouncing
- WiFi connectivity support (commented out by default)
- Serial communication for debugging
- PlatformIO configuration for easy development

## Hardware Requirements

- ESP32 development board (ESP32 DevKit, NodeMCU-32S, etc.)
- USB cable for programming and power
- Optional: External LED and button (if not using built-in ones)

## Pin Connections

- **LED**: GPIO 2 (built-in LED on most boards)
- **Button**: GPIO 0 (boot button on most boards)

## Getting Started

### Prerequisites

1. Install PlatformIO Core:
   ```bash
   pip install platformio
   # or
   pipx install platformio
   ```

2. Install PlatformIO IDE (optional but recommended):
   - VS Code with PlatformIO extension
   - CLion with PlatformIO plugin

### Building and Uploading

1. Navigate to the project directory:
   ```bash
   cd esp32_project
   ```

2. Build the project:
   ```bash
   pio run
   ```

3. Upload to your ESP32:
   ```bash
   pio run --target upload
   ```

4. Monitor serial output:
   ```bash
   pio device monitor
   ```

### Configuration

1. **WiFi Setup**: Uncomment and modify the WiFi credentials in `src/main.cpp`
2. **Custom Pins**: Modify the pin definitions at the top of `src/main.cpp`
3. **Build Flags**: Add custom build flags in `platformio.ini`

## Project Structure

```
esp32_project/
├── platformio.ini          # PlatformIO configuration
├── src/
│   └── main.cpp            # Main Arduino sketch
├── include/                 # Header files
├── lib/                     # Custom libraries
├── test/                    # Unit tests
└── README.md               # This file
```

## Troubleshooting

### Common Issues

1. **Upload fails**: Check USB connection and board selection
2. **Serial monitor not working**: Verify baud rate (115200)
3. **WiFi connection issues**: Check credentials and network availability

### Debugging

- Use `pio run --target upload --target monitor` to upload and monitor in one command
- Enable debug output by uncommenting debug options in `platformio.ini`

## Customization

### Adding Libraries

1. Add library dependencies to `platformio.ini`:
   ```ini
   lib_deps = 
       library_name
       another_library@^1.0.0
   ```

2. Or install manually:
   ```bash
   pio lib install "library_name"
   ```

### Adding Custom Code

- Place custom functions in `src/main.cpp`
- Create header files in `include/` directory
- Add custom libraries in `lib/` directory

## License

This project is open source and available under the MIT License.

## Contributing

Feel free to submit issues and enhancement requests!