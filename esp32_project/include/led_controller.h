#ifndef LED_CONTROLLER_H
#define LED_CONTROLLER_H

#include <Arduino.h>

class LEDController {
private:
    int pin;
    bool state;
    unsigned long lastBlink;
    unsigned long interval;
    bool autoBlink;

public:
    LEDController(int ledPin);
    void begin();
    void turnOn();
    void turnOff();
    void toggle();
    void setBlinkInterval(unsigned long blinkInterval);
    void startAutoBlink();
    void stopAutoBlink();
    void update();
    bool getState();
};

#endif