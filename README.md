Xli Library
===========

Xli is a modular and cross platform app development framework for C++.


### Table of Contents

- [Documentation](#documentation)
- [Building](#building)
    - [Android](#android)
    - [iOS](#ios)
    - [Linux](#linux)
    - [OS X](#os-x)
    - [Raspberry PI](#raspberry-pi)
    - [Windows](#windows)
- [License](#license)
    - [3rdparty dependencies](#3rdparty-dependencies)



# Documentation

Library documentation can be generated using `doxygen`.



# Building

## Android

### Prerequisites

- Android NDK (`ndk-build` must be available in PATH)
- Unix Shell (Windows only)

### Instructions - OS X, Linux, MSYS

1. Open terminal and `cd` to Xli directory
2. Execute `./build-android.sh`
   * This should produce .so files located here:
     - `lib/android/armeabi-v7a`

Debug binaries can be produced by replacing command in step 2 with `./build-android.sh --debug`. 

**Note:** Android build will output debug and release binaries to the same folder -- do a `./build-android.sh clean` before rebuilding just to be safe. Also, debug binaries should be huge. For instance `lib/android/armeabi-v7a/libXli.so` should be roughly ~1,5MB after a debug build, while ~200KB in release.

### Instructions - Windows

Windows users need a way to execute unix shell scripts. This can be done using i.e. [MSYS](http://www.mingw.org/wiki/MSYS).

Uno users can use the *Uno Native Build environment* provided by Outracks, containing both MSYS and the Android NDK. Open `start.bat`, type `bash -li` to get a bash shell, then follow the instructions given above.


## iOS

### Prerequisites

- OS X (>= 10.7)
- Xcode with most recent iOS SDK
- Xcode command line tools
- CMake

**MacPorts** users can execute `sudo port install cmake` to install CMake.

### Instructions

1. Open terminal and `cd` to Xli directory
2. Execute `./build-iOS.sh`
   * This should produce static libraries located here:
     - `lib/iOS/Debug-iphoneos/`
     - `lib/iOS/Debug-iphonesimulator/`
     - `lib/iOS/Release-iphoneos/`
     - `lib/iOS/Release-iphonesimulator/`


## Linux

### Prerequisites

- GNU make, C++ compiler, etc
- CMake
- curl (with OpenSSL support)
- freetype
- GLEW
- [libjpeg]
- png
- portaudio
- [SDL2]

**APT** users can execute `sudo apt-get install -y libpng12-dev libjpeg-dev libfreetype6-dev libglew-dev portaudio19-dev libcurl4-openssl-dev cmake g++`

[SDL2] can be built and installed from source. To install from Mercurial, do this:

    hg clone http://hg.libsdl.org/SDL
    cd SDL
    ./configure
    make -j 4
    sudo make install

### Instructions

1. Open terminal and `cd` to Xli directory
2. Execute `./build.sh`
   * This should produce .so files located here:
     - `lib/linux/x86_$ARCH/`
3. Optional step: `sudo ./build.sh install`

Debug binaries can be produced by replacing command in step 2 with `./build.sh --debug`.

List of available options can be shown using `./build.sh --help`.


## OS X

### Prerequisites

- OS X (>= 10.7)
- Xcode
- Xcode command line tools
- CMake
- portaudio (universal)

**MacPorts** users can execute `sudo port install cmake portaudio +universal` to install CMake and portaudio.

### Instructions

1. Open terminal and `cd` to Xli directory
2. Execute `./build.sh`
   * This should produce universal binaries located here:
     - `lib/OSX/x86/`
3. Optional step: `sudo ./build.sh install`

Debug binaries can be produced by replacing command in step 2 with `./build.sh --debug`.

List of available options can be shown using `./build.sh --help`.

**Note:** When building applications using Xcode, it would be useful to also build Xli using a generated Xcode project for better integration with debugger and such. This can be achieved using these alternative instructions:

1. Open terminal and `cd` to Xli directory
2. Execute `./build.sh --target=xcode`
   * This should produce universal binaries located here:
     - `lib/OSX/x86/Debug/`
     - `lib/OSX/x86/Release/`
   * Generated Xcode project located here:
     - `build/xcode/Xli Library.xcodeproj`


## Raspberry PI

### Prerequisites

- GNU make, C++ compiler, etc
- CMake
- curl (with OpenSSL support)
- freetype
- [libjpeg]
- png
- portaudio
- [SDL2]

**APT** users can execute `sudo apt-get install -y libpng12-dev libjpeg-dev libfreetype6-dev portaudio19-dev libcurl4-openssl-dev cmake g++`

[SDL2] can be built and installed from source. To install from Mercurial, do this:

    hg clone http://hg.libsdl.org/SDL
    cd SDL
    ./configure
    make -j 2
    sudo make install

### Instructions

1. Open terminal and `cd` to Xli directory
2. Execute `./build.sh`
   * This should produce .so files located here:
     - `lib/linux/arm/`
3. Optional step: `sudo ./build.sh install`

Debug binaries can be produced by replacing command in step 2 with `./build.sh --debug`.

List of available options can be shown using `./build.sh --help`.


## Windows

### Prerequisites

- Windows (>= Vista)
- Visual Studio 2013

### Instructions

1. Open solution file located at `Xli\projects\vs2013\Xli Library.sln`.
2. Do a batch build. (From menu bar: **BUILD->Batch Build...**, **Select All**, **Build**)
   * This should produce static libraries located here:
     - `lib\vs2013\x86\Debug\`
     - `lib\vs2013\x86\Release\`
     - `lib\vs2013\x64\Debug\`
     - `lib\vs2013\x64\Release\`


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
[SDL2]        | MIT-style | Core      | -       | static  | static  | dynamic | -
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

[SDL2]: http://libsdl.org/
[libjpeg]: http://www.ijg.org/
