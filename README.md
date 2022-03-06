# Serial Data Logger
Logs data received via serial on an SD card

## User Setup
This is a guide to have the serial data logger firmware running on the Adafruit Feather M0 Adalogger board.
### Steps
1. Connect the `Adafruit Feather m0` board to your computer via USB cable.
2. Double tapping the `reset` button. Once the bootloader is active, a new drive should appear with name `FEATHERBOOT`.
3. Open the drive, then drag and drop `firmware.uf2` file to the drive. After the file is finished copying, the bootloader will automatically restart. You may get a complaint that the drive was ejected without warning, don't worry about this.
4. The new firmware should be up and running.

Note: If for some reason the `FEATHERBOOT` or any other drive name ending with `BOOT` doesn't show up when in bootloader mode, try changing the USB cable.

## Development Setup
Guide to generate a new uf2 file to upload on the Adafruit Feather M0 Adalogger board.
### Steps
1. Download the [Uf2 repo](https://github.com/Microsoft/uf2) and unzip.
2. Open the folder and copy the full path to the `uf2conv.py` file present in `utils` folder.
3. On the SerialDataLogger project PlatformIO terminal, build the project using `pio run -e m0`.
4. Next, use the command `<full/relative path to utils folder>/uf2conv.py .pio/build/m0/firmware.bin -o firmware.uf2` to generate a new `firmware.uf2` file.
5. Connect the `Adafruit Feather m0` board to your computer via USB cable.
6. Double tapping the `reset` button. Once the bootloader is active, a new drive should appear with name `FEATHERBOOT`.
7. Open the drive, then drag and drop `firmware.uf2` file to the drive. After the file is finished copying, the bootloader will automatically restart. You may get a complaint that the drive was ejected without warning, don't worry about this.
8. The new firmware should be up and running.
