Xli
===

Xli is a modular and cross-platform app development framework for C++. It is being used to implement the Uno C++ runtime and core library among some other things.

Header documentation can be generated using `doxygen`. 

Please see the <a href="modules.html">Modules</a> page for an overview of the functionality.

Questions and other feedback can be posted in the beta zone forum.

### Table of Contents

1. [Build instructions](#build-instructions)
    1. [Android](#android)
    2. [iOS](#ios)
    3. [Linux](#linux)
    4. [OS X](#os-x)
    5. [Raspberry PI](#raspberry-pi)
    6. [Windows](#windows)


# Build instructions

## Android

### Prerequisites

- CMake
- Android NDK (`ndk-build` must be available in *PATH*)
- Unix Shell (Windows only)

### Instructions - OS X, Linux, MSYS

1. Open terminal and `cd` to Xli directory
2. Execute `./build-android.sh`
   * This should produce .so files located here:
     - `lib/android/armeabi-v7a`

Debug binaries can be produced by replacing command in step 2 with `./build-android.sh --debug`. 

List of available options can be shown using `./build-android.sh --help`.

### Instructions - Windows

Windows users need a way to execute unix shell scripts. This can be done using i.e. [MSYS](http://www.mingw.org/wiki/MSYS).

Uno users can use the *Uno Native Build environment* provided by Outracks, containing both CMake, MSYS and the Android NDK. Open `start.bat`, type `bash -li` to get a bash shell, then follow the instructions given above.


## iOS

### Prerequisites

- CMake (~>2.8) - NB: 3.0 not supported
- Xcode with most recent iOS SDK
- Xcode command line tools
- OS X (>= 10.7)

**MacPorts** users can execute `sudo port install cmake` to install CMake.

### Instructions

1. Open terminal and `cd` to Xli directory
2. Execute `./build-iOS.sh`
   * This should produce static libraries located here:
     - `lib/iOS/Debug-iphoneos/`
     - `lib/iOS/Debug-iphonesimulator/`
     - `lib/iOS/Release-iphoneos/`
     - `lib/iOS/Release-iphonesimulator/`
   * Generated Xcode projects located here:
     - `build/iOS/OS/Xli.xcodeproj`
     - `build/iOS/SIMULATOR/Xli.xcodeproj`


## Linux

### Prerequisites

- CMake
- GNU make, C++ compiler, etc
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
     - `lib/linux/x86_32/` *OR* `lib/linux/x86_64/`
3. Optional step: `sudo ./build.sh install`

Debug binaries can be produced by replacing command in step 2 with `./build.sh --debug`.

List of available options can be shown using `./build.sh --help`.


## OS X

### Prerequisites

- CMake
- Xcode
- Xcode command line tools
- OS X (>= 10.7)

**MacPorts** users can execute `sudo port install cmake` to install CMake.

### Instructions

1. Open terminal and `cd` to Xli directory
2. Execute `./build.sh`
   * This should produce universal binaries located here:
     - `lib/OSX/x86/`
3. Optional step: `sudo ./build.sh install`

Debug binaries can be produced by replacing command in step 2 with `./build.sh --debug`.

List of available options can be shown using `./build.sh --help`.

**Note:** When building applications using Xcode, it would be useful to also build Xli using Xcode for better integration with debugger and such. This can be achieved using these alternative instructions:

1. Open terminal and `cd` to Xli directory
2. Execute `./build.sh --platform=xcode`
   * This should produce universal binaries located here:
     - `lib/OSX/x86/Debug/`
     - `lib/OSX/x86/Release/`
   * Generated Xcode project located here:
     - `build/Xcode/Xli.xcodeproj`


## Raspberry PI

### Prerequisites

- CMake
- GNU make, C++ compiler, etc
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
