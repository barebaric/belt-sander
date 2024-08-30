#ifndef ODRIVE_H
#define ODRIVE_H

#include <SoftwareSerial.h>

enum AxisState {
    AXIS_STATE_UNDEFINED                     = 0,
    AXIS_STATE_IDLE                          = 1,
    AXIS_STATE_STARTUP_SEQUENCE              = 2,
    AXIS_STATE_FULL_CALIBRATION_SEQUENCE     = 3,
    AXIS_STATE_MOTOR_CALIBRATION             = 4,
    AXIS_STATE_ENCODER_INDEX_SEARCH          = 6,
    AXIS_STATE_ENCODER_OFFSET_CALIBRATION    = 7,
    AXIS_STATE_CLOSED_LOOP_CONTROL           = 8,
    AXIS_STATE_LOCKIN_SPIN                   = 9,
    AXIS_STATE_ENCODER_DIR_FIND              = 10,
    AXIS_STATE_HOMING                        = 11,
    AXIS_STATE_ENCODER_HALL_POLARITY_CALIBRATION = 12,
    AXIS_STATE_ENCODER_HALL_PHASE_CALIBRATION = 13
};

// Axis error codes
enum AxisError {
    AXIS_ERROR_NONE = 0x00,
    AXIS_ERROR_INVALID_STATE = 0x01,
    AXIS_ERROR_DC_BUS_UNDER_VOLTAGE = 0x02,
    AXIS_ERROR_DC_BUS_OVER_VOLTAGE = 0x04,
    AXIS_ERROR_CURRENT_MEASUREMENT_TIMEOUT = 0x08,
    AXIS_ERROR_BRAKE_RESISTOR_DISARMED = 0x10,
    AXIS_ERROR_MOTOR_DISARMED = 0x20,
    AXIS_ERROR_MOTOR_FAILED = 0x40,
    AXIS_ERROR_SENSORLESS_ESTIMATOR_FAILED = 0x80,
    AXIS_ERROR_ENCODER_FAILED = 0x100,
    AXIS_ERROR_CONTROLLER_FAILED = 0x200,
    AXIS_ERROR_POS_CTRL_DURING_SENSORLESS = 0x400,
    AXIS_ERROR_WATCHDOG_TIMER_EXPIRED = 0x800
};

// Motor error codes
enum MotorError {
    MOTOR_ERROR_NONE = 0x00,
    MOTOR_ERROR_PHASE_RESISTANCE_OUT_OF_RANGE = 0x01,
    MOTOR_ERROR_PHASE_INDUCTANCE_OUT_OF_RANGE = 0x02,
    MOTOR_ERROR_DRV_FAULT = 0x04,
    MOTOR_ERROR_CONTROL_DEADLINE_MISSED = 0x08,
    MOTOR_ERROR_MODULATION_MAGNITUDE = 0x10,
    MOTOR_ERROR_CURRENT_SENSE_SATURATION = 0x20,
    MOTOR_ERROR_CURRENT_LIMIT_VIOLATION = 0x40,
    MOTOR_ERROR_MODULATION_IS_NAN = 0x80,
    MOTOR_ERROR_MOTOR_THERMISTOR_OVER_TEMP = 0x100,
    MOTOR_ERROR_FET_THERMISTOR_OVER_TEMP = 0x200,
    MOTOR_ERROR_TIMER_UPDATE_MISSED = 0x400,
    MOTOR_ERROR_CURRENT_UNSTABLE = 0x800
};

// Encoder error codes
enum EncoderError {
    ENCODER_ERROR_NONE = 0x00,
    ENCODER_ERROR_UNSTABLE_GAIN = 0x01,
    ENCODER_ERROR_CPR_OUT_OF_RANGE = 0x02,
    ENCODER_ERROR_NO_RESPONSE = 0x04,
    ENCODER_ERROR_UNSUPPORTED_ENCODER_MODE = 0x08,
    ENCODER_ERROR_ILLEGAL_HALL_STATE = 0x10,
    ENCODER_ERROR_INDEX_NOT_FOUND_YET = 0x20,
    ENCODER_ERROR_ABS_SPI_COM_FAIL = 0x40,
    ENCODER_ERROR_ABS_SPI_NOT_READY = 0x80
};

// Controller error codes
enum ControllerError {
    CONTROLLER_ERROR_NONE = 0x00,
    CONTROLLER_ERROR_OVERSPEED = 0x01,
    CONTROLLER_ERROR_INVALID_INPUT_MODE = 0x02,
    CONTROLLER_ERROR_UNSTABLE_GAIN = 0x04,
    CONTROLLER_ERROR_INVALID_MIRROR_AXIS = 0x08,
    CONTROLLER_ERROR_INVALID_LOAD = 0x10,
    CONTROLLER_ERROR_CURRENT_LIMIT_VIOLATION = 0x20
};

class ODrive {
public:
    // Constructor
    ODrive(uint8_t rx, uint8_t tx);

    // Initialize the connection to the ODrive.
    void begin();

    // Runs the full calibration sequence for the given axis, and waits for it to complete.
    void calibrate_axis(int axis);

    void set_axis_requested_state(int axis, AxisState state);

    AxisState get_axis_state(int axis);

    float get_vbus_voltage();

    void set_velocity(int axis, int vel);

    float get_current_velocity(int axis);

    AxisError get_axis_error(int axis);

    MotorError get_motor_error(int axis);

    EncoderError get_encoder_error(int axis);

    ControllerError get_controller_error(int axis);

    String get_errors(int axis);

private:
    String readString();

    void drain();

    SoftwareSerial _serial;
};

#endif // ODRIVE_H