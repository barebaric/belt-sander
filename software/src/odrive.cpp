#include <SoftwareSerial.h>
#include <type_traits>

#include "util.h"
#include "odrive.h"

// Printing with stream operator. Note that between chars a delay of one ms is added; otherwise ODrive will
// not receive the command reliably. Found a similar problem described here, but in native mode:
// https://discourse.odriverobotics.com/t/odrive-ignores-most-requests-in-uart-native-mode/7368/3
template<class T> inline Print& operator <<(Print &obj,   T arg)         { obj.print(arg);    return obj; }
template<>        inline Print& operator <<(Print &obj, const char *arg) { String a = arg; for (char c : a)   { obj.print(c); delay(1); } return obj; }
template<>        inline Print& operator <<(Print &obj, String arg)      {                 for (char c : arg) { obj.print(c); delay(1); } return obj; }
template<>        inline Print& operator <<(Print &obj, int arg)         { obj.print(arg);    return obj; }
template<>        inline Print& operator <<(Print &obj, float arg)       { obj.print(arg, 4); return obj; }

// Error messages
const char* axisErrorMessages[] = {
    "No error",
    "Invalid state",
    "DC bus under voltage",
    "DC bus over voltage",
    "Current measurement timeout",
    "Brake resistor disarmed",
    "Motor disarmed",
    "Motor failed",
    "Sensorless estimator failed",
    "Encoder failed",
    "Controller failed",
    "Position control during sensorless mode",
    "Watchdog timer expired"
    "Min endstop pressed",
    "Max endstop pressed",
    "Emergency stop requested",
    "Homing without endstop",
    "Axis over temperature",
    "Axis in unknown position",
};

const char* motorErrorMessages[] = {
    "No error",
    "Phase resistance out of range",
    "Phase inductance out of range",
    "ADC failed",
    "DRV fault",
    "Control deadline missed",
    "Not implemented motor type",
    "Brake current out of range",
    "Modulation magnitude out of range",
    "Brake deadtime violation",
    "Unexpected timer callback",
    "Current sense saturation",
    "Current limit violation",
    "Modulation is NaN",
    "Motor thermistor over temp",
    "FET thermistor over temp",
    "Timer update missed",
    "Current measurement unavailable",
    "Controller failed",
    "I bus out of range",
    "Brake resistor disarmed",
    "System level error",
    "Bad timing",
    "Unknown phase estimate",
    "Unknown phase velocity",
    "Unknown torque",
    "Unknown current command",
    "Unknown current measurement",
    "Unknown VBUS voltage",
    "Unknown voltage command",
    "Unknown gains",
    "Controller initializing",
    "Unbalanced phases"
};

const char* encoderErrorMessages[] = {
    "No error",
    "Unstable gain",
    "CPR polepairs mismatch",
    "No response",
    "Unsupported encoder mode",
    "Illegal hall state",
    "Index not found yet",
    "ABS SPI timeout",
    "ABS SPI communication failure",
    "ABS SPI not ready",
    "Hall sensor not calibrated yet"
};

const char* controllerErrorMessages[] = {
    "No error",
    "Overspeed",
    "Invalid input mode",
    "Unstable gain",
    "Invalid mirror axis",
    "Invalid load encoder",
    "Invalid estimate",
    "Invalid circular range",
    "Spinout detected"
};


String errorToString(uint64_t errorMask, const char** errorList) {
    String errors = "";

    // Iterate over all possible error codes
    uint64_t mask = 1;
    for (uint idx = 0; idx <= 40; idx++) {
        mask = pow(2, idx);
        if (errorMask & mask) {
            //Serial << "IDX: " << idx << " -> 0x" << String(mask, HEX) << "\n";
            if (!errors.isEmpty())
                errors += ", ";
            errors += errorList[idx];
        }
    }
    return errors;
}

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

AxisError ODrive::get_axis_error(int axis) {
    drain();
    _serial << "r axis" << axis << ".error\n";
    return static_cast<AxisError>(readString().toInt());
}

MotorError ODrive::get_motor_error(int axis) {
    drain();
    _serial << "r axis" << axis << ".motor.error\n";
    return static_cast<MotorError>(readString().toInt());
}

EncoderError ODrive::get_encoder_error(int axis) {
    drain();
    _serial << "r axis" << axis << ".encoder.error\n";
    return static_cast<EncoderError>(readString().toInt());
}

ControllerError ODrive::get_controller_error(int axis) {
    drain();
    _serial << "r axis" << axis << ".controller.error\n";
    return static_cast<ControllerError>(readString().toInt());
}

String ODrive::get_errors(int axis) {
    AxisError axis_err = get_axis_error(axis);
    MotorError motor_err = get_motor_error(axis);
    EncoderError encoder_err = get_encoder_error(axis);
    ControllerError controller_err = get_controller_error(axis);

    String axisErrorMsg = errorToString(axis_err, axisErrorMessages);
    String motorErrorMsg = errorToString(motor_err, motorErrorMessages);
    String encoderErrorMsg = errorToString(encoder_err, encoderErrorMessages);
    String controllerErrorMsg = errorToString(controller_err, controllerErrorMessages);

    return joinStrings("\n",
        axisErrorMsg,
        motorErrorMsg,
        encoderErrorMsg,
        controllerErrorMsg);
}