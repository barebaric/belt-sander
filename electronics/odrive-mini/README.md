# MKS XDrive Mini (Makerbase, ODrive)

This is the BLDC controller chosen for the belt sander motor.

![MKS XDrive Mini](odrive.jpg)

## Status of trying to get this product to work

### Via USB

- Connecting via USB worked with the odrivetool from the supplier repository (linked below). The official odrivetool also worked, but did not offer all configuration names of the old firmware used by the XDrive.

- I was able to make a motor spin in AXIS_STATE_LOCKIN_SPIN mode only. Other modes the motor just didn't spin, more debugging needed.
  
  Maybe further hints can be found on [this Chinese website](https://blog.csdn.net/gjy_skyblue/category_10813011.html?spm=1001.2014.3001.5482)

- Under load, the controller went into ENCODER_ERROR_ILLEGAL_HALL_STATE. The issue may possibly be resolved by adding filter capacitors: [Encoder error ERROR_ILLEGAL_HALL_STATE](https://discourse.odriverobotics.com/t/encoder-error-error-illegal-hall-state/1047/7)

### Via SPI

I found out that the SPI connector only supports encoders and cannot be used to control the ODrive :-(.

### Via UART

So far I still haven't gotten UART connectivity to work - and quite possibly it cannot be made to work.

Some collected information:

- I found out that the MKS XDrive Mini has an ancient firmware version 0.5.1, though they also mangled the firmware version number output (shows v0.0.0.dev) so I don't know if they made any other changes.

- In that firmware, you have to connect to the ODrive via USB first, setting `odrv0.config.enable_uart = True` (and saving and rebooting).

- Then, according to the some old v0.5.1 docs that I found, the UART pins are supposedly GPIO1 and GPIO2. But I was still unable to connect.

- There may be some other bugs that prevent this from working at all:
  
   * [According to this thread](https://discourse.odriverobotics.com/t/odrive-communication-over-uart/8575) "There is however a little bug in 0.5.1 that you have to fix first. You have to add these #ifdefs in Firmware/communication/interface_uart.cpp and then recompile the firmware". The problem with that is: The XDrive firmware refuses DFU mode, so the only way to install a patched firmware is with an STLink (which I don't have).
  
   * Firmware 0.5.2 also seems broken [according to some other thread](https://discourse.odriverobotics.com/t/odrive-ignores-most-requests-in-uart-native-mode/7368/17). Though I may be able to live without native mode support (using ASCII mode instead).

### Via I2C

Not investigated much yet. There is a setting `odrv0.config.enable_i2c_instead_of_can`, but I am not sure if it is enough to use that - it seems at least the jumper next to the CAN port needs to be removed, too, perhaps more would be needed.

## Supplier provided specs

Supplier doc/firmware repository: [GitHub - makerbase-motor/ODrive-MKS](https://github.com/makerbase-motor/ODrive-MKS)

![Pinout](pinout.webp)
![Specs](specs.webp)
