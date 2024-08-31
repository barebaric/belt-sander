#include <Arduino.h>
#include "util.h"

String rstrip(const String& input) {
    // Create a copy of the input string
    String result = input;

    // Remove trailing whitespace characters
    int length = result.length();
    while (length > 0 && (result[length - 1] == ' ' || result[length - 1] == '\t' || result[length - 1] == '\n' || result[length - 1] == '\r')) {
        length--;
    }

    // Set the length of the result string
    result.remove(length);

    return result;
}

String concatenateWithNewlines(const char* separator, const String& first, ...) {
    String result;
    bool firstString = true;

    va_list args;
    va_start(args, first);

    const String* current = &first;

    while (current != NULL) {
        if (!current->isEmpty()) {
            if (!firstString)
                result += '\n';
            result += *current;
            firstString = false;
        }

        current = va_arg(args, const String*);
    }

    va_end(args);
    result.trim();
    return result;
}

void sortIntArray(int* array, int size) {
    for (int i = 0; i < size - 1; ++i) {
        for (int j = 0; j < size - i - 1; ++j) {
            if (array[j] > array[j + 1]) {
                float temp = array[j];
                array[j] = array[j + 1];
                array[j + 1] = temp;
            }
        }
    }
}

int medianFromIntArray(int* array, int size) {
    int* tempValues = new int[size];
    for (int i = 0; i < size; ++i)
        tempValues[i] = array[i];
    sortIntArray(tempValues, size);

    float median = (size % 2 == 0) ? 
        (tempValues[size / 2 - 1] + tempValues[size / 2]) / 2.0 : 
        tempValues[size / 2];

    delete[] tempValues;
    return median;
}