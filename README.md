# Arduino Nano Python EEPROM programmer through serial data connection

This project is based in 99% on Ben Eater's EEPROM programmer. So all credits go to him [for this wonderful invention](https://github.com/beneater/eeprom-programmer).

Copyright 2017 Ben Eater

This code and schematic are [MIT licensed](http://en.wikipedia.org/wiki/MIT_License).

## Circuit

This is a simple circuit for programming the 28C16, 28C64, 28C256, and similar parallel EEPROMs using an Arduino. Since the Arduino doesn’t have enough pins to directly control all of the address, data, and control lines of the EEPROM, two 74HC595 shift registers are used for the 11 address lines (15 for the 28C256) and the output enable control line.

![Schematic of EEPROM programmer](https://raw.githubusercontent.com/handmade0octopus/python-arduino-eeprom-programmer/master/schematic.png)



### How does it work?

The base Arduino code is nearly identical to Ben Eater's one (see link above). Small changes were made to the code. It features additional functions to debug and see what's inside EEPROM even without pluggin it in.

It uses serial connection and first it sends 1 byte of data to tell Arduino how big the rom is. It takes whatever is coded to bin1.bin and then it reads from EEPROM and saves it to bin2.bin so you can compare if writing was correct. I used Windows HxD program to decode HEX file as it's easy to use.

Edit ['nano_programmer.py'](/nano_programmer/nano_programmer.py) and change COM6 port to the one which your Arduino is using.

Edit ['makerom.py'](/nano_programmer/makerom.py) to your liking. It's setup so it only creates 1024 bytes for faster programming (whole 32KB ROM takes over 5 min to program!). You can program it once so 7FFC and 7FFD are set up correctly and you can edit only 1024 so it saves a lot of time!