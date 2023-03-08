# <img src="Images/logo_square.png" width="50"/> ERapGBC

ERapGBC is a Game Boy Color emulator written in C++.

This is not intended to be completely functional at this time: it's just a personal project to study emulators development and understand how the Game Boy Color hardware works.

[SFML](https://www.sfml-dev.org/) is used for rendering. Apart from that, all remaining code has been written from scratch by reading manuals that describe the hardware.
Many inconsistencies have been debugged by comparing this program's output with the one of [bgb](https://bgb.bircd.org/).

## Screenshots

<img src="Images/screenshot.png" />

## Building

### Windows
Open `ERapGBC.sln` with Visual Studio 2019 or later, and follow the usual build process.

### Linux
Install SFML dependencies with your package manager. Here you can find a couple of examples:

- Debian based distros: `sudo apt-get install libsfml-dev`
- Arch based distros: `sudo pacman -S sfml`

Then, build the executable by running `make` in the root of this repository.
The resulting executable will be available at `build/erapGBC`.