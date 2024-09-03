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

struct ErrorEntry {
    uint64_t code;
    const char* message;
};

// Error messages
const ErrorEntry axisErrorMessages[] = {
    { AXIS_ERROR_NONE, "No error" },
    { AXIS_ERROR_INVALID_STATE, "Invalid state" },
    { AXIS_ERROR_DC_BUS_UNDER_VOLTAGE, "DC bus under voltage" },
    { AXIS_ERROR_DC_BUS_OVER_VOLTAGE, "DC bus over voltage" },
    { AXIS_ERROR_CURRENT_MEASUREMENT_TIMEOUT, "Current measurement timeout" },
    { AXIS_ERROR_BRAKE_RESISTOR_DISARMED, "Brake resistor disarmed" },
    { AXIS_ERROR_MOTOR_DISARMED, "Motor disarmed" },
    { AXIS_ERROR_MOTOR_FAILED, "Motor failed" },
    { AXIS_ERROR_SENSORLESS_ESTIMATOR_FAILED, "Sensorless estimator failed" },
    { AXIS_ERROR_ENCODER_FAILED, "Encoder failed" },
    { AXIS_ERROR_CONTROLLER_FAILED, "Controller failed" },
    { AXIS_ERROR_POS_CTRL_DURING_SENSORLESS, "Position control during sensorless mode" },
    { AXIS_ERROR_WATCHDOG_TIMER_EXPIRED, "Watchdog timer expired" },
    { AXIS_ERROR_MIN_ENDSTOP_PRESSED, "Min endstop pressed" },
    { AXIS_ERROR_MAX_ENDSTOP_PRESSED, "Max endstop pressed" },
    { AXIS_ERROR_ESTOP_REQUESTED, "Emergency stop requested" },
    { AXIS_ERROR_HOMING_WITHOUT_ENDSTOP, "Homing without endstop" },
    { AXIS_ERROR_OVER_TEMP, "Axis over temperature" },
    { AXIS_ERROR_UNKNOWN_POSITION, "Axis in unknown position" },
    { AXIS_ERROR_UNKNOWN, "Unknown error" }
};

const ErrorEntry motorErrorMessages[] = {
    { MOTOR_ERROR_NONE, "No error" },
    { MOTOR_ERROR_PHASE_RESISTANCE_OUT_OF_RANGE, "Phase resistance out of range" },
    { MOTOR_ERROR_PHASE_INDUCTANCE_OUT_OF_RANGE, "Phase inductance out of range" },
    { MOTOR_ERROR_ADC_FAILED, "ADC failed" },
    { MOTOR_ERROR_DRV_FAULT, "DRV fault" },
    { MOTOR_ERROR_CONTROL_DEADLINE_MISSED, "Control deadline missed" },
    { MOTOR_ERROR_NOT_IMPLEMENTED_MOTOR_TYPE, "Not implemented motor type" },
    { MOTOR_ERROR_BRAKE_CURRENT_OUT_OF_RANGE, "Brake current out of range" },
    { MOTOR_ERROR_MODULATION_MAGNITUDE, "Modulation magnitude out of range" },
    { MOTOR_ERROR_BRAKE_DEADTIME_VIOLATION, "Brake deadtime violation" },
    { MOTOR_ERROR_UNEXPECTED_TIMER_CALLBACK, "Unexpected timer callback" },
    { MOTOR_ERROR_CURRENT_SENSE_SATURATION, "Current sense saturation" },
    { MOTOR_ERROR_CURRENT_LIMIT_VIOLATION, "Current limit violation" },
    { MOTOR_ERROR_MODULATION_IS_NAN, "Modulation is NaN" },
    { MOTOR_ERROR_MOTOR_THERMISTOR_OVER_TEMP, "Motor thermistor over temp" },
    { MOTOR_ERROR_FET_THERMISTOR_OVER_TEMP, "FET thermistor over temp" },
    { MOTOR_ERROR_TIMER_UPDATE_MISSED, "Timer update missed" },
    { MOTOR_ERROR_CURRENT_MEASUREMENT_UNAVAILABLE, "Current measurement unavailable" },
    { MOTOR_ERROR_CONTROLLER_FAILED, "Controller failed" },
    { MOTOR_ERROR_I_BUS_OUT_OF_RANGE, "I bus out of range" },
    { MOTOR_ERROR_BRAKE_RESISTOR_DISARMED, "Brake resistor disarmed" },
    { MOTOR_ERROR_SYSTEM_LEVEL, "System level error" },
    { MOTOR_ERROR_BAD_TIMING, "Bad timing" },
    { MOTOR_ERROR_UNKNOWN_PHASE_ESTIMATE, "Unknown phase estimate" },
    { MOTOR_ERROR_UNKNOWN_PHASE_VEL, "Unknown phase velocity" },
    { MOTOR_ERROR_UNKNOWN_TORQUE, "Unknown torque" },
    { MOTOR_ERROR_UNKNOWN_CURRENT_COMMAND, "Unknown current command" },
    { MOTOR_ERROR_UNKNOWN_CURRENT_MEASUREMENT, "Unknown current measurement" },
    { MOTOR_ERROR_UNKNOWN_VBUS_VOLTAGE, "Unknown VBUS voltage" },
    { MOTOR_ERROR_UNKNOWN_VOLTAGE_COMMAND, "Unknown voltage command" },
    { MOTOR_ERROR_UNKNOWN_GAINS, "Unknown gains" },
    { MOTOR_ERROR_CONTROLLER_INITIALIZING, "Controller initializing" },
    { MOTOR_ERROR_UNBALANCED_PHASES, "Unbalanced phases" }
};

