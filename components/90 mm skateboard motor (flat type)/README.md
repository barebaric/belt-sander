# Flat skateboard motor

## Data given by supplier (a lot of which turned out to be wrong!)

70mm 90mm Electrical Skateboard 350W 600W Motor Wireless Hub Motor PU Cover for Single Drive or Dual Drive Electrical longboard

- Brake: Yes, the force varies according to speed

- Rated voltage: 24-36V

- Rated power: about 250W

- Top speed: 38km / hour

- 70mm maximum power: 350W

- 90mm maximum power: 600W

- Maximum current: about 38A

- No-load speed: 2640rpm

- No-load current: 500MA

- Efficiency: 85%

- Maximum load: 100KG
1. Use voltage: 24-36V
   2.24 pole stator, smooth start, high torque and high efficiency!
   3.kv value: 110KV (rotation speed of 2640rpm under 24v voltage)
2. The tire can be replaced
3. Closed bearing support at both ends, the whole motor is waterproof and dustproof.
4. Hall supports up to 60V

## Actual data identified

- 14 poles (7 pole pairs)

- Hall sensor resolution: 6 (=42 cpr)

- Hall sensor bandwidth 100 seems to work ok

## Some useful ODrive Mini test commands (but apply below config first!)

```python
odrv0.axis0.requested_state = AXIS_STATE_FULL_CALIBRATION_SEQUENCE
dump_errors(odrv0)
odrv0.save_configuration()

odrv0.axis0.config.general_lockin.vel = 3000
odrv0.axis0.config.general_lockin.accel = 600
odrv0.axis0.requested_state = AXIS_STATE_LOCKIN_SPIN
odrv0.axis0.requested_state = AXIS_STATE_IDLE
```

## Working XDrive Mini Config for 36V (tested in spin mode)

