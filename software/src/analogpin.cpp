#include <Arduino.h>
#include <driver/adc.h>
#include "analogpin.h"

AnalogPin::AnalogPin(uint8_t pin, uint low, uint high)
    : _pin(pin), _low(low), _high(high) {
}

void AnalogPin::begin() {
    //adc1_config_width(ADC_WIDTH_BIT_13);
    //adc1_config_channel_atten(_channel, ADC_ATTEN_MAX);
}

int AnalogPin::raw_value() {
    return analogRead(_pin);
}

float_t AnalogPin::voltage() {
    return 1.0 * raw_value() / _high * 3.3;
}

float_t AnalogPin::value() {
    float_t raw = raw_value();
    return (raw - _low) / (_high - _low);
}