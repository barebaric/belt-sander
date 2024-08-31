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
    uint8_t maxCharHeight = _u8g2.getMaxCharHeight();
    uint8_t maxWidth = _u8g2.getDisplayWidth() - x;

    String input = String(str);
    uint32_t stringLength = input.length();
    uint32_t currentLine = 0;
    uint32_t lineStart = 0;

    while (lineStart < stringLength) {
        // Skip any spaces or newlines at the start of the next line
        if (isspace(static_cast<unsigned char>(input[lineStart]))) {
            lineStart++;
            continue;
        }

        // Find the end of the line or the last space within the line
        String line = "";
        for (uint8_t lineEndCandidate = lineStart; lineEndCandidate < stringLength; ++lineEndCandidate) {
            String lineCandidate = input.substring(lineStart, lineEndCandidate + 1);

            // Current char already over the display width? Force a line break.
            if (_u8g2.getStrWidth(lineCandidate.c_str()) > maxWidth) {
                if (line.isEmpty())
                    line = input.substring(lineStart, lineEndCandidate);
                break;
            }

            // Newline in input also forces a line break.
            char currentChar = input[lineEndCandidate];
            if (currentChar == '\n') {
                line = lineCandidate;
                break;
            }

            // So we have not reached the display width yet. If we found a space,
            // we consider that the best result found so far. But in any case,
            // we continue to try and fit more.
            if (isspace(static_cast<unsigned char>(currentChar))
                || lineEndCandidate + 1 == stringLength)
                line = lineCandidate;
        }

        // Ending up here, line contains a string that should fit on the screen.
        _u8g2.drawUTF8(x, y + currentLine*maxCharHeight, line.c_str());

        lineStart += line.length();
        currentLine++;
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
    drawStringMultiLine(0, 10, err);
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