#include "led_controller.h"

LEDController::LEDController(int ledPin) {
    pin = ledPin;
    state = false;
    lastBlink = 0;
    interval = 1000;
    autoBlink = false;
}

void LEDController::begin() {
    pinMode(pin, OUTPUT);
    digitalWrite(pin, LOW);
}

void LEDController::turnOn() {
    state = true;
    digitalWrite(pin, HIGH);
}

void LEDController::turnOff() {
    state = false;
    digitalWrite(pin, LOW);
}

void LEDController::toggle() {
    state = !state;
    digitalWrite(pin, state);
}

void LEDController::setBlinkInterval(unsigned long blinkInterval) {
    interval = blinkInterval;
}

void LEDController::startAutoBlink() {
    autoBlink = true;
}

void LEDController::stopAutoBlink() {
    autoBlink = false;
}

void LEDController::update() {
    if (autoBlink && millis() - lastBlink >= interval) {
        toggle();
        lastBlink = millis();
    }
}

bool LEDController::getState() {
    return state;
}