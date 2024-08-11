#ifndef POTENTIOMETER_H
#define POTENTIOMETER_H

#include <Arduino.h>

class Potentiometer {
public:
    // Constructor
    Potentiometer(uint8_t pin);

    // Initialize the potentiometer
    void begin();

    // Returns the value as returned by analogRead()
    uint16_t rawValue();

    // Returns the value as a normalized float (i.e. between 0 and 1)
    float_t value();

private:
    uint8_t _pin;
};

#endif // POTENTIOMETER_H