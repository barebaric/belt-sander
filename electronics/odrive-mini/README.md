# MKS XDrive Mini (Makerbase, ODrive)

This is the BLDC controller chosen for the belt sander motor.

![MKS XDrive Mini](odrive.jpg)

## Status of trying to get this product to work

### Via USB

- Connecting via USB worked with the odrivetool from the supplier repository (linked below). The official odrivetool also worked, but did not offer all configuration names of the old firmware used by the XDrive.

- I was able to make a motor spin correctly in AXIS_STATE_LOCKIN_SPIN mode only. Other modes the motor just did spin, but went into MOTOR_ERROR_DC_BUS_OVER_CURRENT.
  * Note that the hall sensor needs to be calibrated after every(!) reboot of the odrive.
  * odrv0.axis0.requested_state = AXIS_STATE_CLOSED_LOOP_CONTROL
  * odrv0.axis0.controller.config.control_mode=CONTROL_MODE_VELOCITY_CONTROL
  * odrv0.axis0.controller.input_vel = 10
  
  Maybe further hints can be found on [this Chinese website](https://blog.csdn.net/gjy_skyblue/category_10813011.html?spm=1001.2014.3001.5482)

- Under load, the controller went into ENCODER_ERROR_ILLEGAL_HALL_STATE. The issue may possibly be resolved by adding filter capacitors: [Encoder error ERROR_ILLEGAL_HALL_STATE](https://discourse.odriverobotics.com/t/encoder-error-error-illegal-hall-state/1047/7)
  * I did just that (used 47nF) and have so far not seen the error anymore.

### Via SPI

I found out that the SPI connector only supports encoders and cannot be used to control the ODrive :-(.

### Via UART

UART confirmed to work fine. But a logic level converter is needed, as the ODrive outputs 5V on the
GPIO pins. I used a TXS0108E because I had it at hand.

Additional collected information:

- I found out that the MKS XDrive Mini has an ancient firmware version 0.5.1, though they also mangled the firmware version number output (shows v0.0.0.dev) so I don't know if they made any other changes.

- In that firmware, you have to connect to the ODrive via USB first, setting `odrv0.config.enable_uart = True` (and saving and rebooting).

- Then, the UART pins are GPIO1 and GPIO2, and they support up to 115200 baud, which worked well.

### Via I2C

Not investigated much yet. There is a setting `odrv0.config.enable_i2c_instead_of_can`, but I am not sure if it is enough to use that - it seems at least the jumper next to the CAN port needs to be removed, too, perhaps more would be needed.

## Supplier provided specs

Supplier doc/firmware repository: [GitHub - makerbase-motor/ODrive-MKS](https://github.com/makerbase-motor/ODrive-MKS)

![Pinout](pinout.webp)
![Specs](specs.webp)
