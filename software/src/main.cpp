#include <Arduino.h>
#include "display.h"

const int potentiometerPin = A0; // ESP8266 analog pin
Display display(D1, D2);

void setup() {
  // Initialize serial communication.
  Serial.begin(115200);
  while (!Serial) {}
  Serial.println("Serial initialized");

  // Initialize OLED display.
  //display.begin();
  //display.clear();
  //Serial.println("Display initialized");
}

void loop() {
  // Read the value from the potentiometer
  int speed = analogRead(potentiometerPin);
  Serial.print("Potentiometer Value: ");
  Serial.println(speed);

  // Write result to the display.
  //display.drawString(0, 10, "Hello, World!");

  // Wait for a short period before reading the value again
  delay(500); // Adjust the delay as needed
}
