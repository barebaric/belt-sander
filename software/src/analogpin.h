#ifndef ANALOGPIN_H
#define ANALOGPIN_H

#include <Arduino.h>
#include <driver/adc.h>

class AnalogPin {
public:
    // Constructor.
    // buffersize specifies the number of samples used for smoothing.
    // outlierThreshold specifies the maximum difference between two readings;
    // if it is exceeded the reading is considered an outlier and is ignored.
    AnalogPin(uint8_t pin,
              uint low=14,
              uint high=pow(2, SOC_ADC_MAX_BITWIDTH));

    ~AnalogPin();

    // Initialize the pin
    void begin();

    // Returns the value as returned by analogRead().
    int value();

    // Returns the value as a voltage.
    float_t voltage();

    // Returns the value as a normalized float (i.e. between 0 and 1).
    float_t percent();

protected:
    uint8_t _pin;
    uint _low;
    uint _high;
};


class SmoothedAnalogPin : public AnalogPin {
public:
    // Constructor.
    // buffersize specifies the number of samples used for smoothing.
    // outlierThreshold specifies the maximum difference between two readings
    // as a multiplier. If a value is considered an outlier, is ignored.
    SmoothedAnalogPin(uint8_t pin,
              int bufferSize=10,
              float outlierThreshold=1.5,
              uint low=14,
              uint high=pow(2, SOC_ADC_MAX_BITWIDTH));

    ~SmoothedAnalogPin();

    // Initialize the pin
    void begin();

    // Returns the value as returned by analogRead(), without smoothing.
    int raw_value();

    // Like raw_value(), but with smoothing applied.
    int value();

    // Returns the value as a voltage, without smoothing.
    float_t raw_voltage();

    // Returns the value as a voltage, after smoothing.
    float_t voltage();

    // Returns the value as a normalized float (i.e. between 0 and 1),
    // without smoothing.
    float_t raw_percent();

    // Returns the value as a normalized float (i.e. between 0 and 1),
    // after smoothing.
    float_t percent();

protected:
    int _add_value();          // Read one more value into the buffer (and return it, too)
    int _compute_median();     // Returns median of all values in the buffer
    int _compute_threshold();  // Returns maximum difference for outliers

    int* _values;              // Array to store the recent values for smoothing
    int _bufferSize;           // Size of the moving average array
    int _index;                // Current index in the array
    int _sum;                  // Sum of the values in the array
    int _count;                // Number of values currently in the array
    float _outlierThreshold;   // Threshold to determine outliers
};

#endif // ANALOGPIN_H