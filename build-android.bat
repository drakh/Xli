@echo off
set START_DIR=%CD%

cd "%~dp0\projects\android"
call ndk-build -j %NUMBER_OF_PROCESSORS% %*

if %ERRORLEVEL% neq 0 (
	cd %START_DIR%
	exit /b -1
)

echo.

if not exist "obj\local\armeabi" goto V7A
echo Writing files to: %~dp0\lib\android\armeabi\
if not exist "..\..\lib\android\armeabi" mkdir "..\..\lib\android\armeabi"
copy /Y "obj\local\armeabi\*.a" "..\..\lib\android\armeabi"
copy /Y "obj\local\armeabi\*.so" "..\..\lib\android\armeabi"

:V7A
if not exist "obj\local\armeabi-v7a" goto X86
echo Writing files to: %~dp0\lib\android\armeabi-v7a\
if not exist "..\..\lib\android\armeabi-v7a" mkdir "..\..\lib\android\armeabi-v7a"
copy /Y "obj\local\armeabi-v7a\*.a" "..\..\lib\android\armeabi-v7a"
copy /Y "obj\local\armeabi-v7a\*.so" "..\..\lib\android\armeabi-v7a"

:X86
if not exist "obj\local\x86" goto DONE
echo Writing files to: %~dp0\lib\android\x86\
if not exist "..\..\lib\android\x86" mkdir "..\..\lib\android\x86"
copy /Y "obj\local\x86\*.a" "..\..\lib\android\x86"
copy /Y "obj\local\x86\*.so" "..\..\lib\android\x86"

:DONE
cd %START_DIR%
