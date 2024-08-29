# fan_mixer
Cheap PWM fan mixer based on Blue Pill

Supported MCUs: STM32F103C6T6

The idea is to read two PWM singals from source like motherboard, and set one PWM output
according to the inputs. Default logic sums the signals, but it can be easily adjusted in
fan.c file.

To flash the board, connect blue pill to stlink, plug blue pill to power, and stlink to PC.
Build project with "make build" and flash the board with "make flash" command.

commands:

Build project:
$ make build

Clean build files:
$ make clean

Test project:
$ make test

Flash board:
$ make flash

Required dependencies:
arm-none-eabi-gcc
arm-none-eabi-g++
arm-none-eabi-objcopy"
arm-none-eabi-objdump"
make
cmake
openocd
gdb