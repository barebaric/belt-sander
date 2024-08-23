#include <SoftwareSerial.h>
#include <type_traits>

#include "odrive.h"

// Printing with stream operator. Note that between chars a delay of one ms is added; otherwise ODrive will
// not receive the command reliably. Found a similar problem described here, but in native mode:
// https://discourse.odriverobotics.com/t/odrive-ignores-most-requests-in-uart-native-mode/7368/3
template<class T> inline Print& operator <<(Print &obj,   T arg)         { obj.print(arg);    return obj; }
template<>        inline Print& operator <<(Print &obj, const char *arg) { String a = arg; for (char c : a)   { obj.print(c); delay(1); } return obj; }
template<>        inline Print& operator <<(Print &obj, String arg)      {                 for (char c : arg) { obj.print(c); delay(1); } return obj; }
template<>        inline Print& operator <<(Print &obj, int arg)         { obj.print(arg);    return obj; }
template<>        inline Print& operator <<(Print &obj, float arg)       { obj.print(arg, 4); return obj; }

ODrive::ODrive(uint8_t rx, uint8_t tx) :
  _serial(rx, tx) //RX (ODrive TX), TX (ODrive RX)
{
}

void ODrive::begin() {
    _serial.begin(115200);
    //while (!_serial);
    _serial.setTimeout(200);

    // Say hello.
    while (!get_vbus_voltage()) {
        delay(100);
    }
}

void ODrive::drain() {
    String str = "";
    while (_serial.available()) {
        str += _serial.read();
    }
    /*
    if (str != "")
        Serial << "Drained: " << str << "\n";
    */
}

String ODrive::readString() {
    return _serial.readStringUntil('\n');
}

void ODrive::calibrate_axis(int axis) {
    set_axis_requested_state(axis, AXIS_STATE_ENCODER_OFFSET_CALIBRATION);

    while (get_axis_state(axis) != AXIS_STATE_IDLE)
        delay(100);
}

void ODrive::set_axis_requested_state(int axis, AxisState state) {
    _serial << "w axis" << axis << ".requested_state " << state << '\n';
}

AxisState ODrive::get_axis_state(int axis) {
    _serial << "r axis" << axis << ".current_state\n";
    return static_cast<AxisState>(readString().toInt());
}

float ODrive::get_vbus_voltage() {
    drain();
    _serial << "r vbus_voltage\n";
    return readString().toFloat();
}

void ODrive::set_velocity(int axis, int vel) {
    _serial << "w axis" << axis << ".controller.input_vel " << vel << '\n';
}

float ODrive::get_current_velocity(int axis) {
    drain();
    _serial << "r axis" << axis << ".controller.input_vel\n";
    return readString().toFloat();
}