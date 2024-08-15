#include <iostream>
#include <string>
#include <iostream>
#include <Arduino.h>
#include "display.h"
#include "potentiometer.h"
#include "odrive.h"

Potentiometer potentiometer(A0);
Display display(T8, T9);
ODrive odrive(T4, T5);

void setup() {
    // Initialize serial communication.
    Serial.begin(115200);
    //while (!Serial) {}
    Serial.println("Serial initialized");
    delay(1000);

    // Initialize potentiometer.
    potentiometer.begin();
    Serial.println("Potentiometer initialized");

    // Initialize OLED display.
    display.begin();
    Serial.println("Display initialized");

    // Connect to ODrive controller.
    display.clearBuffer();
    display.drawString(0, 32, "Searching motor...");
    display.sendBuffer();
    odrive.begin();
    Serial.println("ODrive connected");

    // Calibrate motor (ODrive requires this for hall sensor driven motors).
    display.clearBuffer();
    display.drawString(0, 32, "Calibrating motor...");
    display.sendBuffer();
    odrive.calibrate_axis(0);
    Serial.println("Motor calibrated");
}

void loop() {
    // Write current speed to the display.
    display.clearBuffer();
    display.drawSpeed(potentiometer.value());

    float voltage = odrive.get_vbus_voltage();
    float velo = odrive.get_current_velocity(0);
    String msg = "Bus: " + String(voltage, 1) + "V  Spd: " + String(velo);
    display.drawString(0, 60, msg.c_str());
    Serial.println(msg);

    display.sendBuffer();
    delay(20);
}
