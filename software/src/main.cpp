#include <Arduino.h>

const int potentiometerPin = A0; // ESP8266 analog pin

void setup() {
  // Initialize serial communication at 115200 baud rate
  Serial.begin(115200);

  // Wait for the serial port to connect - useful for some boards
  while (!Serial) {
    ; // Wait for serial port to connect. Needed for native USB port only
  }

  // Print a message to the serial monitor to indicate setup is complete
  Serial.println("ESP8266 Potentiometer Reading Initialized");
}

void loop() {
  // Read the value from the potentiometer
  int potentiometerValue = analogRead(potentiometerPin);

  // Print the potentiometer value to the serial monitor
  Serial.print("Potentiometer Value: ");
  Serial.println(potentiometerValue);

  // Wait for a short period before reading the value again
  delay(500); // Adjust the delay as needed
}
