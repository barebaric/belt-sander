#include <Arduino.h>
#include "potentiometer.h"

const int potentiometerMin = 14;
const int potentiometerMax = 1024;

Potentiometer::Potentiometer(uint8_t pin)
    : _pin(pin) {
}

void Potentiometer::begin() {
}

uint16_t Potentiometer::rawValue() {
    return analogRead(_pin);
}

float_t Potentiometer::value() {
    float_t raw = rawValue();
    return (raw - potentiometerMin) / (potentiometerMax - potentiometerMin);
}