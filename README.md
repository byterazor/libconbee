# libconbee

## Description

The ConBee2 USB Stick is a Zigbee radio dongle . It supports the communication with
Zigbee Light Link (ZLL) devices like Philips HUE and OSRAM Lightify.

This library provides a raw communication interface to the USB dongle to configure
the stick and read and write raw Application Support Layer (APS) frames. This
library does *not* handle any Zigbee communication. You require an additional library or
application if you want to do Zigbee communication.

## Authors

- Dominik Meyer <dmeyer@federationhq.de>


## Licenses

### library related souce code
This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, version 3.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with this program. If not, see <http://www.gnu.org/licenses/>.

### external code

- *cmake/Modules/doxygen.cmake* - cmake package for easy doxygen configuration, Copyright (C) 2018 by George Cave - gcave@stablecoder.ca, Apache License, Version 2.0

## Building

This project uses cmake for generating the build system and building the library.

### releases

```bash
mkdir build
cd build
cmake ..
make
```

### with documentation

```bash
mkdir build
cd build
cmake -D BUILD_DOCUMENTATION=yes ..
make
make doc-libconbee
```

The doxygen documentation can be found under build/doxygen.

### debugging

```bash
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=DEBUG ..
make
```