```python
In [443]: odrv0.config
Out[443]: 
enable_uart = True (bool)
uart_baudrate = 115200 (int)
enable_i2c_instead_of_can = False (bool)
enable_ascii_protocol_on_usb = True (bool)
max_regen_current = 0.0 (float)
brake_resistance = 2.0 (float)
dc_bus_undervoltage_trip_level = 33.0 (float)
dc_bus_overvoltage_trip_level = 42.0 (float)
enable_dc_bus_overvoltage_ramp = False (bool)
dc_bus_overvoltage_ramp_start = 59.92000198364258 (float)
dc_bus_overvoltage_ramp_end = 59.92000198364258 (float)
dc_max_positive_current = 5.0 (float)
dc_max_negative_current = -0.009999999776482582 (float)
gpio1_pwm_mapping:
  endpoint = (0, 0) (RemoteProperty)
  min = 0.0 (float)
  max = 0.0 (float)
gpio2_pwm_mapping:
  endpoint = (0, 0) (RemoteProperty)
  min = 0.0 (float)
  max = 0.0 (float)
gpio3_pwm_mapping:
  endpoint = (0, 0) (RemoteProperty)
  min = 0.0 (float)
  max = 0.0 (float)
gpio4_pwm_mapping:
  endpoint = (0, 0) (RemoteProperty)
  min = 0.0 (float)
  max = 0.0 (float)
gpio3_analog_mapping:
  endpoint = (0, 0) (RemoteProperty)
  min = 0.0 (float)
  max = 0.0 (float)
gpio4_analog_mapping:
  endpoint = (0, 0) (RemoteProperty)
  min = 0.0 (float)
  max = 0.0 (float)

In [444]: odrv0.axis0.config
Out[444]: 
startup_motor_calibration = False (bool)
startup_encoder_index_search = False (bool)
startup_encoder_offset_calibration = False (bool)
startup_closed_loop_control = False (bool)
startup_sensorless_control = False (bool)
startup_homing = False (bool)
enable_step_dir = False (bool)
step_dir_always_on = False (bool)
turns_per_step = 0.0009765625 (float)
watchdog_timeout = 0.0 (float)
enable_watchdog = False (bool)
step_gpio_pin = 1 (int)
dir_gpio_pin = 2 (int)
calibration_lockin:
  current = 10.0 (float)
  ramp_time = 0.4000000059604645 (float)
  ramp_distance = 3.1415927410125732 (float)
  accel = 20.0 (float)
  vel = 40.0 (float)
sensorless_ramp:
  current = 10.0 (float)
  ramp_time = 0.4000000059604645 (float)
  ramp_distance = 3.1415927410125732 (float)
  accel = 200.0 (float)
  vel = 400.0 (float)
  finish_distance = 100.0 (float)
  finish_on_vel = True (bool)
  finish_on_distance = False (bool)
  finish_on_enc_idx = False (bool)
general_lockin:
  current = 10.0 (float)
  ramp_time = 0.4000000059604645 (float)
  ramp_distance = 3.1415927410125732 (float)
  accel = 600.0 (float)
  vel = 3000.0 (float)
  finish_distance = 100.0 (float)
  finish_on_vel = False (bool)
  finish_on_distance = False (bool)
  finish_on_enc_idx = False (bool)
can_node_id = 0 (int)
can_node_id_extended = False (bool)
can_heartbeat_rate_ms = 100 (int)

In [445]: odrv0.axis0.motor.config
Out[445]: 
pre_calibrated = True (bool)
pole_pairs = 7 (int)
calibration_current = 5.0 (float)
resistance_calib_max_voltage = 2.0 (float)
phase_inductance = 0.00010969385039061308 (float)
phase_resistance = 0.2149658501148224 (float)
torque_constant = 0.07518181949853897 (float)
direction = 1 (int)
motor_type = 0 (int)
current_lim = 25.0 (float)
current_lim_margin = 8.0 (float)
torque_lim = inf (float)
inverter_temp_limit_lower = 100.0 (float)
inverter_temp_limit_upper = 120.0 (float)
requested_current_range = 20.0 (float)
current_control_bandwidth = 1000.0 (float)
acim_slip_velocity = 14.706000328063965 (float)
acim_gain_min_flux = 10.0 (float)
acim_autoflux_min_Id = 10.0 (float)
acim_autoflux_enable = False (bool)
acim_autoflux_attack_gain = 10.0 (float)
acim_autoflux_decay_gain = 1.0 (float)

In [446]: odrv0.axis0.trap_traj.config
Out[446]: 
vel_limit = 2500.0 (float)
accel_limit = 0.5 (float)
decel_limit = 0.5 (float)

In [447]: odrv0.axis0.encoder.config
Out[447]: 
mode = 1 (int)
use_index = False (bool)
find_idx_on_lockin_only = False (bool)
abs_spi_cs_gpio_pin = 1 (int)
zero_count_on_find_idx = True (bool)
cpr = 42 (int)
offset = 24 (int)
pre_calibrated = False (bool)
offset_float = 0.5135156512260437 (float)
enable_phase_interpolation = True (bool)
bandwidth = 100.0 (float)
calib_range = 0.019999999552965164 (float)
calib_scan_distance = 50.26548385620117 (float)
calib_scan_omega = 12.566370964050293 (float)
idx_search_unidirectional = False (bool)
ignore_illegal_hall_state = False (bool)
sincos_gpio_pin_sin = 3 (int)
sincos_gpio_pin_cos = 4 (int)

In [448]: odrv0.axis0.controller.config
Out[448]: 
gain_scheduling_width = 10.0 (float)
enable_vel_limit = True (bool)
enable_current_mode_vel_limit = True (bool)
enable_gain_scheduling = False (bool)
enable_overspeed_error = True (bool)
control_mode = 2 (int)
input_mode = 1 (int)
pos_gain = 20.0 (float)
vel_gain = 0.1666666716337204 (float)
vel_integrator_gain = 0.3333333432674408 (float)
vel_limit = 2000.0 (float)
vel_limit_tolerance = 1.2000000476837158 (float)
vel_ramp_rate = 1.0 (float)
torque_ramp_rate = 0.009999999776482582 (float)
circular_setpoints = False (bool)
circular_setpoint_range = 1.0 (float)
homing_speed = 0.25 (float)
inertia = 0.0 (float)
axis_to_mirror = 255 (int)
mirror_ratio = 1.0 (float)
load_encoder_axis = 0 (int)
input_filter_bandwidth = 2.0 (float)
anticogging:
  index = 0 (int)
  pre_calibrated = False (bool)
  calib_anticogging = True (bool)
  calib_pos_threshold = 1.0 (float)
  calib_vel_threshold = 1.0 (float)
  cogging_ratio = 1.0 (float)
  anticogging_enabled = True (bool)
```
