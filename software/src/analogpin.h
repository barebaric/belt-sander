#ifndef ANALOGPIN_H
#define ANALOGPIN_H

#include <Arduino.h>
#include <driver/adc.h>

class AnalogPin {
public:
    // Constructor
    AnalogPin(uint8_t pin, uint low=14, uint high=pow(2, SOC_ADC_MAX_BITWIDTH));

    // Initialize the pin
    void begin();

    // Returns the value as returned by analogRead()
    int raw_value();

    // Returns the value as a voltage
    float_t voltage();

    // Returns the value as a normalized float (i.e. between 0 and 1)
    float_t value();

    uint8_t _pin;
    uint _low;
    uint _high;
};

#endif // ANALOGPIN_H