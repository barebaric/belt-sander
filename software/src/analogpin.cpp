#include <Arduino.h>
#include <driver/adc.h>
#include "analogpin.h"
#include "util.h"

AnalogPin::AnalogPin(uint8_t pin, uint low, uint high)
    : _pin(pin),
      _low(low),
      _high(high) {
}

AnalogPin::~AnalogPin() {
}

void AnalogPin::begin() {
    //adc1_config_width(ADC_WIDTH_BIT_13);
    //adc1_config_channel_atten(_channel, ADC_ATTEN_MAX);
}

int AnalogPin::value() {
    return analogRead(_pin);
}

float_t AnalogPin::voltage() {
    return 1.0 * value() / _high * 3.3;
}

float_t AnalogPin::percent() {
    return (value() - _low) / (_high - _low);
}



SmoothedAnalogPin::SmoothedAnalogPin(uint8_t pin, int bufferSize, float outlierThreshold, uint low, uint high)
    : AnalogPin(pin, low, high),
      _bufferSize(bufferSize),
      _outlierThreshold(outlierThreshold), 
      _index(0),
      _sum(0.0),
      _count(0) {
    _values = new int[_bufferSize];
    for (int i = 0; i < _bufferSize; ++i)
        _values[i] = 0;
}

SmoothedAnalogPin::~SmoothedAnalogPin() {
    delete[] _values;
}

void SmoothedAnalogPin::begin() {
    AnalogPin::begin();
    _add_value();
}

int SmoothedAnalogPin::raw_value() {
    return AnalogPin::value();
}

float_t SmoothedAnalogPin::raw_voltage() {
    return AnalogPin::voltage();
}

float_t SmoothedAnalogPin::raw_percent() {
    return AnalogPin::percent();
}

int SmoothedAnalogPin::_add_value() {
    int thevalue = raw_value();
    _values[_index] = thevalue;

    // Move to the next index
    _index = (_index + 1) % _bufferSize;

    // Update the count (until we fill the array)
    if (_count < _bufferSize)
        _count++;

/*
    Serial.print("BUFFER AFTER: ");
    for (int i = 0; i < _count; i++)
        Serial.print(_values[i]);
    Serial.println();
*/
    return thevalue;
}

int SmoothedAnalogPin::_compute_median() {
    return medianFromIntArray(_values, _count);
}

int SmoothedAnalogPin::_compute_threshold() {
    float minVal = _values[0];
    float maxVal = _values[0];
    for (int i = 1; i < _count; ++i) {
        if (_values[i] < minVal) minVal = _values[i];
        if (_values[i] > maxVal) maxVal = _values[i];
    }
    return (maxVal - minVal) * _outlierThreshold;
}

int SmoothedAnalogPin::value() {
    int newValue = _add_value();
    int median = _compute_median();
    int threshold = _compute_threshold();

    // Compute the average of all values, excluding outliers.
    int sum = 0;
    int validCount = 0;

    for (int i = 0; i < _count; ++i) {
        if (_values[i] >= median - threshold && _values[i] <= median + threshold) {
            sum += _values[i];
            ++validCount;
        }
    }

    if (validCount == 0)
        return median;  // Fallback if no valid values

    return sum / validCount;
}

float_t SmoothedAnalogPin::voltage() {
    return 1.0 * value() / _high * 3.3;
}

float_t SmoothedAnalogPin::percent() {
    float_t val = value();
    return (val - _low) / (_high - _low);
}