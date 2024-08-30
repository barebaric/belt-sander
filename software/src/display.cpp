#include <U8g2lib.h>
#include "display.h"

#define LINE_SPACING_MULTILINE_TEXT_PX 2

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
    _u8g2.setFont(u8g2_font_ncenB08_tr);
    _u8g2.drawStr(x, y, str);
    _u8g2.sendBuffer();
}

void Display::drawStringMultiLine(int x, int y, const char* str) {
    uint8_t maxCharWidth = _u8g2.getMaxCharWidth();
    uint8_t screenWidth = _u8g2.getDisplayWidth();
    uint8_t maxCharsPerLine = screenWidth / maxCharWidth;

    uint32_t stringLengthBytes = strlen(str);
    char partialString[maxCharsPerLine + 1] = {'\0'};
    uint8_t lineStart = 0;
    uint8_t maxCharHeight = _u8g2.getMaxCharHeight();

    while (lineStart < stringLengthBytes) {
        uint8_t lineEnd = lineStart + maxCharsPerLine;
        uint8_t lastSpaceIdx = lineStart;
        uint8_t partialStrLen = 0;

        // Find the end of the line or the last space within the line
        for (uint8_t i = lineStart; i < stringLengthBytes && partialStrLen < maxCharsPerLine; ++i) {
            char currentChar = str[i];
            if (currentChar == '\n') {
                lastSpaceIdx = i;
                break;
            } else if (isspace(static_cast<unsigned char>(currentChar))) {
                lastSpaceIdx = i;
            }

            // Increment length only for the first byte of each UTF-8 glyph
            if ((currentChar & 0xc0) != 0x80) {
                partialStrLen++;
            }
        }

        // Adjust lineEnd to not cut off words
        if (lastSpaceIdx > lineStart && (str[lineEnd] != '\n') && partialStrLen >= maxCharsPerLine) {
            lineEnd = lastSpaceIdx;
        }

        // Copy the relevant part of the string to partialString
        strncpy(partialString, &str[lineStart], lineEnd - lineStart);
        partialString[lineEnd - lineStart] = '\0'; // Ensure null termination

        // Draw the string using the _u8g2 attribute
        _u8g2.drawUTF8(x, y, partialString);

        // Move to the next line
        y += maxCharHeight + LINE_SPACING_MULTILINE_TEXT_PX;
        lineStart = lineEnd;

        // Skip any spaces or newlines at the start of the next line
        while (lineStart < stringLengthBytes && isspace(static_cast<unsigned char>(str[lineStart]))) {
            lineStart++;
        }
    }
    _u8g2.sendBuffer();
}

// Draw a string at the center
void Display::drawMessage(const char* msg, const char* substr) {
    int centerX = _u8g2.getDisplayWidth()/2;
    int centerY = _u8g2.getDisplayHeight()/2;

    _u8g2.setFont(u8g2_font_ncenB08_tf);
    int textWidth = _u8g2.getStrWidth(msg);
    _u8g2.drawStr(centerX-textWidth/2, centerY-10, msg);

    _u8g2.setFont(u8g2_font_timB08_tf);
    textWidth = _u8g2.getStrWidth(substr);
    _u8g2.drawStr(centerX-textWidth/2, centerY+10, substr);

    _u8g2.sendBuffer();
}

// Draw a string at the center
void Display::drawError(const char* err) {
    _u8g2.setFont(u8g2_font_baby_tf);
    drawStringMultiLine(0, 0, err);
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
    int centerX = _u8g2.getDisplayWidth()/2;
    int centerY = _u8g2.getDisplayHeight()/2;
    int radius = centerY-2;   // Radius of the circle
    int innerRadius = radius-10;

    float startAngle = -200.0;
    float maxAngle = 220.0;

    // Calculate the angle corresponding to the percentage
    float angle = speed * maxAngle;
    angle = (angle < 2) ? 2 : angle;

    // Draw the circular sector (arc)
    _u8g2.setDrawColor(1);
    drawFilledSector(centerX, centerY, radius, startAngle, startAngle+angle);
    _u8g2.setDrawColor(0);
    drawFilledSector(centerX, centerY, innerRadius, startAngle-3, startAngle+angle+3);
    _u8g2.setDrawColor(1);

    // Draw the percentage text in the center
    char percentStr[5];
    snprintf(percentStr, sizeof(percentStr), "%d%%", (int)(speed*100));
    _u8g2.setFont(u8g2_font_tenthinnerguys_tf);
    int textWidth = _u8g2.getStrWidth(percentStr);
    _u8g2.drawStr(centerX - (textWidth / 2), centerY + 4, percentStr);
}

void Display::drawTemp(float temp) {
    char str[10];
    snprintf(str, sizeof(str), "%d°C", (int)(temp));
    _u8g2.setFont(u8g2_font_tinyunicode_tf);
    int textWidth = _u8g2.getStrWidth(str);
    _u8g2.drawUTF8(_u8g2.getDisplayWidth() - textWidth, 6, str);
}