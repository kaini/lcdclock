# 7-Segment LCD Clock

Schematic, PCB and firmware (C++) for a 7-segment LCD clock. For details please
see [my site](https://me.pushrax.com/lcd-clock/).

## Build Instructions

Just use `make`. If you want a release build use `make RELEASE=1`. If you
have a SEGGER J-Link you can use `make flash` to flash the build output
onto a compatible microcontroller.