const ErrorEntry encoderErrorMessages[] = {
    { ENCODER_ERROR_NONE, "No error" },
    { ENCODER_ERROR_UNSTABLE_GAIN, "Unstable gain" },
    { ENCODER_ERROR_CPR_POLEPAIRS_MISMATCH, "CPR polepairs mismatch" },
    { ENCODER_ERROR_NO_RESPONSE, "No response" },
    { ENCODER_ERROR_UNSUPPORTED_ENCODER_MODE, "Unsupported encoder mode" },
    { ENCODER_ERROR_ILLEGAL_HALL_STATE, "Illegal hall state" },
    { ENCODER_ERROR_INDEX_NOT_FOUND_YET, "Index not found yet" },
    { ENCODER_ERROR_ABS_SPI_TIMEOUT, "ABS SPI timeout" },
    { ENCODER_ERROR_ABS_SPI_COM_FAIL, "ABS SPI communication failure" },
    { ENCODER_ERROR_ABS_SPI_NOT_READY, "ABS SPI not ready" },
    { ENCODER_ERROR_HALL_NOT_CALIBRATED_YET, "Hall sensor not calibrated yet" }
};

const ErrorEntry controllerErrorMessages[] = {
    { CONTROLLER_ERROR_NONE, "No error" },
    { CONTROLLER_ERROR_OVERSPEED, "Overspeed" },
    { CONTROLLER_ERROR_INVALID_INPUT_MODE, "Invalid input mode" },
    { CONTROLLER_ERROR_UNSTABLE_GAIN, "Unstable gain" },
    { CONTROLLER_ERROR_INVALID_MIRROR_AXIS, "Invalid mirror axis" },
    { CONTROLLER_ERROR_INVALID_LOAD_ENCODER, "Invalid load encoder" },
    { CONTROLLER_ERROR_INVALID_ESTIMATE, "Invalid estimate" },
    { CONTROLLER_ERROR_INVALID_CIRCULAR_RANGE, "Invalid circular range" },
    { CONTROLLER_ERROR_SPINOUT_DETECTED, "Spinout detected" }
};


String errorToString(uint64_t errorMask, const ErrorEntry* errorList, size_t numEntries) {
    String errors = "";

    // Iterate over all possible error codes
    for (uint idx = 1; idx <= numEntries; idx++) {
        ErrorEntry entry = errorList[idx];
        if (errorMask & entry.code) {
            if (!errors.isEmpty())
                errors += ", ";
            errors += entry.message;
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
    delay(10);

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

void ODrive::set_velocity(int axis, float vel) {
    _serial << "w axis" << axis << ".controller.input_vel " << String(vel, 2) << '\n';
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

    String axisErrorMsg = errorToString(axis_err, axisErrorMessages, sizeof(axisErrorMessages)/sizeof(axisErrorMessages[0]));
    String motorErrorMsg = errorToString(motor_err, motorErrorMessages, sizeof(motorErrorMessages)/sizeof(motorErrorMessages[0]));
    String encoderErrorMsg = errorToString(encoder_err, encoderErrorMessages, sizeof(encoderErrorMessages)/sizeof(encoderErrorMessages[0]));
    String controllerErrorMsg = errorToString(controller_err, controllerErrorMessages, sizeof(controllerErrorMessages)/sizeof(controllerErrorMessages[0]));

    return joinStrings("\n",
        axisErrorMsg,
        motorErrorMsg,
        encoderErrorMsg,
        controllerErrorMsg);
}