@echo off
cd ..\..
call build-android.bat
cd examples\GLWindow\project-android
call ndk-build
cd ..
