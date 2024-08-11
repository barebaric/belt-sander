#include <U8g2lib.h>
#include "display.h"

Display::Display(uint8_t sda, uint8_t scl)
    : _sda(sda),
      _scl(scl),
      _u8g2(U8G2_R0, U8X8_PIN_NONE /* reset */, _scl, _sda) {
}

// Initialize the display
void Display::begin() {
    _u8g2.setI2CAddress(0x3C << 1); // Default I2C address for SH1106
    _u8g2.begin();
    clear();
}

void Display::clearBuffer() {
    _u8g2.clearBuffer();
}

void Display::sendBuffer() {
    _u8g2.sendBuffer();
}

// Clear the display
void Display::clear() {
    clearBuffer();
    sendBuffer();
}

// Draw a string at the specified position
void Display::drawString(int x, int y, const char* str) {
    _u8g2.setFont(u8g2_font_ncenB08_tr); // Choose a suitable font
    _u8g2.drawStr(x, y, str);
    _u8g2.sendBuffer();
}

void Display::drawFilledSector(int centerX, int centerY, int radius, float startAngle, float endAngle) {
    int x0, y0, x1, y1;
    float angleStep = 1.0;  // Angle step to increase precision

    // Draw filled sector using triangle strips
    for (float angle = startAngle; angle <= endAngle; angle += angleStep) {
        x0 = centerX + radius * cos(angle * M_PI / 180);
        y0 = centerY + radius * sin(angle * M_PI / 180);
        x1 = centerX + radius * cos((angle + angleStep) * M_PI / 180);
        y1 = centerY + radius * sin((angle + angleStep) * M_PI / 180);

        // Draw triangle to fill the sector
        _u8g2.drawTriangle(centerX, centerY, x0, y0, x1, y1);
    }
}

void Display::drawSpeed(float speed) {
    int centerX = 64;  // Center X of the screen
    int centerY = 32;  // Center Y of the screen
    int radius = 30;   // Radius of the circle
    int innerRadius = 20;

    float startAngle = -200.0;
    float maxAngle = 220.0;

    // Calculate the angle corresponding to the percentage
    float angle = speed * maxAngle;
    angle = (angle < 1) ? 1 : angle;

    // Draw the circular sector (arc)
    _u8g2.setDrawColor(1);
    drawFilledSector(centerX, centerY, radius, startAngle, startAngle+angle);
    _u8g2.setDrawColor(0);
    drawFilledSector(centerX, centerY, innerRadius, startAngle-1, startAngle+angle+2);
    _u8g2.setDrawColor(1);

    // Convert percentage value to string
    char percentStr[5];
    snprintf(percentStr, sizeof(percentStr), "%d%%", (int)(speed*100));

    // Draw the percentage text in the center
    _u8g2.setFont(u8g2_font_ncenB08_tr);  // Choose a font
    int textWidth = _u8g2.getStrWidth(percentStr);
    _u8g2.drawStr(centerX - (textWidth / 2), centerY + 4, percentStr);
}