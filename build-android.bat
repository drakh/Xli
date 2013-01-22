@echo off

cd projects\android-ndk
cmd /C ndk-build -j 8
@echo.

if not exist "obj\local\armeabi\libXliAndroidStatic.a" goto V7A
echo Writing file: XLI_ROOT\lib\armeabi\libXliAndroidStatic.a
if not exist "..\..\lib\armeabi" mkdir "..\..\lib\armeabi"
copy /Y "obj\local\armeabi\libXliAndroidStatic.a" "..\..\lib\armeabi"

:V7A
if not exist "obj\local\armeabi-v7a\libXliAndroidStatic.a" goto X86
echo Writing file: XLI_ROOT\lib\armeabi-v7a\libXliAndroidStatic.a
if not exist "..\..\lib\armeabi-v7a" mkdir "..\..\lib\armeabi-v7a"
copy /Y "obj\local\armeabi-v7a\libXliAndroidStatic.a" "..\..\lib\armeabi-v7a"

:X86
if not exist "obj\local\x86\libXliAndroidStatic.a" goto DONE
echo Writing file: XLI_ROOT\lib\x86\libXliAndroidStatic.a
if not exist "..\..\lib\x86" mkdir "..\..\lib\x86"
copy /Y "obj\local\x86\libXliAndroidStatic.a" "..\..\lib\x86"

:DONE
cd ..\..
