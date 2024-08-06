#include <U8g2lib.h>
#include "display.h"

// Constructor implementation
Display::Display(uint8_t sda, uint8_t scl)
    : _sda(sda),
      _scl(scl),
      _u8g2(U8G2_R0, U8X8_PIN_NONE /* reset */, _scl, _sda) {
}

// Initialize the display
void Display::begin() {
    _u8g2.setI2CAddress(0x3C << 1); // Default I2C address for SH1106
    _u8g2.begin();
}

// Clear the display
void Display::clear() {
    _u8g2.clearBuffer();
    _u8g2.sendBuffer();
}

// Draw a string at the specified position
void Display::drawString(int x, int y, const char* str) {
    _u8g2.setFont(u8g2_font_ncenB08_tr); // Choose a suitable font
    _u8g2.drawStr(x, y, str);
    _u8g2.sendBuffer();
}
