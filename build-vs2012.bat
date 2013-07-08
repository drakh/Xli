@echo off
call "%VS110COMNTOOLS%\vsvars32.bat"
MSBuild "projects/vs2012/Xli Library.sln" /p:Configuration=Debug /p:Platform=Win32 /m
MSBuild "projects/vs2012/Xli Library.sln" /p:Configuration=Release /p:Platform=Win32 /m
MSBuild "projects/vs2012/Xli Library.sln" /p:Configuration=Debug /p:Platform=x64 /m
MSBuild "projects/vs2012/Xli Library.sln" /p:Configuration=Release /p:Platform=x64 /m
