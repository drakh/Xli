@echo off

cd projects\android
cmd /C ndk-build -j 8
@echo.

if not exist "obj\local\armeabi" goto V7A
echo Writing files to: XLI_ROOT\lib\android\armeabi\
if not exist "..\..\lib\android\armeabi" mkdir "..\..\lib\android\armeabi"
copy /Y "obj\local\armeabi\libXli*.a" "..\..\lib\android\armeabi"

:V7A
if not exist "obj\local\armeabi-v7a" goto X86
echo Writing files to: XLI_ROOT\lib\android\armeabi-v7a\
if not exist "..\..\lib\android\armeabi-v7a" mkdir "..\..\lib\android\armeabi-v7a"
copy /Y "obj\local\armeabi-v7a\libXli*.a" "..\..\lib\android\armeabi-v7a"

:X86
if not exist "obj\local\x86" goto DONE
echo Writing file: XLI_ROOT\lib\android\x86\
if not exist "..\..\lib\android\x86" mkdir "..\..\lib\android\x86"
copy /Y "obj\local\x86\libXli*.a" "..\..\lib\android\x86"

:DONE
cd ..\..
