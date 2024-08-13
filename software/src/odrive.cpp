#include <SoftwareSerial.h>
#include "odrive.h"

// Printing with stream operator
template<class T> inline Print& operator <<(Print &obj,     T arg) { obj.print(arg);    return obj; }
template<>        inline Print& operator <<(Print &obj, float arg) { obj.print(arg, 4); return obj; }

ODrive::ODrive(uint8_t rx, uint8_t tx) :
    _serial(rx, tx) //RX (ODrive TX), TX (ODrive RX)
{
}

void ODrive::begin() {
    _serial.begin(115200);

    // Say hello.
    while (!get_vbus_voltage()) {
        delay(10);
    }
}

String ODrive::readString() {
    String str = "";
    static const unsigned long timeout = 1000;
    unsigned long timeout_start = millis();
    for (;;) {
        while (!_serial.available()) {
            if (millis() - timeout_start >= timeout) {
                return str;
            }
        }
        char c = _serial.read();
        if (c == '\n')
            break;
        str += c;
    }
    return str;
}

void ODrive::calibrate_axis(int axis) {
    set_axis_requested_state(axis, AXIS_STATE_FULL_CALIBRATION_SEQUENCE);

    while (get_axis_state(axis) != AXIS_STATE_IDLE)
        delay(10);
}

void ODrive::set_axis_requested_state(int axis, AxisState state) {
    _serial << "w axis" << axis << ".requested_state " << state << '\n';
}

AxisState ODrive::get_axis_state(int axis) {
    _serial << "r axis" << axis << ".current_state\n";
    return static_cast<AxisState>(readString().toInt());
}

float ODrive::get_vbus_voltage() {
    String cmd = "r vbus_voltage\n";
    _serial << cmd;
    return readString().toFloat();
}

void ODrive::set_velocity(int axis, int vel) {
    _serial << "w axis" << axis << ".controller.input_vel " << 50.0f << '\n';
}

float ODrive::get_current_velocity(int axis) {
    _serial << "r axis" << axis << ".controller.config.vel_limit\n";
    return readString().toFloat();
}