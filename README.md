# Serial Data Logger
Logs data received via serial on an SD card

## User Setup
This is a guide to have the serial data logger firmware running on the Adafruit Feather M0 Adalogger board.
### Steps
1. Connect the `Adafruit Feather m0` board to your computer via USB cable.
2. Double tapping the `reset` button. Once the bootloader is active, a new drive should appear with name `FEATHERBOOT`.
3. Open the drive, then drag and drop `firmware.uf2` file to the drive. After the file is finished copying, the bootloader will automatically restart.
4. You may also get get a complaint that the drive was ejected without warning, don't worry about this.
5. The new firmware should be up and running.

Note: If for some reason the `FEATHERBOOT` or any other drive name ending with `BOOT` doesn't show up when in bootloader mode, try changing the USB cable.

## Development Setup
...... In progress
