#include <iostream>
#include <string>
#include <Arduino.h>
#include "display.h"
#include "potentiometer.h"

Potentiometer potentiometer(A0);
Display display(D1, D2);

void setup() {
  // Initialize serial communication.
  Serial.begin(115200);
  while (!Serial) {}
  Serial.println("Serial initialized");

  // Initialize potentiometer.
  potentiometer.begin();
  Serial.println("Potentiometer initialized");

  // Initialize OLED display.
  display.begin();
  Serial.println("Display initialized");
}

void loop() {
  // Write current speed to the display.
  display.clearBuffer();
  display.drawSpeed(potentiometer.value());
  display.sendBuffer();

  delay(20);
}
