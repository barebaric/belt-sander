#include <Arduino.h>
#include <driver/adc.h>
#include "thermistor.h"

Thermistor::Thermistor(uint8_t pin, uint16_t resistance)
    : SmoothedAnalogPin(pin), _resistance(resistance) {
}

float_t Thermistor::resistance() {
    return _resistance * (3.3 / voltage() - 1.0);
}

float_t Thermistor::kelvin() {
    // Use the Steinhart-Hart equation to convert resistance to temperature in Kelvin
    float T0 = 280.15;  // Temperature in Kelvin at _resistance
    float B = 3950.0;  // Typical B-value for an NTC thermistor
    return 1.0 / ((1.0 / T0) + (1.0 / B) * log(resistance() / _resistance));
}

float_t Thermistor::celsius() {
    return kelvin() - 273.15;
}