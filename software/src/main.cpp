#include <iostream>
#include <string>
#include <iostream>
#include <Arduino.h>
#include "display.h"
#include "analogpin.h"
#include "thermistor.h"
#include "odrive.h"

AnalogPin potentiometer(A0);
float smoothed_velocity;
Thermistor thermistor(A1, 10000);
float smoothed_temp;
Display display(T8, T9);
ODrive odrive(T4, T5);
int last_tick = 0;

void setup() {
    // Initialize serial communication.
    Serial.begin(115200);
    //while (!Serial) {}
    Serial.println("Serial initialized");
    delay(1000);

    // Initialize potentiometer.
    potentiometer.begin();
    smoothed_velocity = potentiometer.value();
    Serial.println("Potentiometer initialized");

    // Initialize thermistor.
    thermistor.begin();
    smoothed_temp = thermistor.celsius();
    Serial.println("Thermistor initialized");

    // Initialize OLED display.
    display.begin();
    Serial.println("Display initialized");

    // Connect to ODrive controller.
    display.clearBuffer();
    display.drawString(0, 32, "Searching motor...");
    display.sendBuffer();
    //odrive.begin();
    Serial.println("ODrive connected");

    // Calibrate motor (ODrive requires this for hall sensor driven motors).
    display.clearBuffer();
    display.drawString(0, 32, "Calibrating motor...");
    display.sendBuffer();
    //odrive.calibrate_axis(0);
    Serial.println("Motor calibrated");
}

void loop() {
    display.clearBuffer();

    // Don't do_log stuff to serial every loop; limit output a bit.
    int time_diff = millis() - last_tick;
    bool do_log = false;
    if (time_diff > 1000) {
        last_tick = millis();
        do_log = true;
    }

    // Write current speed to the display.
    if (do_log) {
        Serial.print("Potentiometer: ");
        Serial.print(potentiometer.value());
        Serial.print(", ");
        Serial.println(potentiometer.raw_value());
    }
    smoothed_velocity += (potentiometer.value() - smoothed_velocity)*0.15;
    display.drawSpeed(smoothed_velocity);

    // Write current temperature to the display.
    if (do_log) {
        Serial.print("Temp: ");
        Serial.print(thermistor.celsius());
        Serial.print("°C, ");
        Serial.println(thermistor.raw_value());
    }
    smoothed_temp += (thermistor.celsius() - smoothed_temp)*0.1;
    display.drawTemp(smoothed_temp);

    // Show status line on display.
    //float voltage = odrive.get_vbus_voltage();
    //float velo = odrive.get_current_velocity(0);
    //String msg = "Bus: " + String(voltage, 1) + "V  Spd: " + String(velo);
    //display.drawString(0, 60, msg.c_str());
    //Serial.println(msg);

    display.sendBuffer();
    delay(10);
}
