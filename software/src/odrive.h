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

private:
    String readString();

    void drain();

    SoftwareSerial _serial;
};

#endif // ODRIVE_H