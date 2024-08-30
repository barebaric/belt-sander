#include <iostream>
#include <string>
#include <iostream>
#include <Arduino.h>
#include "display.h"
#include "analogpin.h"
#include "thermistor.h"
#include "odrive.h"

AnalogPin potentiometer(A0);
float smoothed_velocity = 0;  // between 0 and 1.0
float last_velocity = 0;  // between 0 and 1.0
float current_velocity = 0;

Thermistor thermistor(A1, 10000);
float smoothed_temp;   // in Celsius
float overheat_limit = 75; // Celsius. PETG glassing temp is 85 degrees, adding some margin
float overheat_restart = 65; // Celsius
bool overheated = false;

Display display(T8, T9);
ODrive odrive(T6, T7);

int last_log = 0;
float current_voltage = 0;
String error = "";

TaskHandle_t UITask;
TaskHandle_t MotorTask;

void show_message(String msg) {
    Serial.println(msg);
    display.clearBuffer();
    display.drawString(0, 32, msg.c_str());
    display.sendBuffer();
}

void update_ui() {
    display.clearBuffer();

    // Don't do_log stuff to serial every loop; limit output a bit.
    bool do_log = false;
    if (millis() - last_log > 1000) {
        last_log = millis();
        do_log = true;
    }
        Serial.println("tick");

    // Write current temperature to the display.
    if (do_log) {
        Serial.print("Temp: ");
        Serial.print(thermistor.celsius());
        Serial.print("°C, ");
        Serial.println(thermistor.raw_value());
    }
    smoothed_temp += (thermistor.celsius() - smoothed_temp)*0.1;
    display.drawTemp(smoothed_temp);
    if (smoothed_temp > overheat_limit)
        overheated = true;
    if (smoothed_temp < overheat_restart)
        overheated = false;
    if (overheated) {
        display.drawMessage("Motor too hot!",
                            "Waiting to cool down.");
        return;
    }

    // Display errors, if any.
    if (!error.isEmpty()) {
        display.drawStringMultiLine(0, 0, error.c_str());
        Serial.print("ERROR, bailing out: \"");
        Serial.print(error);
        Serial.println("\"");
        return;
    }

    // Write current speed to the display.
    if (do_log) {
        Serial.print("Potentiometer: ");
        Serial.print(potentiometer.value());
        Serial.print(", ");
        Serial.println(potentiometer.raw_value());
    }
    smoothed_velocity += (potentiometer.value() - smoothed_velocity)*0.4;
    display.drawSpeed(smoothed_velocity);

    // Show status line on display.
    String msg = "Bus: " + String(current_voltage, 1) 
               + "V  Spd: " + String(current_velocity);
    display.drawString(0, 60, msg.c_str());
    if (do_log) {
        Serial.println(msg);
    }

    display.sendBuffer();
}

void ui_loop(void *parameter) {
    for (;;) {
        update_ui();
        vTaskDelay(10 / portTICK_PERIOD_MS);
    }
}

void update_motor_state() {
    // Overheat safehold.
    if (overheated) {
        Serial.println("Motor overheated, going to IDLE");
        odrive.set_axis_requested_state(0, AXIS_STATE_IDLE);
        last_velocity = 0;
        return;
    }

    // Retrieve current state.
    error = odrive.get_errors(0);
    Serial.print("Received errors: ");
    Serial.println(error);
    current_voltage = odrive.get_vbus_voltage();
    Serial.print("Received voltage: ");
    Serial.println(current_voltage);
    current_velocity = odrive.get_current_velocity(0);
    Serial.print("Received velocity: ");
    Serial.println(current_velocity);

    if (!error.isEmpty())
        return;

    // Set velocity.
    float vel_diff = abs(last_velocity - smoothed_velocity);
    if (vel_diff > 0.05) {
        Serial.print("Sending velocity: ");
        Serial.println(smoothed_velocity);
        odrive.set_velocity(0, smoothed_velocity * 25);
        if (smoothed_velocity < 0.1) {
            Serial.println("Motor going to IDLE");
            odrive.set_axis_requested_state(0, AXIS_STATE_IDLE);
        }
        else if (last_velocity < 0.1 && smoothed_velocity >= 0.1) {
            Serial.println("Motor going to CLOSED_LOOP_CONTROL");
            odrive.set_axis_requested_state(0, AXIS_STATE_CLOSED_LOOP_CONTROL);
        }
        last_velocity = smoothed_velocity;
    }
}

void motor_state_loop(void *parameter) {
    for(;;) {
        update_motor_state();
        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
}

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
    show_message("Searching motor...");
    odrive.begin();
    Serial.println("ODrive connected");

    // Calibrate motor (ODrive requires this for hall sensor driven motors).
    show_message("Calibrating motor...");
    odrive.calibrate_axis(0);
    Serial.println("Motor calibrated");

/*
    xTaskCreate(
        motor_state_loop,
        "MotorTask",
        2048,
        NULL,
        0,
        &MotorTask);
    xTaskCreate(
        ui_loop,
        "UITask",
        2048,
        NULL,
        1,
        &UITask);
*/
}

void loop() {
    update_motor_state();
    update_ui();
}