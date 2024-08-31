#ifndef THERMISTOR_H
#define THERMISTOR_H

#include <Arduino.h>
#include <driver/adc.h>
#include "analogpin.h"

class Thermistor : public SmoothedAnalogPin {
public:
    // Constructor
    Thermistor(uint8_t pin, uint16_t resistance);

    // Returns current measured resistance.
    float_t resistance();

    // Returns the value in degrees Kelvin.
    float_t kelvin();

    // Returns the value in degrees Celsius.
    float_t celsius();

    uint16_t _resistance;
};

#endif // THERMISTOR_H