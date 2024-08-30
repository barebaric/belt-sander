#ifndef UTIL_H
#define UTIL_H

#include <Arduino.h>  // Include for the String class in Arduino environments

#define joinStrings(separator, ...) concatenateWithNewlines(separator, __VA_ARGS__, NULL)

String concatenateWithNewlines(const char* separator, const String& first, ...);

String rstrip(const String& input);

#endif // UTIL_H