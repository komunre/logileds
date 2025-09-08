# Purpose
Any game that supports OutGauge protocol (for example, BeamNG.drive) will be able to send vehicle data (crucially, rpm) to logileds, and logileds will toggle LEDs on Logitech G29 according to percent from maximum RPM.

In other words, G29 LEDs are now on linux natively!

# Dependencies
[hidapi](https://github.com/libusb/hidapi)

## Debian/Ubuntu
`sudo apt install libhidapi-dev`

# Building
Check Makefile variables to see if any of them need to be modified for your system (for example, hidapi include path). 

Then run `make`
