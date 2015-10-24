# STM32F4 OV7670 Demo

Demonstration of interfacing the Nucleo STM32F446RE with a OV7670 camera module.

![Screenshot](camera.png)

## Building

To build the project you will also need to set up an Eclipse (OpenSTM32) workspace containing the stdperiph library. Except from this there are no external dependencies.

##Running it

After building and flashing the resulting elf file using openocd you can connect to the serial port of the device.
Frames are continuously sent over the serial connection as they are captured by the camera.

##Credits

This project contains lots of code written by other (very awesome) developers.

* [http://supuntharanga.blogspot.co.uk/2014/04/stm32f4-discovery-board-ov7660-or.html](http://supuntharanga.blogspot.co.uk/2014/04/stm32f4-discovery-board-ov7660-or.html)
* [http://embeddedprogrammer.blogspot.co.uk/2012/07/hacking-ov7670-camera-module-sccb-cheat.html](http://embeddedprogrammer.blogspot.co.uk/2012/07/hacking-ov7670-camera-module-sccb-cheat.html)
* [http://www.urel.feec.vutbr.cz/MPOA/2014/cam-ov7670](http://www.urel.feec.vutbr.cz/MPOA/2014/cam-ov7670)
