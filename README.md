# Yet Another Argument Parser (YAAP)

YAAP is a simple command line argument parser for C. It uses functions and structures to add arguments to the parsing context and parse the command line.

## Author
Dominik Meyer <dmeyer@federationhq.de>

## License

Copyright 2018 by Dominik Meyer Licensed under GPLv3

## Features

* git like command support (git commit)
* short and long flag support ( -t YAAP or –title=YAAP)
* string argument support ( -t YAAP or – title=YAAP)
* simple flags (-v, -V, -h, –help)
* integer arguments (-c 10)

## Coming

* hex arguments ( -c 0x5A)

## Installation

YAAP is intented to be copied into your project directory. Different approaches are available:

* simpe copy argparse.h and argparse.c into your source directory
* add the github repository as a submodule
* add the github repository as a subtree
* add the github repository as a subrepo

In all cases you have to make sure that argparse.c is compiled into an object file and is appended to your link stage.

## Documentation

The Doxygen documentation can be found at https://byterazor.github.io/yaap/ .

## Signed Commits
All commits are signed with the GPG key for dmeyer@federationhq.de valid from 2017-07-30

Fingerprint:  8A96 F2F7 FC07 3485 C724  9879 B4C3 12B6 0060 6B64
