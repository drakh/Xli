Xli Library
===========

Xli is a modular and cross platform app development framework for C++.


## Table of Contents

- [Documentation](#documentation)
- [Building](#building)
	- [Windows](#windows)
	- [OS X](#os-x)
	- [Linux](#linux)
- [License](#license)
	- [3rdparty dependencies](#3rdparty-dependencies)



# Documentation

Library documentation can be generated using `doxygen`.



# Building

## Windows

### Prerequisites

- Visual Studio 2013


### Instructions

1. Open solution located at `Xli\projects\vs2013\Xli Library.sln`.
2. Do a batch build.
   * This should produce static libraries located here:
     - `lib\vs2013\x86\Debug\`
     - `lib\vs2013\x86\Release\`
     - `lib\vs2013\x64\Debug\`
     - `lib\vs2013\x64\Release\`


## OS X

### Prerequisites

- Xcode
- Xcode command line tools
- cmake
- portaudio (universal)

MacPorts users can execute `sudo port install cmake portaudio +universal`

### Instructions - Command line

1. Open terminal and `cd` to Xli directory
2. Execute `./build.sh`
   * This should produce universal binaries located here:
     - `lib/OSX/x86/`
3. Optional step: `./build.sh install`

Debug binaries can be produced by replacing command in step 2 with `./build.sh --debug`.

List of available options can be shown using `./build.sh --help`.

### Instructions - Xcode

1. Open terminal and `cd` to Xli directory
2. Execute `./build.sh --target=xcode`
   * This should produce universal binaries located here:
     - `lib/OSX/x86/Debug/`
     - `lib/OSX/x86/Release/`
   * Xcode project located here:
     - `build/xcode/Xli Library.xcodeproj`


## Linux

### Prerequisites

- GNU make, C++ compiler, etc
- cmake
- curl
- freetype
- GLEW
- [libjpeg]
- OpenSSL
- png
- portaudio
- SDL2

`apt-get` users can execute `sudo apt-get install -y libpng12-dev libjpeg-dev libfreetype6-dev libglew-dev portaudio19-dev libcurl4-openssl-dev cmake g++`

SDL2 can be built and installed from source. Instructions here: http://libsdl.org

### Instructions - Command line

1. Open terminal and `cd` to Xli directory
2. Execute `./build.sh`
   * This should produce .so files located here:
     - `lib/linux/x86_$ARCH/`
4. Optional step: `./build.sh install`

Debug binaries can be produced by replacing command in step 2 with `./build.sh --debug`.

List of available options can be shown using `./build.sh --help`.


# License

The Xli Library source code is released under a MIT-style license:

> Copyright (C) 2010-2014 Outracks Technologies AS
>
> Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
>
> The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
>
> THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

**Note:** When linkning against Xli components you must also accept the licenses for the libraries Xli uses internally on your platform(s). See the section below for details.


## 3rdparty dependencies

The table below shows the list of libraries Xli uses under the hood, their licenses and how they are linked on the different target platforms.

Library       | License   | Component | Android | iOS     | OS X    | *nix    | Win32 
--------------|:---------:|:---------:|:-------:|:-------:|:-------:|:-------:|:-------:
glew          | BSD-style | Core      | -       | -       | static  | dynamic | static
SDL2          | MIT-style | Core      | -       | static  | static  | dynamic | -
ConvertUTF    | ?         | Core      | static  | static  | static  | static  | static
freetype      | BSD       | Media     | static  | static  | static  | dynamic | static
[libjpeg]     | ?         | Media     | static  | -       | static  | dynamic | static
json_parser   | ?         | Media     | static  | static  | static  | static  | static
pnglib        | zlib      | Media     | static  | -       | static  | dynamic | static
tinyxml       | ?         | Media     | static  | static  | static  | static  | static
unzip         | ?         | Media     | static  | static  | static  | static  | static
zlib          | zlib      | Media     | dynamic | static  | dynamic | dynamic | static
portaudio     | ?         | Audio     | -       | -       | dynamic | dynamic | static
curl          | ?         | Http      | -       | -       | dynamic | dynamic | static

[libjpeg]: http://www.ijg.org/
