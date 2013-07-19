@echo off
set START_DIR=%CD%

mkdir "%~dp0\build\mingw"
cd "%~dp0\build\mingw"

cmake ..\.. -G"Unix Makefiles"
make

cd %START_DIR%
