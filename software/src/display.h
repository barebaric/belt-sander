#ifndef DISPLAY_H
#define DISPLAY_H

#include <Arduino.h>
#include <U8g2lib.h>

class Display {
public:
    // Constructor
    Display(uint8_t sda, uint8_t scl);

    // Initialize the display
    void begin();

    // Clear the display
    void clear();

    // Draw a string at the specified position
    void drawString(int x, int y, const char* str);

private:
    uint8_t _sda;
    uint8_t _scl;
    U8G2_SH1106_128X64_NONAME_F_HW_I2C _u8g2;
};

#endif // DISPLAY_H